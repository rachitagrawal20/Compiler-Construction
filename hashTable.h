// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the implementation details of hashTable data structure.
*/

#ifndef hashTable
#define hashTable
# include "constants.h"

// Computes and returns the first probe hash value of 'in'
extern int get_hash_value(char* in); 

// Stores the 'in' in corresponding hashtable with starting probe index 'init' and returns the final hash value
extern int put_hash(char hashingTable[][TERMINAL_NONTERMINAL_LEN], int init, char* in); 

#endif