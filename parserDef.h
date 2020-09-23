// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the definitions of data structures used by the parser.
*/

#ifndef parserDef
#define parserDef
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "constants.h"

char terminalHashTable[HASHTABLE_LEN][TERMINAL_NONTERMINAL_LEN];
char nonterminalHashTable[HASHTABLE_LEN][TERMINAL_NONTERMINAL_LEN];

typedef struct ruleNode
{
	int isNonterminal;
	int hashValue;
	struct ruleNode * next;
}ruleNode;

typedef struct SymbolTableParameters 
{ 
	int sc;
	int type;
	int subtype;
	int lb; 
	int ub; 
	int offset; 
	int width;
	int dec;
	int inInputList;
	int inOutputList;
	int inForloop;
	int isDynamic;
	char module_name[MODULENAME_LEN];
} SymbolTableParameters;

typedef struct TreeNode
{
	struct TreeNode* parent;
	struct TreeNode* child;	
	struct TreeNode* next;
	int isNonterminal;
	Token* tokenPtr;
	int hashValue;
	SymbolTableParameters* symbolTablePtr;
}TreeNode;

typedef struct Grammar
{
	ruleNode** ruleHead;
	ruleNode** ruleTail;
	int numberOfRules;	
}Grammar;

typedef struct FirstAndFollow
{
	int first[HASHTABLE_LEN][MAX_TERMINALS_IN_SET];
	int follow[HASHTABLE_LEN][MAX_TERMINALS_IN_SET];
}FirstAndFollow;

typedef struct ParseTable
{
	int table[HASHTABLE_LEN][HASHTABLE_LEN];
}ParseTable;

#endif
