.section .text

.extern _ZN16InterruptManager15handleInterruptEhj


int_bottom:
    
    pusha
    push %ds
    push %es
    push %fs
    pushl %gs


    push %esp
    push (interruptnumber)
    call _ZN16InterruptManager15handleInterruptEhj
    # addl $5, %esp
    movl %ebp, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

.data
    interruptnumber: .byte 0

