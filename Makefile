CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 -Wall -Iinclude

SRC_COMMON_DIR := src/common
SRC_FW_DIR := src/firmware
SRC_BOOT_DIR := src/boot

OBJ_DIR := obj
OBJ_COMMON_DIR := obj/common
OBJ_FW_DIR := obj/fw
OBJ_BOOT_DIR := obj/boot

SRC_COMMON := $(wildcard $(SRC_COMMON_DIR)/*.c)
SRC_FW := $(wildcard $(SRC_FW_DIR)/*.c)
SRC_BOOT := $(wildcard $(SRC_BOOT_DIR)/*.c)

OBJ_FW := $(SRC_COMMON:$(SRC_COMMON_DIR)/%.c=$(OBJ_COMMON_DIR)/%.o) $(SRC_FW:$(SRC_FW_DIR)/%.c=$(OBJ_FW_DIR)/%.o)
OBJ_BOOT := $(SRC_COMMON:$(SRC_COMMON_DIR)/%.c=$(OBJ_COMMON_DIR)/%.o) $(SRC_BOOT:$(SRC_BOOT_DIR)/%.c=$(OBJ_BOOT_DIR)/%.o)

.PHONY: directories all clean

# $(info $$OBJ_FW is [${OBJ_FW}])
all: directories boot.bin app.bin
directories: $(OBJ_FW_DIR) $(OBJ_BOOT_DIR) $(OBJ_COMMON_DIR)

$(OBJ_FW_DIR)/crt.o: $(SRC_FW_DIR)/crt.s
	$(AS) -o $(OBJ_FW_DIR)/crt.o $(SRC_FW_DIR)/crt.s

$(OBJ_BOOT_DIR)/crt.o: $(SRC_BOOT_DIR)/crt.s
	$(AS) -o $(OBJ_BOOT_DIR)/crt.o $(SRC_BOOT_DIR)/crt.s

$(OBJ_COMMON_DIR)/%.o: $(SRC_COMMON_DIR)/%.c | $(OBJ_COMMON_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_FW_DIR)/%.o: $(SRC_FW_DIR)/%.c | $(OBJ_FW_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_BOOT_DIR)/%.o: $(SRC_BOOT_DIR)/%.c | $(OBJ_BOOT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

app.elf: linker_fw.ld $(OBJ_FW_DIR)/crt.o $(OBJ_FW)
	$(LD) -T linker_fw.ld -o app.elf $(OBJ_FW_DIR)/crt.o $(OBJ_FW) $(OBJ_COMMON)
app.bin: app.elf
	$(BIN) -O binary app.elf app.bin

boot.elf: linker_boot.ld $(OBJ_BOOT_DIR)/crt.o $(OBJ_BOOT)
	$(LD) -T linker_boot.ld -o boot.elf $(OBJ_BOOT_DIR)/crt.o $(OBJ_BOOT) $(OBJ_COMMON)
boot.bin: boot.elf
	$(BIN) -O binary boot.elf boot.bin

$(OBJ_FW_DIR):
	mkdir -p $@
$(OBJ_COMMON_DIR):
	mkdir -p $@
$(OBJ_BOOT_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJ_COMMON_DIR)/*.o $(OBJ_BOOT_DIR)/*.o $(OBJ_FW_DIR)/*.o *.elf *.bin
flash: boot.bin
	$(STL) write boot.bin 0x8000000
erase:
	$(STL) erase
