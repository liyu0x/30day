# 第二十一天的内容

今天主要完成了:

1. 保护操作系统

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
16. `mtask.c`: 多任务管理
17. `hello.nas`: 输出`hello`
18. `hello2.nas`: 输出`hello`
19. `crack1.c`:  c语言恶意修改内存
20. `crack2.nas`:  汇编恶意修改内存



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1.  为了保证操作系统正常运行, 需要为应用程序提供专用的内存空间, 并且无法访问其他内存空间.
2.  内存段的分配:
    1.  操作系统用代码段: $2*8$
    2.  操作系统用数据段: $1*8$
    3.  应用程序用代码段: $1003*8$
    4.  应用程序用数据段: $1004* 8$
    5.  TSS所用段: $3*8$~$1002*8$
3.  创建专用段后, 将DS和SS指向该段地址.
4.  `x86`架构在定义段的时候, 如果在访问权限加上`0x60`的话, 会有如下效果:
    1.  如果CS中的段地址为应用程序段地址, CPU会认为"当前正在运行应用程序", 如果这时候存入操作系统用的段地址就会产生异常.
5.  `x86`架构不允许使用`far-jmp`或`far-call`去调用应用程序, 所以可以先将函数地址PUSH到栈中, 然后执行`RETF`指令就可以了.



## 我的运行环境


|Term|Value|
|-|-|
|Device name|DESKTOP-LT12D0F|
|Processor|Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz|
|Installed RAM|32.0 GB (31.9 GB usable)|
|Device ID|4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC|
|Product ID|00330-80000-00000-AA701|
|System type|64-bit operating system, x64-based processor|
|Pen and touch|No pen or touch input is available for this display|