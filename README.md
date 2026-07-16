
## About

**Neutra** is a personal operating system project created by one person with the goal of building a unique operating system with custom features and low-level technologies.

The project focuses on understanding how operating systems work by developing components such as boot processes, kernels, memory management, drivers, and system-level features.

Neutra uses the **Limine bootloader** to handle the boot process and aims to support features such as automatically launching Assembly-based programs during startup while still allowing users to return to the main OS environment.

## Features

Current and planned features:

* Custom kernel development
* Limine bootloader integration
* x86-64 support
* Low-level programming with C and Assembly
* Custom memory management
* Hardware interaction
* Driver development
* Custom security features (NSCB - Neutra Secure Core Boot)
* Assembly-based startup applications

## Technologies

Neutra is built using:

* **C** - Kernel and system development
* **Assembly (x86-64)** - Low-level operations and startup code
* **Limine** - Bootloader
* **Make** - Build system

## Project Structure


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


## NSCB

**NSCB (Neutra Secure Core Boot)** is a planned security layer for Neutra.

Its purpose is to create a verified boot process by checking important system components before starting the kernel.

Example boot chain:


Firmware
   |
   v
Limine Bootloader
   |
   v
NSCB
   |
   v
Neutra Kernel


## Building

Requirements:

* GCC cross compiler
* NASM
* Make
* QEMU (for testing)

Build:

bash:
make


Run with QEMU:

bash:
make run


## Goals

The main goal of Neutra is to explore operating system development and create a unique OS architecture while learning how computers work at the lowest level.

Future goals include:

* Better hardware support
* User space applications
* File system support
* Networking
* Graphics system
* Improved security
* Graphical Mode
* NSCB

## Contributing

Neutra is currently a personal development project, but contributions, ideas, and feedback are welcome.

## License

Neutra is licensed under the GPL-3.0 License.

```
```
