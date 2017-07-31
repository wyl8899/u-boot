CCPREFIX ?= mips-sde-elf-

CC := $(CCPREFIX)gcc
LD := $(CCPREFIX)ld
OBJCOPY := $(CCPREFIX)objcopy

CFLAGS	:=  -fno-builtin -nostdlib  -nostdinc -g  -EL -G0 -Wformat -msoft-float
CFLAGS  +=  -O2

loader.bin: bootasm.S
	$(CC) $(CFLAGS) -g -c -o loader.o $^
	$(OBJCOPY) -O binary  -S loader.o $@
	$(LD) -EL -n -G0 -Ttext 0xbfc00000 -o loader loader.o
