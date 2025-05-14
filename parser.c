#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "front.h"

static int error();

/**
 * This parsesr begins with stmt and calls the appropriate functions to test if a file adheres to the language. This includes recursion. It should
 * read each token and decide whether or not its valid in the sentence. Basically if its not valid it calls error and returns 1
 *
 * */

/*The stmt function deals with V=E; as well as if statements, if else statements, and input and outputs. This is where the program begins reading everthing and where it should end.*/
void stmt(){
    printf("Enter <stmt>\n");

    //This handles V = E; It reads ident first because it should always begin with ident
    if(nextToken == IDENT){
        lex();
        //Checks for assign op and then calls expression which ensures its a valid expression
        if(nextToken == ASSIGN_OP){
            lex();
            expr();
            //checks for semicolon at the end of the expression
            if(nextToken != SEMICOLON){
                error();
                printf("No semicolon\n");
            }
            
        }
        else{
            error();
        }
    }
    //This handles the input(V), it's very simple and does not require calling other functions as it only has ident and not expression or anything.
    else if(nextToken == KEY_IN){
        lex();
        if (nextToken == LEFT_PAREN) {
            lex();
            if(nextToken == IDENT){
                lex();
                if(nextToken == RIGHT_PAREN){
                    lex();
                    //checks for semicolon
                    if(nextToken != SEMICOLON){
                        error();
                    }
                }
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    //More complex than input, output uses expression but other than that its the same structure
    else if(nextToken == KEY_OUT){
        lex();
        if (nextToken == LEFT_PAREN) {
            lex();
            expr(); //checks if expression is valid
            if (nextToken == RIGHT_PAREN) {
                lex(); 
                if(nextToken != SEMICOLON){
                    error();
                }
            } 
            else { 
                error();
            }
   
        }
    }
    //This is the most complicated rule in stmt, this checks if statements
    else if(nextToken == KEY_IF){
        lex();
        if(nextToken == LEFT_PAREN){
            lex();
            cooke(); //checks if compare is valid (it took me this long to realize c is for compare but im not changing the name of my cooke function so you'll just have to be ok with that lol)
            if(nextToken == RIGHT_PAREN){
                lex();
                //requires first open curl
                if(nextToken == OPEN_CURL){
                    lex();
                    //this is a while loop so that basically as many statements can be in the if function as needed
                    while(nextToken == IDENT || nextToken == KEY_IF || nextToken == KEY_IN || nextToken == KEY_OUT){
                        stmt(); //calls stmt, recursion YAY
                        lex();
                    }
                    //requires close curl
                    if(nextToken == CLOSE_CURL){
                        lex();
                        //Does not require else
                        if(nextToken == KEY_ELSE){
                            lex();
                            if(nextToken == OPEN_CURL){
                                lex();
                                stmt();
                                lex(); //lex again because if i dont it gets caught up on that semicolon
                                if(nextToken != CLOSE_CURL){
                                    error();
                                }
                            }
                            else{
                                error();
                            }
                        }
                        //if there isnt an else we just call stmt again because if i dont it exits stmt and ends up skipping a token over in front.c land
                        else{
                            stmt();
                        }
                    }
                    else{
                        error();
                    }
                }
                else{
                    error();
                }
            }
            else{
                error();
            }
        }
        else{
            error();
        }
    }
    //For some unknown reason my while loop in front.c was not catching the token being EOF and would enter parser so I added this to catch it and exit with 0.
    else if(nextToken == EOF){
        //also syntax validated would never print in front.c so it makes an appearance here as well
        printf("Syntax Validated\n");
        exit(0);
    }
    else{
        error();
    }

    printf("Exit <stmt>\n");

}

/*My cooke function aka compare function basically tests to make sure the compare statements are valid. it handles the many forms of E (compared with) E and then also
    C (bool val) C
*/
void cooke(){
    printf("Enter <cooke>\n");
    
    //this is the first expression in our compare
    expr();

    //this handles the compare as well as the second expression
    while(nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken == EQUAL_OP || nextToken == NEQUAL_OP || nextToken == GEQUAL_OP || nextToken == LEQUAL_OP){
        lex();
        expr();
    }
    
    //this handles C (bool val) C, basically just calling itself for however many times it needs
    while(nextToken == BOOL_NOT || nextToken == BOOL_AND || nextToken == BOOL_OR){
        lex();
        cooke();
    }
    printf("Exit <cooke>\n");
}

/* expr
 * Parses strings in the language generated by the rule:
 * <expr> -> <term> {(+ | -) <term>}
 */
void expr() 
{
    printf("Enter <expr>\n");

    /* Parse the first term */
    term();

    /* As long as the next token is + or -, get
    the next token and parse the next term */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }

    printf("Exit <expr>\n");
} /* End of function expr */

/* term
 * Parses strings in the language generated by the rule:
 * <term> -> <factor> {(* | /) <factor>)
 */
void term() 
{
    printf("Enter <term>\n");
    /* Parse the first factor */
    factor();
    /* As long as the next token is * or / or %, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
            lex();
            factor();
    }
    printf("Exit <term>\n");
} /* End of function term */

/* factor
 * Parses strings in the language generated by the rule:
 * <factor> -> id | int_constant | ( <expr )
 * */
void factor() 
{
    printf("Enter <factor>\n");
    /* Determine which RHS */
    if (nextToken == IDENT || nextToken == INT_LIT) {
        lex(); /* Get the next token */
    } else {
        /* If the RHS is (<expr>), call lex to pass over the 
        left parenthesis, call expr, and check for the right 
        parenthesis */
        if (nextToken == LEFT_PAREN) {
            lex(); 
            expr();

            if (nextToken == RIGHT_PAREN) {
                lex(); 
            } else { 
                error();
            }
        } /* End of if (nextToken == ... */
        /* It was not an id, an integer literal, or a left parenthesis */
        else 
        { 
            error(); 
        }
    } /* End of else */
    printf("Exit <factor>\n");
} /* End of function factor */

static int error() 
{
    //prints error message
    printf("Error encounter on line %d: The next lexeme was %s and the next token was %s\n", count, lexeme, Token);
    //returns 1 and ends program
    exit(1);
}
