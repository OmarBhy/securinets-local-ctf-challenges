CC=arm-none-eabi-gcc

CFLAGS_COMMON=-mcpu=cortex-m3 -mthumb -O2 -ffreestanding -fno-builtin -Wall -Wextra
LDFLAGS=-T src/linker.ld -nostartfiles -Wl,--gc-sections
LIBS=-lc -lnosys

SRCS=src/main.c src/startup.c src/uart.c src/systick.c src/cli.c
BUILD=build

PUBLIC_ELF=$(BUILD)/firmware_public.elf
REMOTE_ELF=$(BUILD)/firmware_remote.elf

all: $(PUBLIC_ELF) $(REMOTE_ELF)

$(BUILD):
	mkdir -p $(BUILD)

$(PUBLIC_ELF): $(SRCS) | $(BUILD)
	$(CC) $(CFLAGS_COMMON) $(SRCS) $(LDFLAGS) -o $@ $(LIBS)

$(REMOTE_ELF): $(SRCS) | $(BUILD)
	$(CC) $(CFLAGS_COMMON) -DFLAG_STR="\"CyberQuest{9Rib_i2adhan}\"" \
		$(SRCS) $(LDFLAGS) -o $@ $(LIBS)

clean:
	rm -rf $(BUILD)
