# Neutra

An open-source desktop operating system written from scratch in mostly C and x86-64 Assembly.

> **Status:** Neutra is currently in active development and is **not yet ready for daily use**.

---

# About

**Neutra** is an independent desktop operating system built from scratch with the goal of becoming a modern, secure, and high-performance operating system for everyday desktop use.

Unlike projects based on existing operating systems, Neutra is developed independently and aims to implement its own architecture, technologies, and system components.

The project focuses on building every major part of an operating system, including the boot process, kernel, memory management, scheduling, drivers, security, file systems, networking, and a graphical desktop environment.

Neutra uses the **Limine Bootloader** and is designed to support unique technologies such as Assembly-based startup applications, its own scheduler, custom security systems, and many other original features.

---

# Features

## Current

- x86-64 support
- Custom kernel
- Limine Bootloader integration
- C and x86-64 Assembly codebase
- Low-level hardware interaction
- Modular project structure

## Planned

- Custom memory management
- Driver framework
- User-space applications
- FAT32 file system support
- Networking
- Graphics system
- Desktop environment
- Virtual memory
- Process management
- System calls
- Dynamic memory allocator
- NSCB (Neutra Secure Core Boot)
- NPS (Neutra Priority Scheduler)
- NKP (Neutra Kernel Panic)
- Watchdog support
- Assembly-based startup applications

---

# Technologies

Neutra is built using:

- **C** — Kernel and system development
- **x86-64 Assembly** — Low-level programming and startup code
- **Limine Bootloader** — Bootloader
- **GNU Make** — Build system
- **QEMU** — Testing environment

---

# Project Structure

```text
Neutra/
├── boot/
│   └── boot.asm
├── kernel/
│   ├── main.c
│   └── scheduler.c
├── drivers/
├── security/
│   └── nscb.asm
├── include/
├── Makefile
├── LICENSE
└── README.md
```

---

# NSCB

**NSCB (Neutra Secure Core Boot)** is a planned security component responsible for verifying important system components before the kernel starts.

Planned boot sequence:

```text
Firmware
   │
   ▼
Limine Bootloader
   │
   ▼
NSCB
   │
   ▼
Neutra Kernel
```

---

# Building

## Requirements

- GCC Cross Compiler
- NASM
- GNU Make
- QEMU

## Build

```bash
make
```

## Run

```bash
make run
```

---

# Goals

The long-term goal of Neutra is to become a fully featured desktop operating system suitable for everyday use.

Future development includes:

- Better hardware support
- User-space applications
- FAT32 and additional file system support
- Networking
- Graphics system
- Desktop environment
- Improved security
- Virtual memory
- Process management
- Driver ecosystem
- NSCB (Neutra Secure Core Boot)
- NPS (Neutra Priority Scheduler)
- NKP (Neutra Kernel Panic)
- Watchdog support

---

# Roadmap

### Phase 1
- Bootloader integration
- Basic kernel
- Memory management
- Text terminal

### Phase 2
- Scheduler
- Drivers
- Virtual memory
- File system support

### Phase 3
- User-space
- System calls
- Networking
- Security improvements

### Phase 4
- Desktop environment
- Graphics
- Applications
- Performance optimization

---

# Contributing

Neutra is primarily an independent project, but contributions, suggestions, bug reports, and feedback are always welcome.

---

# License

Neutra is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.
