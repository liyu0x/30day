# 第十八天的内容

今天主要完成了:

1. 窗口激活切换
2. 对回车键的支持
3. 对命令行滚动的这次hi
4. mem命令(查看内存)
5. cls命令(清屏幕)
6. dir命令

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



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

1.  要实现`dir`命令, 就需要读取文件, 可是文件在哪儿呢? 在内存里面, 我们需要清楚这么几个事实:
   1. 系统在启动时, 从磁盘读取了10个柱面的数据(day 3rd)
   2. 读出的数据放在`0x00100000~0x00267fff`(day 8th)
   3. 文件信息在`0x002600`之后, 也就是内存地址的`0x00102600`
   4. 文件信息是32个字节:
      1. `0~7`: 保存了文件名, 但又有一下情况:
         1. 如果第一个字节为`0xe5`: 代表文件已经被删除
         2. 如果第一个字节为`0x00`: 代表不包含任何文件名信息
      2. `8~10`: 扩展名, 和文件一样, 不足3个字节时用空格补足
      3. `11`: 存放文件属性, 一般文件不是`0x00`就是`0x20`, 跟linux命令一样, 这些属性是可以累加的, 其他特殊的值:
         1. `0x01`: 只读文件
         2. `0x02`: 隐藏文件
         3. `0x04`: 系统文件
         4. `0x08`: 非文件信息(比如磁盘名称等)
         5. `0x10`: 目录
      4. `12~21`: 保留字节
      5. `22~23`: `WORD`整数, 存放文件的时间.
      6. `24~25`: `WORD`整数, 代表文件从磁盘上的哪个扇区开始
      7. `26~30`: `DWORD`整数, 存放文件大小.
      8. /todo 至于为什么只有30个字节, 等我搞清楚了再来更新, 因为文件名可能8个字节存不下来,会采用其他的方式来处理
   5. 从磁盘映像的`0x004200`开始存放文件`haribote.sys`, 意味着文件信息最多可以存放224个$(0x004200-0x002600)/(32*8)$

## 关于我的环境


|Term|Value|
|-|-|
|Device name|DESKTOP-LT12D0F|
|Processor|Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz|
|Installed RAM|32.0 GB (31.9 GB usable)|
|Device ID|4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC|
|Product ID|00330-80000-00000-AA701|
|System type|64-bit operating system, x64-based processor|
|Pen and touch|No pen or touch input is available for this display|