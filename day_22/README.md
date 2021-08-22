# 第二十二天的内容

今天主要完成了:

1. 应用程序错误信息打印
2. `Shift+F1`强制结束应用程序
3. 字符打印API
4. 窗口API

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
21. `winhelo.c`: 可执行的程序, 创建一个窗口
22. `winhelo2.c`: 可执行的程序, 创建一个带有字符串和颜色的窗口



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1.  异常使用的中断:
    1.  `0x0c`: 栈异常中断
    2.  `0x00`: 除0异常中断
    3.  `0x06`: 非法指令中断
2.  因为`0x00`到`0x1f`都是异常使用的中断, 因此, `IQR`的中断号都是从`0x20`之后开始的
3.  CPU产生异常的目的主要是为了保护操作系统, 而不是去发现程序的BUG
4.  `.hrb`(类似windows的`.exe`)由两部分组成:
    1.  代码部分
    2.  数据部分(如果程序中没有使用字符串和外部变量(即在函数外面定义的变量)时, 就不会生成含有数据部分的`.hrb`文件)
5.  `.hrb`的结构:
    1.  `0x0000`(DWORD): 数据段大小, 在这个系统中, 数据段大小固定为64kb,
    2.  `0x0004`(DWORD): 存放`Hari`这4个字节, 相当于魔法值, 用来识别是否为该系统使用的`.hrb`文件
    3.  `0x0008`(DWORD): 数据段内预备空间大小, 目前没有, 值为0
    4.  `0x000c`(DWORD): 程序启动时ESP寄存器的初始值, 即这个地址之前的部分会被作为栈来使用
    5.  `0x0010`(DWORD): 存放的是需要向数据段传送的部分的总计字节数
    6.  `0x0014`(DWORD): 需要向数据段传送的部分在`.hrb`文件中的起始地址
    7.  `0x0018`(DWORD): 存放的是`0xe9000000`这个数值, 在内存中存放的形式为`00 00 00 E9`, `E9`加上后面四个字节, 就表示`JMP`到应用程序的入口地址
    8.  `0x001c`(DWORD): 存放应用程序入口地址减去`0x20`后的值.  为什么要减去`0x20`, 因为在`0x0018`(实际是`0x001b`)中写了一个JMP指令, 这样可以通过JMP指令跳转到应用程序入口. 通过这样处理, 只要先JMP到`0x001b`这个地址, 程序就可以开始运行了
    9.  `0x0020`(DWORD): 存放的是将来编写应用程序用malloc函数时要使用的地址.



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