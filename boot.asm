bits 16
org 0x7C00

; --- エントリーポイント ---
_start:
    cli             ; 割り込み禁止
    xor ax, ax      ; セグメントレジスタの初期化
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00  ; スタックをブートローダの手前に配置

    ; 1. A20ラインの有効化 (キーボードコントローラ経由の高速有効化)
    in al, 0x92
    or al, 2
    out 0x92, al

    ; 2. 自作GDTのロード
    lgdt [gdt_descriptor]

    ; 3. CR0レジスタのPEビットを1にして保護モードへ移行
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; 4. 32bitコードセグメントへ遠隔ジャンプ（パイプラインフラッシュ）
    jmp 0x08:init_32bit

bits 32
init_32bit:
    ; 5. 32bitデータセグメントの初期化
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000 ; カーネル用スタックポインタ

    ; 6. C言語カーネルへのジャンプ
    ; 512バイト（0x200）後ろの 0x7E00 番地に配置された kernel_main へ直接突っ込みます
    jmp 0x08:0x7E00

.halt:
    hlt
    jmp .halt

; --- GDT (グローバル記述子テーブル) ---
align 4
gdt_start:
    dd 0x00000000, 0x00000000 ; ヌル記述子

    ; コードセグメント (0x08): Base=0, Limit=4GB
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9A
    db 0xCF
    db 0x00

    ; データセグメント (0x10): Base=0, Limit=4GB
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xCF
    db 0x00
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; --- ブートシグネチャ (512バイトに調整) ---
times 510-($-$$) db 0
dw 0xAA55
