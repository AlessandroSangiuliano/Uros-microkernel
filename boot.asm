org 0 ;invece di settarlo a 0x7c00 lo facci iniziare dall'indirizzo 0 perchè non tutti i BIOS settano 0x7c00 come indirizzo di inizio standard
bits 16

_start:
    jmp short start
    nop

times 33 db 0
start:
    jmp 0x7c0:step2 ; settando l'indirizzo prima di start, obbloghiamo il; resgistro CS a diventare 0x07c
step2:
    cli ; disabilito le interrupt per settare i registri segmenti a 0x7c00
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti ; riabilito le interrupt. se non di fa tutto qullo sopra, il BIOS potrebbe settare i registri in altri modi che non permetterebbero il boot

    mov ah, 2 ; funzione bios di lkettura settore
    mov al, 1 ; 1 settore da leggere
    mov ch, 0 ; leggiamo dal cilindro zero
    mov cl, 2 ; leggo il settore numero 2
    mov dh, 0 ; leggo dalla testina 0
    mov bx, buffer    
    int 0x13
    jc error

    mov si, buffer
    call print

    jmp $

print:
    mov bx, 0
.loop:
    lodsb ; carica il primo carattere di message in al e si sposta sul successivo
    cmp al, 0
    je .end
    call print_char
    jmp .loop

.end:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

error:
    mov si, error_message
    call print
    jmp $

error_message: db 'Failed to read sector!'

;message: db 'Hello World!', 0

times 510-($-$$) db 0
dw 0xAA55

buffer: