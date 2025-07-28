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
TEST_TOKEN_SRC := $(TEST_DIR)/test_token.c
TEST_LEXER_SRC := $(TEST_DIR)/test_lexer.c

# Output executables
TOKEN_EXE := $(BIN_DIR)/test_token
LEXER_EXE := $(BIN_DIR)/test_lexer

# Ensure bin directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Default target builds all tests
all: $(BIN_DIR) $(TOKEN_EXE) $(LEXER_EXE)

# Build test_token
$(TOKEN_EXE): $(TOKEN_SRC) $(TEST_TOKEN_SRC) $(TOKEN_HDR)
	@echo "Compiling test_token..."
	$(CC) $(CFLAGS) -o $@ $(TEST_TOKEN_SRC) $(TOKEN_SRC)

# Build test_lexer
$(LEXER_EXE): $(LEXER_SRC) $(TOKEN_SRC) $(TEST_LEXER_SRC) $(LEXER_HDR) $(TOKEN_HDR)
	@echo "Compiling test_lexer..."
	$(CC) $(CFLAGS) -o $@ $(TEST_LEXER_SRC) $(LEXER_SRC) $(TOKEN_SRC)

# Separate targets to build and run each test
test_token: $(TOKEN_EXE)
	@echo "\n--- Running test_token ---"
	@$(TOKEN_EXE)

test_lexer: $(LEXER_EXE)
	@echo "\n--- Running test_lexer ---"
	@$(LEXER_EXE)

# Run both
run: test_token test_lexer

# Clean
clean:
	@echo "Cleaning..."
	@rm -rf $(BIN_DIR)

.PHONY: all clean run test_token test_lexer
