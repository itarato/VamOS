C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o}
CC = /home/itarato/code/os/i386elfgcc/bin/i386-elf-gcc
# GDB = /home/itarato/code/os/i386elfgcc/bin/i386-elf-gdb
GDB = /usr/bin/gdb
CFLAGS = -g
QEMU = qemu-system-i386

os-image.bin: boot/boot.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

run: os-image.bin
	${QEMU} -fda $<

debug: os-image.bin kernel.elf
	${QEMU} -s -S -fda $< &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "b kernel/kernel.c:main"

%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.o *.dis *.elf
	rm -rf kernel/*.o boot/*.bin boot/*.o
