
AS=i686-elf-as
GCC=i686-elf-gcc
OBJS=boot.o kernel.o

all: myos.bin

boot.o: boot.s
	$(AS) $< -o $@

kernel.o: kernel.c
	$(GCC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

myos.bin: linker.ld $(OBJS)
	$(GCC) -T $< -o $@ -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

clean:
	-rm -f *.o
