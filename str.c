int stlen(char* str) {
  int i;
  for(i = 0; str[i] != '\0'; i++) {
    
  }
  return i;
}
int H_strcmp(char *a,char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return 1;
        } else {
            i++;
        }
    }
    if (a[i] == b[i]) {
        return 0;
    } else if (a[i] != b[i]) {
        return 1;
    }
}
char *H_strcpy(char *src, char *dst) {
    int j = 0;
    while (src[j] != '\0') {
        dst[j] = src[j];
        j++;
    }
    dst[j] = '\0';
    return dst;
}

char *H_strcat(char *src,char *dst,int n) {
    int k = 0;
    int pos = stlen(dst);
    while (src[k] != '\0') {
        if ((pos + k) < n) {
            dst[pos+k] = src[k];
            k++;
        } else {
            break;
        }
    }
    dst[pos+k] = '\0';
    return dst;
}
