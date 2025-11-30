#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <input.sl>"
    exit 1
fi

INPUT_FILE="$1"
BASE_NAME=$(basename "$INPUT_FILE" .sl)
ASM_FILE="output/${BASE_NAME}.asm"

echo "=== Compiling SimpleLang to Assembly ==="
./bin/simplelang "$INPUT_FILE"

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo ""
echo "=== Assembly Generated ==="
echo "Assembly file created: $ASM_FILE"
echo "Note: 8-bit CPU assembler not included in this repository"