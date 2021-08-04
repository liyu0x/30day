#include "bootpack.h"

void init_gdtidt(void)
{
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)0x00270000;
    struct GATE_DESCRIPTOR *idt = (struct GATE_DESCRIPTOR *)0x0026f800;
    int i;

    /* GDT 初始化 */
    for (i = 0; i <= LIMIT_GDT / 8; i++)
    {
        set_segmdesc(gdt + i, 0, 0, 0); // 虽然是 + n，由于是结构体，实际上是加 n*结构体大小
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, AR_DATA32_RW);   // 段1 大小为4GB
    set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER); // 段2 大小为512KB, 用这个段执行bootpack.hrb
    load_gdtr(LIMIT_GDT, ADR_GDT);                                 // 将 0x270000 ~ 0x27ffff 设定为 GDT

    /* IDT 初始化 */
    for (i = 0; i < 256; i++)
    {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(LIMIT_IDT, ADR_IDT);

    /* IDT的设定 */
    set_gatedesc(idt + 0x20, (int)asm_inthandler20, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x21, (int)asm_inthandler21, 2 * 8, AR_INTGATE32); // 表示将asm_inthandler21注册在idt的第0x21号. 这样,如果发生中断了,CPU就会自动调用asm_inthandler21. 这里的2*8表示asm_inthandler21属于哪一个段, 即段号为2, 乘8是因为低三位必须为0, 也可以写成 2 << 3
    set_gatedesc(idt + 0x27, (int)asm_inthandler27, 2 * 8, AR_INTGATE32);
    set_gatedesc(idt + 0x2c, (int)asm_inthandler2c, 2 * 8, AR_INTGATE32);

    return;
}

void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
    if (limit > 0xfffff)
    {
        ar |= 0x8000; /* G_bit = 1 如果这个标志位为1, 则大小不解释为byte而是page, page = 4kb*/
        limit /= 0x1000;
    }
    sd->limit_low = limit & 0xffff;
    sd->base_low = base & 0xffff;
    sd->base_mid = (base >> 16) & 0xff;
    sd->access_right = ar & 0xff;
    sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->base_high = (base >> 24) & 0xff;
    return;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar)
{
    gd->offset_low = offset & 0xffff;
    gd->selector = selector;
    gd->dw_count = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high = (offset >> 16) & 0xffff;
    return;
}