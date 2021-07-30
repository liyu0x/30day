# 第十天的内容

今天讲的是窗口重叠(图层)的处理



其实README.md中写不了多少内容, 我打算在这30多天完了后使用github page开通一个静态博客, 整理下这些天的内容,顺便更新下学习的内容





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



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1. 今天这张主要讲了图层:
   1. 将操作系统的界面按高度分为不同图层,每个图层中包含不同内容, 比如之前鼠标移动在状态栏上会导致状态栏消失
   2. 图层的原理:
      1. 创建256个图层, 图层包含了开始坐标和大小
      2. 鼠标每次移动,小于鼠标高度的所有图层都将被刷新, 刷新区域,文中列举了2个
         1. 刷新整个图层
         2. 刷新图层指定区域
   3. 这样在每次移动鼠标后,鼠标下的图层都被刷新了,就能保持应该有的样子了



## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display