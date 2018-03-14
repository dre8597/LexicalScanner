#include <iostream>
#include "Parser.h"
//
//  main.cpp
//  Lexical Scanner
//
//  Created by Demondre Livingston Michaela Agee on 3/12/18.
//  Copyright © 2018 Demondre Livingston. All rights reserved.
using namespace std;

/* Global declarations */

/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

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
#define RIGHT_BRACKET 27
#define LEFT_BRACKET 28
#define COMMA 29
#define SEMICOLON 30
#define KEYWORD 31

/******************************************************/
/* main driver */
int main() {

    Parser parser;
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("program1.txt", "r")) == NULL)
        printf("ERROR - cannot open \n");
    else {
        cout << "/////////////////////////" << endl;
        cout << "TEST PROGRAM 1:" << endl;
        cout << "/////////////////////////" << endl;
        getChar();
        //lexical analyzer
        lex();
        //parser/syntax analyzer
        parser.program();

        cout << "The test program is correct" << endl << endl << endl <<
             "|||||||||||||||||||||||||||||||||||||||||||||||" << endl << endl << endl;
    }

    cout << "/////////////////////////" << endl;
    cout << "TEST PROGRAM 2:" << endl;
    cout << "/////////////////////////" << endl;
    if ((in_fp = fopen("program2.txt", "r")) == NULL)
        printf("ERROR - cannot open \n");
    else {
        getChar();
        //lexical analyzer
        lex();
        //parser functions
        parser.program();


        cout << "The test program is correct" << endl;
    }
}
/*****************************************************/
//lexical analyzer code
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup(char ch) {
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

        case '{':
            addChar();
            nextToken = LEFT_BRACKET;
            break;

        case '}':
            addChar();
            nextToken = RIGHT_BRACKET;
            break;

        case ',':
            addChar();
            nextToken = COMMA;
            break;

        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;

        case ';':
            addChar();
            nextToken = SEMICOLON;
            break;

        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;

}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {

        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
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

        default:break;
    } /* End of switch */

    printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);

    return nextToken;
} /* End of function lex */

/////////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************/
//parser code

/* program - Parses strings in the language generated by the rule:
 <program> → <keyword> <funcname> () { <declare> <assign> }
 */

void Parser::program() {
    printf("Enter <program>\n");
    keyword();
    funcname();
    if (nextToken == LEFT_PAREN) {
        lex();
        if (nextToken == RIGHT_PAREN) {
            lex();
            if (nextToken == LEFT_BRACKET) {
                lex();
                declare();
                assign();
                if (nextToken == RIGHT_BRACKET) {
                    lex();
                }
                else {
                    cout << "missing right bracket" << endl;

                    return;
                }

            }
            else {
                cout << "missing left bracket" << endl;

                return;
            }
        }
        else {
            cout << "missing right parentheses" << endl;

            return;
        }
    }
    else {
        cout << "missing left parentheses" << endl;

        return;
    }

    printf("Exit <program>\n");
}

/* expr - Parses strings in the language generated by the rule:
 <expr> → <term> {(+ | -) <term>}
 */

void Parser::expr() {
    printf("Enter <expr>\n");
    term();
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
}

/* keyword - Parses strings in the language generated by the rule:
 <keyword> → int */

void Parser::keyword() {
    printf("Enter <keyword>\n");
    if (lexeme[0] == 'i' && lexeme[1] == 'n' && lexeme[2] == 't' &&
        lexeme[3] == 0) {
        lex();
    }
    else {
        cout << "missing keyword" << endl;

        return;
    }
    printf("Exit <keyword>\n");
}

/* declare - Parses strings in the language generated by the rule:
 <declare> → int <ident>; | int <ident>, <declarident> */

void Parser::declare() {
    printf("Enter <declare>\n");
    if(lexeme[0] == 'i' && lexeme[1] == 'n' && lexeme[2] == 't' &&
       lexeme[3] == 0) {
        lex();
        ident();
        if (nextToken == SEMICOLON) {
            lex();

        }
        else if (nextToken == COMMA) {
            lex();
            declarident();
        }
        else {
            cout << "missing a semicolon or comma" << endl;

            return;
        }
    }
    else {
        cout << "missing keyword" << endl;

        return;
    }
    printf("Exit <declare>\n");
}

/* term - Parses strings in the language generated by the rule:
 <term> → <ident> | (<expr>) */

void Parser::term() {

    printf("Enter <term>\n");
    if (nextToken == LEFT_PAREN) {
        lex();
        expr();
        if (nextToken == RIGHT_PAREN) {
            lex();
        }
        else {
            cout << "missing right parenthese" << endl;

            return;
        }
    }
    else
        ident();

    printf("Exit <term>\n");
}

/* funcname - Parses strings in the language generated by the rule:
 <funcname> → <ident> */

void Parser::funcname() {
    printf("Enter <funcname>\n");
    ident();
    printf("Exit <funcname>\n");
}

/* assign - Parses strings in the language generated by the rule:
 <assign> → <ident> = <expr>; */

void Parser::assign() {
    printf("Enter <assign>\n");
    ident();
    if (nextToken == ASSIGN_OP) {
        lex();
        expr();
        if (nextToken == SEMICOLON) {
            lex();
        }
        else {
            cout << "missing semicolon" << endl;
            return;
        }
    }
    else {
        cout << "missing assignment operator" << endl;
        return;
    }
    printf("Exit <assign>\n");
}

/* declarident - Parses strings in the language generated by the rule:
 <declarident> → <ident>, <declarident> | <ident>;

void Parser::declarident() {

    printf("Enter <declarident>\n");
    ident();
    if (nextToken == COMMA) {
        lex();
        declarident();
    }
    else if (nextToken == SEMICOLON) {
        lex();
    }
    else {
        cout << "missing comma or semicolon" << endl;
        return;
    }
    printf("Exit <declarident>\n");
}

/* ident - Parses strings in the language generated by the rule:
 <ident> → a<ident> | b<ident> | ... |z<ident>|ε
 */

void Parser::ident() {
    printf("Enter <ident>\n");
    if (nextToken == IDENT) {
        lex();
    }
    else {
        cout << "missing identifier" << endl;

        return;
    }
    printf("Exit <ident>\n");
}
