# 第二十天的内容

今天主要完成了:

1. 开发一个API供应用程序调用

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



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1.  `CALL`指令: 
    1.  与`JMP`指令类似, 区别在于`CALL`会先将要返回的目标地址PUSH到栈中. 
    2.  程序对API执行`CALL`的时候, 需要加上段号, 在这里应用程序的所在段`1003*8`, 操作系统的段号为`2*8`, 所以需要使用`far-call`, 这个类似于`far-jmp`, 需要指定段和偏移量
    3.  `far-call`的返回需要用到`FRET`指令而不能使用`RET`
    4.  `far-call`指令需要7个字节而`INT`只需要两个字节
2.  使用`INT`指令调用的时候会被当做中断来处理, 用`RETF`时无法返回的, 需要使用`IRETD`指令.
3.  `PUSHAD`和`POPAD`可以用于备份寄存器内容.



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