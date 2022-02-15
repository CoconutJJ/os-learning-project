CC=i686-elf-gcc
ASM=nasm -felf32
FLAGS=-ffreestanding -Wall -Wextra -O2 -std=gnu99 -nostdlib

all: boot.o kernel.o font.o
	$(CC) $(FLAGS) -T linker.ld -o myos.bin boot.o kernel.o font.o -lgcc
	mv myos.bin isodir/boot
	grub-mkrescue -o myos.iso isodir


boot.o: boot.asm
	$(ASM) boot.asm -o boot.o

kernel.o: kernel.c multiboot.h
	$(CC) $(FLAGS) -c kernel.c -o kernel.o

font.o: fonts/8x16.psfu
	objcopy -O elf32-i386 -B i386 -I binary fonts/8x16.psfu font.o

clean:
	rm *.o
