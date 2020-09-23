// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "ast.h"


int check_op(char str[50])
{
	if(strcmp(str,"NEGATE")==0)
		return NEGATION;
	else if(strcmp(str,"PLUS")==0 || strcmp(str,"MINUS")==0 || strcmp(str,"MUL")==0 || strcmp(str,"DIV")==0)
		return ARITHMETICOP;
	else if(strcmp(str,"AND")==0 || strcmp(str,"OR")==0)
		return LOGICALOP;
	else if(strcmp(str,"LT")==0 || strcmp(str,"LE")==0 || strcmp(str,"GT")==0 || strcmp(str,"GE")==0 || strcmp(str,"EQ")==0 || strcmp(str,"NE")==0)
		return RELATIONALOP;
	else if(strcmp(str,"ASSIGNOP")==0)
		return ASSIGNOP;
	else
		return 0;
}

int is_imp(char str[50])
{
	if(strcmp(str,"RANGEOP")==0)
		return 0;
	if(strcmp(str,"BO")==0)
		return 0;
	if(strcmp(str,"BC")==0)
		return 0;
	if(strcmp(str,"START")==0)
		return 0;
	
	//Including END in ast to know the end point of for loop
	if(strcmp(str,"END")==0)
		return 1;
	
	if(strcmp(str,"IN")==0)
		return 0;
	if(strcmp(str,"COLON")==0)
		return 0;
	if(strcmp(str,"SEMICOL")==0)
		return 0;
	if(strcmp(str,"BREAK")==0)
		return 0;
	if(strcmp(str,"COMMA")==0)
		return 0;
	//if(strcmp(str,"SQBO")==0)
	//	return 0;
	//if(strcmp(str,"SQBC")==0)
	//	return 0;
	//if(strcmp(str,"ASSIGNOP")==0)
	//	return 0;
	if(strcmp(str,"USE")==0)
		return 0;
	if(strcmp(str,"MODULE")==0)
		return 0;
	if(strcmp(str,"WITH")==0)
		return 0;
	if(strcmp(str,"PARAMETERS")==0)
		return 0;
	if(strcmp(str,"OF")==0)
		return 0;
	if(strcmp(str,"TAKES")==0)
		return 0;
	if(strcmp(str,"INPUT")==0)
		return 0;
	if(strcmp(str,"DEF")==0)
		return 0;
	if(strcmp(str,"ENDDEF")==0)
		return 0;
	if(strcmp(str,"DRIVERDEF")==0)
		return 0;
	if(strcmp(str,"DRIVER")==0)
		return 0;
	if(strcmp(str,"PROGRAM")==0)
		return 0;
	if(strcmp(str,"DRIVERENDDEF")==0)
		return 0;
	return 1;
}

void delete_node(TreeNode* head)
{
	if(head == NULL)
		return;
	if(head == head->parent->child)
	{
		head->parent->child = head->next;
		free(head);
		return;
	}
	TreeNode* temp = head->parent->child;
	while(temp->next != head)
		temp = temp->next;
	temp->next = head->next;
	free(head);
	return;
}

TreeNode* generateAst(TreeNode* head){
	if(head == NULL)
		return head;

	TreeNode* f;
	if(!head->isNonterminal  && head->hashValue != epsilon && !is_imp(head->tokenPtr->tokenName))
	{
		f = head->next;
		//printf("Deleting %s\n",(head->tokenPtr)->tokenName);
		delete_node(head);
		return f;
	}
	else if(!head->isNonterminal && check_op(head->tokenPtr->tokenName))
	{
		//printf("collapsing %s with %s\n",head->tokenPtr->tokenName,hash_nonterm[head->parent->hashValue]);
		f = head->next;
		TreeNode* temp = head->parent;
		
		//for handling <expression> - > MINUS BO <arithmeticExpr> BC
		temp->tokenPtr = head->tokenPtr;
		if(strcmp(head->tokenPtr->tokenName,"MINUS") == 0 && head->parent->hashValue == EXPR)
			strcpy(temp->tokenPtr->tokenName,"NEGATE");
		
		temp->isNonterminal = 0;
		temp->hashValue = head->hashValue;
		delete_node(head);
		return f;
	}
	else if(head->child != NULL && (head->child)->next == NULL) //only one child
	{
		//printf("Deleting %s\n",hash_nonterm[head->hashValue]+1	);
		TreeNode* f = head->child;
		if(head == head->parent->child)
		{
			head->parent->child = head->child;
			head->child->next = head->next;
			head->child->parent = head->parent;
			free(head);
		}
		else
		{
			TreeNode* temp = head->parent->child;
			while(temp->next!=head)
				temp = temp->next;
			temp->next = head->child;
			head->child->parent = head->parent;
			head->child->next = head->next;
			free(head);
		}
		return generateAst(f);
	}

	TreeNode* temp =  head->child;
	while(temp!=NULL)
		temp = generateAst(temp);
	
	return head->next;
}