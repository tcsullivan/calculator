CROSS = arm-none-eabi-
CC = gcc
AS = as
AR = ar
OBJCOPY = objcopy
STRIP = strip

MCUFLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
AFLAGS = $(MCUFLAGS) 
CFLAGS = $(MCUFLAGS) -ggdb \
	-Iinclude -Iinclude/it -Iinclude/cmsis \
	-fno-builtin -fsigned-char -ffreestanding \
	-Wall -Werror -Wextra -pedantic \
	-Wno-overlength-strings -Wno-discarded-qualifiers
LFLAGS = -T link.ld

CFILES = $(wildcard src/*.c)
AFILES = $(wildcard src/*.s) 

OUTDIR = out
OFILES = $(patsubst src/%.c, $(OUTDIR)/%.o, $(CFILES)) \
	 $(patsubst src/%.s, $(OUTDIR)/%.asm.o, $(AFILES))

OUT = out/main.elf
INITRD = initrd.img

all: $(OUT)

#@$(CROSS)$(STRIP) --only-keep-debug $(OUT)
$(OUT): $(OFILES) initrd/init libinterp.a
	@echo "  INITRD " $(INITRD)
	@rm -f $(INITRD)
	@$(CROSS)$(AR) r $(INITRD) initrd/*
	@$(CROSS)$(OBJCOPY) -B arm -I binary -O elf32-littlearm $(INITRD) out/initrd.img.o
	@echo "  LINK   " $(OUT)
	@$(CROSS)$(CC) $(CFLAGS) $(LFLAGS) out/*.o -o $(OUT) -L. -linterp

$(OUTDIR)/%.o: src/%.c
	@echo "  CC     " $<
	@$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/%.asm.o: src/%.s
	@echo "  AS     " $<
	@$(CROSS)$(AS) $(AFLAGS) -c $< -o $@

clean:
	@echo "  CLEAN"
	@rm -rf out/*


