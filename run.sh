#!/bin/bash

#openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg \
#	-c "init; reset halt; flash write_image erase main.hex; reset run; exit"

openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg > /dev/null &
gdb-multiarch -iex "target remote localhost:3333" out/main.elf
