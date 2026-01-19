# Compiler, default is gcc
CC = gcc
# Compiler flags, enable verbose warnings and optimize for speed
CFLAGS = -Wall -Wextra -O2
# Libraries, xcb and xcb keysyms helper
LIBS = -lxcb -lxcb-keysyms
# Build directory
BUILD_DIR = build
# Source directory
SOURCE_DIR = src
# Files to be compiled
SRCS = $(shell find $(SOURCE_DIR) -name "*.c")
# What files will be compiled into
OBJS := $(SRCS:src/%.c=$(BUILD_DIR)/%.o)
# Final executable name
TARGET := onyxwm

all: $(TARGET) 
# Aliases
r: run
fmt: format

run: $(TARGET)
	startx ./$(TARGET)

format:
	clang-format -i $(SRCS) $(shell find $(SOURCE_DIR) -name "*.h")

# target onyxwm
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean  r fmt run format
