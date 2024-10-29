# Makefile for Raspberry Pi 4 Bootloader

# Compiler and Linker
CC = arm-none-eabi-gcc

# Flags
CFLAGS = -Wall -Wextra -O2 -ffreestanding -nostdlib -marm -mcpu=cortex-a72
LDFLAGS = -T boot.ld

# Source files
SRCS = uart.c bootcode.c boot.s
OBJS = $(SRCS:.c=.o) $(SRCS:.s=.o)  # Generate object files from source files

# Output files
TARGET = bootcode.elf
BIN_TARGET = bootcode.bin

# Default target
all: $(TARGET) $(BIN_TARGET)

# Rule to create bootcode.elf
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# Rule to create bootcode.bin from bootcode.elf
$(BIN_TARGET): $(TARGET)
	arm-none-eabi-objcopy -O binary $< $@

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .s files into .o files
%.o: %.s
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET) $(BIN_TARGET)

.PHONY: all clean
