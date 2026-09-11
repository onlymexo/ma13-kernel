#!/bin/bash
set -e

echo "=== Compilazione MA13 Kernel ==="

# 1. Compila l'Assembly
nasm -f elf32 src/boot.asm -o iso/boot/boot.o

# 2. Compila il C con -fno-stack-protector per evitare crash con ld
gcc -m32 -c src/kernel.c -o iso/boot/kernel.o -std=gnu99 -ffreestanding -fno-stack-protector -O2 -Wall -Wextra

# 3. Linka i file
ld -m elf_i386 --no-warn-rwx-segments -T src/linker.ld -o iso/boot/ma13_kernel.bin iso/boot/boot.o iso/boot/kernel.o

# 4. Configura GRUB
cat << EOF > iso/boot/grub/grub.cfg
set timeout=5
set default=0

menuentry "MA13 Kernel" {
    multiboot /boot/ma13_kernel.bin
    boot
}
EOF

# 5. Crea l'ISO
grub-mkrescue -o ma13-kernel.iso iso/

echo "✅ ISO Creata! Avvio QEMU..."
qemu-system-x86_64 -cdrom ma13-kernel.iso -display gtk
