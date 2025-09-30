# NOS

## Overview
NOS is a minimal educational operating system kernel written in C and x86 assembly. It is designed to be booted via GRUB in 32-bit protected mode. The kernel demonstrates the fundamentals of low-level system initialization, interrupt handling, and simple user interaction through a terminal interface.

---

## Boot Sequence
On startup, NOS follows a structured initialization routine. The steps are logged directly to the terminal with color-coded messages for clarity:

1. Initialize the terminal for text output  
2. Install and load the Global Descriptor Table (GDT) to define memory segments  
3. Install and load the Interrupt Descriptor Table (IDT) to prepare for exception and interrupt handling  
4. Install CPU exception handlers (ISRs)  
5. Install and enable hardware interrupt handlers (IRQs) with the Programmable Interrupt Controller remapped and interrupts unmasked  
6. Install the system timer to provide periodic ticks for timekeeping and scheduling primitives  
7. Install the keyboard driver for user input  
8. Drop into the shell prompt once initialization is complete  

---

## Features
- **Terminal driver** for VGA text mode, capable of colored output and basic line management  
- **Global Descriptor Table (GDT)** providing flat 32-bit memory addressing  
- **Interrupt Descriptor Table (IDT)** configured for CPU exceptions and hardware interrupts  
- **ISR handlers** for CPU faults and traps  
- **IRQ subsystem** for handling hardware devices, with interrupts enabled after setup  
- **Programmable Interval Timer (PIT)** configured for basic timekeeping  
- **Keyboard driver** supporting scancode processing and character input  
- **Shell environment** with minimal command support  

---

## Shell Commands
- **`clear`** — clears the terminal screen and resets the cursor to the top  
- **`echo <text>`** — prints the provided text to the terminal  

Both commands are implemented with minimal overhead and are intended as examples of extending user-level interaction.

---

## Requirements
- GRUB or a GRUB-compatible bootloader to load the kernel  
- i386-compatible CPU or emulator such as QEMU or Bochs  

---

## Testing
The kernel is typically tested in QEMU for rapid iteration. Booting on bare metal is possible but requires a proper GRUB setup on a bootable medium.

---

## Limitations
- Only VGA text mode is supported  
- No memory management, filesystem, or multitasking is implemented  
- Shell is limited to two commands  
- Drivers are minimal and blocking  

---

## Future Work
- Expand the shell with more commands  
- Add dynamic memory management  
- Implement a simple filesystem layer  
- Provide multitasking and scheduling  
- Improve keyboard input with buffering and editing support  
