#include "bootpack.h"

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040

struct TIMERCTL timerctl;

#define TIMER_FLAGS_LEISURE 0 /* 空闲 */
#define TIMER_FLAGS_ALLOC 1   /* 已配置状态 */
#define TIMER_FLAGS_USING 2   /* 定时器运行中 */

void init_pit(void)
{
    int i;
    struct TIMER *t;
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    timerctl.count = 0;
    for (i = 0; i < MAX_TIMER; i++)
    {
        timerctl.timers0[i].flags = TIMER_FLAGS_LEISURE;
    }
    t = timer_alloc();
    t->timeout = 0xffffffff;
    t->flags = TIMER_FLAGS_USING;
    t->next = 0;
    timerctl.t0 = t;
    timerctl.next = 0xffffffff;
    return;
}

struct TIMER *timer_alloc(void)
// 分配一个空闲的计时器
{
    int i;
    for (i = 0; i < MAX_TIMER; i++)
    {
        if (timerctl.timers0[i].flags == TIMER_FLAGS_LEISURE)
        {
            timerctl.timers0[i].flags = TIMER_FLAGS_ALLOC;
            timerctl.timers0[i].flags = 0;
            return &timerctl.timers0[i];
        }
    }
    return 0; /* 没找到 */
}

void timer_free(struct TIMER *timer)
{
    timer->flags = TIMER_FLAGS_LEISURE;
    return;
}

void timer_init(struct TIMER *timer, struct FIFO32 *fifo, int data)
{
    timer->fifo = fifo;
    timer->data = data;
    return;
}

void timer_settime(struct TIMER *timer, unsigned int timeout)
// timeout 单位 ms
{
    int e;
    struct TIMER *t, *s;
    timer->timeout = timeout + timerctl.count;
    timer->flags = TIMER_FLAGS_USING;
    e = io_load_eflags();
    io_cli();
    t = timerctl.t0;
    if (timer->timeout <= t->timeout)
    {
        /* 插入最前面的情况 */
        timerctl.t0 = timer;
        timer->next = t; /* 下面是设定t */
        timerctl.next = timer->timeout;
        io_store_eflags(e);
        return;
    }
    /* 搜寻插入位置 */
    for (;;)
    {
        s = t;
        t = t->next;
        if (timer->timeout <= t->timeout)
        {
            /* 插入s和t之间的情况 */
            s->next = timer; /* s下一个是timer */
            timer->next = t; /* timer的下一个是t */
            io_store_eflags(e);
            return;
        }
    }
}

void inthandler20(int *esp)
{

    struct TIMER *timer;
    char ts = 0;
    io_out8(PIC0_OCW2, 0x60); /* 把IRQ-00信号接收完了的信息通知给PIC */
    timerctl.count++;
    if (timerctl.next > timerctl.count)
    {
        return;
    }
    timer = timerctl.t0; /* 指向链表的第一个节点 */
    for (;;)
    {
        /* 因为timers的定时器都处于动作中, 所以不确认flags */
        if (timer->timeout > timerctl.count)
        {
            break;
        }
        /* 超时 */
        timer->flags = TIMER_FLAGS_ALLOC;
        if (timer != task_timer)
        {
            fifo32_put(timer->fifo, timer->data);
        }
        else
        {
            ts = 1; /* task_timer超时 */
        }
        timer = timer->next; /* 将下一定时器的地址带入timer */
    }
    timerctl.t0 = timer;
    timerctl.next = timer->timeout;
    if (ts != 0)
    {
        task_switch();
    }
    return;
}

int timer_cancel(struct TIMER *timer)
{
    int e;
    struct TIMER *t;
    e = io_load_eflags();
    io_cli();
    if (timer->flags == TIMER_FLAGS_USING)
    {
        if (timer == timerctl.t0)
        {
            // 取消第一个定时器
            t = timer->next;
            timerctl.t0 = t;
            timerctl.next = t->timeout;
        }
        else
        {
            t = timerctl.t0;
            // 找到timer的前一个定时器
            for (;;)
            {
                if (t->next == timer)
                {
                    break;
                }
                t = t->next;
            }
            t->next = timer->next;
        }
        timer->flags = TIMER_FLAGS_ALLOC;
        io_store_eflags(e);
        return 1;
    }
    io_store_eflags(e);
    return 0;
}

void timer_cancelall(struct FIFO32 *fifo)
{
    int e, i;
    struct TIMER *t;
    e = io_load_eflags();
    io_cli();
    for (i = 0; i < MAX_TIMER; i++)
    {
        t = &timerctl.timers0[i];
        if (t->flags != 0 && t->flags2 != 0 && t->fifo == fifo)
        {
            timer_cancel(t);
            timer_free(t);
        }
    }
    io_store_eflags(e);
    return;
}
