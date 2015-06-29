
AS=i686-elf-as
GCC=i686-elf-gcc
OBJS=boot.o terminal.o string.o kernel.o

all: myos.bin

boot.o: boot.s
	$(AS) $< -o $@

%.o: %.c
	$(GCC) -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

myos.bin: linker.ld $(OBJS)
	$(GCC) -T $< -o $@ -ffreestanding -O2 -nostdlib $(OBJS) -lgcc

clean:
	-rm -f *.o
