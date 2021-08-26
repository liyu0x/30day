# 第二十五天的内容

今天主要完成了:

1. 使用蜂鸣器(模拟器无法发声)
2. 增加更多色彩
3. 新增一个命令行窗口

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
23. `winhelo3.c`: 可执行的程序, 创建一个带有字符串和颜色的窗口
24. `noodle.c`: 泡面定时器



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

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