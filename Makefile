SRCDIR = src
ISODIR = isodir
GRUBDIR = isodir/boot/grub
CC = i686-elf-gcc
KERNEL = $(ISODIR)/boot/printf.bin
ISO = printf.iso

CFLAGS += -std=c99 -ffreestanding -O2 -Wall -Wextra -Werror -Wno-unused-function -I$(SRCDIR) -pedantic-errors -DDEBUG

OBJ = $(SRCDIR)/boot.o                                          \
      $(SRCDIR)/ctype/isdigit.o                                 \
      $(SRCDIR)/errno/__errno_location.o                        \
      $(SRCDIR)/kernel_main.o                                   \
      $(SRCDIR)/stdio/internal/init.o                           \
      $(SRCDIR)/stdio/internal/printf_char.o                    \
      $(SRCDIR)/stdio/internal/printf_char_string.o             \
      $(SRCDIR)/stdio/internal/printf_emit.o                    \
      $(SRCDIR)/stdio/internal/printf_impl.o                    \
      $(SRCDIR)/stdio/internal/printf_unimplemented_specifier.o \
      $(SRCDIR)/stdio/printf.o                                  \
      $(SRCDIR)/stdio/putchar.o                                 \
      $(SRCDIR)/stdio/puts.o                                    \
      $(SRCDIR)/stdlib/itoa.o                                   \
      $(SRCDIR)/string/memcpy.o                                 \
      $(SRCDIR)/string/memset.o                                 \
      $(SRCDIR)/string/strchr.o                                 \
      $(SRCDIR)/string/strcmp.o                                 \
      $(SRCDIR)/string/strlen.o                                 \
      $(SRCDIR)/string/strpbrk.o                                \
      $(SRCDIR)/term.o                       

.PHONY: image kernel $(ISO) clean

all: image

image: $(KERNEL) $(GRUBDIR)/grub.cfg
	grub2-file --is-x86-multiboot $(KERNEL)
	grub2-mkrescue -o $(ISO) isodir

kernel: $(KERNEL)

$(KERNEL): $(OBJ) $(GRUBDIR)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib $(OBJ)

$(GRUBDIR):
	mkdir -p $@

$(GRUBDIR)/grub.cfg: $(GRUBDIR)
	touch $@
	echo 'menuentry "printf" { multiboot /boot/printf.bin }' >> $@

clean:
	rm -rf $(OBJ)
	rm -f $(ISO)
	rm -rf $(ISODIR)

