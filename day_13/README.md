# 第十三天的内容

今天这一个章节主要是优化中断和定时器的代码. 作者很痴迷减少代码行数.我有点气



~~但是出现了一个很严重的问题:~~

~~鼠标无法顺利移动~~



~~将所有的IO中断缓存区合成一个缓存区, 我认为因为中断太频繁了, 导致鼠标的IO信号在缓冲区里面滞后太严重了. 期间有大量的定时器中断进入.~~

上面的问题是由于从day4开始,我将`_io_in8`函数所使用的寄存器搞错了,导致这个问题,目前已经修复







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

没什么, 可以看看代码, 将定时器集合从数组改为了链表.

## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display