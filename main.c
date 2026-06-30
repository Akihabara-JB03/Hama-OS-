#include "str.h"
void kernel_main() {
    // 1バイト（8ビット）ずつ細かくメモリを叩くために「unsigned char*」にする
    unsigned char* video = (unsigned char*)0xb8000;
    char* mozi = "Hama OS [Version 1.0.0]";  
    int mozinagasa = stlen(mozi);
    // 文字列の長さは24文字 ＝ 文字と色で合計48バイト分ループを回す
    for (int i = 0; i < mozinagasa * 2; i += 2) {
        video[i] = mozi[i / 2];   // 偶数番地には「文字コード」を書き込む
        video[i + 1] = 0x0F;      // 奇数番地には「色属性（白文字/黒背景）」を書き込む
    }

    while (1) {
        // 無限ループで待機
    }
}
