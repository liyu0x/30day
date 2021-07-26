# 第六天的内容

"分割编译与中断处理"

这一章节主要做了:

1. 分割源文件, 减少代码行数
2. 初始化PIC(中断控制器)
3. 制作中断程序





## 文件解释

1. `asmhead.nas`: 许多必要的汇编代码
2. `bootpack.c`: 主函数
3. `bootpack.h`: 头文件
4. `dsctbl.c`: 中断表等初始化
5. `grahic.c`: 图形处理函数
6. `hankaku.txt`: 字库文件
7. `int.c`: 鼠标键盘中断初始化
8. `haribote.nas`: 启动时,启动区加载的程序,在`0x004200`以后的地方
9. `ipl.nas`: 启动区代码
10. `make.bat`: `makefile`程序
11. `naskfunc.nas`: 汇编定义了系统所需要的简单的汇编函数



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1. 仅有函数声明和`#define`等组成的文件，称之为头文件. 比如`bootpack.h`。
2. `#include`语句的两种符号：
   1. 引号: 优先搜索用户路径下的文件，如果找不到就搜索编译器的系统路径下。
   2. 方括号：直接搜索编译器的配置路径下的文件。
3. `GDTR`寄存器：
   1. 一个特别的48位寄存器， 并不能用`MOV`指令来赋值，赋值时需要指定一个内存地址， 从指定地址读取6字节，然后赋值给`GDTR`寄存器，完成一任务的指令就是`LGDT`.
   2. 该寄存器的低16位（即内存的最初的2个字节）是段上限， 它等于“GDT的有效字节数-1”， 段上线指的是表示量的大小，一般为“字节数-1”
   3. 剩下的高32位（即剩余的4字节），代表GDT的开始地址。
4. 段上限：
   1. 段上限最大是4GB，也就是32位的数值，这个数值本身要占用4字节，在加上基址，一共就要8字节，就占满了整个结构体(SEGMENT_DESCRIPTOR)。
   2. 因此段上限只能使用20位，及1MB。 但段属性中有一个标志位叫做`Gbit`, 这个标志位是1的时候，limit的单位不解释称字节(byte)，而是页(page). 在电脑的CPU里面， 1页指的4KB.
   3. 20位的段上限分别写入`limit_low`和`limit_high`里, 看起来有24位,但实际上`limit_high`的上4位里面写入了段属性.
5. 段属性:
   1. 段属性12位
   2. 段属性的高4位放在`limit_high`的高4位里
6. `PIC`(programmable interrupt controller) 可编程中断控制器
   1. 要使用中断, 必须正确初始化`GDT`和`IDT`(见昨天)
   2. PIC是将8个中断信号集合成一个中断信号的装置. 因为CPU只有一个中断信号管脚. 但不同CPU不一定有一个PIC, 文中有两个,主从
   3. 主PIC直接与CPU连接, 负责处理0-7 IRQ(中断信号), 从PIC负责处理8-15 IRQ,  从PIC与主PIC的2号 IRQ相连
   4. PIC的寄存器都是8位,
      1. IRM(interrupt mask register), 中断屏蔽寄存器: 如果某一位是1,代表该位所对应的IRQ信号被屏蔽, 因为随意中断可能导致系统异常
      2. ICW(initial control word), 初始化控制数据, 一共有4个:
         1. ICW 1 和ICW 4与PIC主板配线方式, 中断信号的电气特性等有关
         2. ICW3 关于主从连接设定
         3. ICW2 决定以哪一号中断通知CPU
   5. PIC通过想CPU发送数据`0xcd 0x??`来让CPU执行中断, `oxcd`就是`INT`指令. CPU无法分别是数据还是指令,所以就按照指令来执行
   6. `INT 0x20~0x2f`接受指令, 之所以加上`0x20`,是因为`0x00~0x1f`用于系统保护通知.
   7. 栈
      1. 缓冲区一般采用"队列"和"栈"的形式
      2. `PUSH EAX`指令相当于`ADD ESP,-4`, `MOV [SS:ESP], EAX`
      3. `POP EAX` 指令相当于 `MOV EAX,[SS:ESP]`, `ADD ESP, 4` 
      4. `PUSH`指令通常用来中断时备份当前寄存器的数据, `POP`则可以将数据恢复, 无论之前被怎么样修改





## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display