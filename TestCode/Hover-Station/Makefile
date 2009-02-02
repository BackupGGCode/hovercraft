# Name: Makefile
# Author: Marco Yuen
# Copyright: Copyright 2009 Marco Yuen
# License: ...

#------------------------------------------------------------------------------
# Project Source
#------------------------------------------------------------------------------
SOURCE_FILES = Hover-StationMain.c \
			   radio/radio.c 	   \
			   ../Motor/Motor.c    \
			   ../uart/uart.c      \
			   ../Servo/servo.c    \
			   ../Sonar/sonar.c

#------------------------------------------------------------------------------
# Project Settings
#------------------------------------------------------------------------------
PROJECT = Hover-Station
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
INC     = -I. -I..

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

ocd-fuse:
	$(AVRDUDE) $(OCD_FUSES)

orig-fuse:
	$(AVRDUDE) $(ORIG_FUSES)

avrdude-flash: all
	$(AVRDUDE) -U flash:w:$(TARGET):i

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash

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

