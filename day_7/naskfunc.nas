; naskfunc
; TAB=4

[FORMAT "WCOFF"]                ; 制作目标文件的模式
[INSTRSET "i486p"]              ; 声明用于486CPU
[BITS 32]                       ; 制作32位模式用的机器语言

; 制作目标文件的信息

[FILE "naskfunc.nas"]           ; 源文件名信息
        GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt           ; 程序中包含的函数名
	GLOBAL	_io_in8,  _io_in16,  _io_in32
	GLOBAL	_io_out8, _io_out16, _io_out32
	GLOBAL	_io_load_eflags, _io_store_eflags
        GLOBAL	_load_gdtr, _load_idtr
        GLOBAL	_asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
        EXTERN	_inthandler21, _inthandler27, _inthandler2c



; 以下是实际的函数

[SECTION .text]                 ; 目标文件中写了这些之后再写程序


_io_hlt:        ; void io_hlt(void);
        HLT
        RET

_io_cli:        ; void io_cli(void);
        CLI     
        RET

_io_sti:        ; void io_sti(void);
        STI
        RET

_io_stihlt:     ; void io_stihlt(void);
        STI
        HLT
        RET

_io_in8:        ; int io_in8(int port);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,0
        IN      AL,DX
        RET

_io_in16:       ; int io_in16(int port);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,0
        IN      AX,DX
        RET

_io_in32:	; int io_in32(int port);
        MOV	EDX,[ESP+4]		; port
        IN	EAX,DX
        RET

_io_out8:	; void io_out8(int port, int data);
        MOV	EDX,[ESP+4]		; port
        MOV	AL,[ESP+8]		; data
        OUT	DX,AL
        RET

_io_out16:	; void io_out16(int port, int data);
        MOV	EDX,[ESP+4]		; port
        MOV	EAX,[ESP+8]		; data
        OUT	DX,AX
        RET

_io_out32:	; void io_out32(int port, int data);
        MOV	EDX,[ESP+4]		; port
        MOV	EAX,[ESP+8]		; data
        OUT	DX,EAX
        RET

_io_load_eflags:	; int io_load_eflags(void);
        PUSHFD	        ; 指 PUSH EFLAGS 
        POP	EAX
        RET

_io_store_eflags:	; void io_store_eflags(int eflags);
        MOV	EAX,[ESP+4]
        PUSH	EAX
        POPFD		; 指 POP EFLAGS
        RET


_write_mem8:            ; viod write_mem8(int addr, int data);
        MOV     ECX, [ESP+4]    ; [ESP+4]中存放的是地址,将其读入ECX
        MOV     AL, [ESP+8]     ; [ESP+8]中存放的是数据,将其读入AL
        MOV     [ECX], AL
        RET


; 这个函数用来将指定的段上线(limit)和地址值赋值给名为GDTR的48位寄存器.
_load_gdtr:	        ; void load_gdtr(int limit, int addr);
        MOV     AX,[ESP+4]      ; limit
        MOV     [ESP+6],AX
        LGDT	[ESP+6]
        RET

_load_idtr:		; void load_idtr(int limit, int addr);
        MOV     AX,[ESP+4]	; limit
        MOV	[ESP+6],AX
        LIDT	[ESP+6]
        RET


; RET 指令相当于return

; 与EFLAGS相关指令: EFLAGS 没有MOV指令， 只有PUSHFD(push flags double-word)和POPFD(pop flags double-word)指令.

; 关于GDTR寄存器, 不能使用MOV来赋值, 唯一的方法是指定一个内存地址, 从指定的地址读取6个字节, 然后赋值给GDTR寄存器. 该寄存器的低16位,是段上限, ,它等于"GDT的有效字节数-1". 剩下的高32位代表GDT的开始地址.


_asm_inthandler21:
        PUSH	ES
        PUSH	DS
        PUSHAD
        MOV		EAX,ESP
        PUSH	EAX
        MOV		AX,SS
        MOV		DS,AX
        MOV		ES,AX
        CALL	_inthandler21
        POP		EAX
        POPAD
        POP		DS
        POP		ES
        IRETD

_asm_inthandler27:
        PUSH	ES
        PUSH	DS
        PUSHAD
        MOV		EAX,ESP
        PUSH	EAX
        MOV		AX,SS
        MOV		DS,AX
        MOV		ES,AX
        CALL	_inthandler27
        POP		EAX
        POPAD
        POP		DS
        POP		ES
        IRETD

_asm_inthandler2c:
        PUSH	ES
        PUSH	DS
        PUSHAD
        MOV		EAX,ESP
        PUSH	EAX
        MOV		AX,SS
        MOV		DS,AX
        MOV		ES,AX
        CALL	_inthandler2c
        POP		EAX
        POPAD
        POP		DS
        POP		ES
        IRETD