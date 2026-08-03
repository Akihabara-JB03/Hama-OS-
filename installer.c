#include "str.h"
#include "io.h"

// 画面制御用の外部変数（あなたのコードの変数）
extern int cursor_x;
extern int cursor_y;

// 💡 画面の色を「青背景・白文字（0x1F）」にして文字を描く、セットアップ専用のprint関数
void setup_print(char* str) {
  unsigned char* video = (unsigned char*)0xb8000;
  int mozinagasa = stlen(str);

  for (int i = 0; i < mozinagasa; i++) {
    if (cursor_x == 80) {
      cursor_y++;
      cursor_x = 0;
    }
    
    // 改行コードの処理
    if (str[i] == '\n') {
      cursor_x = 0;
      cursor_y++;
      continue;
    }

    // 💡 0x1F = 背景：青(1)、文字：白(F) ＝ Windowsのセットアップ画面の色！
    video[(cursor_y * 80 + cursor_x) * 2] = str[i];
    video[(cursor_y * 80 + cursor_x) * 2 + 1] = 0x1F;
    cursor_x++;
  }
}

// 💡 画面全体を青色（0x1F）のスペースで埋め尽くす関数
void setup_clear() {
  unsigned char* video = (unsigned char*)0xb8000;
  for (int i = 0; i < 2000; i++) {
    video[i * 2] = ' ';
    video[i * 2 + 1] = 0x1F; // 青背景
  }
  cursor_x = 0;
  cursor_y = 0;
}

// 💡 下部のアクションバー（グレーの帯）を描く関数
void draw_bottom_bar(char* text) {
  unsigned char* video = (unsigned char*)0xb8000;
  // 一番下の行（24行目）をグレー背景・黒文字（0x70）で埋める
  for (int x = 0; x < 80; x++) {
    video[(24 * 80 + x) * 2] = ' ';
    video[(24 * 80 + x) * 2 + 1] = 0x70;
  }
  
  // 指定された文字を一番下の行に描画
  int old_x = cursor_x;
  int old_y = cursor_y;
  cursor_x = 2; // 少し左にスペースを空ける
  cursor_y = 24;
  
  // 文字を描く（一時的に属性を0x70にして上書き）
  int len = stlen(text);
  for (int i = 0; i < len; i++) {
    video[(24 * 80 + cursor_x) * 2] = text[i];
    video[(24 * 80 + cursor_x) * 2 + 1] = 0x70;
    cursor_x++;
  }
  
  cursor_x = old_x;
  cursor_y = old_y;
}

// 💡 インストーラーのメイン画面
void run_hama_os_installer() {
    setup_clear();
    
    // タイトルバー
    cursor_x = 24; cursor_y = 1;
    setup_print("HAMA OS [VER 0.01.2] SETUP\n");
    cursor_x = 24; cursor_y = 2;
    setup_print("==========================\n\n");
    
    // メインメッセージ
    cursor_x = 5; cursor_y = 5;
    setup_print("Welcome to HAMA OS Installation Wizard!\n\n");
    
    cursor_x = 5; cursor_y = 7;
    setup_print("This setup program will prepare HAMA OS to run on this computer.\n");
    cursor_x = 5; cursor_y = 8;
    setup_print("All existing data on the target drive will be overwritten.\n\n");
    
    cursor_x = 7; cursor_y = 11;
    setup_print("* To start the installation, press ENTER.\n");
    cursor_x = 7; cursor_y = 13;
    setup_print("* To repair an existing installation, press R.\n");
    cursor_x = 7; cursor_y = 15;
    setup_print("* To quit setup without installing, press F3.\n");
    
    // 一番下にWindows風の操作案内を表示
    draw_bottom_bar("ENTER=Continue   R=Repair   F3=Quit");
}

void kernel_main() {
    // 💡 起動してすぐにインストーラー画面を立ち上げる！
    run_hama_os_installer();
    
    // キーボードの割り込み（init_interrupts）をオンにして、
    // ENTERが押されたら「ハードディスクへの書き込み関数」へ進むループを作る
    while (1) {
        // ここでキー入力を待つ
    }
}
