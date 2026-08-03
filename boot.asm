bits 16
org 0x7C00

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [boot_drive], dl    ; BIOSから渡された起動ドライブ番号を保存

    ; ディスク読み込み先の初期化
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x7E00          ; kernel.c（セクタ2）が展開されるアドレス

    mov ch, 0               ; シリンダ 0
    mov dh, 0               ; ヘッド 0
    mov cl, 2               ; セクタ 2 から開始
    mov si, 10              ; 💡ぴったり10セクタ（5120バイト）読み込む

.load_loop:
    cmp si, 0
    jle .load_end

    mov ah, 0x02
    mov al, 1               ; 1セクタずつ処理
    mov dl, [boot_drive]
    int 0x13
    jc .error

    add bx, 0x0200          ; 次のバッファへ (512バイト進める)
    dec si                  ; 残りセクタ数を減らす

    inc cl
    cmp cl, 64
    jl .load_loop

    mov cl, 1
    inc dh
    cmp dh, 2
    jl .load_loop

    mov dh, 0
    inc ch
    jmp .load_loop          ; ループ先頭の条件判定へ戻る

.error:
    mov ax, 0x0B800
    mov es, ax
    mov word [es:0], 0x4F45 ; 読み込み失敗時は画面左上に赤文字で「E」を表示
    jmp $

.load_end:
    ; A20ラインの有効化 (1MB以上のメモリにアクセスするため)
    in al, 0x92
    or al, 2
    out 0x92, al

    ; GDT（グローバル記述子テーブル）の読み込み
    lgdt [gdt_descriptor]

    ; 32bit保護モードへ移行
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; パイプラインをクリアし、32bitコードへロングジャンプ
    jmp 0x08:init_32bit

bits 32
init_32bit:
    ; 各セグメントレジスタをデータセグメント（0x10）で初期化
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        ; スタックポインタを安全な位置に設定

    ; kernel.c のメイン処理（0x7E00）へジャンプして引き継ぐ
    jmp 0x08:0x7E00

; ----------------------------------------------------
; データ・GDT管理エリア（すべてセクタ1の中に隠蔽）
; ----------------------------------------------------
boot_drive: db 0

align 4
gdt_start:
    dd 0x00000000, 0x00000000           ; ヌルディスクリプタ
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF   ; コードセグメント (ベース:0, リミット:4GB)
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF   ; データセグメント (ベース:0, リミット:4GB)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ブートシグネチャ（これでぴったり512バイトのバイナリになります）
times 510-($-$$) db 0
dw 0xAA55
