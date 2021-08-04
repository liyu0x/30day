# 第十二天的内容

今天讲的是操作系统的定时器(Timer), 这是一个很重要的东西, 正如其名, 它主要的作用是用来计时.





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
12. `fifo.c`: 栈实现
13. `memory.c`: 内存管理代码
14. `sheet.c`: 图层管理
15. `timer.c`: 定时器



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1. 定时器(Timer)对于操作系统非常重要, 它每隔一段时间会就发送一个中断信号给CPU.
2. CPU想要计时但又没有定时器的话,就只能记住每个指令的时钟周期(Clock). 但"时钟周期"并非固定值, 而是取决于CPU的主频.
3. 计算机要使用定时器, 只需对PIT(Programmable Interval Timer)-可编程的间隔型定时器.
4. PIT:
   1. PIT连着IRQ 0号, 也就是说只要设定了PIT, 相当于设定了IRQ 0号的中断间隔
   2. PIT在旧机型上是单独的芯片安装在主板上, 现在已经和PIC一样集成到其他芯片了
   3. 文中所说的是8254芯片
   4. IRQ-0的中断周期变更:
      1. AL=0x34:OUT(0x43, AL)
      2. AL=中断周期的低8位; OUT(0x40, AL)
      3. AL=中断周期的高8位; OUT(0x40, AL)
      4. 如果中断周期为0, 会被看作是指定为65536. 实际的中断产生频率是单位时间时钟周期数(即主频)/设定的数值. 
5. 超时(timeout): 设定时间, 到期会收到通知

## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display