# Neutra
An open-source desktop operating system written from scratch in mostly C and x86-64 Assembly.
> **Status:** Neutra is currently in active development and is **not yet ready for daily use**.

---

# About
**Neutra** is an independent desktop operating system built from scratch with the goal of becoming a modern, secure, and high-performance operating system for everyday desktop use.

Unlike projects based on existing operating systems, Neutra is developed independently and aims to implement its own architecture, technologies, and system components.

The project focuses on building every major part of an operating system, including the boot process, kernel, memory management, scheduling, drivers, security, file systems, networking, and a graphical desktop environment.

Neutra uses the **Limine Bootloader** and is designed to support unique technologies such as:
- **Safety Boot Menu** — In-OS boot configuration (no BIOS firmware access needed)
- **Assembly-based startup applications** — Direct hardware access at boot
- **Custom scheduler** — NPS (Neutra Priority Scheduler)
- **Watchdog system** — Process monitoring and recovery
- **NSCB** — Secure core boot verification

---

# Features

## Current (Phase 1)
- x86-64 support (Higher Half kernel at 0xFFFFFFFF80000000)
- Custom kernel in C
- Limine Bootloader 12.4.2 integration
- Boot sequence: Firmware → Limine → Neutra Kernel
- C and x86-64 Assembly codebase
- Low-level hardware interaction
- Custom linker script (higher-half memory)
- Multiboot2 header support
- Serial debugging (COM1)
- Basic scheduler framework (Round Robin)
- Interrupt handling structure (IDT/GDT)
- Modular project structure
- Build system (GNU Make + NASM + GCC)

## Planned (Phase 2-4)

### Bootloader & Security
- [ ] Safety Boot Menu (Ctrl+Shift+Esc in-OS boot menu)
- [ ] Assembly file loader at startup
- [ ] NSCB (Neutra Secure Core Boot)
- [ ] Secure Boot verification

### Graphics & Display
- [ ] Framebuffer graphics (currently broken - Limine boot_info passing issue)
- [ ] Text rendering engine
- [ ] Pixel drawing API
- [ ] Display driver framework
- [ ] Resolution detection
- [ ] Color depth support

### Memory Management
- [ ] Physical Memory Manager (PMM)
- [ ] Paging (Virtual Memory)
- [ ] Memory allocator (malloc/free)
- [ ] Heap management
- [ ] Memory protection

### Scheduling & Process Management
- [ ] NPS (Neutra Priority Scheduler)
- [ ] Context switching
- [ ] Process/Task states
- [ ] Priority levels
- [ ] Watchdog system (process recovery)
- [ ] Task supervision trees

### Interrupts & System Calls
- [ ] IDT fully implemented
- [ ] GDT fully implemented
- [ ] TSS (Task State Segment)
- [ ] System calls (syscalls)
- [ ] User mode (Ring 3) support
- [ ] Privilege level switching

### Drivers
- [ ] Keyboard driver (PS/2)
- [ ] Serial driver (UART)
- [ ] Disk driver (ATA/SATA)
- [ ] USB support
- [ ] Network card driver

### File System
- [ ] FAT32 support
- [ ] Custom Neutra FS
- [ ] VFS (Virtual File System)
- [ ] File operations (open, read, write, close)

### Networking
- [ ] Network stack
- [ ] TCP/IP support
- [ ] Ethernet driver
- [ ] Socket API

### User-Space
- [ ] System calls interface
- [ ] User-space applications
- [ ] C library (libc)
- [ ] Shell

### Desktop Environment
- [ ] Window manager
- [ ] Graphical interface
- [ ] Desktop applications
- [ ] Application framework

---

# Technologies
Neutra is built using:
- **C** — Kernel and system development
- **x86-64 Assembly** — Low-level programming and startup code
- **Limine Bootloader** — Modern bootloader
- **GNU Make** — Build system
- **NASM** — Assembler
- **GCC** — C compiler (freestanding)
- **ld (GNU Linker)** — Custom linker script
- **QEMU** — Testing environment (x86-64 emulation)

---

# Project Structure
```text
neutra-os/
├── src/
│   └── System Files/kernel/
│       ├── boot.asm              (Bootloader entry point)
│       ├── kernel.c              (Kernel main)
│       ├── scheduler.c           (Task scheduling)
│       ├── scheduler.h
│       ├── framebuffer.c         (Graphics - WIP)
│       ├── framebuffer.h
│       ├── interrupthandler.c    (Interrupt management)
│       ├── interrupthandler.h
│       └── limine.h              (Bootloader interface)
├── build/                        (Compiled output)
│   └── kernel.elf
├── iso/                          (ISO image structure)
├── limine-binary/                (Bootloader files)
├── linker.ld                     (Kernel linker script)
├── Makefile                      (Build system)
├── limine.conf                   (Boot configuration)
├── LICENSE
└── README.md
```

---

# Current Issues (Known Bugs)

## Framebuffer Graphics
- **Issue:** Limine boot_info.framebuffer is not passed correctly
- **Status:** Investigating Limine boot protocol
- **Next Steps:** Serial debugging, Limine request headers, or alternative bootloader

## Kernel Boot
- **Status:** Kernel loads via Limine but may not initialize properly
- **Debug:** Serial output not printing "KERNEL STARTED!"
- **Possible Causes:** Entry point issue, memory mapping, Limine protocol mismatch

---

# NSCB (Neutra Secure Core Boot)
**NSCB** is a planned security component that will verify system integrity at boot.

**Planned boot sequence:**
---
# Safety Boot Menu (Unique Feature)
Unlike traditional OS bootloaders that require BIOS/UEFI access (risky and intimidating), Neutra will provide an **in-OS boot menu**.

**Planned features:**
- **Ctrl+Shift+Esc** to enter boot menu during OS runtime
- Change boot order without BIOS
- Load and execute Assembly files at startup
- Memory inspector
- CPU register editor
- Settings manager
- Safe mode options
- No risk of bricking firmware

---

# Building

## Requirements
- GCC (freestanding, x86-64 target)
- NASM (Assembler)
- GNU Make
- GNU ld (Linker)
- QEMU (x86-64 emulation)
- xorriso (ISO creation)

## Build
```bash
make clean
make
```

## Run in QEMU
```bash
qemu-system-x86_64 -cdrom neutra.iso -boot d -m 256M -serial stdio
```

## Clean
```bash
make clean
```

---

# Development Roadmap

### Phase 1: Core Kernel (Current)
- ✓ Bootloader integration
- ✓ Kernel entry point
- ✓ Basic scheduler framework
- ⚠️ Framebuffer graphics (debugging)
- [ ] Serial debugging working
- [ ] Memory management basics
- [ ] Interrupt handling

### Phase 2: Essential Components
- [ ] Working graphics (pixels on screen)
- [ ] Text rendering engine
- [ ] Physical memory manager
- [ ] Virtual memory/paging
- [ ] Syscall interface
- [ ] User mode support
- [ ] Basic drivers (keyboard, disk)

### Phase 3: Advanced Features
- [ ] File system (FAT32)
- [ ] Networking stack
- [ ] Process management
- [ ] Watchdog system
- [ ] Safety Boot Menu
- [ ] Assembly app loader
- [ ] NSCB verification

### Phase 4: Desktop
- [ ] Graphics system
- [ ] Window manager
- [ ] Desktop environment
- [ ] User-space applications
- [ ] Shell
- [ ] Full application support

---

# Contributing
Neutra is primarily an independent project, but contributions, suggestions, bug reports, and feedback are always welcome.

Please report issues and suggest features via GitHub Issues.

---

# License
Neutra is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.

See LICENSE file for details.

---

# Quick Start
```bash
# Clone & Build
git clone https://github.com/yourusername/neutra-os.git
cd neutra-os

# Build kernel
make clean
make

# Run in QEMU
qemu-system-x86_64 -cdrom neutra.iso -boot d -m 256M -serial stdio
```

---

# Status
**Last Updated:** July 2026
**Current Phase:** Phase 1 (Core Kernel Development)
**Next Milestone:** Fix framebuffer graphics, implement working display
