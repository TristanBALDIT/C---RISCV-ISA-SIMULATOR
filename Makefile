# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c99 -Iinclude

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN := main

# Source and object files
SRCS := $(wildcard $(SRC_DIR)/*.c) main.c
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Default target
all: $(BIN)

# Link object files into final binary
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each .c file into .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN)

.PHONY: all clean
