# 第二天的内容

"C语言与画面显示的练习"



主要讲了C语言的指针

和怎么向显卡RAM中写入数据让显示器显示内容







## 文件解释

1. `asmhead.nas`: 未知,书上没有讲解
2. `bootpack.c`: 一段简单的C语言代码
3. `haribote.nas`: 启动时,启动区加载的程序,在`0x004200`以后的地方
4. `ipl.nas`: 启动区代码
8. `make.bat`: `makefile`程序
6. `naskfunc.nas`: 汇编定义了一个C语言函数



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
3. 运行`make run`命令







## 一些知识点

1. `naskfun.nas`中定义的C语言方法, 参数地址是[ESP+n],  n代表数据类型大小, 依次往后取出
2. 在这个上下文中, 与C语言联合使用的时候,不是所有寄存器都能用, 这里能用的只有`EAX`, `ECX`, `EDX` 3个
3. `或`运算让指定位变为1, 比如 `0100 OR 0010 -> 0110`.
4. `与`运算让指定位变成0, 比如 `0100 AND 1101 -> 0100`
5. `异或`运算让指定位反转, 比如 `0100 XOR 0010 -> 0110`
6. 操作寄存器的时候必须指定数据类型, 除非另一方也是寄存器
7. C语言中的地址变量是4字节, 汇编中地址也是4字节
8. `char *p`, `p`不是指针, 而是地址变量
9. `p[i]`与`*(p+i)` 意思完全相同.
10. 关于"video DA converter"(调色板)的使用说明:
    1. 首先先在一连串的访问中屏蔽中断( 比如CLI )
    2. 将想要设定的调色板号码写入`0x03c8`, 紧接着, 按RGB的顺序写入`0x03c9`. 如果还想继续设定下一个调色板, 则省略调色板号码, 再按照RGB的顺序写入`0x03c9`就行了.
    3. 如果想要读出当前调色板的状态, 首先要将调色板的号码写入`0x03c7`, 再从`0x03c9`读取3次. 读出顺序就是RGB, 如果要继续读出下一个调色板,同样也是省略调色板号码的设定,按RGB的顺序执行.
    4. 如果最初执行了`CLI`, 那么最后要执行`STI`
11. 按照C语言规约, 执行RET语句时, EAX中的值就被看作是函数的返回值, 所以这样就可以.



## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display