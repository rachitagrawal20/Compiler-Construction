// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the implementation of hashTable data structure.
*/

#include "hashTable.h"
#include <string.h>
#include "helper.h"

int get_hash_value(char* in)
{
	int ret = 0;
	int l = strlen(in);
	int i = 0;
	for(; i < l; i++)
	{
		ret = (ret*97 + in[i]) % HASHTABLE_LEN;
	}
	return ret;
}

int put_hash(char hashingTable[][TERMINAL_NONTERMINAL_LEN], int init, char* in)
{
	int ret = -10;
	int add = 1;
	int iter = 0;
	// printf("%s\n",in);
	while(1)
	{
		iter++;
		if(iter == HASHTABLE_LEN)
			return -1;
		if(hashingTable[init][0] == '\0')
		{
			// printf("Success put_hash\n");
			// mk : strcpy(hashingTable[init], in);
			// changed to
			strcpy(hashingTable[init], in);
			// printf("part2 :%s\n",in);
			ret = init;
			break;
		}
		else
		if(compareStrings(hashingTable[init], in) == 0)
		{
			ret = init;
			break;
		}
		else
		{
			// Quadratic probing
			init=(init+add*add) % HASHTABLE_LEN;
			add++;
		}
    }
	return ret;
}

