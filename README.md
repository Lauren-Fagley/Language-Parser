# Cooke Analyzer

This project implements a simple **recursive descent parser** and **lexical analyzer** for a custom programming language, written in C. The parser validates the syntax of a source file based on defined grammar rules, and the lexical analyzer scans and tokenizes input.

## Overview

The project is composed of two main components:

- `front.c` — the **lexer (lexical analyzer)**: breaks source code into a stream of tokens.
- `parser.c` — the **parser**: analyzes the sequence of tokens to ensure the source follows the correct grammar.

A `Makefile` is included to compile the source files.

## Language Features Supported

The parser supports the following constructs:

- Variable assignments: `x = y + 1;`
- Input/output: `input(x);`, `output(x);`
- Conditional statements:
  ```c
  if (x < y) {
      // statements
  } else {
      // statements
  }

    Boolean logic: &&, ||, !

    Comparison operators: ==, !=, <, <=, >, >=

    Arithmetic operations: +, -, *, /, %

# Grammar Rules (Simplified)

    stmt → variable assignment, input/output, if/else

    expr → term { (+|-) term }

    term → factor { (*|/|%) factor }

    factor → identifier | integer | ( expr )

    cooke (comparison) → expr [comparison_op expr] [bool_op cooke]

# Files
front.c

  - Implements the lexer.

  - Uses getChar, lookup, lookup2, and lex to scan and classify input characters.

  - Handles whitespace and comments.

  - Tracks line numbers via count.

  - Reports unknown symbols without crashing.

parser.c

  - Implements the recursive descent parser.
  - Functions include:

        stmt() — Entry point for statements.

        cooke() — Parses comparison and boolean expressions.

        expr(), term(), factor() — Arithmetic expression parsing.

  - Includes helpful debug prints like Enter <expr> and Exit <expr>.

  - Validates proper use of semicolons, parentheses, and braces.

parser.h and front.h

   - Header files for external declarations of functions and variables shared between modules.

# How to Run

./analyzer path/to/source.txt

Example:

./analyzer test_input.txt

# Error Handling

  If a syntax error is found, the program prints:

  Error encounter on line <line_number>: The next lexeme was <lexeme> and the next token was <token>

  The program exits immediately with status 1.

# Notes

  The function cooke() is humorously named but functions as the "compare" function.

  EOF is specially handled to ensure the analyzer terminates correctly.

  All parsing functions are recursive to support nested expressions.

# Author

Lauren Fagley
