// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the core implementation of lexer
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "helper.h"

char buffer[BUFFER_LEN];
char cleanString[BUFFER_LEN];

int dfaState, cleanStringLen, lineNumber;

SymbolTable *symbolTableListHead = NULL;
SymbolTable *symbolTableListTail = NULL;

struct Token *tokenListHead = NULL;
struct Token *tokenListTail = NULL;

int error = 0;

void printSymbolTable()
{
	SymbolTable *head = symbolTableListHead;
    while(head!=NULL)
    {
        printf("%s %s\n",head->tokenName,head->tokenVal);
        head = head->next;
    }
}

// creates and returns the node of Symbol Table 
SymbolTable* createSymboleTableNode(char* tokenName,char* tokenVal)
{
    SymbolTable* temp = (SymbolTable*)malloc(sizeof(SymbolTable));
    strcpy(temp->tokenName,tokenName);
    if(tokenVal!=NULL)
        strcpy(temp->tokenVal,tokenVal);
    else
        temp->tokenVal[0] = '\0';
    temp->next = NULL;
    return temp;
}

// insert the tokenName and tokenValue to Symbol Table
void insertToSymbolTable(char* tokenName,char* tokenVal)
{
    SymbolTable* temp = createSymboleTableNode(tokenName,tokenVal);
    if(symbolTableListHead == NULL)
    {
        symbolTableListHead = temp;
        symbolTableListTail = temp;
    }
    else
    {
        symbolTableListTail->next = temp;
        symbolTableListTail = temp;
    }
}


void searchSymbolTable(char *tokenValue,char *tokenName)
{
	SymbolTable *head = symbolTableListHead;
	while(head!=NULL)
	{
		if(compareStrings(head->tokenVal, tokenValue)==0)
		{
			struct Token* temp = (struct Token*)malloc(sizeof(struct Token));
			strcpy(temp->tokenName,head->tokenName);
			temp->tokenVal = head;
			temp->lineNumber = lineNumber;
			temp->next = NULL;

			//inserting in the token list
			if(tokenListHead==NULL)
		    {
		        tokenListHead = temp;
		        tokenListTail = temp;
		    }
		    else
		    {
		        tokenListTail->next = temp;
		        tokenListTail = temp;
		    }
			return;
		}
		head = head->next;
	}

	if(compareStrings(tokenName,"ID") == 0 && strlen(tokenValue) > IDENTIFIER_LEN) 	
	{
		error = 1;
		printf("ERROR_1: %d: Identifier is longer than the prescribed length\n", lineNumber);
		return;
	}

	Token* temp = (struct Token*)malloc(sizeof(struct Token));
	
	insertToSymbolTable(tokenName, tokenValue);
	
	strcpy(temp->tokenName,tokenName);
	temp->tokenVal = symbolTableListTail;
	temp->lineNumber = lineNumber;
	temp->next = NULL;

	//inserting in the token list
	if(tokenListHead==NULL)
	{
	    tokenListHead = temp;
	    tokenListTail = temp;
	}
	else
	{
		tokenListTail->next = temp;
		tokenListTail = temp;
	}
	return;
}

void readSymbolTable()
{
	char _tokenVal[TOKENVAL_LEN];
	char _tokenName[TOKENNAME_LEN];
	
	FILE *fp = fopen("keywords.txt","r");
	if(fp != NULL){
		while(fscanf(fp,"%s %s", _tokenVal, _tokenName) == 2)
			insertToSymbolTable(_tokenName, _tokenVal);
	}
	fclose(fp);
}

void insertTokenToTokenList(char* tokenName, SymbolTable* tokenVal)
{
	struct Token* temp = (struct Token*)malloc(sizeof(struct Token));
	strcpy(temp->tokenName,tokenName);
	temp->tokenVal = tokenVal;
	temp->lineNumber = lineNumber;
	temp->next = NULL;

    if(tokenListHead==NULL)
    {
        tokenListHead = temp;
        tokenListTail = temp;
    }
    else
    {
        tokenListTail->next = temp;
        tokenListTail = temp;
    }
}

void getNextToken()
{
	int start = 0;
	while(1)
	{
		if(buffer[start]=='\0') {
			break;
		}
		switch(dfaState)
		{
			case 0: cleanStringLen = 0;
					if((buffer[start]>='a' && buffer[start]<='z') || (buffer[start]>='A' && buffer[start]<='Z'))
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 1;
					}
					else if(buffer[start]>='0' && buffer[start]<='9')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 32;
					}
					else if(buffer[start]=='+')
					{
						insertTokenToTokenList("PLUS",NULL);
						start++;
					}
					else if(buffer[start]=='-')
					{
						insertTokenToTokenList("MINUS",NULL);
						start++;
					}
					else if(buffer[start]=='/')
					{
						insertTokenToTokenList("DIV",NULL);
						start++;
					}
					else if(buffer[start]=='<')
					{
						start++;
						dfaState = 17;
					}
					else if(buffer[start]=='>')
					{
						start++;
						dfaState = 21;
					}
					else if(buffer[start]=='=')
					{
						start++;
						dfaState = 31;
					}
					else if(buffer[start]=='!')
					{
						start++;
						dfaState = 30;
					}
					else if(buffer[start]==':')
					{
						start++;
						dfaState = 26;
					}
					else if(buffer[start]=='.')
					{
						start++;
						dfaState = 28;
					}
					else if(buffer[start]==';')
					{
						start++;
						insertTokenToTokenList("SEMICOL",NULL);
					}
					else if(buffer[start]==',')
					{
						start++;
						insertTokenToTokenList("COMMA",NULL);
					}
					else if(buffer[start]=='[')
					{
						start++;
						insertTokenToTokenList("SQBO",NULL);
					}
					else if(buffer[start]==']')
					{
						start++;
						insertTokenToTokenList("SQBC",NULL);
					}
					else if(buffer[start]=='(')
					{
						start++;
						insertTokenToTokenList("BO",NULL);
					}
					else if(buffer[start]==')')
					{
						start++;
						insertTokenToTokenList("BC",NULL);
					}
					else if(buffer[start]=='\n')
					{
						start++;
						lineNumber++;
					}
					else if(buffer[start]=='*')
					{
						start++;
						dfaState = 4;
					}
					else if(buffer[start]==' ' || buffer[start]=='\t' || buffer[start]=='\r')
					{
						start++;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_2: Unknown Symbol %c at line %d\n",buffer[start],lineNumber);
						start++;
					}
					break;

			case 1: 
					if((buffer[start]>='a' && buffer[start]<='z') || (buffer[start]>='A' && buffer[start]<='Z') || (buffer[start]>='0' && buffer[start]<='9') || buffer[start]=='_')
						cleanString[cleanStringLen++] = buffer[start++];
					else
					{
						//if(buffer[start]=='\0')
						//	exec=0;
						cleanString[cleanStringLen] = '\0';
						searchSymbolTable(cleanString,"ID");
						dfaState = 0;
					}
					break;

			case 32: 
					if(buffer[start]>='0' && buffer[start]<='9')
						cleanString[cleanStringLen++] = buffer[start++];
					// else if(buffer[start]=='E' || buffer[start]=='e')
					// {
					// 	cleanString[cleanStringLen++] = buffer[start++];
					// 	dfaState = 7;
					// }
					else if(buffer[start]=='.')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 34;	
					}
					else
					{
						cleanString[cleanStringLen] = '\0';
						searchSymbolTable(cleanString,"NUM");
						dfaState = 0;
					}
					break;

			case 34: 
					if(buffer[start]>='0' && buffer[start]<='9')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 35;
					}
					else if(buffer[start]=='.')
					{
						cleanString[cleanStringLen-1] = '\0';
						start--;
						searchSymbolTable(cleanString,"NUM");
						dfaState = 0;
					}
					else
					{
						//ERROR
						error = 1;
						cleanString[cleanStringLen] = '\0';
						printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
						dfaState = 0;
					}
					break;

			case 35: 
					if(buffer[start]>='0' && buffer[start]<='9')
						cleanString[cleanStringLen++] = buffer[start++];
					else if(buffer[start]=='E' || buffer[start]=='e')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 38;
					}
					else
					{
						cleanString[cleanStringLen] = '\0';
						searchSymbolTable(cleanString,"RNUM");
						dfaState = 0;
					}
					break;

			case 38: 
			        if(buffer[start]=='+' || buffer[start]=='-')
			        {
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 40;
					}
					else if(buffer[start]>='0' && buffer[start]<='9')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 39;
					}
					else
					{
						//ERROR
						error = 1;
						cleanString[cleanStringLen] = '\0';
						printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
						dfaState = 0;
					}
					break;

			case 40: if(buffer[start]>='0' && buffer[start]<='9')
					{
						cleanString[cleanStringLen++] = buffer[start++];
						dfaState = 39;
					}
					else
					{
						//ERROR
						error = 1;
						cleanString[cleanStringLen] = '\0';
						printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
						dfaState = 0;
					}
					break;

			case 39: if(buffer[start]>='0' && buffer[start]<='9')
						cleanString[cleanStringLen++] = buffer[start++];
					else
					{
						cleanString[cleanStringLen] = '\0';
						searchSymbolTable(cleanString,"RNUM");
						dfaState = 0;
					}
					break;

			case 17:if(buffer[start]=='=')
					{
						start++;
						insertTokenToTokenList("LE",NULL);
						dfaState = 0;
					} 
					else if(buffer[start]=='<')
					{
						start++;
						//insertTokenToTokenList("DEF",NULL);
						dfaState = 19;
					}
					else
					{
						insertTokenToTokenList("LT",NULL);
						dfaState = 0;
					}
					break;

			case 19: if(buffer[start]=='<')
					 {
					 	start++;
						insertTokenToTokenList("DRIVERDEF",NULL);
						dfaState = 0;
					 }
					 else
					 {
					 	insertTokenToTokenList("DEF",NULL);
						dfaState = 0;
					 }
					 break;

			case 21: if(buffer[start]=='=')
					{
						start++;
						insertTokenToTokenList("GE",NULL);
						dfaState = 0;
					} 
					else if(buffer[start]=='>')
					{
						start++;
						//insertTokenToTokenList("ENDDEF",NULL);
						dfaState = 23;
					}
					else
					{
						insertTokenToTokenList("GT",NULL);
						dfaState = 0;
					}
					break;

			case 23: if(buffer[start]=='>')
					 {
					 	start++;
						insertTokenToTokenList("DRIVERENDDEF",NULL);
						dfaState = 0;
					 }
					 else
					 {
					 	insertTokenToTokenList("ENDDEF",NULL);
						dfaState = 0;
					 }
					 break;

			case 4: if(buffer[start]=='*')
					{
						//implement comment ignoring part here
					}
					else
					{
						insertTokenToTokenList("MUL",NULL);
						dfaState = 0;
					}
					break;

			case 31: if(buffer[start]=='=')
					{
						start++;
						insertTokenToTokenList("EQ",NULL);
						dfaState = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: %d: Unknown pattern =\n",lineNumber);
						dfaState = 0;
					}
					break;

			case 30: if(buffer[start]=='=')
					{
						start++;
						insertTokenToTokenList("NE",NULL);
						dfaState = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: %d: Unknown pattern\n",lineNumber);
						dfaState = 0;
					}
					break;

			case 26: if(buffer[start]=='=')
					{
						start++;
						insertTokenToTokenList("ASSIGNOP",NULL);
						dfaState = 0;
					}
					else
					{
						insertTokenToTokenList("COLON",NULL);
						dfaState = 0;
					}
					break;

			case 28: if(buffer[start]=='.')
					{
						start++;
						insertTokenToTokenList("RANGEOP",NULL);
						dfaState = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: %d: Unknown pattern .\n",lineNumber);
						dfaState = 0;
					}
					break;
		}
	}
}

FILE *getStream(FILE *fp) {
	tokenListHead = NULL;
	tokenListTail = NULL;
	dfaState = 0;
	cleanStringLen = 0;
	lineNumber = 1;
	int len_read;
	while(len_read = fread(buffer, 1, BUFFER_LEN-1, fp))
	{
		buffer[len_read] = '\0';
		getNextToken();
	}

	switch(dfaState)
	{
			case 1: 
					cleanString[cleanStringLen] = '\0';
					searchSymbolTable(cleanString,"ID");
					break;

			case 32: 
					cleanString[cleanStringLen] = '\0';
					searchSymbolTable(cleanString,"NUM");
					break;

			case 34: 
					//ERROR
					error = 1;
					cleanString[cleanStringLen] = '\0';
					printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
					break;

			case 35: 
					cleanString[cleanStringLen] = '\0';
					searchSymbolTable(cleanString,"RNUM");
					break;

			case 38: 
					//ERROR
					error = 1;
					cleanString[cleanStringLen] = '\0';
					printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
					break;

			case 40: 
					//ERROR
					error = 1;
					cleanString[cleanStringLen] = '\0';
					printf("ERROR_3: %d: Unknown pattern %s\n",lineNumber,cleanString);
					break;

			case 39: 
					cleanString[cleanStringLen] = '\0';
					searchSymbolTable(cleanString,"RNUM");
					break;

			case 17:
					insertTokenToTokenList("LT",NULL);
					break;

			case 19:
					insertTokenToTokenList("DEF",NULL);
					break;

			case 21:
					insertTokenToTokenList("GT",NULL);
					break;
			case 23:
					insertTokenToTokenList("ENDDEF",NULL);
					break;
			case 4:
					insertTokenToTokenList("MUL",NULL);
					break;

			case 31:
					//ERROR
					error = 1;
					printf("ERROR_3: %d: Unknown pattern\n",lineNumber);
					break;

			case 30:
					//ERROR
					error = 1;
					printf("ERROR_3: %d: Unknown pattern\n",lineNumber);
					break;

			case 26:
					insertTokenToTokenList("COLON",NULL);
					break;

			case 28:
					//ERROR
					error = 1;
					printf("ERROR_3: %d: Unknown pattern .\n",lineNumber);
					break;
	}
	return fp;
}

// contains DFA implementation of comments logic
void parseDFAComments(FILE *fp) {
	int bufferIndex = 0;
	while(1) {

		if(cleanStringLen == BUFFER_LEN-1) {
			fwrite(cleanString, 1, BUFFER_LEN-1, fp);
			cleanStringLen = 0;
		}

		if(buffer[bufferIndex] == '\0') {
			break;
		}

		switch(dfaState) {
			case 0: if(buffer[bufferIndex] == '*') {
						bufferIndex++;
						dfaState = 1;
					} else {
						cleanString[cleanStringLen++] = buffer[bufferIndex++];
					}
					break;

			case 1: if(buffer[bufferIndex] == '*') {
						dfaState = 2;
						bufferIndex++;
					} else {
						cleanString[cleanStringLen++] = '*';
						dfaState = 0;
					}
					break;

			case 2: if(buffer[bufferIndex] == '*') {
						bufferIndex++;
						dfaState = 3;
					} else {
						if(buffer[bufferIndex] == '\n' || buffer[bufferIndex] == '\r')
							cleanString[cleanStringLen++] = buffer[bufferIndex];
						bufferIndex++;
					}
					break;

			case 3: if(buffer[bufferIndex] == '*') {
						dfaState = 0;
						bufferIndex++;
					} else {
						if(buffer[bufferIndex] == '\n' || buffer[bufferIndex] == '\r')
							cleanString[cleanStringLen++] = buffer[bufferIndex];
						dfaState = 2;
					}
					break;		
		}
	}
}

void removeComments(char *testcaseFile, char *cleanFile) {
	FILE *sourceFileFp = fopen(testcaseFile,"r");
	FILE *cleanFileFp = fopen(cleanFile,"w");
	dfaState = 0;
	cleanStringLen = 0;
	int len_read;
	while(len_read = fread(buffer, 1, BUFFER_LEN-1, sourceFileFp))
	{
		buffer[len_read] = '\0';
		parseDFAComments(cleanFileFp);
	}
	if(dfaState == 1)
		cleanString[cleanStringLen++] = '*';
	fwrite(cleanString, 1, cleanStringLen, cleanFileFp);
	fclose(cleanFileFp);
	fclose(sourceFileFp);
}
