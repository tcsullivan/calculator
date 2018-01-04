# calculator
An OS for an STM32-based calculator  
  
Trying to make an operating system for a calculator based on the STM32L476RG, using its associated Nucleo board. Starting from scratch because it's fun.  
  
Required packages:
* arm-none-eabi toolchain
* openocd
* make
  
Use ```run.sh``` to upload the final output to the processor.  
  
To make an initrd, add files to ```initrd/``` and then run ```./mkinitrd.sh```.
