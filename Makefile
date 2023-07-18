SRCDIR = src
OBJDIR = obj
ISODIR = isodir
GRUBDIR = isodir/boot/grub
CC = i686-elf-gcc
KERNEL = $(ISODIR)/boot/printf.bin
ISO = printf.iso

CFLAGS += -std=c99 -ffreestanding -O2 -Wall -Wextra -Werror -Wno-unused-function -Iinclude -pedantic-errors -DDEBUG

OBJ = $(OBJDIR)/boot.o                                          \
      $(OBJDIR)/ctype/isdigit.o                                 \
      $(OBJDIR)/errno/__errno_location.o                        \
      $(OBJDIR)/kernel_main.o                                   \
      $(OBJDIR)/stdio/internal/init.o                           \
      $(OBJDIR)/stdio/internal/printf_char.o                    \
      $(OBJDIR)/stdio/internal/printf_char_string.o             \
      $(OBJDIR)/stdio/internal/printf_emit.o                    \
      $(OBJDIR)/stdio/internal/printf_impl.o                    \
      $(OBJDIR)/stdio/internal/printf_unimplemented_specifier.o \
      $(OBJDIR)/stdio/printf.o                                  \
      $(OBJDIR)/stdio/putchar.o                                 \
      $(OBJDIR)/stdio/puts.o                                    \
      $(OBJDIR)/stdio/snprintf.o                                \
      $(OBJDIR)/stdio/sprintf.o                                 \
      $(OBJDIR)/stdlib/itoa.o                                   \
      $(OBJDIR)/string/memcpy.o                                 \
      $(OBJDIR)/string/memset.o                                 \
      $(OBJDIR)/string/strchr.o                                 \
      $(OBJDIR)/string/strcmp.o                                 \
      $(OBJDIR)/string/strlen.o                                 \
      $(OBJDIR)/string/strpbrk.o                                \
      $(OBJDIR)/term.o                       

.PHONY: image kernel $(ISO) clean

all: image

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

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
	rm -rf $(OBJDIR)
	rm -f $(ISO)
	rm -rf $(ISODIR)

