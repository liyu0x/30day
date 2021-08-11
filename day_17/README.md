# 第十七天的内容

今天主要完成了:

1. 闲置任务
2. 创建命令行窗口
3. 切换字符输入
4. 大小写字母
5. 锁定键的支持



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

1. 为了保证多任务切换没有问题， 在LEVEL10中加入了`io_hlt();`代码， 这样即使上层所有任务不执行，也不会让程序跳转到错误的地方。
2. 通过向`binfo`中的`leds`查询功能键状态：
   1. `binfo->leds`第4位: `ScrollLock`状态
   2. `binfo->leds`第5位: `NumLock`状态
   3. `binfo->leds`第6位: `CapsLock`状态
3. 功能键编码：
   1. `CapsLock`: `0x3a`
   2. `NumLock`:`0x45`
   3. `ScrollLock`: `0x46`
4. 关于`NumLock`和`CapsLock`等LED的控制是如下流程:
   1. 读取状态寄存器， 等待`bit1`的值变为`0`
   2. 向数据输出`0060`写入要发送的1个字节数据
   3. 等待键盘返回1个字节的信息，这和等待键盘输入所采用的方法相同：
      1. 用IRQ等待
      2. 轮询状态寄存器的`bit 1`的值直到其变为0
   4. 返回的信息：
      1. 如果是`0xfa`， 则表明1个字节的数据已经发给键盘
      2. 如果是`0xfe`，则表明发给送失败，需要重新发送
   5. 控制LED需要按照上述方法操作两次：
      1. 向键盘发送`EDxx`的数据，`xx`表示
         1. `xx`的`bit 0`：`Scroll-Lock`
         2. `xx`的`bit 1`：`NumLock`
         3. `xx`的`bit 2`：`CapsLock`(0表示熄灭， 1表示点亮)
         4. `xx`的`bit 3~7`：保留位

关于我的环境


|Term|Value|
|-|-|
|Device name|DESKTOP-LT12D0F|
|Processor|Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz|
|Installed RAM|32.0 GB (31.9 GB usable)|
|Device ID|4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC|
|Product ID|00330-80000-00000-AA701|
|System type|64-bit operating system, x64-based processor|
|Pen and touch|No pen or touch input is available for this display|