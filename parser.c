// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the core implementation of parser.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "hashTable.h"
#include "helper.h"

TreeNode* getParseTreeNode(int hashValue, int isNonterminal, TreeNode* parent)
{
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
	newNode->child  = NULL;
	newNode->next = NULL;
	newNode->tokenPtr = NULL;
	newNode->symbolTablePtr = NULL;
	newNode->hashValue = hashValue;
	newNode->isNonterminal = isNonterminal;
	newNode->parent = parent;
	
	return newNode;
}

// Pushes grammar rule given by rule_idx on to the top of parsingStack
void stackPush(Grammar *G, TreeNode* parsingStack[], int *stackTop, TreeNode* parent, int rule_idx)
{
	int first = 0;
	TreeNode* prev = NULL;
	TreeNode* arr[MAX_CONSEQUENTS];
	ruleNode* trav = G->ruleHead[rule_idx]->next;
	int arrptr = 0;
	
	if(trav->hashValue == EPSILON)
	{
		parent->child = getParseTreeNode(trav->hashValue, trav->isNonterminal, parent);
		parent->child->tokenPtr = (struct Token*)malloc(sizeof(struct Token));
		strcpy(parent->child->tokenPtr->tokenName, "epsilon");
		(parent->child)->parent = parent;	
	}
	while(trav != NULL && trav->hashValue != EPSILON)
	{
		arr[arrptr] = getParseTreeNode(trav->hashValue, trav->isNonterminal, parent);
		if(first == 0)
		{
			parent->child = arr[arrptr];
			first = 1;
		}
		if(prev!=NULL)
			prev->next = arr[arrptr];	
		
		prev = arr[arrptr];
		arrptr++;
		trav = trav->next;
	}
	arrptr--;
	while(arrptr >= 0)
	{
		parsingStack[*stackTop] = arr[arrptr];
		arrptr--;
		(*stackTop)++;
	}
}

void stackPop(int* stackTop)
{
	(*stackTop)--;
}

// Checks if terminal j exists in first set of i and adds if not already present (i and j are corresponding hash values)
int checkAndAddFirst(FirstAndFollow *F, int* hasNullTerminal, int i, int j)
{
	if(j == EPSILON)
		hasNullTerminal[i] = 1;
	int temp;
	for(temp=0; F->first[i][temp]!=-1; temp++)
	{
		if(F->first[i][temp]==j)
			return 0;
	}
	F->first[i][temp] = j;
	return 1;
}

void findFirstSet(Grammar *G, FirstAndFollow *F)
{
	int hasNullTerminal[HASHTABLE_LEN] = {0};
	int change = 0;	
	int i;
	int k;
	memset(F->first, -1, sizeof(F->first));
	do
	{
		change = 0;
		for(i = 0; i <= G->numberOfRules; i++)
		{	
			ruleNode *cur = G->ruleHead[i];
			ruleNode *trav = cur->next;
			ruleNode *prev = G->ruleHead[i];
			do
			{
				if(trav->isNonterminal == 0)
				{
					change |= checkAndAddFirst(F, hasNullTerminal, cur->hashValue,trav->hashValue);
					break;
				}
				int tmp = trav->hashValue;
				for(k = 0; F->first[tmp][k]!=-1; k++)
				{
					if(F->first[tmp][k] == EPSILON)
						continue;
					int get = checkAndAddFirst(F, hasNullTerminal, cur->hashValue, F->first[tmp][k]);
					change |= get;
				}
				if(hasNullTerminal[trav->hashValue])
				{
					prev = trav;
					trav = trav->next;
				}
				else
					break;
			}while(trav!=NULL);
			if(trav == NULL && hasNullTerminal[prev->hashValue] == 1)
				checkAndAddFirst(F, hasNullTerminal,cur->hashValue, EPSILON);
		}
	}while(change!=0);
}

// Checks if terminal j exists in follow set of i and adds if not already present (i and j are corresponding hash values)
int checkAndAddFollow(FirstAndFollow *F, int i,int j)
{
	int temp;
	for(temp=0; F->follow[i][temp]!=-1; temp++)
	{
		if(F->follow[i][temp]==j)
			return 0;
	}
	F->follow[i][temp] = j;
	return 1;
}

void findFollowSet(Grammar *G, FirstAndFollow *F)
{
	int i, change, k;
	memset(F->follow, -1, sizeof(F->follow));
	checkAndAddFollow(F, START,DOLLAR);
	do
	{
		change = 0;
		for (i = 0; i <= G->numberOfRules; ++i)
		{
			ruleNode *cur = G->ruleHead[i];
			ruleNode *trav = cur->next;
			while(trav!=NULL)
			{
				if(trav->isNonterminal == 0)
				{
					trav = trav->next;
				}
				else
				{
					ruleNode* tnext = trav->next;
					while(1)
					{
						if(tnext == NULL)
						{
							for(k = 0; F->follow[cur->hashValue][k]!=-1; k++)
							{
								change |= checkAndAddFollow(F, trav->hashValue, F->follow[cur->hashValue][k]);
							}
							break;
						}
						else if(tnext->isNonterminal==0)
						{
							change |= checkAndAddFollow(F, trav->hashValue,tnext->hashValue);
							break;
						}
						else
						{
							int got_epsilon = 0;							
							for(k = 0; F->first[tnext->hashValue][k]!=-1;k++)
							{
								if(F->first[tnext->hashValue][k] != EPSILON)
									change |= checkAndAddFollow(F, trav->hashValue, F->first[tnext->hashValue][k]);
								else
									got_epsilon = 1;
							}
							if(!got_epsilon)
								break;
						}
						tnext = tnext->next;
					 }
					 trav = trav->next;
				}
			}
		}
	}while(change != 0);
}

void ComputeFirstAndFollowSets(Grammar *G, FirstAndFollow *F)
{
	findFirstSet(G, F);
	findFollowSet(G, F);
}

void createParseTable(Grammar *G, FirstAndFollow *F, ParseTable *T)
{
	memset(T->table, -1, sizeof(T->table));
	int i, j;
	for(i = 0; i <= G->numberOfRules; i++)
	{
		int lhs = G->ruleHead[i]->hashValue;
	    ruleNode* trav = G->ruleHead[i]->next;
	    int is_eps = 0;
	    while(trav != NULL)
	    {
	    	if(trav->hashValue == EPSILON)
	    	{
	    		is_eps = 1;
	    		break;
	    	}
	    	else if(trav->isNonterminal == 0)
	    	{
	    		T->table[lhs][trav->hashValue] = i;
	    		break;
	    	}
	    	else 
	    	{
	    		int is_eps2 = 0;
	    		for(j = 0;F->first[trav->hashValue][j]!=-1;j++)
	    		{
	    			if(F->first[trav->hashValue][j] == EPSILON)
	    			{
	    				is_eps2 = 1;
	    			}
	    			else
	    			{
	    				T->table[lhs][F->first[trav->hashValue][j]] = i;
	    			}
	    		}
	    		if(is_eps2 == 0)
	    			break;
	    	}
	    	trav = trav->next;
	    }
	    if(trav == NULL)
	    	is_eps = 1;
	    if(is_eps)
	    {
	    	for(j = 0;F->follow[lhs][j]!=-1;j++)
	    	{
	    		T->table[lhs][F->follow[lhs][j]] = i;
	    	}
	    }
	}
}

void printFirstAndFollowSet(FirstAndFollow *F, char *firstFile, char *followFile){
	FILE *fp = fopen(firstFile, "w");
	for(int i = 0; i < HASHTABLE_LEN; i++){
		fprintf(fp, "%s: \t", nonterminalHashTable[i]);
		for(int j=0; j < MAX_TERMINALS_IN_SET; j++){
			if(F->first[i][j] == -1){
				break;
			}
			fprintf(fp, "%s, ", terminalHashTable[F->first[i][j]]);
		}
		fprintf(fp, "\n");		
	}
	fclose(fp);

	fp = fopen(followFile, "w");
	for(int i = 0; i < HASHTABLE_LEN; i++){
		fprintf(fp, "%s: \t", nonterminalHashTable[i]);
		for(int j=0; j < MAX_TERMINALS_IN_SET; j++){
			if(F->follow[i][j] == -1){
				break;
			}
			fprintf(fp, "%s, ", terminalHashTable[F->follow[i][j]]);
		}
		fprintf(fp, "\n");		
	}
	fclose(fp);
}

void printParseTable(ParseTable *T, char *outFile){
	FILE *fp = fopen(outFile, "w");
	for(int i=0; i < HASHTABLE_LEN; i++){
		for(int j=0; j < HASHTABLE_LEN; j++){
			if(T->table[i][j] != -1)
			fprintf(fp, "%s\t%s:\t%d\n", nonterminalHashTable[i], terminalHashTable[j], T->table[i][j]);
		}		
	}
	fclose(fp);
}

void printStack(TreeNode* parsingStack[], int stackTop){
	printf("Stack:\t");
	for(int i=stackTop-1; i>=0; i--){
		if(parsingStack[i]->isNonterminal){
			printf("%s\t", nonterminalHashTable[parsingStack[i]->hashValue]);
		}else{
			printf("%s\t", terminalHashTable[parsingStack[i]->hashValue]);
		}
	}
	printf("\n");
} 

// Checks if terminalHashValue is present in first or follow set of nonterminalHashValue, used for error recovery
int isInFirstOrFollow(FirstAndFollow *F, int nonTerminalHashValue, int terminalHashValue){

	for(int i=0; i<MAX_TERMINALS_IN_SET; i++){
		if(F->first[nonTerminalHashValue][i] == terminalHashValue){
			return 1; //present in first set
		}
	}

	for(int i=0; i<MAX_TERMINALS_IN_SET; i++){
		if(F->follow[nonTerminalHashValue][i] == terminalHashValue){
			return 2; //present in follow set
		}
	}
	return 0; //not present in either sets
}

TreeNode* parseInputSourceCode(Grammar *G, ParseTable *T, FirstAndFollow *F)
{
	struct Token* temp_dollar = (struct Token*)malloc(sizeof(struct Token));
	strcpy(temp_dollar->tokenName, "$");
	temp_dollar->tokenVal = NULL;
	temp_dollar->lineNumber = -1;
	temp_dollar->next = NULL;
	tokenListTail->next = temp_dollar;


	TreeNode* parsingStack[MAX_STACK_SIZE] = {NULL};
	int stackTop = 0;

    TreeNode *parseTreeHead = getParseTreeNode(START, 1, NULL);
	parsingStack[stackTop] = parseTreeHead;
	stackTop++;
	TreeNode* store_head;

	// printTokenList(tokenListHead);

	Token* token_ptr = tokenListHead;

	int flag = 0;// error not shown for current token
	
	while(stackTop != 0 && token_ptr != NULL)
	{
		// printStack(parsingStack, stackTop);
		// printf("Next Token: %s\n\n", token_ptr->tokenName);

		int hval = parsingStack[stackTop-1]->hashValue;
		if (parsingStack[stackTop-1]->isNonterminal == 0)
		{
			if(compareStrings(terminalHashTable[hval], token_ptr->tokenName) == 0)
			{
				parsingStack[stackTop-1]->tokenPtr = token_ptr;
				stackPop(&stackTop);
				token_ptr = token_ptr->next;
				flag = 0;
			}
			else
			{
				error = 1;
				if(!flag){
					printf("ERROR_5: %d: Error in the token %s for lexeme ", token_ptr->lineNumber, token_ptr->tokenName);
					if(token_ptr->tokenVal == NULL)
						printLexemeToConsole(token_ptr->tokenName);
					else
						printf("%s ",(token_ptr->tokenVal)->tokenVal);
					printf("as expected token is %s\n", terminalHashTable[hval]);		
				}
				flag = 1; // error shown for current token
				stackPop(&stackTop);
			}
		}
		else
		{
			int hash_of_name = get_hash_value(token_ptr->tokenName);
			hash_of_name = put_hash(terminalHashTable, hash_of_name, token_ptr->tokenName);
			int rule_idx = T->table[hval][hash_of_name];
			if (rule_idx == -1)
			{	
				error = 1;
				char expected[TERMINAL_NONTERMINAL_LEN];
				// Why 0? Can be any terminal for that non-terminal, why first one considered as expected
				strcpy(expected, terminalHashTable[F->first[hval][0]]);

				Token *errorToken = token_ptr;

				while(token_ptr != NULL)
				{	
					if(compareStrings(token_ptr->tokenName,"$")==0)
					{
						token_ptr = token_ptr->next;
						break;
					}	

					hash_of_name = get_hash_value(token_ptr->tokenName);
					hash_of_name = put_hash(terminalHashTable, hash_of_name, token_ptr->tokenName);
					
					int isPresent = isInFirstOrFollow(F, hval, hash_of_name);
					
					if(isPresent == 1){
						//present in first
						break;
					}else if(isPresent == 2){
						//present in follow
						stackPop(&stackTop);
						break;
					}
					token_ptr = token_ptr->next;
				}
				if(!flag){
					flag = 1;
					printf("ERROR_5: %d: Error in the token %s for lexeme ", errorToken->lineNumber, errorToken->tokenName);
					if(errorToken->tokenVal == NULL)
						printLexemeToConsole(errorToken->tokenName);
					else
						printf("%s ",(errorToken->tokenVal)->tokenVal);
					printf("as expected token is %s\n", expected);
				}
				if(errorToken != token_ptr){
					flag = 0;
				}
			}
			else
			{
				store_head = parsingStack[stackTop-1];
				stackPop(&stackTop);
				stackPush(G, parsingStack, &stackTop, store_head, rule_idx);
			}
		}
	}

	while(stackTop!=0)
		stackPop(&stackTop);

	tokenListTail->next = NULL;
	free(temp_dollar);
	return parseTreeHead;
}

// Recursive utility function to print parse tree rooted at head
void printTreeUtil(TreeNode* head, int* nodeCount, int flag, FILE *fp)
{
	if(head == NULL)
		return;

	//incrementing node count
	(*nodeCount)++;

	printTreeUtil(head->child, nodeCount, flag, fp);
	
	if(flag == 1)
	{
		if(head->isNonterminal==0)
		{
			if(head->hashValue == EPSILON)
			{
				//LEXEME
				fprintf(fp, "----\t");

				//LINENO
				fprintf(fp, "----\t");

				//TOKEN
				fprintf(fp, "----\t");

				//valueIfNumber
				fprintf(fp, "----\t");

				//parentNodeSymbol
				if(head->parent==NULL)
					fprintf(fp, "ROOT\t");
				else
					fprintf(fp, "%s\t",nonterminalHashTable[(head->parent)->hashValue]+1);

				//isLeafNode
				fprintf(fp, "yes\t");

				//NodeSymbol
				fprintf(fp, "%s\t",terminalHashTable[head->hashValue]);
			}
			else
			{
				//LEXEME
				if((head->tokenPtr)->tokenVal==NULL)
					printLexemeToFile(fp, (head->tokenPtr)->tokenName);
				else
					fprintf(fp, "%s\t",((head->tokenPtr)->tokenVal)->tokenVal);

				//LINENO
				fprintf(fp, "%d\t",(head->tokenPtr)->lineNumber);

				//TOKEN
				fprintf(fp, "%s\t",(head->tokenPtr)->tokenName);

				//valueIfNumber
				if(compareStrings((head->tokenPtr)->tokenName,"NUM")==0 || compareStrings((head->tokenPtr)->tokenName,"RNUM")==0)
					fprintf(fp, "%s\t",((head->tokenPtr)->tokenVal)->tokenVal);
				else
					fprintf(fp, "----\t");

				//parentNodeSymbol
				if(head->parent==NULL)
					fprintf(fp, "ROOT\t");
				else{
					if(strlen(nonterminalHashTable[(head->parent)->hashValue]+1)!=0)
						fprintf(fp, "%s  ",nonterminalHashTable[(head->parent)->hashValue]+1);
					// fprintf(fp, "%d  ",(head->parent)->hashValue);
					else
						fprintf(fp, "%s\t ",terminalHashTable[(head->parent)->hashValue]);
				}

				//isLeafNode
				fprintf(fp, "yes\t");

				//NodeSymbol
				fprintf(fp, "----\t");
			}
		}
		else
		{
			//LEXEME
			fprintf(fp, "----\t");

			//LINENO
			fprintf(fp, "----\t");

			//TOKEN
			fprintf(fp, "----\t");

			//valueIfNumber
			fprintf(fp, "----\t");

			//parentNodeSymbol
			if(head->parent==NULL)
				fprintf(fp, "ROOT\t");
			else{
				if(strlen(nonterminalHashTable[(head->parent)->hashValue]+1)!=0)
					fprintf(fp, "%s\t",nonterminalHashTable[(head->parent)->hashValue]+1);
				// fprintf(fp, "%d  ",(head->parent)->hashValue);
				else
					fprintf(fp, "%s\t ",terminalHashTable[(head->parent)->hashValue]);
			}

			//isLeafNode
			fprintf(fp, "no\t");

			//NodeSymbol
			fprintf(fp, "%s\t",nonterminalHashTable[head->hashValue]+1);
		}
		fprintf(fp, "\n");
	}
	//PRINTING SIBLINGS
	if(head->child != NULL)
	{
		TreeNode* siblings = (head->child)->next;
		while(siblings != NULL)
		{
			printTreeUtil(siblings, nodeCount, flag, fp);
			siblings = siblings->next;
		}
	}  
}

int printTree(TreeNode *PT, char* outFile){
	FILE *fp = fopen(outFile, "w"); 
	//PRINTING THE NODE
	fprintf(fp, "lexemeCurrentNode\t lineno\t token\t valueIfNumber\t parentNodeSymbol\t isLeafNode\t NodeSymbol\n");
	int nodeCount = 0;
	printTreeUtil(PT, &nodeCount, 1, fp);
	fclose(fp);
	return nodeCount;
}

void writeHashTables(){
	FILE * fp = fopen("generated/terminalHash.txt", "w");
	for(int i=0; i < HASHTABLE_LEN; i++){
		fprintf(fp, "%d -> %s\n", i, terminalHashTable[i]);
	}
	fclose(fp);
	fp = fopen("generated/nonterminalHash.txt", "w");
	for(int i=0; i < HASHTABLE_LEN; i++){
		fprintf(fp, "%d -> %s\n", i, nonterminalHashTable[i]);
	}
	fclose(fp);
}

// Inserts terminal/non-terminal in rule list of grammar rule at position pos 
void insertRule(Grammar *G, int pos, int nonterm, int hashval)
{
	ruleNode* newruleNode = (ruleNode*)malloc(sizeof(ruleNode));
	newruleNode->isNonterminal = nonterm;
	newruleNode->hashValue = hashval;
	newruleNode->next = NULL;

	if(G->ruleHead[pos] == NULL || G->ruleTail[pos] == NULL)
	{
		G->ruleHead[pos] = newruleNode;
		G->ruleTail[pos] = newruleNode;
	}
	else
	{
		G->ruleTail[pos]->next = newruleNode;
		G->ruleTail[pos] = G->ruleTail[pos]->next;
	}
}

void printGrammar(Grammar *G, char* outFile){
	FILE *fp = fopen(outFile, "w");
	for(int i=0; i<=G->numberOfRules; i++){
		ruleNode *temp = G->ruleHead[i];
		fprintf(fp, "%s -> \t", nonterminalHashTable[temp->hashValue]);
		temp = temp->next;
		while(temp){
			if(temp->isNonterminal){
				fprintf(fp, "%s,\t", nonterminalHashTable[temp->hashValue]);
			}else{
				fprintf(fp, "%s,\t", terminalHashTable[temp->hashValue]);
			}
			temp = temp->next;
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

Grammar* readGrammar()
{

	char separatedGrammarRules[MAX_RULES][MAX_RULE_LEN] = {'\0'}; // $ is used as a separator
	FILE* fp = fopen("Updated_Grammar.txt","r");
	char *line = NULL;
	size_t len = 0;
	int ruleNo = -1;
	int i, j;
	
	while(getline(&line, &len, fp) != -1)
	{
		int l = strlen(line);
		char nt[50] = {'\0'};
		int ptr = 0;
		int tptr = 0;
		int gptr = 0;
		while(ptr < l)
		{
			tptr = 0;
			while(ptr < l && line[ptr]!='-' && line[ptr]!=' ' && line[ptr]!='|' && line[ptr]!='\t' && line[ptr]!='\n' && line[ptr]!='\r')
			{
				nt[tptr] = line[ptr];
				ptr++; tptr++;
			}
			if(tptr != 0 || line[ptr] == '|')
			{
				if(line[ptr] == '-')
				{
					ruleNo++;
					gptr = 0;
				}

				for(i = 0; i < tptr; i++)
						separatedGrammarRules[ruleNo][gptr++] = nt[i];
				
				if(tptr)
					separatedGrammarRules[ruleNo][gptr++] = '$';
				
				if(line[ptr] == '|')
				{
					int nxtrul = ruleNo + 1;
					int newptr = 0;
					while(separatedGrammarRules[ruleNo][newptr]!='$')
					{
						separatedGrammarRules[nxtrul][newptr] = separatedGrammarRules[ruleNo][newptr];
						newptr++;
					}
					separatedGrammarRules[nxtrul][newptr++] = '$';
					gptr = newptr;
					ruleNo = nxtrul;
				}
			}
			ptr++;

		}
	}

	Grammar *G = (Grammar *)malloc(sizeof(Grammar));
	G->ruleHead = (ruleNode **)malloc(sizeof(ruleNode*) * MAX_RULES);
	G->ruleTail = (ruleNode **)malloc(sizeof(ruleNode*) * MAX_RULES);
	G->numberOfRules = ruleNo;

	for(int i=0; i<=ruleNo; i++){
		G->ruleHead[i] = NULL;
		G->ruleTail[i] = NULL;
	}

	char dol[5] = "$"; 
	for(i = 0; i <= ruleNo; i++)
	{
		char unit[TERMINAL_NONTERMINAL_LEN];
		int l = strlen(separatedGrammarRules[i]);
		int ptr = 0;
		int first = 1;
		while(ptr < l)
		{
			int uptr = 0;
			int isNonterminal;
			while(separatedGrammarRules[i][ptr] != '$')
			{
				unit[uptr] = separatedGrammarRules[i][ptr];
				uptr++;
				ptr++;
			}
			if(unit[uptr-1] != '>'){
				isNonterminal = 0;
			}else{
				isNonterminal = 1;
				unit[0] = ' ';
				unit[uptr-1] = '\0';
			}
			unit[uptr] = '\0';

			ptr++;
			
			int initval = get_hash_value(unit);
			int hashval;
			
			if(initval < 0){
				printf("%d -> %s\n%s",i, separatedGrammarRules[i], unit);
			}

			if(isNonterminal){
				hashval = put_hash(nonterminalHashTable, initval, unit);
			}else{
				 hashval = put_hash(terminalHashTable, initval, unit);
			}

			if(hashval == -1)
			{
				printf("ERROR_4: Unable to hash %s!\n", unit);
				return NULL;
			}

			insertRule(G, i, isNonterminal, hashval);
		}
	}
	put_hash(terminalHashTable, get_hash_value(dol), dol);
	fclose(fp);



	// FILE* fp1 = fopen("generated/separatedGrammar.txt","+w");
	// for(i = 0; i <= ruleNo; i++)
	// {
	// 	fprintf(fp1,"%s\n", separatedGrammarRules[i]);
	// }
	// fclose(fp1);
	return  G;
}
