bits 16
org 0x7C00

_start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; 💡 [変更点] ハードディスク（通常は0x80番ドライブ）を固定で指定
    ; もしBIOSから渡された番号をそのまま使う場合は mov [boot_drive], dl にしてください
    mov byte [boot_drive], 0x80

    ; ----------------------------------------------------
    ; 💾 【全自動処理】HDDから通常OS（os.img）をメモリに読み込む
    ; ----------------------------------------------------
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x7E00          ; 通常OS（C言語カーネル）を展開するメモリ番地

    mov ch, 0               ; シリンダ 0
    mov dh, 0               ; ヘッド 0
    mov cl, 2               ; セクタ 2 から読み込み開始
    mov si, 20              ; 💡 [余裕設計] 通常OS（10KiB分＝ぴったり20セクタ）を一気に読み込む！

.load_loop:
    cmp si, 0
    jle .load_end

    mov ah, 0x02
    mov al, 1               ; 1セクタずつ安全に処理
    mov dl, [boot_drive]
    int 0x13
    jc .error               ; 読み込みに失敗したらエラー画面へ

    add bx, 0x0200          ; メモリのバッファを512バイト進める
    dec si                  ; 残りセクタ数をマイナス1

    ; 💡 HDDの大容量セクタ（1トラック63セクタ）に対応した安全な自動繰り上げ
    inc cl
    cmp cl, 64              ; 63セクタを超えたら次のヘッドへ
    jl .load_loop

    mov cl, 1
    inc dh
    cmp dh, 2
    jl .load_loop

    mov dh, 0
    inc ch
    jmp .load_loop          ; ループの先頭へ戻る

.error:
    ; 画面左上に赤文字で「I」（Installer ErrorのI）を表示して停止
    mov ax, 0x0B800
    mov es, ax
    mov word [es:0], 0x4F49 
    jmp $

.load_end:
    ; ----------------------------------------------------
    ; 🚀 32bit保護モード（PMode）への突入処理
    ; ----------------------------------------------------
    ; Fast A20ライン有効化
    in al, 0x92
    or al, 2
    out 0x92, al

    ; GDT（グローバル記述子テーブル）の登録
    lgdt [gdt_descriptor]

    ; 保護モードON
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; 32bitの世界へロングジャンプ（0x7E00へ突撃！）
    jmp 0x08:init_32bit

bits 32
init_32bit:
    ; 各種セグメントレジスタの初期化
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000        ; スタックポインタを安全な位置に

    ; 読み込み終わった通常OSのメイン処理（0x7E00）へ制御を引き渡す！
    jmp 0x08:0x7E00

; ----------------------------------------------------
; データ・GDT管理エリア
; ----------------------------------------------------
boot_drive: db 0

align 4
gdt_start:
    dd 0x00000000, 0x00000000           ; ヌルディスクリプタ
    dw 0xFFFF, 0x0000, 0x9A00, 0x00CF   ; コードセグメント
    dw 0xFFFF, 0x0000, 0x9200, 0x00CF   ; データセグメント
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

; ブートシグネチャ（512バイトぴったりにする）
times 510-($-$$) db 0
dw 0xAA55
