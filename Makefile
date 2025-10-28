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

# Test input and expected output
TESTFILE ?= test/addlarge.bin
BASENAME = $(basename $(notdir $(TESTFILE)))
MYDUMP = test/$(BASENAME)-answer.res
EXPECTED = test/$(BASENAME).res

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

# Run test and compare output
test: $(BIN)
	./$(BIN) $(TESTFILE)
	@echo "Comparing register contents with the provided answer bin..."
	@diff -u $(EXPECTED) $(MYDUMP) > diff.out && \
		echo "Register contents match!" || \
        (echo "Register contents don't match!\n"; cat diff.out)
	@rm -f diff.out

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN) $(MYDUMP)

.PHONY: all clean
