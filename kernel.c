#include "io.h"
#include "keyboard.h"
#include "str.h"
void execute_command(void) {
        
    char line[81]; // 画面1行分（80文字）＋ 終わりの '\0'（1文字）が入る箱
    int x;

    // 【ステップ1】今の行（cursor_y）の文字を、左端（0）から右端（79）まで集めるループ
    for (x = 0; x < 80; x++) {
        // あなたが作った current_char(x, cursor_y) を使って、1文字ずつ line[x] にしまう！
        line[x] = current_char(x, cursor_y);
    }
    // ループが終わったら、文字列のルールとして最後に必ず終了の印を入れる
    line[x] = '\0';
    if (H_strcmp(line, "hello") == 0) {
        print("\nHello! This is HAMA OS!\n");
    }     
    else if (H_strcmp(line, "clear") == 0) {
        clear();
    } 
    else {
        print("\nUnknown Command...\n");
    }

    // 【ステップ3】次の入力のために、改行してプロンプトを表示する
    print("HAMA_OS> ");
}

void kernel_main() {
    clear();
    print("HAMA OS [VER 0.01.2] HELLO WORLD");
    init_interrupts();

    while (1) {
        // 無限ループで待機
    }
}
