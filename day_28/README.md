# 第二十八天的内容

今天主要完成了:

1. 文件操作API(对文件的增删改查)
2. 命令行API(获取命令行的命令参数)
3. 日文显示(JIS编码, 未全部做完)

## 文件解释

无

## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run_full`命令

## 一些知识点

1.  C语言编译器规定, 如果栈中的变量超过4KB, 则需要调用`__alloca`这个函数, 这个函数的主要功能时根据操作系统的规格来获取栈中的空间. 如果栈中变量小于4KB的话,只需要对ESP进行减法运算即可

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