#include "str.h"
void print(char* str) {
  unsigned char* video = (unsigned char*)0xb8000;
  int mozinagasa = stlen(str);
  int cursor_x = 0;
  int cursor_y = 0;
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
