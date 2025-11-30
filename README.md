# SimpleLang Compiler

A complete programming language compiler implementation written in C, featuring lexical analysis, parsing, Abstract Syntax Tree (AST) generation, and code generation for 8-bit CPU architecture.

## Language Constructs

SimpleLang supports the following programming constructs:

### 1. Variable Declaration
- **Syntax:** `int <identifier>;` or `int <identifier> = <expression>;`
- **Examples:**
  ```simplelang
  int a;
  int b = 10;
  int result = a + b;
  ```

### 2. Assignment
- **Syntax:** `<identifier> = <expression>;`
- **Examples:**
  ```simplelang
  a = 5;
  b = a + 10;
  result = (a + b) - c;
  ```

### 3. Arithmetic Operations
- **Supported operators:** `+` (addition), `-` (subtraction)
- **Examples:**
  ```simplelang
  sum = a + b;
  difference = a - b;
  complex = (a + b) - (c + d);
  ```

### 4. Comparison Operations
- **Supported operator:** `==` (equality)
- **Usage:** Primarily in conditional expressions
- **Example:**
  ```simplelang
  if (a == 5) {
      b = 10;
  }
  ```

### 5. Conditional Statements
- **Syntax:** `if (<expression>) { <statements> }` or `if (<expression>) { <statements> } else { <statements> }`
- **Examples:**
  ```simplelang
  if (a == b) {
      a = a + 1;
  }
  
  if (x == 10) {
      y = 20;
  } else {
      y = 30;
  }
  ```

## Grammar (BNF Notation)

```bnf
program → statement*
statement → declaration | assignment | if_statement
declaration → 'int' IDENTIFIER ('=' expression)? ';'
assignment → IDENTIFIER '=' expression ';'
if_statement → 'if' '(' expression ')' block ('else' block)?
block → '{' statement* '}'
expression → term ('==' term)*
term → factor (('+' | '-') factor)*
factor → NUMBER | IDENTIFIER | '(' expression ')'
```

## Project Structure

```
simple-compiler/
├── include/
│   ├── token.h       # Token definitions and functions
│   ├── lexer.h       # Lexical analyzer interface
│   ├── parser.h      # Parser interface
│   ├── ast.h         # Abstract Syntax Tree definitions
│   └── codegen.h     # Code generator interface
├── src/
│   ├── token.c       # Token implementation
│   ├── lexer.c       # Lexical analyzer implementation
│   ├── parser.c      # Parser implementation
│   ├── ast.c         # AST implementation
│   ├── codegen.c     # Code generator implementation
│   └── main.c        # Main compiler driver
├── tests/
│   ├── test_token.c  # Token module tests
│   ├── test_lexer.c  # Lexer module tests
│   ├── test_parser.c # Parser module tests
│   ├── test_codegen.c # Code generator tests
│   └── test_8bit_integration.c # 8-bit CPU integration tests
├── examples/
│   ├── simple_math.sl # Sample arithmetic program
│   ├── conditional.sl # Sample conditional program
│   └── counter.sl     # Sample counter program
├── build/            # Object files (.o)
├── bin/              # Compiled executables
├── output/           # Generated assembly files
├── compile_and_assemble.sh # Complete compilation pipeline
├── Makefile
└── README.md
```

## Building the Project

### Prerequisites
- GCC compiler
- Make utility
- Valgrind (optional, for memory leak detection)

#### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install gcc make valgrind
```

**CentOS/RHEL/Fedora:**
```bash
# CentOS/RHEL
sudo yum install gcc make valgrind

# Fedora
sudo dnf install gcc make valgrind
```

**macOS:**
```bash
# Install Xcode Command Line Tools (includes gcc and make)
xcode-select --install

# Install valgrind via Homebrew
brew install valgrind
```

**Windows (WSL):**
```bash
# First install WSL with Ubuntu, then:
sudo apt update
sudo apt install gcc make valgrind
```

### Makefile Commands

#### Build Main Compiler
```bash
# Build the main compiler executable
make all

# Alternative: build without running
make bin/simplelang
```

#### Run the Compiler
```bash
# Compile and run on input.sl
make run

# Equivalent to:
# make bin/simplelang
# ./bin/simplelang input.sl
```

#### Build and Run Individual Tests
```bash
# Build and run token tests
make test-token

# Build and run lexer tests  
make test-lexer

# Build and run parser tests
make test-parser

# Build and run code generator tests
make test-codegen

# Build and run 8-bit CPU integration tests
make test-8bit
```

#### Memory Leak Detection
```bash
# Run all tests with Valgrind memory checking
make valgrind
```

#### Clean Build Files
```bash
# Remove all compiled files and directories
make clean
```

### Key Makefile Features

- **Automatic Directory Creation**: Creates `build/` and `bin/` directories as needed
- **Separate Object Files**: Keeps `.o` files in `build/` directory
- **Debug Symbols**: Includes `-g` flag for debugging
- **Memory Testing**: Integrated Valgrind support for leak detection
- **Incremental Builds**: Only recompiles changed source files

## Usage Examples

### Compiling a SimpleLang Program
```bash
# Build the compiler
make all

# Run compiler on input file
make run

# Or run manually:
./bin/simplelang input.sl
```

### Sample Program (input.sl)
```simplelang
int a = 5;
if(a == 5){
    a = 6;
}
```

### Expected Output
```
Lexical Analysis (Tokenization)

INT          int        Line :  1 Col: 1
IDENTIFIER   a          Line :  1 Col: 5
ASSIGN       =          Line :  1 Col: 7
NUMBER       5          Line :  1 Col: 9
SEMICOLON    ;          Line :  1 Col:10
NEWLINE      \n         Line :  1 Col:11
IF           if         Line :  2 Col: 1
LPAREN       (          Line :  2 Col: 3
IDENTIFIER   a          Line :  2 Col: 4
EQUAL        ==         Line :  2 Col: 6
NUMBER       5          Line :  2 Col: 9
RPAREN       )          Line :  2 Col:10
LBRACE       {          Line :  2 Col:11
NEWLINE      \n         Line :  2 Col:12
IDENTIFIER   a          Line :  3 Col: 5
ASSIGN       =          Line :  3 Col: 7
NUMBER       6          Line :  3 Col: 9
SEMICOLON    ;          Line :  3 Col:10
NEWLINE      \n         Line :  3 Col:11
RBRACE       }          Line :  4 Col: 1
EOF          EOF        Line :  4 Col: 2
Total tokens: 19

Syntax Analysis (Parsing)
PARSING SUCCESSFUL!
Generated Abstract Syntax Tree (AST):

PROGRAM
├── DECLARATION: a
│   └── NUMBER: 5
└── IF
    ├── CONDITION:
    │   └── BINARY_OP: EQUAL
    │       ├── IDENTIFIER: a
    │       └── NUMBER: 5
    └── THEN:
        └── BLOCK
            └── ASSIGNMENT: a
                └── NUMBER: 6
```

## Testing

### Running Individual Tests
```bash
# Test token functionality
make test-token

# Test lexer functionality  
make test-lexer

# Test parser functionality
make test-parser
```

### Memory Leak Testing
```bash
# Run all tests with Valgrind
make valgrind
```

### Test Output Examples
Each test will show:
- **Token Tests**: Token creation, type conversion, and memory management
- **Lexer Tests**: Tokenization of various SimpleLang constructs  
- **Parser Tests**: AST generation for different language features

## Compiler Pipeline

1. **Lexical Analysis (Tokenization)**: Converts source code into tokens
2. **Syntax Analysis (Parsing)**: Builds Abstract Syntax Tree from tokens
3. **Code Generation**: Translates AST to 8-bit CPU assembly
4. **AST Visualization**: Pretty-prints the generated AST structure

### 8-bit CPU Code Generator

- **Stack-based expression evaluation** for arithmetic operations
- **Variable management** with memory allocation (%var_<name> labels)
- **Assembly generation** with .text and .data sections
- **Instruction set support**: ldi, lda, sta, push, pop, mov, add, sub, cmp, hlt

## Error Handling

The compiler provides detailed error messages with line and column information:
- Lexical errors for invalid characters
- Syntax errors for malformed expressions
- Memory allocation error handling

## Supported Features

### ✅ Implemented
- Variable declarations with initialization
- Arithmetic expressions (+, -)
- Variable assignments
- Sequential statement execution
- 8-bit CPU assembly code generation

### ⚠️ Partial Support
- Conditional statements (generates assembly)

### 🔄 Future Extensions
- Loop constructs (while, for)
- Function definitions and calls
- Arrays and complex data types
- Optimized code generation
