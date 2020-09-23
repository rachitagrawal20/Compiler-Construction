// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the implementation details of different helper functions used.
*/

#ifndef helper
#define helper
#include "lexerDef.h"
#include <stdio.h>

// Prints the lexeme corresponding to token str in the file pointed by fp
extern FILE *printLexemeToFile(FILE *fp, char* str);

// Prints the lexeme corresponding to token str in console
extern void printLexemeToConsole(char* str);

// Prints the token list of cleaned source code with head tokenListHead to the console
extern void printTokenList();

// Utility function to compare two strings similar to strcmp
extern int compareStrings(char *str1, char *str2);

extern int max(int a,int b);

#endif
