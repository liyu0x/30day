#include "bootpack.h"
#include <stdio.h>

#define KEYCMD_LED 0xed

void keywin_off(struct SHEET *key_win);
void keywin_on(struct SHEET *key_win);

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40];
	int mx, my, i;
	struct MOUSE_DEC mdec;

	struct MEMMAN *memman = (struct MEMMAN *)MEMMAN_ADDR;
	unsigned int memtotal;

	struct SHTCTL *shtctl;
	struct SHEET *sht_back, *sht_mouse, *sht_cons[2];
	unsigned char *buf_back, buf_mouse[256], *buf_cons[2];

	struct FIFO32 fifo, keycmd;
	int fifobuf[128], keycmd_buf[32], *cons_fifo[2];
	/*
		公共缓冲区:
			0 ~ 1	-	光标闪烁用定时器
			3		-	3秒定时器
			10		-	10秒定时器
			256~511	-	键盘输入(从键盘控制器读入的值再加上256)
			512~767	-	鼠标输入(从键盘控制器读入的值再加上512)
	*/

	static char keytable0[0x80] = {
		0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0, 0, ']', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0x5c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5c, 0, 0};
	static char keytable1[0x80] = {
		0, 0, '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0x08, 0,
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0x0a, 0, 'A', 'S',
		'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0, 0, '}', 'Z', 'X', 'C', 'V',
		'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
		'2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, '_', 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0};

	/* 多任务处理定义变量 */
	struct TASK *task_a, *task_cons[2], *task;
	/* 多任务处理定义变量结束 */
	int key_to = 0, key_shift = 0, key_leds = (binfo->leds >> 4) & 7,
		/* -1: 空闲状态, 可以发送指令; 不为-1: 正在发送等待指令 */ keycmd_wait = -1;
	struct CONSOLE *cons;
	/* 鼠标选中窗口 */
	int j, x, y, mmx = -1, mmy = -1;
	struct SHEET *sht = 0, *key_win;

	/* 初始化中断功能和硬件 */
	init_gdtidt();
	init_pic();
	io_sti(); // 将CPU的中断标志位设置为1, 即允许中断, 因为CPU只有一个中断信号脚
	fifo32_init(&fifo, 128, fifobuf, 0);
	init_pit();
	init_keyboard(&fifo, 256);
	enable_mouse(&fifo, 512, &mdec);
	io_out8(PIC0_IMR, 0xf8); // PIT和PIC1和键盘设置为许可(11111000)
	io_out8(PIC1_IMR, 0xef); // 鼠标设置为许可(11101111)
	fifo32_init(&keycmd, 32, keycmd_buf, 0);

	/* 初始化内存管理 */
	memtotal = memtest(0x00400000, 0xbfffffff);
	memman_init(memman);
	memman_free(memman, 0x00001000, 0x0009e000); /* 0x00001000 - 0x0009efff */
	memman_free(memman, 0x00400000, memtotal - 0x00400000);

	/* 初始化调色板 */
	init_palette();
	shtctl = shtctl_init(memman, binfo->vram, binfo->scrnx, binfo->scrny);
	task_a = task_init(memman);
	fifo.task = task_a;
	task_run(task_a, 1, 2);
	*((int *)0x0fe4) = (int)shtctl;

	/* sht_back */
	sht_back = sheet_alloc(shtctl);
	buf_back = (unsigned char *)memman_alloc_4k(memman, binfo->scrnx * binfo->scrny);
	sheet_setbuf(sht_back, buf_back, binfo->scrnx, binfo->scrny, -1); /* 不透明 */
	init_screen8(buf_back, binfo->scrnx, binfo->scrny);

	/* sht_cons */
	for (i = 0; i < 2; i++)
	{
		sht_cons[i] = sheet_alloc(shtctl);
		buf_cons[i] = (unsigned char *)memman_alloc_4k(memman, 256 * 165);
		sheet_setbuf(sht_cons[i], buf_cons[i], 256, 165, -1); /* 不透明 */
		make_window8(buf_cons[i], 256, 165, "console", 0);
		make_textbox8(sht_cons[i], 8, 28, 240, 128, COL8_000000);
		task_cons[i] = task_alloc();
		task_cons[i]->tss.esp = memman_alloc_4k(memman, 64 * 1024) + 64 * 1024 - 12;
		task_cons[i]->tss.eip = (int)&console_task;
		task_cons[i]->tss.es = 1 * 8;
		task_cons[i]->tss.cs = 2 * 8;
		task_cons[i]->tss.ss = 1 * 8;
		task_cons[i]->tss.ds = 1 * 8;
		task_cons[i]->tss.fs = 1 * 8;
		task_cons[i]->tss.gs = 1 * 8;
		*((int *)(task_cons[i]->tss.esp + 4)) = (int)sht_cons[i];
		*((int *)(task_cons[i]->tss.esp + 8)) = memtotal;
		task_run(task_cons[i], 2, 2); /* level=2, priority=2 */
		sht_cons[i]->task = task_cons[i];
		sht_cons[i]->flags |= 0x20; // 0x20 有光标
		cons_fifo[i] = (int *)memman_alloc_4k(memman, 128 * 4);
		fifo32_init(&task_cons[i]->fifo, 128, cons_fifo[i], task_cons[i]);
	}

	/* sht_mouse */
	sht_mouse = sheet_alloc(shtctl);
	sheet_setbuf(sht_mouse, buf_mouse, 16, 16, 99);
	init_mouse_cursor8(buf_mouse, 99);
	mx = (binfo->scrnx - 16) / 2; /* 计算中央坐标 */
	my = (binfo->scrny - 28 - 16) / 2;

	sheet_slide(sht_back, 0, 0);
	sheet_slide(sht_cons[1], 56, 6);
	sheet_slide(sht_cons[0], 8, 2);
	sheet_slide(sht_mouse, mx, my);
	sheet_updown(sht_back, 0);
	sheet_updown(sht_cons[1], 1);
	sheet_updown(sht_cons[0], 2);
	sheet_updown(sht_mouse, 3);

	key_win = sht_cons[0];
	keywin_on(key_win);

	fifo32_put(&keycmd, KEYCMD_LED);
	fifo32_put(&keycmd, key_leds);
	for (;;)
	{
		if (fifo32_status(&keycmd) > 0 && keycmd_wait < 0)
		{
			/* 如果存在向键盘控制器发的数据, 则发送它 */
			keycmd_wait = fifo32_get(&keycmd);
			wait_KBC_sendready();
			io_out8(PORT_KEYDAT, keycmd_wait);
		}
		io_cli();
		if (fifo32_status(&fifo) == 0)
		{
			task_sleep(task_a);
			io_sti();
		}
		else
		{
			i = fifo32_get(&fifo);
			io_sti();

			if (key_win->flags == 0)
			{
				//输入窗口被关闭
				key_win = shtctl->sheets[shtctl->top - 1];
				keywin_on(key_win);
			}
			if (256 <= i && i <= 511) // 键盘数据
			{
				if (i < 0x80 + 256)
				{ /* 按键盘编码转换为字符编码 */
					if (key_shift == 0)
					{
						s[0] = keytable0[i - 256];
					}
					else
					{
						s[0] = keytable1[i - 256];
					}
				}
				else
				{
					s[0] = 0;
				}
				if ('A' <= s[0] && s[0] <= 'Z')
				{ /* 当输入字符为英文字母时 */
					if (((key_leds & 4) == 0 && key_shift == 0) ||
						((key_leds & 4) != 0 && key_shift != 0))
					{
						s[0] += 0x20; /* 将大写字母转换为小写字母 */
					}
				}
				if (s[0] != 0)
				{ /* 一般字符 */
					fifo32_put(&key_win->task->fifo, s[0] + 256);
				}
				if (i == 256 + 0x0e)
				{ // 退格键
					wait_KBC_sendready();
					io_out8(PORT_KEYDAT, keycmd_wait);
				}
				if (i == 256 + 0x0f)
				{ /* Tab */
					keywin_off(key_win);
					j = key_win->height - 1;
					if (j == 0)
					{
						j = shtctl->top - 1;
					}
					key_win = shtctl->sheets[j];
					keywin_on(key_win);
				}
				if (i == 256 + 0x2a)
				{ /* 左Shift ON */
					key_shift |= 1;
				}
				if (i == 256 + 0x36)
				{ /* 右Shift ON */
					key_shift |= 2;
				}
				if (i == 256 + 0xaa)
				{ /* 左Shift OFF */
					key_shift &= ~1;
				}
				if (i == 256 + 0xb6)
				{ /* 右Shift OFF */
					key_shift &= ~2;
				}
				if (i == 256 + 0x3a)
				{ /* CapsLock */
					key_leds ^= 4;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x45)
				{ /* NumLock */
					key_leds ^= 2;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x46)
				{ /* ScrollLock */
					key_leds ^= 1;
					fifo32_put(&keycmd, KEYCMD_LED);
					fifo32_put(&keycmd, key_leds);
				}
				if (i == 256 + 0x3b && key_shift != 0)
				{
					task = key_win->task;
					if (task != 0 && task->tss.ss0 != 0)
					{ /* Shift+F1 */
						cons_putstr0(task->cons, "\nBreak(key) :\n");
						io_cli(); /* 不能在改变寄存器值时切换到其他任务 */
						task->tss.eax = (int)&(task->tss.esp0);
						task->tss.eip = (int)asm_end_app;
						io_sti();
					}
				}
				if (i == 256 + 0x57 && shtctl->top > 2)
				{
					// F11 将最下面的窗口放到最上面
					sheet_updown(shtctl->sheets[1], shtctl->top - 1);
					// shtctl->sheets[1]是倒数第二个图层,倒数第一个图层是背景
					//shtctl->top - 1 第二个图层位置,第一个图层是刷新鼠标
				}
				if (i == 256 + 0xfa)
				{ /* 键盘成功接收到数据 */
					keycmd_wait = -1;
				}
				if (i == 256 + 0xfe)
				{ /* 键盘没有成功接收到数据 */
					wait_KBC_sendready();
					io_out8(PORT_KEYDAT, keycmd_wait);
				}
			}
			else if (512 <= i && i <= 767) // 鼠标数据
			{

				if (mouse_decode(&mdec, i - 512) != 0) // 鼠标的三个字节都齐了, 显示出来
				{
					/* 鼠标指针移动 */
					mx += mdec.x;
					my += mdec.y;
					if (mx < 0)
					{
						mx = 0;
					}
					if (my < 0)
					{
						my = 0;
					}
					if (mx > binfo->scrnx - 1)
					{
						mx = binfo->scrnx - 1;
					}
					if (my > binfo->scrny - 1)
					{
						my = binfo->scrny - 1;
					}
					sheet_slide(sht_mouse, mx, my); /* 刷新图层 */
					if ((mdec.btn & 0x01) != 0)
					{
						/* 按下左键 */
						if (mmx < 0)
						{
							// 如果处于通常模式
							// 按照从上到下的顺序寻找鼠标所指向的图层
							for (j = shtctl->top - 1; j > 0; j--)
							{
								sht = shtctl->sheets[j];
								x = mx - sht->vx0;
								y = my - sht->vy0;
								if (0 <= x && x < sht->bxsize && 0 <= y && y < sht->bysize)
								{
									// 判断点击区域是否在图层
									if (sht->buf[y * sht->bxsize + x] != sht->col_inv)
									{
										sheet_updown(sht, shtctl->top - 1);
										if (sht != key_win)
										{
											keywin_off(key_win);
											key_win = sht;
											keywin_on(key_win);
										}
										// 判断点击区域是否在标题栏
										if (3 <= x && x < sht->bxsize - 3 && 3 <= y && y < 21)
										{
											// 进入窗口移动模式
											mmx = mx;
											mmy = my;
										}
										// 判断是否点击了关闭图标
										if (sht->bxsize - 21 <= x && x < sht->bxsize - 5 && 5 <= y && y < 19)
										{
											// 该窗口是否为应用程序窗口
											if ((sht->flags & 0x10) != 0)
											{
												task = sht->task;
												cons_putstr0(cons, "\nBreak(mouse):\n");
												// 开始强制结束任务
												io_cli();
												task->tss.eax = (int)&(task->tss.esp0);
												task->tss.eip = (int)asm_end_app;
												io_sti();
											}
										}
										break;
									}
								}
							}
						}
						else
						{
							// 如果处于窗口移动模式
							x = mx - mmx;
							y = my - mmy;
							sheet_slide(sht, sht->vx0 + x, sht->vy0 + y);
							// 更新为移动后的坐标
							mmx = mx;
							mmy = my;
						}
					}
					else
					{
						// 如果没有按下左键, 回到普通模式
						mmx = -1;
					}
				}
			}
		}
	}
}

void keywin_off(struct SHEET *key_win)
{
	change_wtitle8(key_win, 0);
	if ((key_win->flags & 0x20) != 0)
	{
		fifo32_put(&key_win->task->fifo, 3); // 命令行窗口光标OFF
	}
}

void keywin_on(struct SHEET *key_win)
{
	change_wtitle8(key_win, 1);
	if ((key_win->flags & 0x20) != 0)
	{
		fifo32_put(&key_win->task->fifo, 2); // 命令行窗口光标ON
	}
}