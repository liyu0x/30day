; haribote-os boot asm
; TAB=4

[INSTRSET "i486p"]

BOTPAK	EQU		0x00280000		; bootpack�̃��[�h��
DSKCAC	EQU		0x00100000		; �f�B�X�N�L���b�V���̏ꏊ
DSKCAC0	EQU		0x00008000		; �f�B�X�N�L���b�V���̏ꏊ�i���A�����[�h�j

; BOOT_INFO�֌W
CYLS	EQU		0x0ff0			; �u�[�g�Z�N�^���ݒ肷��
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2			; �F���Ɋւ�����B���r�b�g�J���[���H
SCRNX	EQU		0x0ff4			; �𑜓x��X
SCRNY	EQU		0x0ff6			; �𑜓x��Y
VRAM	EQU		0x0ff8			; �O���t�B�b�N�o�b�t�@�̊J�n�Ԓn

		ORG		0xc200			; ���̃v���O�������ǂ��ɓǂݍ��܂��̂�


VBEMODE	EQU		0x105
; VBE画面模式号码如下:
; 	0x101	-	640*480*8bit 彩色
;	0x103	-	800*600*8bit 彩色
;	0x105	-	1024*768*8bit 彩色
;	0x107	-	1280*1024*8bit 彩色
;   在EQMU下不能指定0x107的模式, 实际指定上面的值的时候, 需要将画面模式号码值加上0x4000, 再赋值到BX中去.

; 确认是否能使用VBE, gei ES赋值为0x9000, 给DI赋值为0, 给AX赋值为0x4f00, 再执行"INT 0x10", 如果有VBE的话
; AX就会变为0x004f, 至于为什么要给AX和DI赋值是因为显卡的VBE信息会写入ES:DI开始的512字节中, 赋值是为
; 了指定地址
		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320


; 检查VBE版本
		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320			; if (AX < 0x0200) goto scrn320


; 取得画面模式信息
		MOV		CX,VBEMODE
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320
; 画面模式信息, 重要的信息有如下6个:
; WORD	[ES:DI+0x00]	:	模式属性, bit7, 不是1就不好办(能加上0x4000)
; WORD	[ES:DI+0x12]	:	X的分辨率
; WORD	[ES:DI+0x14]	:	Y的分辨率
; WORD	[ES:DI+0X19]	:	颜色数, 必须为8
; WORD	[ES:DI+0x1b]	:	颜色的指定方法, 必须为4(4是调色板模式)
; WORD	[ES:Di+0x28]	: 	VRAM的地址

; 确认显示模式信息

		CMP		BYTE [ES:DI+0x19],8		; 	颜色数为8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4		;	调色板模式
		JNE		scrn320
		MOV		AX,[ES:DI+0x00] 
		AND		AX,0x0080
		JZ		scrn320			; 模式属性的bit7是0, 所以放弃

; 画面模式的切换

		MOV		BX,VBEMODE+0x4000
		MOV		AX,0x4f02
		INT		0x10
		MOV		BYTE [VMODE],8	; 记下画面模式
		MOV		AX,[ES:DI+0x12]
		MOV		[SCRNX],AX
		MOV		AX,[ES:DI+0x14]
		MOV		[SCRNY],AX
		MOV		EAX,[ES:DI+0x28]
		MOV		[VRAM],EAX
		JMP		keystatus

scrn320:
		MOV		AL,0x13			; VGA模式
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 记下画面模式
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000


; 使用VBE的画面设定模式 
;		MOV		BX,0x4101		; VBE的640*480*8bit彩色
;		MOV		AX,0x4f02
;		INT		0x10
;		MOV		BYTE [VMODE],8	; 记下画面模式
;		MOV		WORD [SCRNX],640
;		MOV		WORD [SCRNY],480
;		MOV		DWORD [VRAM],0xe0000000

; 不使用VBE模式的显示设置(仅支持320*200)
;		MOV		AL,0x13			; VGA的320*200*8bit彩色
;		MOV		AH,0x00
;		INT		0x10
;		MOV		BYTE [VMODE],8	; 记下画面模式
;		MOV		WORD [SCRNX],320
;		MOV		WORD [SCRNY],200
;		MOV		DWORD [VRAM],0x000a0000


; 检查键盘状态
keystatus:
		MOV		AH,0x02
		INT		0x16 			; keyboard BIOS
		MOV		[LEDS],AL



; PIC 关闭一切中断
;	根据AT兼容机的规格, 如果要初始化PIC
;	必须在CLI之前进行, 否则有时候会挂起
;	随后进行PIC的初始化

		MOV		AL,0xff
		OUT		0x21,AL
		NOP						; NOP是让CPU休息一个时钟长的时间, 如果连续执行OUT命令, 有些机种会无法正常运行
		OUT		0xa1,AL

		CLI						; 禁止CPU级别的中断

; 为了让CPU能够访问1MB以上的内存空间, 设定A20GATE

		CALL	waitkbdout
		MOV		AL,0xd1
		OUT		0x64,AL
		CALL	waitkbdout
		MOV		AL,0xdf			; enable A20
		OUT		0x60,AL
		CALL	waitkbdout

; 切换到保护模式
[INSTRSET "i486p"]				; "想要使用486指令"的叙述

		LGDT	[GDTR0]			; 设定临时GDT
		MOV		EAX,CR0
		AND		EAX,0x7fffffff	; 设bit31为0 (为了禁止颁)
		OR		EAX,0x00000001	; 设bit0为1 (为了切换到保护模式)
		MOV		CR0,EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX,1*8			;  可读写的段 32bit
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		GS,AX
		MOV		SS,AX

; INSTRSET 指令: 是为了能够使用386以后的LGDT, EAX, CR0等关键字
; CR0 只有操作系统才能使用的32位特殊寄存器
; 保护模式: 在这种模式下, 应用程序既不能随便改变段的设定, 又不能使用操作系统专用的段, 操作系统收到CPU的保护.

; bootpack 转送

		MOV		ESI,bootpack	; 转送源
		MOV		EDI,BOTPAK		; 转送目的地
		MOV		ECX,512*1024/4	; 转送数据大小以双字节为单位
		CALL	memcpy

; 磁盘数据最终送到它本来的位置去

; 首先从启动扇区开始

		MOV		ESI,0x7c00		; 转送源
		MOV		EDI,DSKCAC		; 转送目的地
		MOV		ECX,512/4
		CALL	memcpy

; 所有剩下的

		MOV		ESI,DSKCAC0+512	; 转送源
		MOV		EDI,DSKCAC+512	; 转送目的地
		MOV		ECX,0
		MOV		CL,BYTE [CYLS]
		IMUL	ECX,512*18*2/4	; 从柱面数变换为字节数/4
		SUB		ECX,512/4		; 减去IPL
		CALL	memcpy



; 必须由asmhead来完成的工作,至此已经全部完成
;	以后就交由bootpack来完成

; bootpack的启动

		MOV		EBX,BOTPAK
		MOV		ECX,[EBX+16]
		ADD		ECX,3			; ECX += 3;
		SHR		ECX,2			; ECX /= 4;
		JZ		skip			; 没有要转送的东西时
		MOV		ESI,[EBX+20]	; 转送源
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	; 转送目的地
		CALL	memcpy
skip:
		MOV		ESP,[EBX+12]	; 栈初始值
		JMP		DWORD 2*8:0x0000001b  ; JMP的far模式, 再想EIP存入0x1b的同时, 将CS设置为2*8
; 它对bootpack.hrb的header(头部内容进行解析), 将执行所必须的数据传送过去. EBX里面带入的是
; BOTPAK, 所以值如下:
; [EBX + 16]......bootpack.hrb之后的16号地址, 值是0x11a8
; [EBX + 20]......bootpack.hrb之后的20号地址, 值是0x10c8
; [EBX + 12]......bootpack.hrb之后的12号地址, 值是0x00310000
; SHR 右移指令; JZ(jump if zero) 条件跳转指令, 根据前一个计算结构是否为0来决定是否跳转

; 这里主要是将bootpack.hrb第0x10c8字节开始的0x11a8字节复制到0x00310000号地址中.

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		JNZ		waitkbdout		; AND的结果如果不是0, 就跳到waitkbdout
		RET

memcpy:
		MOV		EAX,[ESI]
		ADD		ESI,4
		MOV		[EDI],EAX
		ADD		EDI,4
		SUB		ECX,1
		JNZ		memcpy			; 减法运算的结果如果不是0, 就跳转到memcpy
		RET


		ALIGNB	16 			; 一直添加DBO, 直到地址能被16整除为止, 如果GDT0的地址不是8的整数倍, 向段寄存器赋值MOV的指令就会慢一些
GDT0:
		RESB	8				; NULL selector
		DW		0xffff,0x0000,0x9200,0x00cf	; 可以读写的段(segment) 32bit
		DW		0xffff,0x0000,0x9a28,0x0047	; 可以执行的段(segment) 32bit(bootpack用)

		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0

		ALIGNB	16
bootpack:
