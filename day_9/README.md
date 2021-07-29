# 第九天的内容

今天讲的是内存的管理





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



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令

## 一些知识点

3. 这个系统的内存分布图:
   1. 0x00 - 0x000fffff: 在启动时会用到,之后变空(1MB)
   2. 0x00100000-0x00267fff: 用于保存软盘的内容(1440kb)
   3. 0x00268000-0x0026f7ff: 空(30kb)
   4. 0x0026f800-0x0026ffff: IDT(2KB)
   5. 0x00270000-0x0027ffff: GDT(64KB)
   6. 0x00280000-0x002fffff: bootpack.hrb(512kb)
   7. 0x00300000-0x003fffff: 栈及其他(1MB)
   8. 0x00400000-                 : 空
2. 文中检查内存容量时需要注意:
   1. 通过向某一地址写入数据, 然后进行两次反转操作,如果反转成功则代表地址可用,如果不成功则代表地址不可用
   2. 上面的操作只能通过汇编来实现, 因为多数C语言编译器在编译时会优化掉部分代码导致不可用
3. 文中假设了2种内存管理模型, 最后采用了第二种
   1. 第一种: 简单,但是耗内存: 将内存分为4kb一个小块, 然后创建`内存大小/4096/8`位的区域,每一个位可以写入0和1来标记是否已经使用.
   2. 第二种: 复杂,节省内存: 通过维护一个列表,列表中每个实体保存了开始地址和可用大小, 但困难在于当内存被使用者释放时,需要尽可能的将它合并到列表中已存在的记录而不是重新写一个新的记录,这样可能导致没有大的连续空间可以使用.



## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display