// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains all the constants used in the project.
*/

#ifndef constants
#define constants

#define IDENTIFIER_LEN 20
#define TOKENNAME_LEN 100
#define TOKENVAL_LEN 100
#define BUFFER_LEN 101
#define MODULENAME_LEN 25

#define MAX_STACK_SIZE 1000
#define MAX_RULES 150
#define MAX_RULE_LEN 150

#define TERMINAL_NONTERMINAL_LEN 50
#define HASHTABLE_LEN 1009  // 1009 is next prime greeater than 1000
#define MAX_TERMINALS_IN_SET 100

#define MAX_CONSEQUENTS 15 // the maximum number of terminals / non-terminals occurring on the right hand side of the rule

// hash values
#define START 817 // hash value of <programnt> as given by our hash function
#define DOLLAR 36
#define EPSILON 101

// symbol table constants
#define MOD 1009
#define epsilon 101
#define ARITHMETICOP 1
#define LOGICALOP 2
#define RELATIONALOP 3
#define ASSIGNOP 4
#define NEGATION 5
#define INTEGER 1
#define REAL 2
#define BOOLEAN 3
#define ARRAY 4
#define ERROR 5
#define EXPR 325

#define MAX_SCOPE_VALUE 110 

#endif