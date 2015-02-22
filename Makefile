PREFIX ?= /usr
ARMGNU ?= $(PREFIX)/bin/arm-none-eabi
SOURCES_ASM := $(wildcard *.S)
SOURCES_C   := $(wildcard *.c)
OBJS        := $(patsubst %.S,%.o,$(SOURCES_ASM))
OBJS        += $(patsubst %.c,%.o,$(SOURCES_C))
DEPENDFLAGS := -MD -MP
INCLUDES    := -Iinclude
BASEFLAGS   := -O2 -fpic -nostdlib -std=c99
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
BASEFLAGS   += -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s
BASEFLAGS   += -fdata-sections -ffunction-sections -g -nostartfiles
WARNFLAG    := -Wall
LDLFAGS     := --gc-sections
ASFLAGS     := $(INCLUDES) $(DEPENDFLAGS) -D__ASSEMBLY__
CFLAGS      := $(INCLUDES) $(DEPENDFLAGS) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS      += -std=gnu99
 
all: kernel.img
 
include $(wildcard *.d)
 
kernel.elf: $(OBJS) rpi.ld
	$(ARMGNU)-ld $(OBJS) $(LDLFAGS) -Trpi.ld -o $@
 
kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
 
clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
 
distclean: clean
	$(RM) -f *.d
 
%.o: %.c Makefile
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@
 
%.o: %.S Makefile
	$(ARMGNU)-gcc $(ASFLAGS) -c $< -o $@

