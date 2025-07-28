# Directories
SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BIN_DIR := bin

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -I$(INC_DIR) -g

# Source files
TOKEN_SRC := $(SRC_DIR)/token.c
LEXER_SRC := $(SRC_DIR)/lexer.c

# Header files
TOKEN_HDR := $(INC_DIR)/token.h
LEXER_HDR := $(INC_DIR)/lexer.h

# Test files
TEST_TOKEN := $(TEST_DIR)/test_token.c
TEST_LEXER := $(TEST_DIR)/test_lexer.c

# Output executables
TOKEN_EXE := $(BIN_DIR)/test_token
LEXER_EXE := $(BIN_DIR)/test_lexer

# Create bin directory automatically
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Default target
all: $(BIN_DIR) $(TOKEN_EXE) $(LEXER_EXE)

# Build test_token
$(TOKEN_EXE): $(TOKEN_SRC) $(TEST_TOKEN) $(TOKEN_HDR)
	$(CC) $(CFLAGS) -o $@ $(TEST_TOKEN) $(TOKEN_SRC)

# Build test_lexer
$(LEXER_EXE): $(TOKEN_SRC) $(LEXER_SRC) $(TEST_LEXER) $(LEXER_HDR) $(TOKEN_HDR)
	$(CC) $(CFLAGS) -o $@ $(TEST_LEXER) $(LEXER_SRC) $(TOKEN_SRC)

# Clean build
clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
