bits 16
org 0x7C00

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x7E00

    mov cx, 0x0002
    mov dh, 0
    mov si, 100

.load_loop:
    cmp si, 0
    jle .load_end

    mov ah, 0x02
    mov al, 1
    int 0x13
    jc .error

    add bx, 0x0200
    dec si

    inc cl
    cmp cl, 19
    jl .load_loop

    mov cl, 1
    inc dh
    cmp dh, 2
    jl .load_loop

    mov dh, 0
    inc ch
    jmp .load_loop

.error:
    mov ax, 0x0B800
    mov es, ax
    mov word [es:0], 0x4F45
    jmp $

.load_end:
    in al, 0x92
    or al, 2
    out 0x92, al

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:init_32bit

bits 32
init_32bit:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x08:0x7E00

align 4
gdt_start:
    dd 0x00000000, 0x00000000
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
