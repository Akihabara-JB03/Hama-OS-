#include "str.h"
int cursor_x = 0;
int cursor_y = 0;
void print(char* str) {
  unsigned char* video = (unsigned char*)0xb8000;
  int mozinagasa = stlen(str);
  for (int i = 0; i<mozinagasa; i++) {
    if (cursor_x == 80) {
      cursor_y++;
      cursor_x=0;
    } else {
      
      video[(cursor_y*80+cursor_x)*2] = str[i];
      video[(cursor_y*80+cursor_x)*2+1] = 0x07;
      cursor_x++;
    }
  }
}
void clear() {

  for (int i = 0; i < 2000; i++) {
    // 💡ここに、スペース「" "」を1個画面に出す処理を書く！
    print("\0");
  } 
  // 💡画面を全部スペースで埋めたあと、次の文字のためにカーソルを一番上に戻す処理を書く！
  cursor_x = 0;
  cursor_y = 0;
}
char current_char(int x,int y) {
  unsigned char* video = (unsigned char*)0xb8000;
  return video[(y*80+x)*2];
}
