[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "alloca.nas"]

    GLOBAL	__alloca

[SECTION .text]

__alloca:
    ADD     EAX,-4
    SUB     ESP,EAX
    ADD     ESP,4
    JMP     DWORD [ESP+EAX-4]     ; 代替RET


; __alloca 会在下述情况下被C语言的程序调用(采用near-CALL)
;   要执行的操作从栈中分配EAX个字节的内存空间(ESP =- EAX;)
;   要遵守的规则不能改变ECX, EDX, EBX, EBP, ESI, EDI的值(可以临时改变他们的值, 但是要用PUSH/POP来恢复)
;   