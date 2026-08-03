#include "str.h"
#include "io.h"

// 画面制御用の外部変数
extern int cursor_x;
extern int cursor_y;

// プロトタイプ宣言
void setup_print(char* str);
void setup_clear(void);
void draw_bottom_bar(char* text);
void wait_time(unsigned int count);

// 💡 CPUを空回りさせて、数秒間の「待ち時間」を作る関数
void wait_time(unsigned int count) {
    volatile unsigned int i;
    for (i = 0; i < count; i++) {
        // 全自動で進めるための時間稼ぎ
    }
}

// 💡 青背景・白文字（0x1F）で文字を描くセットアップ専用のprint
void setup_print(char* str) {
  unsigned char* video = (unsigned char*)0xb8000;
  int mozinagasa = stlen(str);

  for (int i = 0; i < mozinagasa; i++) {
    if (cursor_x == 80) {
      cursor_y++;
      cursor_x = 0;
    }
    if (str[i] == '\n') {
      cursor_x = 0;
      cursor_y++;
      continue;
    }
    video[(cursor_y * 80 + cursor_x) * 2] = str[i];
    video[(cursor_y * 80 + cursor_x) * 2 + 1] = 0x1F;
    cursor_x++;
  }
}

// 💡 画面全体を青色（0x1F）で埋め尽くす
void setup_clear() {
  unsigned char* video = (unsigned char*)0xb8000;
  for (int i = 0; i < 2000; i++) {
    video[i * 2] = ' ';
    video[i * 2 + 1] = 0x1F;
  }
  cursor_x = 0;
  cursor_y = 0;
}

// 💡 下部のアクションバーを描く
void draw_bottom_bar(char* text) {
  unsigned char* video = (unsigned char*)0xb8000;
  for (int x = 0; x < 80; x++) {
    video[(24 * 80 + x) * 2] = ' ';
    video[(24 * 80 + x) * 2 + 1] = 0x70;
  }
  int old_x = cursor_x;
  int old_y = cursor_y;
  cursor_x = 2;
  cursor_y = 24;
  int len = stlen(text);
  for (int i = 0; i < len; i++) {
    video[(24 * 80 + cursor_x) * 2] = text[i];
    video[(24 * 80 + cursor_x) * 2 + 1] = 0x70;
    cursor_x++;
  }
  cursor_x = old_x;
  cursor_y = old_y;
}

// 💡 【メイン処理】全自動でステップが進むインストーラー
void run_hama_os_installer() {
    // ----------------------------------------------------
    // ステップ1：ようこそ画面
    // ----------------------------------------------------
    setup_clear();
    cursor_x = 24; cursor_y = 1;
    setup_print("HAMA OS [VER 0.01.2] SETUP\n");
    cursor_x = 24; cursor_y = 2;
    setup_print("==========================\n\n");
    
    cursor_x = 5; cursor_y = 6;
    setup_print("Welcome to HAMA OS Auto-Installation Wizard!\n\n");
    cursor_x = 5; cursor_y = 8;
    setup_print("Setup will now automatically install HAMA OS to your hard drive.\n");
    cursor_x = 5; cursor_y = 9;
    setup_print("Please wait for a moment...\n");
    
    draw_bottom_bar(" Please Wait... (Auto Setup Mode) ");
    
    wait_time(80000000); // 💡 数秒間待つ（QEMUの速度に合わせて調整してね）

    // ----------------------------------------------------
    // ステップ2：インストール進行画面
    // ----------------------------------------------------
    setup_clear();
    cursor_x = 24; cursor_y = 1;
    setup_print("HAMA OS [VER 0.01.2] SETUP\n");
    cursor_x = 24; cursor_y = 2;
    setup_print("==========================\n\n");
    
    cursor_x = 5; cursor_y = 6;
    setup_print("Installing HAMA OS...\n\n");
    cursor_x = 5; cursor_y = 8;
    setup_print("[||||||||||||||||||||||||||||||||          ] 80% Completed\n");
    cursor_x = 5; cursor_y = 10;
    setup_print("Writing system files to target sectors...");
    
    draw_bottom_bar(" Copying files... Do not turn off your computer. ");
    
    wait_time(90000000); // 💡 さらに待つ

    // ----------------------------------------------------
    // ステップ3：インストール完了画面
    // ----------------------------------------------------
    setup_clear();
    cursor_x = 24; cursor_y = 1;
    setup_print("HAMA OS [VER 0.01.2] SETUP\n");
    cursor_x = 24; cursor_y = 2;
    setup_print("==========================\n\n");
    
    cursor_x = 5; cursor_y = 6;
    setup_print("Installation Successful!\n\n");
    cursor_x = 5; cursor_y = 8;
    setup_print("HAMA OS has been successfully installed on this computer.\n");
    cursor_x = 5; cursor_y = 10;
    setup_print("Please remove any installation media and restart QEMU.\n");
    
    draw_bottom_bar(" Installation Finished! ");
}
