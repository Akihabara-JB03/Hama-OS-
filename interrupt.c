#include "interrupt.h"
#include "io.h"

// IDTテーブルの実体（256個の割り込みに対応）
struct IDTEntry idt[256];
struct IDTR idtr;

// CPUのポートに1バイト書き込む関数
inline void out8(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

// CPUのポートから1バイト読み込む関数
inline unsigned char in8(unsigned short port) {
    unsigned char data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "Nd"(port));
    return data;
}

// スキャンコードをASCIIに変換する本物のテーブル
const char scan_to_ascii[256] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*',
    0,   ' ', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   '-', 0,   0,   0,   '+', 0,   0,   0,   0,   0
};

// 💡【最重要】キーボードが押された瞬間に、CPUがダイレクトに突入してくる関数
void keyboard_handler_main(void) {
    // 1. キーボードのポートからデータを引っこ抜く
    unsigned char code = in8(0x60);

    // 2. キーが離されたデータ（0x80以上）でなければ処理する
    if (code < 0x80) {
        char ascii = scan_to_ascii[code];
        if (ascii != 0) {
            // 1文字だけの文字列を作って、あなたのprint関数で画面に出す！
            char str[2] = {ascii, '\0'};
            print(str);
        }
    }

    // 3. PICに対して「割り込み処理が終わったよ」と通知する（重要）
    out8(0x20, 0x20);
}

// アセンブリ言語で割り込みの前後を保護するためのラッパー（インラインアセンブリ）
void keyboard_handler_asm(void);
__asm__(
    ".global keyboard_handler_asm\n"
    "keyboard_handler_asm:\n"
    "    pusha\n"            // CPUのすべてのレジスタを安全に保存
    "    call keyboard_handler_main\n" // C言語の本体を呼び出す
    "    popa\n"             // レジスタを元に戻す
    "    iret\n"             // 割り込みから完全に復帰する専用の命令！
);

// 割り込みの登録関数
void set_idt_entry(unsigned char num, unsigned int handler) {
    idt[num].offset_lower = handler & 0xFFFF;
    idt[num].selector = 0x08; // コードセグメント
    idt[num].zero = 0;
    idt[num].type_attr = 0x8E; // 割り込みゲート
    idt[num].offset_upper = (handler >> 16) & 0xFFFF;
}

void init_interrupts(void) {
    // 1. PIC（割り込みコントローラ）の初期化
    out8(0x20, 0x11); out8(0xA0, 0x11); // ICW1
    out8(0x21, 0x20); out8(0xA1, 0x28); // ICW2: 割り込み番号のベース（0x20から）
    out8(0x23, 0x04); out8(0xA3, 0x02); // ICW3
    out8(0x21, 0x01); out8(0xA1, 0x01); // ICW4

    // 2. キーボード（IRQ1 = 0x21番）の割り込みだけを許可する（他はマスクして無視）
    out8(0x21, 0xFD); // 0xFD = 11111101b (IRQ1だけON)
    out8(0xA1, 0xFF);

    // 3. IDTにキーボードハンドラを登録
    set_idt_entry(0x21, (unsigned int)keyboard_handler_asm);

    // 4. CPUにIDTの場所をロードさせる
    idtr.limit = (sizeof(struct IDTEntry) * 256) - 1;
    idtr.base = (unsigned int)&idt;
    __asm__ __volatile__("lidt %0" : : "m"(idtr));

    // 5. CPU全体の割り込み禁止状態を解除！
    __asm__ __volatile__("sti");
}
