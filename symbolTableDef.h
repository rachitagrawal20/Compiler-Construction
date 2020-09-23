// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P


#ifndef symbolTableDef
#define symbolTableDef
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "helper.h"
#include "hashTable.h"

int dec[HASHTABLE_LEN];// tells if a function with symbolTableHashTable value x is declared or not
int def[HASHTABLE_LEN];// tells if a function with symbolTableHashTable value x is defined or not


int flag[HASHTABLE_LEN];//tells if there is a valid call of the function


int var_chk[HASHTABLE_LEN];
int already_scoped;
int current_module;
char cur_scope_name[20];

typedef struct idlist
{
	int sc;
	int type;
	int subtype;
	int ub;// for arrays
	int h_val;
	int mark;
	struct idlist* next;
}idlist;

idlist *in_list[HASHTABLE_LEN];
idlist *out_list[HASHTABLE_LEN];

typedef struct lazy
{
	int scope;
	TreeNode* in;
	TreeNode* out;
	int lineno; //line number for error reporting in case of output list mismatch - used in deficit_errors print statements
	int x;// symbolTableHashTable value with whom to do parameter testing 
}lazy;

typedef struct errorDec{
	char error_msg[150];
	int x;
}errorDec;

char error_log[500][150];
errorDec errorDecnDef[150];
int err_cnt;
int errorDecnDef_count;

lazy todo[120];
int lzyptr;

SymbolTableParameters emptyNode;

int global_scope_ctr;// init to 0
int c_scope;
char symbolTableHashTable[HASHTABLE_LEN][TERMINAL_NONTERMINAL_LEN];// 8 letters long only

char moduleHashTable[HASHTABLE_LEN][TERMINAL_NONTERMINAL_LEN];
int scopeWidth[HASHTABLE_LEN];

// parent tree of scope
int parent[MAX_SCOPE_VALUE];

SymbolTableParameters scope[MAX_SCOPE_VALUE][HASHTABLE_LEN];// 110 scopes hashed over 1050 symbolTableHashTable space

int scope_map[HASHTABLE_LEN];
int cur_offset[HASHTABLE_LEN];
int start[HASHTABLE_LEN];
int end[HASHTABLE_LEN];
char ret[20];
int ma_offset;
int global_for_test;

extern int semanticError;

#endif