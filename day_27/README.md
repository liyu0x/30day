# 第二十七天的内容

今天主要完成了:

1. 删除了很多之前的程序, 比如BUG和攻击程序
2. 第一次使用了LDT(Local Descriptor Table)
3. 整理了文件

## 文件解释

无

## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run_full`命令

## 一些知识点

1. 新的攻击方式: 使用GDT的分配方式时, 虽然应用程序无法访问操作系统的段, 但是可以操作其他应用程序的段, 就可以攻击其他应用程序
2. 有两种方式解决上面的问题:
   1. 只将正在运行的那个程序的段设置为应用程序用, 其他的应用程序都暂时设置为操作系统用
   2. 使用CPU的提供的LDT
3. LDT(Local (segment) descriptor table), LDT的特性
   1. LDT的段设置只针对某个应用程序有效, 所以应用程序无法访问其他应用程序的LDT的段
   2. 和GDT一样, LDT的容量也是64kb(可容纳8192个段)
   3. 通过在GDT中创建的LDT段来告知CPU., 可以创建多个LDT, 但是不能同是使用两个以上的LDT
   4. CPU的LLDT指令可以改变LDTR寄存器的值, 不过这个指令是操作系统专用指令,处在应用程序段的程序无法使用.

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