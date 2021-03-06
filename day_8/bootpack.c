#include "bootpack.h"
#include <stdio.h>

struct MOUSE_DEC
{
	unsigned char buf[3] /*鼠标的三个字节*/
		,
		phase /*鼠标的四个阶段*/;
	int x, y, btn;
};
extern struct FIFO8 keyfifo, mousefifo;

void enable_mouse(struct MOUSE_DEC *mdec);
void init_keyboard(void);
int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat);

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256], keybuf[32], mousebuf[128];
	int mx, my, i;
	struct MOUSE_DEC mdec;

	init_gdtidt();
	init_pic();
	io_sti(); // 将CPU的中断标志位设置为1, 即允许中断, 因为CPU只有一个中断信号脚
	fifo8_init(&keyfifo, 32, keybuf);
	fifo8_init(&mousefifo, 128, mousebuf);
	init_palette();
	init_keyboard();
	init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
	// 渲染鼠标
	mx = (binfo->scrnx - 16) / 2;
	my = (binfo->scrny - 28 - 16) / 2;
	init_mouse_cursor8(mcursor, COL8_008484);
	putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
	sprintf(s, "(%d, %d)", mx, my);
	putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);

	io_out8(PIC0_IMR, 0xf9); //接受键盘事件的中断
	io_out8(PIC1_IMR, 0xef); //接受鼠标事件的中断

	enable_mouse(&mdec);

	for (;;)
	{
		io_cli(); // 屏蔽中断指令
		if (fifo8_status(&keyfifo) + fifo8_status(&mousefifo) == 0)
		{
			io_stihlt(); // sti和hlt指令如果连续执行, 则CPU不会受理期间的中断信号
		}
		else
		{
			if (fifo8_status(&keyfifo) != 0)
			{
				i = fifo8_get(&keyfifo);
				io_sti();
				sprintf(s, "%02X", i);
				boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
				putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);
			}
			else if (fifo8_status(&mousefifo) != 0)
			{
				i = fifo8_get(&mousefifo);
				io_sti();
				if (mouse_decode(&mdec, i) != 0)
				{
					// 鼠标的三个字节都齐了, 显示出来

					sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
					if ((mdec.btn & 0x01) != 0)
					{
						s[1] = 'L';
					}
					if ((mdec.btn & 0x02) != 0)
					{
						s[3] = 'R';
					}
					if ((mdec.btn & 0x04) != 0)
					{
						s[2] = 'C';
					}
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
					putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);

					/* 鼠标指针移动 */
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15); /* 隐藏鼠标 */
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
					if (mx > binfo->scrnx - 16)
					{
						mx = binfo->scrnx - 16;
					}
					if (my > binfo->scrny - 16)
					{
						my = binfo->scrny - 16;
					}
					sprintf(s, "(%3d, %3d)", mx, my);
					boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15);		 /* 隐藏坐标 */
					putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);		 /* 显示坐标 */
					putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); /* 描画鼠标 */
				}
			}
		}
	}
}

#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47

void wait_KBC_sendready(void)
{
	// 等待键盘控制电路准备完毕
	for (;;)
	{
		if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
		{
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	/* 初始化键盘控制电路 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void enable_mouse(struct MOUSE_DEC *mdec)
{
	/* 激活鼠标 */
	wait_KBC_sendready();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	wait_KBC_sendready();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	/* 顺利的话， 会返回ACK(0xfa) */
	mdec->phase = 0; // 等待0xfa的阶段
	return;
}

int mouse_decode(struct MOUSE_DEC *mdec, unsigned char dat)
{
	if (mdec->phase == 0)
	{
		//等待鼠标的0xfa的状态
		if (dat == 0xfa)
		{
			mdec->phase = 1;
		}
		return 0;
	}
	if (mdec->phase == 1)
	{
		// 等待鼠标的第一字节
		if ((dat & 0xc8) == 0x08) // 判断点击状态是否在8-F及移动反应是否在0-3, 为了防止电气故障
		{
			// 如果第一字节正确
			mdec->buf[0] = dat; //与鼠标点击有关
			mdec->phase = 2;
		}
		return 0;
	}
	if (mdec->phase == 2)
	{
		// 等待鼠标的第二字节
		mdec->buf[1] = dat; // 与鼠标左右移动有关系
		mdec->phase = 3;
		return 0;
	}
	if (mdec->phase == 3)
	{
		//等待鼠标的第三字节
		mdec->buf[2] = dat; // 与鼠标上下移动有关系
		mdec->phase = 1;
		mdec->btn = mdec->buf[0] & 0x07; //鼠标按键状态在低三位
		mdec->x = mdec->buf[1];
		mdec->y = mdec->buf[2];
		if ((mdec->buf[0] & 0x10) != 0) //第一字节判断是否横移
		{
			mdec->x |= 0xffffff00; //忽略8位以后的
		}
		if ((mdec->buf[0] & 0x20) != 0) //第一字节判断是否纵移
		{
			mdec->y |= 0xffffff00; //忽略8位以后的
		}
		mdec->y = -mdec->y; //鼠标的y方向与画面符号相反
		return 1;
	}
	return -1;
}
