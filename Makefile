CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
BIN = arm-none-eabi-objcopy
STL = st-flash
CFLAGS = -mthumb -mcpu=cortex-m3 -Wall
all: app.bin
crt.o: crt.s
	$(AS) -o crt.o crt.s
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c
usb.o: usb.c
	$(CC) $(CFLAGS) -c -o usb.o usb.c
usb_desc.o: usb_desc.c
	$(CC) $(CFLAGS) -c -o usb_desc.o usb_desc.c
lcd.o: lcd.c
	$(CC) $(CFLAGS) -c -o lcd.o lcd.c
ssd1306.o: ssd1306.c
	$(CC) $(CFLAGS) -c -o ssd1306.o ssd1306.c
i2c.o: i2c.c
	$(CC) $(CFLAGS) -c -o i2c.o i2c.c
stm32f103.o: stm32f103.c
	$(CC) $(CFLAGS) -c -o stm32f103.o stm32f103.c
app.elf: linker.ld crt.o main.o usb.o stm32f103.o ssd1306.o i2c.o lcd.o usb_desc.o
	$(LD) -T linker.ld -o app.elf crt.o main.o usb.o stm32f103.o ssd1306.o i2c.o lcd.o usb_desc.o
app.bin: app.elf
	$(BIN) -O binary app.elf app.bin
clean:
	rm -f *.o *.elf *.bin
flash: app.bin
	$(STL) write app.bin 0x8000000
erase:
	$(STL) erase
