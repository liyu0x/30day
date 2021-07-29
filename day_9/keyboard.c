#include "bootpack.h"

#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47

struct FIFO8 keyfifo;

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

void inthandler21(int *esp)
/* PS/2键盘中断处理 */
{
    unsigned char data;
    io_out8(PIC0_OCW2, 0x61); // 通知PIC"IRQ-01已经受理完毕" 0x61代表IRQ 1, 如果是IRQ 3, 则写成 0x63, 如果不发送该信息,PIC将不再监听IRQ1的中断信号
    data = io_in8(PORT_KEYDAT);
    fifo8_put(&keyfifo, data);
    return;
}