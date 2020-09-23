// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P


/*
This file contains the data structures used for the implementation of lexer.
*/

#ifndef lexerDef
#define lexerDef
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

typedef struct SymbolTable{
    char tokenName[TOKENNAME_LEN];
    char tokenVal[TOKENVAL_LEN];
    struct SymbolTable *next;
}SymbolTable;

typedef struct Token{
	char tokenName[TOKENNAME_LEN];
    SymbolTable *tokenVal;
    int lineNumber;
    struct Token *next;	
}Token;

extern int error; // stores the error code

extern struct Token *tokenListHead; // pointer to head of token list
extern struct Token *tokenListTail; // pointer to tail of token list

extern SymbolTable *symbolTableListHead;
extern SymbolTable *symbolTableListTail;

#endif