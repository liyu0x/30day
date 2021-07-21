# 第二天的内容

第一天完成了一个操作系统，第二天是在第一天的工作之上改进的，将字符手动输出到界面上



## 文件解释

1. `asm.bat`: 调用根目录tools文件夹下的汇编编译工具将我们的`helloos.nas` 编译为 `helloos.img.`
2. `run.bat`: 调用根目录下的`qemu`虚拟机去运行我们的`helloos.img`.
3. `install.bat`: 将镜像写入`a:`分区，也就是软盘分区，现代PC不需要.
4. `helloos.nas`: 操作系统的汇编代码，有增加.
5. `ipl.nas`: 启动区汇编代码，一共512KB
6. `asm_ipl.bat`: 编译`ipl.nas`源码，并生成`ipl.bin`文件和`ipl.lst`(指令编译日志)
7. `makeimg.bat`: 将`ipl.bin`编译为镜像
8. `make.bat`: `makefile`程序



## 使用方式(CMD)

1. 在此目录下打开`windows cmd`终端
2. 运行`asm_ipl`命令
3. 运行`makeimg`命令
4. 运行`run`命令
5. 操作系统开始运行



如果使用`Powershell`, 命令不同



## 使用方式(Makefile)

1. 在此目录下打开`windows cmd`终端
2. 运行`make run`命令





## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display