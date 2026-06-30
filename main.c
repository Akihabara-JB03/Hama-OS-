#include "str.h"
void kernel_main() {
    // 1バイト（8ビット）ずつ細かくメモリを叩くために「unsigned char*」にする
    unsigned char* video = (unsigned char*)0xb8000;
    char* mozi = "Hama OS [Version 1.0.0]";  
    int mozinagasa = stlen(mozi);
    for (int i = 0; i < mozinagasa; i ++) {
        video[i*2] = mozi[i];   // 偶数番地には「文字コード」を書き込む
        video[i * 2 + 1] = 0x0F;      // 奇数番地には「色属性（白文字/黒背景）」を書き込む
    }

    while (1) {
        // 無限ループで待機
    }
}
