#!/usr/bin/env bash
set -euo pipefail

SRCDIR="$(cd "$(dirname "$0")" && pwd)"
OUT="$SRCDIR/out"
CC="${CC:-$HOME/opt/cross/bin/i686-elf-gcc}"
GRUB_MKRESCUE="${GRUB_MKRESCUE:-grub-mkrescue}"

# cleanup
rm -f "$OUT"/mykernel.iso "$OUT"/*.o "$SRCDIR"/mykernel.elf "$SRCDIR"/mykernel.iso
mkdir -p "$OUT"

echo "Using CC=$CC"
if ! command -v "$CC" >/dev/null 2>&1; then
  echo "ERROR: cross-compiler $CC not found"; exit 1
fi

# include dirs (adjust if you add more)
INCLUDES="-I$SRCDIR/include -I$SRCDIR/kernel -idirafter $SRCDIR/include/libc"
CFLAGS="-std=gnu99 -ffreestanding -fshort-wchar -fno-builtin -m32 -g $INCLUDES"

# assemble start.s first
echo "Assembling kernel/start.s -> out/start.o"
"$CC" -m32 -ffreestanding -c "$SRCDIR/kernel/start.s" -o "$OUT/start.o"

# collect and compile C sources
SRC_GLOBS=(
  "$SRCDIR/kernel/"*.c
  "$SRCDIR/arch/i386/"*.c
  "$SRCDIR/drivers/"*.c
  "$SRCDIR/libc/"*.c
  "$SRCDIR/apps/"*.c
)

OBJECTS=()
for src in "${SRC_GLOBS[@]}"; do
  [ -f "$src" ] || continue
  # produce unique object name preserving path segments (slashes -> underscores)
  rel="${src#$SRCDIR/}"
  objname="${rel//\//_}"
  obj="$OUT/${objname%.*}.o"
  echo "Compiling $src -> $obj"
  "$CC" $CFLAGS -c "$src" -o "$obj"
  OBJECTS+=("$obj")
done

echo "Linking mykernel.elf"
# ensure start.o is first on command line
LINKER_SCRIPT="$SRCDIR/kernel/linker.ld"
if [ ! -f "$LINKER_SCRIPT" ]; then
  echo "ERROR: linker script $LINKER_SCRIPT not found"; exit 1
fi

# Link with gcc to get proper start file, force ELF32 output
"$CC" -m32 -ffreestanding -nostdlib -g -Wl,-T,"$LINKER_SCRIPT" -Wl,--oformat,elf32-i386 \
  "$OUT/start.o" "${OBJECTS[@]}" -o "$SRCDIR/mykernel.elf" -lgcc

echo "Copying kernel to iso and building grub image"
cp "$SRCDIR/mykernel.elf" "$SRCDIR/iso/boot/kernel.bin"
if ! command -v "$GRUB_MKRESCUE" >/dev/null 2>&1; then
  echo "ERROR: $GRUB_MKRESCUE not found; install grub-efi or grub2 tools"; exit 1
fi
"$GRUB_MKRESCUE" -o "$OUT/mykernel.iso" "$SRCDIR/iso"

# cleanup intermediate install
rm -f "$SRCDIR/iso/boot/kernel.bin"
echo "Build complete: $OUT/mykernel.iso"