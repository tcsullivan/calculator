CROSS = arm-none-eabi-
CC = gcc
AS = as
AR = ar
OBJCOPY = objcopy

MCUFLAGS = -mthumb -mcpu=cortex-m4
AFLAGS = $(MCUFLAGS) 
CFLAGS = $(MCUFLAGS) -Iinclude -ffreestanding -Wall -Werror -Wextra
OFLAGS = -O ihex

CFILES = $(wildcard src/*.c)
AFILES = $(wildcard src/*.s) 

OUTDIR = out
OFILES = $(patsubst src/%.c, $(OUTDIR)/%.o, $(CFILES)) \
	 $(patsubst src/%.s, $(OUTDIR)/%.asm.o, $(AFILES))

LIBDIR = -Llib
LIBS = -llua

HEX = main.hex

all: $(HEX)

$(HEX): $(OFILES)
	@echo "  LINK   " $(HEX)
	@$(CROSS)$(OBJCOPY) -B arm -I binary -O elf32-littlearm initrd.img out/initrd.img.o
	@$(CROSS)$(CC) $(CFLAGS) $(LIBDIR) $(LIBS) -T link.ld out/*.o -o out/main.elf
	@$(CROSS)$(OBJCOPY) $(OFLAGS) out/main.elf $(HEX)

$(OUTDIR)/%.o: src/%.c
	@echo "  CC     " $<
	@$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.asm.o: src/%.s
	@echo "  AS     " $<
	@$(CROSS)$(AS) $(AFLAGS) -c $< -o $@

clean:
	@echo "  CLEAN"
	@rm -rf out/*


