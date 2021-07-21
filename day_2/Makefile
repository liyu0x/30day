# 文件生成规则

default :  
	../z_tools/make.exe img

ipl.bin: ipl.nas Makefile # 为了制作ipl.bin, 首先检查是否存在ipl.nas, Makefile两个文件, 如果存在，直接执行下个命令
	../tools/z_tools/nask.exe ipl.nas ipl.bin ipl.lst

helloos.img: ipl.bin Makefile 
	../tools/z_tools/edimg.exe   imgin:../tools/z_tools/fdimg0at.tek   wbinimg src:ipl.bin len:512 from:0 to:0   imgout:helloos.img

img: # 这种是命令，make后面跟随的参数
	../tools/z_tools/make.exe -r helloos.img

asm:
	../tools/z_tools/make.exe -r ipl.bin

run:
	../tools/z_tools/make.exe img
	copy helloos.img ..\tools\z_tools\qemu\fdimage0.bin
	../tools/z_tools/make.exe -C ../tools/z_tools/qemu
