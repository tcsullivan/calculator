CROSS = arm-none-eabi-
CC = gcc
AS = as
AR = ar
OBJCOPY = objcopy

MCUFLAGS = -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
AFLAGS = $(MCUFLAGS) 
CFLAGS = $(MCUFLAGS) -ggdb \
	-Iinclude -Iinclude/it \
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

all: $(OUT)

$(OUT): $(OFILES) initrd/init libinterp.a
	@echo "  LINK   " $(OUT)
	@./mkinitrd.sh
	@$(CROSS)$(OBJCOPY) -B arm -I binary -O elf32-littlearm initrd.img out/initrd.img.o
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


