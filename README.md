# Neutra

## About

**Neutra** is a personal operating system project developed by a single developer. Its goal is to explore operating system development by building a unique operating system from scratch while learning how computers work at the lowest level.

The project focuses on implementing core operating system components such as the boot process, kernel, memory management, drivers, scheduling, and other low-level system features.

Neutra uses the **Limine Bootloader** and is designed to support unique features, including the ability to automatically launch Assembly-based programs during startup while allowing users to return to the main operating system environment.

> **Note:** Neutra is currently in early development and is intended for educational and experimental purposes.

---

## Features

### Current

- Custom x86-64 kernel
- Limine Bootloader integration
- C and x86-64 Assembly codebase
- Low-level hardware interaction
- Modular kernel architecture

### Planned

- Custom memory management
- Driver framework
- FAT32 file system support
- User-space applications
- Networking
- Graphics system
- Graphical mode
- Improved security
- NSCB (Neutra Secure Core Boot)
- NPS (Neutra Priority Scheduler)
- NKP (Neutra Kernel Panic)
- Watchdog support
- Assembly-based startup applications

---

## Technologies

Neutra is built using:

- **C** — Kernel and system development
- **x86-64 Assembly** — Low-level code and boot process
- **Limine Bootloader** — System bootloader
- **GNU Make** — Build system
- **QEMU** — Virtual machine for testing

---

## Project Structure

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

## NSCB

**NSCB (Neutra Secure Core Boot)** is a planned security component designed to verify critical system components before the kernel starts.

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

## Building

### Requirements

- GCC cross compiler
- NASM
- GNU Make
- QEMU

### Build

```bash
make
```

### Run

```bash
make run
```

---

## Goals

The primary goal of Neutra is to learn operating system development by implementing every major component from scratch while creating a unique operating system architecture.

Future development includes:

- Better hardware support
- User-space applications
- FAT32 file system support
- Networking
- Graphics system
- Improved security
- Graphical mode
- Virtual memory
- Process management
- NSCB (Neutra Secure Core Boot)
- NPS (Neutra Priority Scheduler)
- NKP (Neutra Kernel Panic)
- Watchdog support
- Additional drivers and system services

---

## Contributing

Neutra is currently a personal project, but contributions, suggestions, ideas, bug reports, and feedback are always welcome.

---

## License

This project is licensed under the **GNU General Public License v3.0 (GPL-3.0)**.
