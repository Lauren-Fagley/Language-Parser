#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LESSER_OP 27
#define GREATER_OP 28
#define MOD_OP 29
#define BOOL_NOT 30
#define SEMICOLON 31
#define OPEN_CURL 32
#define CLOSE_CURL 33

#define EQUAL_OP 50
#define NEQUAL_OP 51
#define LEQUAL_OP 52
#define GEQUAL_OP 53
#define BOOL_AND 54
#define BOOL_OR 55

#define KEY_IN 60
#define KEY_OUT 61
#define KEY_IF 62
#define KEY_ELSE 63

#define UNKNOWN2 70

extern char Token[20];
extern char lexeme [100];
extern int count;

int lex();

#endif
