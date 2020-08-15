#
#	Global configuration
# 
MCU = SAMD21G18A
LINKER_SCRIPT = samd21g18a_flash
F_CPU = 8000000
BUILD = build
BIN = gemini-firmware
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size


##############################################################################
.PHONY: all directory clean size firmware


ifeq ($(OS), Windows_NT)
  MKDIR = gmkdir
else
  MKDIR = mkdir
endif

CFLAGS += -W -Wall -Wextra --std=gnu11
CFLAGS += -mcpu=cortex-m0plus -mthumb -msoft-float -mfloat-abi=soft
# Specify dialect options that make things easier for embedded development.
CFLAGS += -funsigned-char -funsigned-bitfields -fshort-enums
# Specify EABI dialects - variable size enums, since that's what the GCC
# toolchain seems to be compiled with.
CFLAGS += -mabi=aapcs
# -MD: output make *.d files
# -MP: This option instructs CPP to add a phony target for each dependency other
# than the main file, causing each to depend on nothing. These dummy rules work around
# errors make gives if you remove header files without updating the Makefile to match
CFLAGS += -MD -MP -MT $(BUILD)/$(*F).o -MF $(BUILD)/$(@F).d

# Optimizations
# Enable dead code elimination
CFLAGS += -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
  CFLAGS += -ggdb -O0

else
  # Optimize for speed instead of size, as this firmware is very small.
  CFLAGS += -O2
  # Consider link-time optimization, but it doesn't seem to
  # work right now.
endif

INCLUDES += \
  -I./src \
  -I./third_party/samd21/include \
  -I./third_party/cmsis/include

SRCS += $(wildcard src/*.c)

SRCS += \
  ./third_party/samd21/gcc/gcc/startup_samd21.c


# Segger RTT
ifeq ($(DEBUG), 1)
  INCLUDES += \
    -I./third_party/rtt
  SRCS += \
    ./third_party/rtt/SEGGER_RTT.c \
    ./third_party/rtt/SEGGER_RTT_printf.c
endif
LDFLAGS += -Wl,--wrap=printf

# Defines
DEFINES += \
  -DARM_MATH_CM0PLUS \
  -DSAMD21 \
  -D__SAMD21_FAMILY \
  -D__SAM_D21_SUBFAMILY \
  -D__$(MCU)__ \
  -DDONT_USE_CMSIS_INIT \
  -DF_CPU=$(F_CPU)

ifeq ($(DEBUG), 1)
  DEFINES += -DDEBUG=1
endif

CFLAGS += $(INCLUDES) $(DEFINES)

OBJS = $(addprefix $(BUILD)/, $(notdir %/$(subst .c,.o, $(SRCS))))

# Linker configuration

LDFLAGS += -mcpu=cortex-m0plus -mthumb
# Perform dead code elimination 
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--script=./third_party/samd21/gcc/gcc/$(LINKER_SCRIPT).ld


all: directory firmware size

$(BUILD)/$(BIN).elf: $(OBJS)
	@echo LD $@
	@$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

$(BUILD)/$(BIN).hex: $(BUILD)/$(BIN).elf
	@echo OBJCOPY $@
	@$(OBJCOPY) -O ihex $^ $@

$(BUILD)/$(BIN).bin: $(BUILD)/$(BIN).elf
	@echo OBJCOPY $@
	@$(OBJCOPY) -O binary $^ $@

%.o:
	@echo CC $@
	@$(CC) $(CFLAGS) $(filter %/$(subst .o,.c,$(notdir $@)), $(SRCS)) -c -o $@

firmware: $(BUILD)/$(BIN).elf
	@echo firmware is at $(BUILD)/$(BIN).elf

directory:
	@$(MKDIR) -p $(BUILD)

size: $(BUILD)/$(BIN).elf
	@echo size:
	@$(SIZE) -t $^

program: firmware
	@echo program
	openocd -f openocd-program.cfg

clean:
	@echo clean
	@-rm -rf $(BUILD)

-include $(wildcard $(BUILD)/*.d)
