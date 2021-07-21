# 第二天的内容

"进入32位模式并导入C语言"







## 文件解释

1. `asmhead.nas`: 未知,书上没有讲解
2. `bootpack.c`: 一段简单的C语言代码
3. `haribote.nas`: 启动时,启动区加载的程序,在`0x004200`以后的地方
4. `ipl.nas`: 启动区代码
8. `make.bat`: `makefile`程序
6. `naskfunc.nas`: 汇编定义了一个C语言函数



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make img`命令
3. 运行`make run`命令



## 关于软盘中加入文件

1. 文件名会写在`0x002600`以后的地方
2. 文件的内容会写在`0x004200`以后的地方



## 一些知识点

1. 段寄存器的使用: `ES:BX`这种方式来表示地址,比如`MOV AL,[ES:BX]`, 它代表`[ES*16+BX]`的内存地址.

2. 不管我们指定什么内存地址,都必须使用段寄存器,但 一般省略的话会把`DS:`作为默认的段寄存器,比如 `MOV CX,[1234]`等同于`MOV CX,[DS:1234]`

3. `bootpack.c`是如何变成机器语言的:

   1. 首先,使用`cc1.exe`从`bootpack.c`生成`bootpack.gas`
   2. 第二步, 使用`gas2nask.exe`从`bootpack.gas`生成`bootpack.nas`
   3. 第三步, 使用`nask.exe`从`bootpack.nas`生成`bootpack.obj`
   4. 第四步, 使用`obi2bim.exe`从`bootpack.obj`生成`bootpack.bim`
   5. 第五步, 使用`bim2hrb.exe`从`bootpack.bim`生成`bootpack.hrb`
   6. 最后使用`copy`指令将`asmhead.bin`与`bootpack.hrb`单纯的结合起来就成了`haribote.sys`

   虽然生成了目标文件,但c语言生成的目标文件并非完全的机器语言,需要`链接`相关的文件才可以组成完整的机器语言. `链接`通常会引起`地址重定位`和`符号统一`两个操作.



## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display