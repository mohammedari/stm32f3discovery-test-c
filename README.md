stm32f3discovery-test
=====================

A test makefile project of stm32f3discovery. 
This project contains LED flashing sample source, serial communication sample code, 
makefile, linker script, gdb script and openocd script.

This project is written in C language and tested with "GNU Tools for ARM Embedded Processors" toolchain.

Build and Debug Steps
------
1. Build project
```bash
make
```

2. Run OpenOCD
```bash
cd openocd
sudo ./openocd.sh
```

3. Debug
```bash
make debug
```

Additional  Informations
------

gdb/gdb.script : gdb debugger script which includes flashing commands

ld/STM32F303VC_FLASH.ld : linker script which is optimized for stm32f303 mpu

openocd/openocd.sh : script which runs OpenOCD (needs root permission)
openocd/stm32f3discovery.cfg : OpenOCD configuration file for STM32F3Discovery
