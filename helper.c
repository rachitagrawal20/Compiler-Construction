// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

/*
This file contains the implementation of different helper functions used.
*/

#include "helper.h"


int max(int a,int b)
{
	if(a > b)
		return a;
	else
		return b;
}

int compareStrings(char *str1, char *str2) {
  for (; *str1 && *str1 == *str2; str2++) str1++;
  return *str1 - *str2;
}

void printLexemeToConsole(char* str)
{
	if(compareStrings(str,"MUL")==0)
		printf("* ");
	else if(compareStrings(str,"GE")==0)
		printf(">= ");
	else if(compareStrings(str,"ENDDEF")==0)
		printf(">> ");
	else if(compareStrings(str,"GT")==0)
		printf("> ");
	else if(compareStrings(str,"DRIVERENDDEF")==0)
		printf(">>> ");
	else if(compareStrings(str,"LE")==0)
		printf("<= ");
	else if(compareStrings(str,"DEF")==0)
		printf("<< ");
	else if(compareStrings(str,"LT")==0)
		printf("< ");
	else if(compareStrings(str,"DRIVERDEF")==0)
		printf("<<< ");
	else if(compareStrings(str,"DIV")==0)
		printf("/ ");
	else if(compareStrings(str,"MINUS")==0)
		printf("- ");
	else if(compareStrings(str,"PLUS")==0)
		printf("+ ");
	else if(compareStrings(str,"EQ")==0)
		printf("== ");
	else if(compareStrings(str,"NE")==0)
		printf("!= ");
	else if(compareStrings(str,"ASSIGNOP")==0)
		printf(":= ");
	else if(compareStrings(str,"COLON")==0)
		printf(": ");
	else if(compareStrings(str,"RANGEOP")==0)
		printf(".. ");
	else if(compareStrings(str,"SEMICOL")==0)
		printf("; ");
	else if(compareStrings(str,"COMMA")==0)
		printf(", ");
	else if(compareStrings(str,"SQBO")==0)
		printf("[ ");
	else if(compareStrings(str,"SQBC")==0)
		printf("] ");
	else if(compareStrings(str,"BO")==0)
		printf("( ");
	else if(compareStrings(str,"BC")==0)
		printf(") ");
	else
		printf("%s ",str);
}

FILE *printLexemeToFile(FILE *fp, char* str)
{
	if(compareStrings(str,"MUL")==0)
		fprintf(fp, "*\t");
	else if(compareStrings(str,"GE")==0)
		fprintf(fp, ">=\t");
	else if(compareStrings(str,"ENDDEF")==0)
		fprintf(fp, ">>\t");
	else if(compareStrings(str,"GT")==0)
		fprintf(fp, ">\t");
	else if(compareStrings(str,"DRIVERENDDEF")==0)
		fprintf(fp, ">>>\t");
	else if(compareStrings(str,"LE")==0)
		fprintf(fp, "<=\t");
	else if(compareStrings(str,"DEF")==0)
		fprintf(fp, "<<\t");
	else if(compareStrings(str,"LT")==0)
		fprintf(fp, "<\t");
	else if(compareStrings(str,"DRIVERDEF")==0)
		fprintf(fp, "<<<\t");
	else if(compareStrings(str,"DIV")==0)
		fprintf(fp, "/\t");
	else if(compareStrings(str,"MINUS")==0)
		fprintf(fp, "-\t");
	else if(compareStrings(str,"PLUS")==0)
		fprintf(fp, "+\t");
	else if(compareStrings(str,"EQ")==0)
		fprintf(fp, "==\t");
	else if(compareStrings(str,"NE")==0)
		fprintf(fp, "!=\t");
	else if(compareStrings(str,"ASSIGNOP")==0)
		fprintf(fp, ":=\t");
	else if(compareStrings(str,"COLON")==0)
		fprintf(fp, ":\t");
	else if(compareStrings(str,"RANGEOP")==0)
		fprintf(fp, "..\t");
	else if(compareStrings(str,"SEMICOL")==0)
		fprintf(fp, ";\t");
	else if(compareStrings(str,"COMMA")==0)
		fprintf(fp, ",\t");
	else if(compareStrings(str,"SQBO")==0)
		fprintf(fp, "[\t");
	else if(compareStrings(str,"SQBC")==0)
		fprintf(fp, "]\t");
	else if(compareStrings(str,"BO")==0)
		fprintf(fp, "(\t");
	else if(compareStrings(str,"BC")==0)
		fprintf(fp, ")\t");
	else
		fprintf(fp, "%s\t",str);
	return fp;
}

void printTokenList()
{
	struct Token *head = tokenListHead;
	printf("Line Number\t Lexeme\t Token Name\n");
    while(head!=NULL)
    {
        printf("\t%d\t",head->lineNumber);
        head->tokenVal==NULL ? printLexemeToConsole(head->tokenName) : printf("%s\t",(head->tokenVal)->tokenVal);
        printf("    %s\n",head->tokenName);

        head = head->next;
    }
}
