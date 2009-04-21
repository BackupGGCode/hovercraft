# Name: Makefile
# Author: Marco Yuen
# Copyright: Copyright 2009 Marco Yuen
# License: ...

#------------------------------------------------------------------------------
# Project Source
#------------------------------------------------------------------------------
OS_SOURCE_FILES = OS/crt0.c \
				  OS/os.c

BASE_SOURCE_FILES = basestation.c \
					uart/uart.c
					
H1_SOURCE_FILES = hovercraft1.c \
				  motor/DualMotors.c \
				  uart/uart.c \
				  radio/radio.c \
				  sonar/sonar.c

H2_SOURCE_FILES = hovercraft2.c \
				  motor/DualMotors.c \
				  uart/uart.c \
				  radio/radio.c

#------------------------------------------------------------------------------
# Project Settings
#------------------------------------------------------------------------------
PROJECT = RTOS
DEVICE  = at90usb1287
CLOCK   = 8000000
# debug or release
CFG     = debug
TARGET  = $(PROJECT)-$(CFG).hex

#------------------------------------------------------------------------------
# Programmer Settings
#------------------------------------------------------------------------------
# Fuses to enable JTAG and OCD (On-Chip Debugging)
OCD_FUSES     = -U hfuse:w:0x19:m -U lfuse:w:0xDE:m -U efuse:w:0xF3:m \
                -U lock:w:0xFF:m
ORIG_FUSES    = -U hfuse:w:0x99:m -U lfuse:w:0x5E:m -U efuse:w:0xF3:m \
                -U lock:w:EF:m
PROGRAMMER_ID = -c jtag2fast -P usb
DEVICE_PARTNO = usb1287
AVRDUDE       = avrdude $(PROGRAMMER_ID) -p $(DEVICE_PARTNO)

# Device Firmware Upgrade
DFU_PROGRAMMER = dfu-programmer $(DEVICE)

#------------------------------------------------------------------------------
# GCC Settings
#------------------------------------------------------------------------------
CC      = avr-gcc
INC     = -I.

CFLAGS  = -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += $(INC) -Winline
ifeq ($(CFG),debug)
	CFLAGS += -ggdb3 -O2
else ifeq ($(CFG), release)
	CFLAGS += -O2
endif

# Dependency
DEP_DIR = deps
CFLAGS += -MD -MP -MF $(DEP_DIR)/$(notdir $@).d
COMPILE = $(CC) $(CFLAGS)

#------------------------------------------------------------------------------
# Don't change 
#------------------------------------------------------------------------------
OBJECTS    = $(SOURCE_FILES:%.c=%.o)

ELF_TARGET = $(TARGET:%.hex=%.elf)

OS_OBJS  = $(OS_SOURCE_FILES:%.c=%.o)

BASE_OBJS       = $(BASE_SOURCE_FILES:%.c=%.o)
BASE_TARGET     = basestation.hex
BASE_ELF_TARGET = $(BASE_TARGET:%.hex=%.elf)

H1_OBJS       = $(H1_SOURCE_FILES:%.c=%.o)
H1_TARGET     = h1.hex
H1_ELF_TARGET = $(H1_TARGET:%.hex=%.elf)

H2_OBJS       = $(H2_SOURCE_FILES:%.c=%.o)
H2_TARGET     = h2.hex
H2_ELF_TARGET = $(H2_TARGET:%.hex=%.elf)

#------------------------------------------------------------------------------
# Make Rules
#------------------------------------------------------------------------------
base: $(DEP_DIR) $(OS_OBJS) $(BASE_OBJS)
	$(COMPILE) -nostartfiles -o $(BASE_ELF_TARGET) $(OS_OBJS) $(BASE_OBJS)
	
$(BASE_TARGET): base
	avr-objcopy -j .text -j .data -O ihex $(BASE_ELF_TARGET) $(BASE_TARGET)
	
base_flash: $(BASE_TARGET)
	$(DFU_PROGRAMMER) erase
	$(DFU_PROGRAMMER) flash $(BASE_TARGET)
	
h1: $(DEP_DIR) $(OS_OBJS) $(H1_OBJS)
	$(COMPILE) -o $(H1_ELF_TARGET) $(H1_OBJS)
	
$(H1_TARGET): h1
	rm -f $(H1_TARGET)
	avr-objcopy -j .text -j .data -O ihex $(H1_ELF_TARGET) $(H1_TARGET)
	
h1_flash: $(H1_TARGET)
	$(DFU_PROGRAMMER) erase
	$(DFU_PROGRAMMER) flash $(H1_TARGET)
	
h2: $(DEP_DIR) $(H2_OBJS)
	$(COMPILE) -o $(H2_ELF_TARGET) $(H2_OBJS)
	
$(H2_TARGET): h2
	rm -f $(H2_TARGET)
	avr-objcopy -j .text -j .data -O ihex $(H2_ELF_TARGET) $(H2_TARGET)
	
h2_flash: $(H2_TARGET)
	$(DFU_PROGRAMMER) erase
	$(DFU_PROGRAMMER) flash $(H2_TARGET)

# symbolic targets:
.PHONY: all
all: $(DEP_DIR) base

$(DEP_DIR):
	mkdir $(DEP_DIR) 2> /dev/null

%.o: %.c
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

#flash:	all
#	$(DFU_PROGRAMMER) erase
#	$(DFU_PROGRAMMER) flash $(TARGET)

ocd-fuse:
	$(AVRDUDE) $(OCD_FUSES)

orig-fuse:
	$(AVRDUDE) $(ORIG_FUSES)

avrdude-flash: all
	$(AVRDUDE) -U flash:w:$(TARGET):i

# Xcode uses the Makefile targets "", "clean" and "install"
install: base_flash

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -rf *~ $(DEP_DIR) $(OS_OBJS) \
			  $(BASE_TARGET) $(BASE_ELF_TARGET) $(BASE_OBJS) \
			  $(H1_TARGET) $(H1_ELF_TARGET) $(H1_OBJS) \
			  $(H2_TARGET) $(H2_ELF_TARGET) $(H2_OBJS)

# file targets:
$(ELF_TARGET): $(OBJECTS)
	$(COMPILE) -o $(ELF_TARGET) $(OBJECTS)

$(TARGET): $(ELF_TARGET)
	rm -f $(TARGET)
	avr-objcopy -j .text -j .data -O ihex $(ELF_TARGET) $(TARGET)
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	$(TARGET_ELF)
	avr-objdump -d $(TARGET_ELF)

cpp:
	$(COMPILE) -E $(firstword $(SOURCE_FILES))

-include $(wildcard $(DEP_DIR)/*)

