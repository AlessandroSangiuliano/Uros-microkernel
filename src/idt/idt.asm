; @author Alex Sangiuliano
; @email alex22_7@hotmail.com
; @create date 2021-12-25 17:49:43
; @modify date 2021-12-25 17:49:43
; @desc [description]
section .asm

global idt_load
global int21h
global no_interrupt

global enable_interrupts
global disable_interrupts

extern int21h_handler
extern no_interrupt_handler

enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret
idt_load:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp
    ret

int21h:
    cli
    pushad ;pushes all the general purpose registers to the stack
    call int21h_handler
    popad ;pop all the general purpose registers from the stack
    sti
    iret

no_interrupt:
    cli
    pushad ;pushes all the general purpose registers to the stack
    call no_interrupt_handler
    popad ;pop all the general purpose registers from the stack
    sti
    iret