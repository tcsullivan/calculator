#!/bin/bash

openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg \
	-c "init; reset halt; flash write_image erase main.hex; reset run; exit"

#openocd -f /usr/share/openocd/scripts/board/st_nucleo_l476rg.cfg > /dev/null &
#gdb-multiarch 
