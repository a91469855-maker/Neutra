# Neutra OS

A minimal 32-bit x86 operating system with GRUB bootloader.

## Quick Start

### Running the OS

```bash
make clean
make run-iso
```

## Creating Custom DCE Programs

DCE (Declarative Code Expression) is a lightweight assembly-like language for Neutra OS.

### Example DCE Program

```asm
.header
    var mode = 0x43444501
    
.section code_
code_top:
    print "Hello World!" -n
code_bottom:

halt
```

The DCE syntax is similar to assembly but with a lighter, more declarative approach.

## Compiling DCE to CDE(Compiled Dynamic Executable (soon counting as dynamic))

### Prerequisites
- Ensure you're in the project root directory

### Compilation Steps

1. **Compile DCE program to CDE:**
   ```bash
   python compiler.py program.dce program.cde
   ```

2. **Move the compiled file:**
   ```bash
   cp program.cde src/kernel/kernel_memory/
   ```

3. **Generate C header file:**
   ```bash
   cd src/kernel/kernel_memory
   xxd program.cde program.h
   ```

4. **Execute in shell:**
   ```bash
   execute
   ```

## File Structure

- `program.dce` - Source code (DCE format)
- `program.cde` - Compiled binary (Compiled DCE)
- `program.h` - C header file (hex dump via xxd)
- `src/kernel/kernel_memory/` - Kernel memory location for program files

## Example Pic in shell
![Example Pic](Pic/Example_shell.png)

