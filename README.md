# calculator
A from-scratch operating system for a graphing calculator.  
  
The goal of this project is to design a functional operating system for a graphing calculator, currently based on the STM32L476RG processor. This OS is designed from scratch to optimize for speed and code size, targeting the features necessary for a graphing calculator application.  
  
Required packages:
* arm-none-eabi toolchain
* openocd
* make
  
Use ```make``` to build the OS, which produces an ```out/main.elf```. Use ```run.sh``` to launch openocd and a gdb session to upload the OS to the device.  
  
  
## design overview
  
Soon: a link to my website with info on the actual device, and its hardware layout.  
  
The core of the operating system is written entirely in C and assembly. This project is paired with the [interpreter project](https://code.bitgloo.com/clyne/interpreter) (licensed under the GPL), a simple low-memory scripting language. Apart from other core functions, the operating system's goal is to expose calls to the interpreter, and then load the ```init``` script from the calculator.
