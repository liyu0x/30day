#include "bootpack.h"
#include <stdio.h>

void HariMain(void)
{

	struct BOOTINFO *binfo = (struct BOOTINFO *)ADR_BOOTINFO;
	char s[40], mcursor[256];
	int mx, my;

	init_pic();
	init_gdtidt();
	io_sti(); // 将CPU的中断标志位设置为1, 即允许中断, 因为CPU只有一个中断信号脚
	init_palette();
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

	for (;;)
	{
		io_hlt();
	}
}
