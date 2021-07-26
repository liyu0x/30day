# 第六天的内容

"分割编译与中断处理"

这一章节主要做了:

1. 分割源文件, 减少代码行数
2. 初始化PIC(中断控制器)
3. 制作中断程序





## 文件解释

1. `asmhead.nas`: 未知,书上没有讲解
2. `bootpack.c`: 一段简单的C语言代码
3. `hankaku.txt`: 字库文件
4. `haribote.nas`: 启动时,启动区加载的程序,在`0x004200`以后的地方
5. `ipl.nas`: 启动区代码
6. `make.bat`: `makefile`程序
7. `naskfunc.nas`: 汇编定义了一个C语言函数



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
3. 运行`make run`命令







## 一些知识点

1. 结构体相关:

   1. `struct  BOOTINFO{`
      	`char cyls, leds, vmode,reserve;`
      	`short scrnx, scrny;`
      	`char *vram`

      `};`

   2. 这里定义了一个结构体,访问结构体有如下方法:

      1. 普通结构体的使用:
         `strcut BOOTINFO abc;`
         `abc.scrnx = 320;`
      2. 指针的方式:
         `struct BOOTINFO *binfo;`
         `binfo = (struct BOOTINFO *)0x0ff0; // 给指针一个初始地址`
         `xsize = (*binfo).scrnx;`
         `ysize = binfo -> scrny;`

## GDT和IDT

### GDT

(global (segment) descriptor table) 全局段号记录表, 将这些数据整齐的排列在内存的某个地方, 然后将内存的起始地址和有效设定个数存放在CPU的`GDTR`特殊寄存器中.

### 分段

将内存分为很多块(block), 每一块的起始地址都是看作0来处理, 这样任何程序都能写上ORG 0, 这样的操作称为分段. 当然也可以用分页.

段寄存器就是来做这个用的, `MOV AL,[DS:EBX]` , CPU会往EBX里面加上某个值来计算地址,在16位下, 就是DS中的值乘以16,但32位模式下, DS所表示的是段的起始地址., 即使省略了段寄存器的地址,也会自动指定DS.



按这种分段方法, 为了表示一个段, 需要有以下信息:

1. 段的大小是多少
2. 段的起始地址在哪儿
3. 段的管理属性(禁止写入, 禁止执行, 系统专用等)

CPU会使用8个字节(64 bit)来表示这些信息. 但是用于指定段的寄存器只有16位, 所以需要有一个段号和段的映射关系.

`16`位可以存储`0-65535`, 由于CPU的设计上的原因, 段寄存器的低3位不能使用.因此能使用的只有13位,即 `0~8191`的长度可以使用





### IDT

(interrupt descriptor table) 中断符号表.



### 中断

即IO设备需要CPU处理时给CPU发送指令让CPU放下当前的事情来处理它的事情.





## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display