# calculator
A from-scratch operating system for a graphing calculator.  
  
The goal of this project is to design a functional operating system for a
graphing calculator based on the STM32L476RG processor. This OS has been
designed from scratch to optimize for speed and code size, targeting the
features necessary for a graphing calculator application.  
  
Required packages:
* arm-none-eabi toolchain
* openocd
* make
  
Use ```run.sh``` to upload the final output to the processor.  
  
  
## design overview
The core of the operating system is written entirely in C and assembly. This
project is paired with the [interpreter project](https://code.bitgloo.com/clyne/interpreter)
(licensed under the GPL),
which parses script from C strings. The operating system exposes calls to the
script parser, and then loads the text file at ```initrd/init```. 
