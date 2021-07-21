; snail-os
; TAB=4
	
		ORG	0x7c00			; 指明程序的装载地址, 0x7c00是主引导记录的内存地址

; 以下这段是标准FAT12格式软盘的专用代码
		JMP entry
		DB	0x90
		DB	"HELLOIPT"		; 启动区的名称可以是任意的字符串(8字节)
		DW	512				; 每个扇区的大小(必须是512)
		DB	1				; 簇的大小(必须为1个扇区)
		DW	1				; FAT的起始位置(一般从第一个扇区开始)
		DB	2				; FAT的个数(必须为2)
		DW	224				; 根目录的大小(一般设置成224项)
		DW 	2880			; 该磁盘的大小(必须是2880扇区)
		DB	0xf0			; 磁盘的种类(必须是0xf0)
		DW	9				; FAT的长度(必须是9扇区)
		DW	18				; 1个磁道有几个扇区(必须是18)
		DW	2				; 磁头数(必须是2)
		DD	0				; 不使用分区，必须是0
		DD 	2880			; 重写一次磁盘大小
		DB	0,0,0x29		; 意义不明，固定
		DD	0xffffffff		; (可能是) 标卷号码
		DB	"SNAIL-OS   "		; 磁盘的名称(11字节)
		DB	"FAT12   "			; 磁盘格式的名称(8字节)
		RESB	18			; 先空出18字节	

; 程序核心

entry:						; 是一个标签的声明, 用于指定JMP指令跳转的目的地等.
		MOV	AX,0			; 初始化寄存器			
		MOV	SS,AX			
		MOV	SP,0x7c00
		MOV DS,AX
		MOV ES,AX

		MOV SI,msg			; 将msg的位置存入SI，它的位置在这里是0x7c74

putloop:
		MOV AL,[SI]			; 方括号代表内存地址
		ADD SI,1			; 给SI加1
		CMP AL,0			

		JE	fin				; 如果上面的判断相等，跳转到哪儿，否则直接执行下面的指令
		MOV AH,0x0e			; 显示一个文字
		MOV BX,15			; 指定字符颜色、
		INT 0x10			; 调用显卡BIOS
		JMP putloop

fin:
		HLT 				; 让CPU停止，等待指令
		JMP	fin				; 无限循环

msg:
		DB 0x0a, 0x0a		; 2个换行 
		DB "hello, world"
		DB 0x0a				; 换行
		DB " from Snail(Day two)"
		DB 0				

		RESB 0x7dfe-$		; 填写0x00, 直到 0x7dfe

		DB 0x55, 0xaa


; 汇编指令解释：

; DB(data byte): 写入一个字节, 或写入一个字符串(编译器自行寻找字符对应的编码并且转换为对应的指令, 不用自己找字符编码对照表)

; DW(data world): 写入双字节, 等同于16位

; DD(data double-word): 写入四字节，等同于32位

; RESB(reserve byte): 从现在的位置开始空出n个字节并且自动填充0x00(仅限于作者提供的编译器中), 关于line-39出现的带有美元符号的使用方法的含义为: 美元代表当前已输入字节，0x1fe-当前已输入字节得到它需要填充0x00的字节数

; ORG(origin): 指令会告诉nask(编译器), 在开始执行的时候，把这些机器语言指令装载到内存中的哪个地址(装载位置的起始地址)。如果没有它，有几个机器指令无法执行。并且美元符号的含义也发生改变：它不在表示输出文件的第几个字节，而是代表将要读入的内存地址. 这个指令也被称为"伪指令", 因为它不是机器指令.

; JMP(jump): 类似于C的GOTO关键字

; ADD: 加法指令

; CMP: 比较指令

; JE(jump if equal): CMP指令如果返回真，则执行它

; INT(interrupt): 中断指令

; HLT: 让CPU进入待机状态

; MOV(move): 赋值指令, 虽然MOVE是移动的意思，但是与真正的移动不一样，当值被赋给其他的变量时，这个变量本身依然持有这个值，所以叫做“赋值”，关于这个指令，拆开解释:
; 1. MOV AX,0			相当于AX=0
; 2. MOV AX,SS			相当于AX=SS
; 如果MOV 后面有括号，表示内存地址

; 关于上面的AX和SS是什么? 这是寄存器，通常由8个寄存器(均为16位寄存器):
; AX ---- accumulator 累加寄存器
; CX ---- counter 计数寄存器
; DX ---- data 数据寄存器
; BX ---- base 基址寄存器
; SP ---- stack pointer 栈指针寄存器
; BP ---- base pointer 基址指针寄存器
; SI ---- source index 源址寄存器
; DI ---- destination index 目的变址寄存器
; 另外，上面的某些寄存器被分为:
; AL ---- accumulator low 累加寄存器低位
; CL ---- counter low
; DL ---- data low
; BL ---- base low
; AH ---- accumulator high 累加寄存器高位
; CH ---- counter high
; DH ---- data high
; BH ---- base high
; 32位寄存器:
; EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI, 在16位寄存器前面加个E就行了
; 段寄存器:
; ES ---- 附加段寄存器(extra segment)
; CS ---- 代码段寄存器(code segment)
; SS ---- 栈段寄存器(stack segment)
; DS ---- 数据段寄存器(data segment)
; FS ---- nameless(segment part 2)
; GS ---- nameless(segment part 3)

; FAT参数解释:
; FAT12(FAT12 Format): windows或ms-dos格式化出来的格式，我们使用它来搭载我们的系统以保证兼容性
; 启动区(boot sector): 软盘的第一个扇区成为启动区, 计算机从软盘上每次读取512字节的内容，所以扇区大小为512, 第一个扇区的最两个字节必须是55 AA, 否则计算机将拒绝承认磁盘上有操作系统、
; IPL(initial program loader): 启动程序加载器，这里只有512字节，但一般的操作系统非常大，这是不够用的。几乎所有的操作系统都把加载操作系统本身的程序放在IPL中，但本系统没有加载程序的功能，所以 "HELLOIPT" 仅仅是用来填充作用的, 如果名字不够8字节还得空格补齐。

; 这段代码的C语言翻译版(部分):
; entry:
;		AX = 0;
;		SS = AX;
;		SP = 0x7c00;
;		DS = AX;
;		ES = AX;
;		SI = msg;
; putloop:
;		AL = BYTE[SI];
;		SI = SI + 1;
;		IF(AL == 0) {goto fin;}
;		AH = 0X0e;
;		BX = 15;
;		INT 0x10;
;		goto putloop;
; fin:
;		HLT;
;		goto fin;
; 就是将寄存器里面的字符一个一个打在屏幕上