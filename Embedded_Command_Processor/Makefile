# Compiler
CC := gcc

# Directories
SRC_DIR := src
BUILD_DIR := build

# Files
SRCS := $(SRC_DIR)/main.c \
        $(SRC_DIR)/ring_buffer.c \
        $(SRC_DIR)/process.c

OBJS := $(BUILD_DIR)/main.o \
        $(BUILD_DIR)/ring_buffer.o \
        $(BUILD_DIR)/process.o

TARGET := $(BUILD_DIR)/app.exe

# Flags
CFLAGS := -g -gdwarf-4 -O0 -Wall -Wextra
LDFLAGS :=

# Default target
all: clean $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

# Run
run: $(TARGET)
	$(TARGET)

# Phony targets
.PHONY: all clean run

