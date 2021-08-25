[INSTRSET "i486p"]
[BITS 32]
		CALL	2*8:0xade  ; bootpack.map 中_io_cli的地址 
		MOV		EDX,4
		INT		0x40
