# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude

# Directories
SRC_DIR = src
TEST_DIR = tests
INCLUDE_DIR = include
BIN_DIR = bin
BUILD_DIR = build

# Executable target
TARGET = $(BIN_DIR)/simplelang

# Source files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/token.c $(SRC_DIR)/lexer.c $(SRC_DIR)/parser.c $(SRC_DIR)/ast.c

# Object files in build/
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Create necessary directories
$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

# Compile object files into build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build main executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

# Run the program
run: $(TARGET)
	$(TARGET) input.sl

# === Test Targets ===
test-token: | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_token $(TEST_DIR)/test_token.c $(SRC_DIR)/token.c
	$(BIN_DIR)/test_token

test-lexer: | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_lexer $(TEST_DIR)/test_lexer.c $(SRC_DIR)/token.c $(SRC_DIR)/lexer.c
	$(BIN_DIR)/test_lexer

test-parser: | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/test_parser $(TEST_DIR)/test_parser.c $(SRC_DIR)/token.c $(SRC_DIR)/lexer.c $(SRC_DIR)/ast.c $(SRC_DIR)/parser.c
	$(BIN_DIR)/test_parser

# === Valgrind Target ===
valgrind: test-token test-lexer test-parser $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/test_token
	valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/test_lexer
	valgrind --leak-check=full --show-leak-kinds=all $(BIN_DIR)/test_parser
	valgrind --leak-check=full --show-leak-kinds=all $(TARGET) input.sl

# === Cleanup ===
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all run clean test-token test-lexer test-parser valgrind
