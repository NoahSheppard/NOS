src/
├── compile.sh
├── run.sh
├── README.md
├── .gitignore
│
├── boot/
│   ├── grub.cfg
│   └── multiboot.s       # Move multiboot header here
│
├── kernel/
│   ├── start.s           # Keep entry point
│   ├── kernel.c          # Main kernel
│   ├── system.h          # Main system header
│   └── linker.ld
│
├── arch/i386/            # Architecture-specific code
│   ├── gdt.c
│   ├── idt.c
│   ├── irq.c
│   └── isrs.c
│
├── drivers/              # Hardware drivers
│   ├── timer.c
│   ├── keyboard.c        # Rename from kb.c
│   └── vga.c            # Move VGA functions here
│
├── libc/                 # Standard C library
│   ├── string.c          # Move strcmpl, strlen here
│   ├── stdio.c           # Move printf, term_print here
│   ├── stdlib.c          # Move itoa here
│   └── utils.c
│
├── include/              # All header files
│   ├── kernel/
│   │   ├── system.h
│   │   ├── gdt.h
│   │   ├── idt.h
│   │   └── irq.h
│   ├── drivers/
│   │   ├── timer.h
│   │   ├── keyboard.h
│   │   └── vga.h
│   ├── libc/
│   │   ├── string.h
│   │   ├── stdio.h
│   │   ├── stdlib.h
│   │   └── stdint.h
│   └── limits.h
│
├── apps/                 # User applications/shells
│   └── basicterm.c
│
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg
│
└── out/                  # Build output
    ├── *.o
    ├── kernel.elf
    └── kernel.iso


# Updated compile.sh
KERNEL_SOURCES="kernel/*.c arch/i386/*.c drivers/*.c libc/*.c"
APP_SOURCES="apps/*.c"
INCLUDE_DIRS="-Iinclude"

$HOME/opt/cross/bin/i686-elf-gcc $INCLUDE_DIRS -c $KERNEL_SOURCES $APP_SOURCES