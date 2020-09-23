// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the implementation details of lexer.
*/

#ifndef lexer
#define lexer
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

// Reads keywords.txt and populates the symbol table
extern void readSymbolTable();

// Prints symbol table to the console
extern void printSymbolTable();

// Reads the input character stream and recognizes lexemes using DFA implementation
extern void getNextToken();

// Populates token list from the source code file pointed by fp
extern FILE *getStream(FILE *fp);

// Removes comments from testcaseFile and stores cleaned code in cleanFile
extern void removeComments(char *testcaseFile, char *cleanFile);

#endif
