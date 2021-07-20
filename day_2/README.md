# 第一天的内容

主要是跟随书上写了下一个简易的操作系统，非常非常简单，它就输出了`"Hello, World"` . 镜像使用`qemu`虚拟机来运行.



## 文件解释

1. `asm.bat`: 调用根目录tools文件夹下的汇编编译工具将我们的`helloos.nas` 编译为 `helloos.img.`
2. `run.bat`: 调用根目录下的`qemu`虚拟机去运行我们的`helloos.img`.
3. `install.bat`: 将镜像写入`a:`分区，也就是软盘分区，现代PC不需要.
4. `helloos.nas`: 操作系统的汇编代码.



## 使用方式(CMD)

1. 在此目录下打开`windows cmd`终端
2. 运行`asm`命令
3. 运行`run`命令
4. 操作系统开始运行



如果使用`Powershell`, 命令不同



## 关于我的环境

Device name	DESKTOP-LT12D0F
Processor	Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz   2.59 GHz
Installed RAM	32.0 GB (31.9 GB usable)
Device ID	4CE923CE-7F29-42D9-BA25-DA99CFCA7ABC
Product ID	00330-80000-00000-AA701
System type	64-bit operating system, x64-based processor
Pen and touch	No pen or touch input is available for this display