[BITS 32]
global _start
extern kernel_main
CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start: 
    mov ax, DATA_SEG
    mov es, ax
    mov ds, ax
    mov fs, ax 
    mov ss, ax
    mov gs, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ;enable a20 line
    in al, 0x92
    or al, 2
    out 0x92, al

.init_pic:
    jmp pic_init

end_pic_init:
    call kernel_main
    
    jmp $


pic_init:
    mov al, 00010001b
    out 0x20, al ; tell master
    ;out 0xA0, al ; tell slave

    mov al, 0x20
    out 0x21, al
    mov al, 00000001b
    out 0x21, al
    ;out 0xA1, al
    jmp end_pic_init

times 512-($ - $$) db 0