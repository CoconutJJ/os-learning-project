CC=i686-elf-gcc
ASM=nasm -felf32
FLAGS=-ffreestanding -Wall -Wextra -O2 -std=gnu99

all: boot.o kernel.o

boot.o: boot.asm
	$(ASM) boot.asm -o boot.o

kernel.o: kernel.c multiboot.h
	$(CC) $(FLAGS) -c kernel.c -o kernel.o

clean:
	rm *.o
