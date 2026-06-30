ASM = nasm
CC  = gcc
LD  = ld

# OS用の特殊なフラグ（標準ライブラリを外す）
CFLAGS  = -m32 -c -fno-pie -fno-stack-protector -nostdlib -fno-builtin -O2
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

all: os.img

# アセンブリをコンパイル
boot.o: boot.asm
	$(ASM) $(ASFLAGS) boot.asm -o boot.o

# カーネルC言語をコンパイル
kernel.o: kernel.c
	$(CC) $(CFLAGS) kernel.c -o kernel.o

# 自作文字列関数をコンパイル
str.o: str.c
	$(CC) $(CFLAGS) str.c -o str.o

# すべてを合体させて512バイトのOSイメージを作る
os.img: boot.o kernel.o str.o linker.ld
	$(LD) $(LDFLAGS) boot.o kernel.o str.o -o os.img

clean:
	rm -f *.o os.img
