MCUFLAGS = -mthumb -mcpu=cortex-m4
CFLAGS = -Iinclude $(MCUFLAGS)

all:
	arm-none-eabi-as $(MCUFLAGS) startup_stm32l476xx.s -c -o out/startup_stm32l476xx.o
	arm-none-eabi-gcc $(CFLAGS) system_stm32l4xx.c -c -o out/system_stm32l4xx.o
	arm-none-eabi-gcc $(CFLAGS) stm32l4xx_it.c -c -o out/stm32l4xx_it.o
	arm-none-eabi-gcc $(CFLAGS) main.c -c -o out/main.o
	arm-none-eabi-gcc $(CFLAGS) -T link.ld out/*.o -o out/main.elf
	arm-none-eabi-objcopy -O ihex out/main.elf main.hex

clean:
	rm -rf out/*
