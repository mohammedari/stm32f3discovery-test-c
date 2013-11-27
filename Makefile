PROJ_NAME = stm32-serialtest-firmware

##########################################
# Toolchain Settings
##########################################
TC = arm-none-eabi

CC      = $(TC)-gcc
OBJCOPY = $(TC)-objcopy
OBJDUMP = $(TC)-objdump
SIZE    = $(TC)-size
DBG     = $(TC)-gdb

##########################################
# Files 
##########################################
USER_SRCS      = $(wildcard src/*.c)
USER_SRCS_CXX  = $(wildcard src/*.cpp)
STDPERIPH_SRCS = $(wildcard Libraries/STM32F30x_StdPeriph_Driver/src/*.c)
STARTUP_SRC    = $(wildcard Libraries/CMSIS/Device/ST/STM32F30x/Source/*.c)
STARTUP_SRC_S  = $(wildcard Libraries/CMSIS/Device/ST/STM32F30x/Source/*.s)
BOARD_SRC      = $(wildcard Libraries/STM32F3_Discovery/*.c)

OBJS       = $(USER_SRCS:.c=.o) \
               $(USER_SRCS_CXX:.cpp=.o) \
               $(STDPERIPH_SRCS:.c=.o) \
               $(STARTUP_SRC:.c=.o) \
               $(STARTUP_SRC_S:.s=.o) \
               $(BOARD_SRC:.c=.o)

INCLUDES   = -ILibraries/STM32F30x_StdPeriph_Driver/inc/ \
               -ILibraries/CMSIS/Include/ \
               -ILibraries/CMSIS/Device/ST/STM32F30x/Include/ \
               -ILibraries/STM32F3_Discovery/ \
               -ILibraries/

LIBS       = -lm -lc

##########################################
# Flag Settings 
##########################################
MCU        = -mthumb -mcpu=cortex-m4
FPU        = -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
DEFINES    = -DSTM32F3XX -DUSE_STDPERIPH_DRIVER \
             -DSTDOUT_USART=1 -DSTDERR_USART=1 -DSTDIN_USART=1

CFLAGS     = $(MCU) $(FPU) $(DEFINES) $(INCLUDES) \
               -g -Wall -O0 -ffunction-sections -fdata-sections
ASFLAGS    = $(MCU) $(FPU) -g -Wa,--warn -x assembler-with-cpp
LDFLAGS    = $(MCU) $(FPU) -g -gdwarf-2 \
               -Tld/STM32F303VC_FLASH.ld \
               -Xlinker --gc-sections -Wl,-Map=$(PROJ_NAME).map \
               $(LIBS) -o \
               $(PROJ_NAME).elf \
               --specs=nano.specs

##########################################
# Flash Settings
##########################################
FLASH = $(FLASH_SPAWN) $(FLASH_HALT) $(FLASH_WRITE) $(FLASH_EXIT) 
FLASH_SPAWN = spawn telnet localhost 4444; 
FLASH_HALT  = expect \"> \"; send \"reset halt\n\";
FLASH_WRITE = expect \"> \"; send \"flash write_image erase ../$(PROJ_NAME).elf\n\";
FLASH_EXIT  = expect \"> \"; send \"exit\n\"; interact

##########################################
# Targets
##########################################
all: $(PROJ_NAME).bin info

$(PROJ_NAME).elf: $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS)
	@echo $@

$(PROJ_NAME).bin: $(PROJ_NAME).elf
	@$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin
	@echo $@

info: $(PROJ_NAME).elf
	@$(SIZE) --format=berkeley $(PROJ_NAME).elf

.s.o:
	@$(CC) $(ASFLAGS) -c -o $@ $<
	@echo $@

.c.o:
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

.cpp.o:
	@$(CC) $(CFLAGS) -c -o $@ $<
	@echo $@

clean:
	rm -f $(OBJS)
	rm -f $(PROJ_NAME).elf
	rm -f $(PROJ_NAME).bin
	rm -f $(PROJ_NAME).map
	rm -f .flash

debug: .flash
	$(DBG) -x gdb/gdb.script \
          -s $(PROJ_NAME).elf \
          -interpreter=mi2

flash: .flash
.flash: $(PROJ_NAME).elf
	expect -c "$(FLASH)" 
	touch .flash
