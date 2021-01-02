C_SOURCES = $(wildcard kernel/*.c)
C_HEADERS = $(wildcard kernel/*.h drivers/*.h)
OBJ = ${C_SOURCES:.c=.o kernel/interrupt_table.o}
CC = /home/itarato/code/os/i386elfgcc/bin/i386-elf-gcc
# GDB = /home/itarato/code/os/i386elfgcc/bin/i386-elf-gdb
GDB = /usr/bin/gdb
CFLAGS = -g -Wall -Wextra -ffreestanding -fno-exceptions -pedantic -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -m32
QEMU = qemu-system-i386

os-image.bin: boot/boot.bin kernel.bin
	cat $^ > $@

kernel.bin: kernel.elf
	i386-elf-objcopy -O binary $^ $@

kernel.elf: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x100000 $^

kernel.dis: kernel.bin
	ndisasm -b 32 $< > $@

run: os-image.bin
	${QEMU} -drive format=raw,media=disk,file=$<,index=0,if=floppy -hda maindrive.img

debug: os-image.bin kernel.elf
	${QEMU} -s -S -drive format=raw,media=disk,file=$<,index=0,if=floppy -hda maindrive.img &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf" -ex "tui enable" -ex "layout split" -ex "focus cmd"

%.o: %.c ${C_HEADERS}
	${CC} ${CFLAGS} -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

build: os-image.bin
	echo Pass

clean:
	rm -rf *.bin *.o *.dis *.elf
	rm -rf kernel/*.o boot/*.bin boot/*.o
