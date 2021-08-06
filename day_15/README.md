# 第十五天的内容

今天主要完成了"多任务管理"的开发, 即操作系统可以通过切换时间片在单核上进行多任务处理(用户角度).







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

1. 在单核的情况下, 多任务通过切换时间片来模拟多任务处理

   1. 切换任务进程的行为本身是需要耗费时间的, 时间主要花费在: 将寄存器中的值存入内存, 待被切换任务执行完毕后, 从内存中读取值放入寄存器

2. 寄存器使用数据结构`TSS(task status segment)`写入内存, 关于`TTS`的说明, 参考`bootpack.c`中`TSS32`结构体的定义, 关于`TSS`的说明:

   1. `TSS`使用之前需要在`GDT`中进行定义.
   2. `TSS`使用到一个特别的寄存器叫做`EIP(extend instruction pointer)`, 也就是"扩展指令指针寄存器". 是一个32位寄存器. `EIP`是用于记录`CPU`下一条需要执行的指令位于内存中哪个地址的寄存器. 每执行一条指令`EIP`就会自动累加, 保证指向正确的地址.

3. `JMP`指令实际上是一个向`EIP`寄存器赋值的指令. `JMP 0x1234` 这种写法,  CPU会解释为`MOV EIP, 0x1234`, 当然, 在汇编中无法直接这样赋值,还是要使用 `JMP`.

4. `JMP`指令分为两种模式:

   1. `near mode`: 只改写`EIP`寄存器
   2. `far mode`: 同时改写`EIP`和`CS`寄存器, 比如`JMP DWORD 2*8:0x0000001b`的含义是: 这条指令在向`EIP`存入`0x1b`的同时, 将`CS`设置为`2*8(=16)`. 像这样在`JMP`目标地址中带有冒号`:`, 就是`far mode`.

5. 如果`JMP`指令所指的目标地址不是可执行的代码, 而是`TSS`的话, `CPU`就不会执行通常的改写`EIP`和`CS`的操作, 而是将这条指令理解为任务切换, 也就是说`CPU`会切换到目标`TSS`所指定的任务,  即`JMP`到一个任务那里去.

   1. `CPU`每次执行带有地址段的指令时, 都会去确认一下`GDT`中的设置, 以便判断要执行的`JMP`指令到底是普通的`far-JMP`还是任务切换, 即从汇编程序翻译出来的机器语言来看, 两者在代码上无差别.
   2. 在使用任务切换的`JMP`指令时, 必须在`JMP`后面使用`RET`命令, 因为正常的情况下, `JMP`直接跳转到其他地址去了, 但用于任务切换的`JMP`, 在被切换任务完成后, 会回到执行`JMP`指令的地方继续向后执行.
   3. 如果使用`far mode`当作切换任务使用, 冒号之前的部分要指向`TSS`, 冒号后面的部分是偏移量, 通常为0.

6. `TR(task register)`寄存器, 它的作用是记住`CPU`当前在运行哪个任务, 给`TR`赋值的时候, 需要把`GDT`的编号乘以8. 赋值指令为:`LTR`

7. `JMP FAR`指令的功能是执行`far`跳转:

   1. 使用时候需要提供一个内存地址, CPU会从这个地址中读取4个字节的数据, 并将其存入EIP寄存器, 再继续读取两个字节, 并将其存入CS寄存器.

8. 关于`task_b_main`不能使用`return`:

   1. 因为`return`相当于`JMP`指令, 但在本章节中, 这个函数`task_b_main`, 并不是由其他程序调用的, 所以无法`JMP`到指定的地方, 当然, 可以向`ESP`中写入一个地址, 这样就可以使用`return`了, 因为返回地址存储在`ESP`寄存器中.

   

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