# Name: Makefile
# Author: Marco Yuen
# Copyright: Copyright 2009 Marco Yuen
# License: ...

#------------------------------------------------------------------------------
# Project Source
#------------------------------------------------------------------------------
SOURCE_FILES = Source/hover_main.c

#------------------------------------------------------------------------------
# Project Settings
#------------------------------------------------------------------------------
PROJECT = Hovercraft
DEVICE  = at90usb1287
CLOCK   = 8000000
# debug or release
CFG     = debug
TARGET  = $(PROJECT)-$(CFG).hex

# Programmer
# Fuses to enable JTAG and OCD (On-Chip Debugging)
JTAG_FUSES = -U hfuse:w:0x19:m -U lfuse:w:0xDE:m -U efuse:w:0xF3:m \
             -U lock:w:0xFF:m
PROGRAMMER = -c jtag2fast -P usb
AVRDUDE    = avrdude $(PROGRAMMER) -p $(DEVICE)

# Device Firmware Upgrade
DFU_PROGRAMMER = dfu-programmer $(DEVICE)

# GCC
CC      = avr-gcc
DEP_DIR = deps
CFLAGS  = -Wall -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)
CFLAGS += -Winline
ifeq ($(CFG),debug)
	CFLAGS += -ggdb3 -O0
else ifeq ($(CFG), release)
	CFLAGS += -Os
endif
# Dependency
CFLAGS += -MD -MP -MF $(DEP_DIR)/$(notdir $@).d
COMPILE = $(CC) $(CFLAGS)

#------------------------------------------------------------------------------
# Don't change 
#------------------------------------------------------------------------------
OBJECTS    = $(SOURCE_FILES:%.c=%.o)
ELF_TARGET = $(TARGET:%.hex=%.elf)

#------------------------------------------------------------------------------
# Make Rules
#------------------------------------------------------------------------------
# symbolic targets:
.PHONY: all
all: $(DEP_DIR) $(TARGET)

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

flash:	all
	$(DFU_PROGRAMMER) erase
	$(DFU_PROGRAMMER) flash $(TARGET)

jtag-fuse:
	$(AVRDUDE) $(JTAG_FUSES)

flash-avrdude: jtag-fuse all
	$(AVRDUDE) -U flash:w:$(TARGET):i

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -rf $(DEP_DIR) $(TARGET) $(ELF_TARGET) $(OBJECTS) *~

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

