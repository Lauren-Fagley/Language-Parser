/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
char Token[20];

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/*Boolean Trackers for double operator*/
bool check = false;
char prev;
int count;


/******************************************************/
/* main driver */
int main(int argc, char *argv[]) 
{
    count = 1; //count to count line numbers
    //checks if file argument was provided
    if(argc < 2){
        fprintf(stderr, "Error: No source file provided. Please specify a file path.\n");
        return 2;
    }

       
    /* Open the input data file and process its contents */
    in_fp = fopen(argv[1], "r");
    //Checks if the file exists 
    if (in_fp == NULL) {
        fprintf(stderr, "Error: The file '%s' does not exist.\n", argv[1]);
        return 3;
    }

    printf("Cooke Analyzer :: R11737101 \n");

    getChar();

    do{
       lex();
        stmt(); 
    }
    while (nextToken != EOF);

     fclose(in_fp);
     printf("Syntax Validated\n");

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;
        case '=': //if char is = it sets check to true to be used for later to check for equal_op, nexttoken will be overwritten if so
            addChar();
            nextToken = ASSIGN_OP;
            check = true;
            prev = '=';
            break;
        case '<': //same as =
            addChar();
            nextToken = LESSER_OP;
            check = true;
            prev = '<';
            break;
        case '>': //same as =
            addChar();
            nextToken = GREATER_OP;
            check = true;
            prev = '>';
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;
            break;
        case '!': //same as =
            addChar();
            nextToken = BOOL_NOT;
            check = true;
            prev = '!';
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;
        case '{':
            addChar();
            nextToken = OPEN_CURL;
            break;
        case '}':
            addChar();
            nextToken = CLOSE_CURL;
            break;
        case '&': //sets nexttoken to unknown until it is confirmed that it is and_op because & is not in the language by itself
            addChar();
            nextToken = UNKNOWN2;
            check = true;
            prev = '&';
            break;
        case '|': //same as &
            addChar();
            nextToken = UNKNOWN2;
            check = true;
            prev = '|';
            break;
        default: //if not in language it is reported as unknown without crashing/ending the program
            addChar();
            nextToken = UNKNOWN2;
            break;
    }
    return nextToken;
}

static int lookup2(char ch){
    //These will override nexttokens value with new value if necessary
    //getChar is called if any of them are true so that value is not lookupped twice
    //if not true it'll set check to false and return like nothing happened
    if (prev == '|' && ch == '|'){
        nextToken = BOOL_OR;
        addChar();
        getChar();
    }
    else if (prev == '&' && ch == '&'){
        nextToken = BOOL_AND;
        addChar();
        getChar();
    }
    else if(prev == '!' && ch == '='){
        nextToken = NEQUAL_OP;
        addChar();
        getChar();
    }
    else if(prev == '>' && ch == '='){
        nextToken = GEQUAL_OP;
        addChar();
        getChar();
    }
    else if(prev == '<' && ch =='='){
        nextToken = LEQUAL_OP;
        addChar();
        getChar();
    }
    else if(prev == '=' && ch == '='){
        nextToken = EQUAL_OP;
        addChar();
        getChar();
    }
    check = false;

    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}


/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER; 
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)){
        if(nextChar == '\n'){ //this incriments count with each newline
            count++;
        }
        getChar();
    } 
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER: //nexttoken is immediately sent to ident to account for words that enter if statements but don't reach the end
                    //as in 'i' would enter and have no nexttoken value if not declared at the beginning, can't declare at the end because that would override key values
                    //nexttoken is overwritten if it is a key word but if it continues as in 'output2' that will report as ident because it will enter the while loop and overwrite nexttoken
                    //to be ident
            nextToken = IDENT;
            if(nextChar == 'i'){
                addChar();
                getChar();    
                if(nextChar == 'f'){
                    addChar();
                    getChar();
                    nextToken = KEY_IF;
                }
                else if (nextChar == 'n'){
                    addChar();
                    getChar();
                    if(nextChar == 'p'){
                        addChar();
                        getChar();
                        if(nextChar == 'u'){
                            addChar();
                            getChar();
                            if(nextChar == 't'){
                                addChar();
                                getChar();
                                nextToken = KEY_IN;
                            }
                        }
                    }
                }
            }
            else if(nextChar == 'o'){
                addChar();
                getChar();
                if(nextChar == 'u'){
                    addChar();
                    getChar();
                    if(nextChar == 't'){
                        addChar();
                        getChar();
                        if(nextChar == 'p'){
                            addChar();
                            getChar();
                            if(nextChar == 'u'){
                                addChar();
                                getChar();
                                if(nextChar == 't'){
                                    addChar();
                                    getChar();
                                    nextToken = KEY_OUT;
                                }
                            }
                        }  
                    }
                }
            }
            else if(nextChar == 'e'){
                addChar();
                getChar();
                if(nextChar == 'l'){
                    addChar();
                    getChar();
                    if(nextChar == 's'){
                        addChar();
                        getChar();
                        if(nextChar == 'e'){
                            addChar();
                            getChar();
                            nextToken = KEY_ELSE;
                            
                        }
                    }
                }
            }
            
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
                nextToken = IDENT;
            }
            
            
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    //I decided to make it charclass != EOF and not charclass == unknown so that it will make check = false
    //this looks up the nextchar values to see if they are like '==' or '&&'
    //only looks this up if check == true
    if(check == true && charClass != EOF){
        lookup2(nextChar);
    }

    //just a bunch of if statements making my integer values into strings
    if(nextToken == 10){
        strcpy(Token, "INT_LIT");
    }
    else if(nextToken == 11){
        strcpy(Token, "IDENT");
    }
    else if(nextToken == 20){
        strcpy(Token, "ASSIGN_OP");
    }
    else if(nextToken == 21){
        strcpy(Token, "ADD_OP");
    }
    else if(nextToken == 22){
        strcpy(Token, "SUB_OP");
    }        
    else if(nextToken == 23){
        strcpy(Token, "MULT_OP");
    }
    else if(nextToken == 24){
        strcpy(Token, "DIV_OP");
    }
    else if(nextToken == 25){
        strcpy(Token, "OPEN_PAREN");
    }
    else if(nextToken == 26){
        strcpy(Token, "CLOSE_PAREN");
    }
    else if(nextToken == 27){
        strcpy(Token, "LESSER_OP");
    }
    else if(nextToken == 28){
        strcpy(Token, "GREATER_OP");
    }
    else if(nextToken == 29){
        strcpy(Token, "MOD_OP");
    }
    else if(nextToken == 30){
        strcpy(Token, "BOOL_NOT");
    }
    else if(nextToken == 31){
        strcpy(Token, "SEMICOLON");
    }
    else if(nextToken == 32){
        strcpy(Token, "OPEN_CURL");
    }
    else if(nextToken == 33){
        strcpy(Token, "CLOSE_CURL");
    }
    else if(nextToken == 50){
        strcpy(Token, "EQUAL_OP");
    }
    else if(nextToken == 51){
        strcpy(Token, "NEQUAL_OP");
    }
    else if(nextToken == 52){
        strcpy(Token, "LEQUAL_OP");
    }
    else if(nextToken == 53){
        strcpy(Token, "GEQUAL_OP");
    }
    else if(nextToken == 54){
        strcpy(Token, "BOOL_AND");
    }
    else if(nextToken == 55){
        strcpy(Token, "BOOL_OR");
    }
    else if(nextToken == 60){
        strcpy(Token, "KEY_IN");
    }
    else if(nextToken == 61){
        strcpy(Token, "KEY_OUT");
    }
    else if(nextToken == 62){
        strcpy(Token, "KEY_IF");
    }
    else if(nextToken == 63){
        strcpy(Token, "KEY_ELSE");
    }
    else{
        strcpy(Token, "UNKNOWN");
    }
        
    if(nextToken != EOF){
        printf("%s\t%s\n", lexeme, Token);
    
    }
    return nextToken;
} /* End of function lex */

