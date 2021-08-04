#include "bootpack.h"

struct FIFO32 *mousefifo;
int mousedata0;

void inthandler2c(int *esp)
/* PS/2鼠标中断处理 */
{
    int data;
    io_out8(PIC1_OCW2, 0x64); /* 通知PIC"IRQ-12已经受理完毕" */
    io_out8(PIC0_OCW2, 0x62); /* 通知PIC"IRQ-02已经受理完毕" 需要通知主PIC的2号进行处理*/
    data = io_in8(PORT_KEYDAT);
    fifo32_put(mousefifo, data + mousedata0);
    return;
}

#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4

void enable_mouse(struct FIFO32 *fifo, int data0, struct MOUSE_DEC *mdec)
{
    /* 将FIFO缓冲区的信息保存在全局变量里 */
    mousefifo = fifo;
    mousedata0 = data0;
    /* 鼠标有效 */
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