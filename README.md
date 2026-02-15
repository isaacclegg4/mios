# SimpleOS - A Minimal Operating System

A minimal x86 operating system built with Assembly, C/C++, that runs in QEMU or VirtualBox.

## Features

- **x86 Real Mode Bootloader**: Loads at 0x7C00 and initializes the system
- **Protected Mode Kernel**: 32-bit protected mode kernel written in C
- **VGA Text Display**: Green-on-black terminal output at 80x25 resolution
- **Minimal Libc Functions**: Custom implementations of outb, inb, print, and memory functions

## Prerequisites

- NASM (Netwide Assembler)
- GCC (with 32-bit support: `gcc -m32`)
- GNU Make
- QEMU or VirtualBox

### Ubuntu/Debian Installation
```bash
sudo apt-get install nasm gcc-multilib make qemu-system-i386
```

### macOS Installation
```bash
brew install nasm binutils gcc
# Install QEMU
brew install qemu
```

### Windows Installation
- Download and install NASM from https://www.nasm.us/
- Install MinGW or use Windows Subsystem for Linux (WSL)
- Install QEMU from https://www.qemu.org/download/

## Building

```bash
make clean
make all
```
