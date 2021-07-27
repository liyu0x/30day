//初始化PIC
#include "bootpack.h"

void init_pic(void)
{
	io_out8(PIC0_IMR, 0xff); /* 禁止所有中断 */
	io_out8(PIC1_IMR, 0xff); /* 禁止所有中断 */

	io_out8(PIC0_ICW1, 0x11);	/* 边沿模式触发(edge trigger mode) */
	io_out8(PIC0_ICW2, 0x20);	/* IRQ 0-7 由 INT20-27接收 */
	io_out8(PIC0_ICW3, 1 << 2); /* PIC1 由 IRQ 2连接 */
	io_out8(PIC0_ICW4, 0x01);	/* 无缓冲区模式 */

	io_out8(PIC1_ICW1, 0x11); /* 边沿模式触发(edge trigger mode) */
	io_out8(PIC1_ICW2, 0x28); /* IRQ 8-15 由 INT28-2f接收 */
	io_out8(PIC1_ICW3, 2);	  /* PIC1 由 IRQ 2连接 */
	io_out8(PIC1_ICW4, 0x01); /* 无缓冲区模式 */

	io_out8(PIC0_IMR, 0xfb); /* 11111011 PIC1以外全部禁止 */
	io_out8(PIC1_IMR, 0xff); /* 11111111 禁止所有中断 */

	return;
}

#define PORT_KEYDAT 0x0060 // 对应键盘

struct FIFO8 keyfifo;

void inthandler21(int *esp)
/* PS/2键盘中断处理 */
{
	unsigned char data;
	io_out8(PIC0_OCW2, 0x61); // 通知PIC"IRQ-01已经受理完毕" 0x61代表IRQ 1, 如果是IRQ 3, 则写成 0x63, 如果不发送该信息,PIC将不再监听IRQ1的中断信号
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&keyfifo, data);
	return;
}

struct FIFO8 mousefifo;

void inthandler2c(int *esp)
/* PS/2鼠标中断处理 */
{
	unsigned char data;
	io_out8(PIC1_OCW2, 0x64); /* 通知PIC"IRQ-12已经受理完毕" */
	io_out8(PIC0_OCW2, 0x62); /* 通知PIC"IRQ-02已经受理完毕" 需要通知主PIC的2号进行处理*/
	data = io_in8(PORT_KEYDAT);
	fifo8_put(&mousefifo, data);
}

void inthandler27(int *esp)
{
	io_out8(PIC0_OCW2, 0x67);
	return;
}
