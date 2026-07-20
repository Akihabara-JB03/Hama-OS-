#ifndef INTERRUPT_H
#define INTERRUPT_H

// 1つの割り込みの行き先を記録する構造体（IDTエントリー）
struct IDTEntry {
    unsigned short offset_lower; // ハンドラのアドレスの下位16ビット
    unsigned short selector;     // コードセグメント選択子（通常は0x08）
    unsigned char  zero;         // 常に0
    unsigned char  type_attr;    // 属性（0x8E：32bit割り込みゲート、存在フラグON）
    unsigned short offset_upper; // ハンドラのアドレスの上位16ビット
} __attribute__((packed));

// IDT全体の場所をCPUに教えるための構造体
struct IDTR {
    unsigned short limit;
    unsigned int   base;
} __attribute__((packed));

// ----------------------------------------------------
// 【新規追加】他のCファイル（kernel.c等）に存在を教えるための宣言
// ----------------------------------------------------

// 外部から呼ぶための初期化・制御関数
void init_interrupts(void);
void out8(unsigned short port, unsigned char data);
unsigned char in8(unsigned short port);
void keyboard_handler_main(void);

// グローバル配列と変数の共有宣言（実体は別ファイルにあることを伝える）
extern const char scan_to_ascii[256];
extern int cursor_y; 

#endif
