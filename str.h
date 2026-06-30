#ifndef KERNEL_H  // 【おまじない①】もし KERNEL_H がまだ登録されていなければ
#define KERNEL_H  // 【おまじない②】KERNEL_H を登録する！

// --- ここに関数の「説明書（プロトタイプ宣言）」を書く ---

// 先ほど自作した strlen 関数の説明書（末尾にセミコロンを付ける）
int stlen(char* str);

// キーボード制御に使う入出力関数の説明書
unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);

#endif // 【おまじない③】おまじないの終わり
