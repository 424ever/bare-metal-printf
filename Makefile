SRCDIR = src
OBJDIR = obj
GENDIR = gen
TOOLSDIR = tools
ISODIR = isodir
GRUBDIR = isodir/boot/grub
CC = i686-elf-gcc
KERNEL = $(ISODIR)/boot/printf.bin
ISO = printf.iso
GRUB_PREFIX ?= grub2
CONSOLE ?= vga

CFLAGS += -ffreestanding -O2 -Wall -Wextra -Werror -Iinclude -pedantic-errors -DDEBUG -Wno-unused-function

OBJ = $(OBJDIR)/boot.o                                          \
      $(OBJDIR)/ctype/isdigit.o                                 \
      $(OBJDIR)/ctype/isupper.o                                 \
      $(OBJDIR)/errno/errno_location.o                          \
      $(OBJDIR)/io/inb.o                                        \
      $(OBJDIR)/io/outb.o                                       \
      $(OBJDIR)/kernel_main.o                                   \
      $(OBJDIR)/stdio/internal/init.o                           \
      $(OBJDIR)/stdio/internal/printf_char.o                    \
      $(OBJDIR)/stdio/internal/printf_char_string.o             \
      $(OBJDIR)/stdio/internal/printf_decimal.o                 \
      $(OBJDIR)/stdio/internal/printf_emit.o                    \
      $(OBJDIR)/stdio/internal/printf_impl.o                    \
      $(OBJDIR)/stdio/internal/printf_num.o                     \
      $(OBJDIR)/stdio/internal/printf_unimplemented_specifier.o \
      $(OBJDIR)/stdio/internal/printf_unsigned.o                \
      $(OBJDIR)/stdio/printf.o                                  \
      $(OBJDIR)/stdio/putchar.o                                 \
      $(OBJDIR)/stdio/puts.o                                    \
      $(OBJDIR)/stdio/snprintf.o                                \
      $(OBJDIR)/stdio/sprintf.o                                 \
      $(OBJDIR)/stdio/vsnprintf.o                               \
      $(OBJDIR)/stdio/vsprintf.o                                \
      $(OBJDIR)/stdlib/itoa.o                                   \
      $(OBJDIR)/string/memcpy.o                                 \
      $(OBJDIR)/string/memset.o                                 \
      $(OBJDIR)/string/strchr.o                                 \
      $(OBJDIR)/string/strcmp.o                                 \
      $(OBJDIR)/string/strlen.o                                 \
      $(OBJDIR)/string/strpbrk.o                                \
      $(OBJDIR)/term/$(CONSOLE)/term.o                          \
      $(OBJDIR)/term/term_common.o                              \
      $(OBJDIR)/tests.o

.PHONY: image kernel $(ISO) clean

all: image

$(GENDIR)/tests.c: printf-tests.txt $(TOOLSDIR)/create-tests-c
	mkdir -p $(GENDIR)
	$(TOOLSDIR)/create-tests-c printf-tests.txt $(GENDIR)/tests.c

$(TOOLSDIR)/create-tests-c: $(TOOLSDIR)/create-tests-c.c
	cc -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJDIR)/%.o: $(GENDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $(CFLAGS) $<

image: $(KERNEL) $(GRUBDIR)/grub.cfg
	$(GRUB_PREFIX)-file --is-x86-multiboot $(KERNEL)
	$(GRUB_PREFIX)-mkrescue -o $(ISO) isodir

printf-tests.txt:
	wget -O printf-tests.txt "https://raw.githubusercontent.com/BartMassey/printf-tests/master/printf-tests.txt"

kernel: $(KERNEL)

$(KERNEL): $(OBJ) $(GRUBDIR)
	$(CC) -T linker.ld -o $@ -ffreestanding -nostdlib $(OBJ) -lgcc

$(GRUBDIR):
	mkdir -p $@

$(GRUBDIR)/grub.cfg: $(GRUBDIR)
	touch $@
	echo 'menuentry "printf" { multiboot /boot/printf.bin }' >> $@

clean:
	rm -rf $(OBJDIR)
	rm -rf $(GENDIR)
	rm -f $(ISO)
	rm -rf $(ISODIR)
	rm -f printf-tests.txt

