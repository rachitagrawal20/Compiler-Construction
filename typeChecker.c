// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typeChecker.h"
#include "parser.h"

// int typecheckStackTop = -1;
int semanticError = 0;

int check_var(char str[50])
{
	if(strcmp(str,"ID")==0)
		return 1;
	if(strcmp(str,"NUM")==0)
		return 1;
	if(strcmp(str,"RNUM")==0)
		return 1;
	if(strcmp(str,"TRUE")==0)
		return 1;
	if(strcmp(str,"FALSE")==0)
		return 1;
	return 0;
}

int assign_type(char str[50])
{
	if(strcmp(str,"NUM")==0)
		return INTEGER;
	if(strcmp(str,"RNUM")==0)
		return REAL;
	if(strcmp(str,"TRUE")==0)
		return BOOLEAN;
	if(strcmp(str,"FALSE")==0)
		return BOOLEAN;
	return ERROR;
}

void typeCheck(TreeNode *head){
	int sqBracketStackTop = -1;
	int typecheckStackTop = -1;
	typeCheckUtil(head, &sqBracketStackTop, &typecheckStackTop);
}

void typeCheckUtil(TreeNode *head, int *sqBracketStackTop, int *typecheckStackTop)
{
	// printf("---- %d ----\n",*typecheckStackTop);
	if(head==NULL)
		return;

	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]+1,"moduleReuseStmt")==0) //type checking on function calls already done
		return;
	
	TreeNode *temp = head->child;
	while(temp!=NULL)
	{
		typeCheckUtil(temp, sqBracketStackTop, typecheckStackTop);
		temp = temp->next;
	}
	int op; 
	//CURRENT NODE
	if(head->isNonterminal==0)
	{
		if(strcmp(head->tokenPtr->tokenName,"SQBO")==0)
			sqBracketStack[++(*sqBracketStackTop)] = '[';
		else if(strcmp(head->tokenPtr->tokenName,"SQBC")==0)
			(*sqBracketStackTop)--;
		else if((*sqBracketStackTop)!=-1)	// should not push id[index] in the stack
		{
			if(head->isNonterminal==0 && strcmp(head->tokenPtr->tokenName,"ID")==0)
			{	
				//TreeNode* temp = head->next;
				//printf("next nodes of %s--\n",head->tokenPtr->tokenVal->tokenVal);
				//printf("next nodes of %s--\n",head->child->tokenPtr->tokenVal->tokenVal);
				// while(temp->tokenPtr->tokenVal->tokenVal!=NULL){
				// 	printf("%s->", temp->tokenPtr->tokenVal->tokenVal);
				// 	temp = temp->next;
				// }
				//printf("\n");

				if(((strcmp(nonterminalHashTable[head->parent->hashValue]+1,"lvalueARRStmt")== 0) || (strcmp(nonterminalHashTable[head->parent->hashValue]+1,"whichId")== 0)) && head->symbolTablePtr->type!=INTEGER)
				{
					semanticError = 1;
					printf("ERROR_19: DATATYPE of the indexing variable must be INTEGER: line no %d\n",head->tokenPtr->lineNumber);
				}

			}
			return;
		}
		else if(check_var(head->tokenPtr->tokenName))
		{
			//printf("inserting %s %s %d of type ",head->ptr->tokenName,head->ptr->tokenVal->tokenVal,head->ptr->rowNum);
			//if(head->sym_ptr!=NULL)
			//	printf("%d",head->sym_ptr->type);

			if(strcmp(terminalHashTable[head->hashValue],"ID")==0 && head->parent!= NULL && strcmp(nonterminalHashTable[head->parent->hashValue]+1, "iterativeStmt")==0){

				head->symbolTablePtr->inForloop = 1;
			}

			typecheckStack[++(*typecheckStackTop)] = head;
			//printf("\n");
			//printf("counter is %d\n",*typecheckStackTop);
		}

		//resetting loop variable*********
		
		if(strcmp(terminalHashTable[head->hashValue],"END")==0 && strcmp(terminalHashTable[head->parent->child->hashValue],"FOR")==0 
			&&  strcmp(terminalHashTable[head->parent->child->next->hashValue],"ID")==0){
			head->parent->child->next->symbolTablePtr->inForloop = 0;
		}

		else if((strcmp(head->tokenPtr->tokenName, "MINUS")==0 || strcmp(head->tokenPtr->tokenName, "PLUS")==0) && (head->parent != NULL && strcmp(terminalHashTable[head->parent->hashValue],"ASSIGNOP")==0))
		{
			int t1;
			//printf("found unary\n");
			//printf("did it\n");

			if(typecheckStack[(*typecheckStackTop)--]->symbolTablePtr==NULL)
			{
				//printf("found NULL %s \n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName);
				t1 = assign_type(typecheckStack[(*typecheckStackTop)+1]->tokenPtr->tokenName);
			}
			else
			{
				//printf("Not NULL %s \n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName);
				t1 = typecheckStack[(*typecheckStackTop)+1]->symbolTablePtr->type;
				TreeNode* operand1 = typecheckStack[*typecheckStackTop+1];
				if(t1 == ARRAY && (operand1->next != NULL && operand1->next->child != NULL && strcmp(terminalHashTable[operand1->next->child->hashValue],"SQBO")==0))
					t1 = typecheckStack[*typecheckStackTop+1]->symbolTablePtr->subtype;;
			}
			
			TreeNode* temp = getParseTreeNode(0,0,NULL);
			temp->tokenPtr = (struct Token*)malloc(sizeof(struct Token));
			temp->tokenPtr->lineNumber = typecheckStack[(*typecheckStackTop)+1]->tokenPtr->lineNumber;
			temp->symbolTablePtr = (SymbolTableParameters*)malloc(sizeof(SymbolTableParameters));

			if(t1==INTEGER || t1==REAL)
			{
				temp->symbolTablePtr->type = t1;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type %d\n",t1);
			}
			else
			{
				semanticError = 1;
				//printf("error in 1st\n");
				if(t1 != -1 && t1 != 5)
					printf("ERROR_19: Type Error (Invalid operation) : Line number %d\n",head->tokenPtr->lineNumber);
				temp->symbolTablePtr->type = ERROR;
				typecheckStack[++(*typecheckStackTop)] = temp;
			}
		}

		else if(op=check_op(head->tokenPtr->tokenName))
		{
			int t1,t2;
			int inFor;
			//printf("again did it\n");
			
			if(typecheckStack[(*typecheckStackTop)--]->symbolTablePtr==NULL)
			{
				t1 = assign_type(typecheckStack[(*typecheckStackTop)+1]->tokenPtr->tokenName);

				//printf("found NULL %s and type is %d\n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName,t1);
			}
			else
			{
				//printf("Not NULL %s \n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName);
				t1 = typecheckStack[(*typecheckStackTop)+1]->symbolTablePtr->type;
				TreeNode* operand1 = typecheckStack[*typecheckStackTop+1];
				if(t1 == ARRAY && (operand1->next != NULL && operand1->next->child != NULL && strcmp(terminalHashTable[operand1->next->child->hashValue],"SQBO")==0))
					t1 = typecheckStack[*typecheckStackTop+1]->symbolTablePtr->subtype;
			}

			if(typecheckStack[(*typecheckStackTop)--]->symbolTablePtr==NULL)
			{
				t2 = assign_type(typecheckStack[(*typecheckStackTop)+1]->tokenPtr->tokenName);
				//printf("found NULL %s and type is %d\n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName,t2);
			}
			else
			{
				//printf("Not NULL %s \n",typecheckStack[*typecheckStackTop+1]->ptr->tokenName);
				t2 = typecheckStack[(*typecheckStackTop)+1]->symbolTablePtr->type;
				inFor = typecheckStack[(*typecheckStackTop)+1]->symbolTablePtr->inForloop;
				TreeNode* operand2 = typecheckStack[*typecheckStackTop+1];
				if(t2 == ARRAY && (operand2->next != NULL && operand2->next->child != NULL && strcmp(terminalHashTable[operand2->next->child->hashValue],"SQBO")==0))
					t2 = typecheckStack[*typecheckStackTop+1]->symbolTablePtr->subtype;
			}

			TreeNode* temp = getParseTreeNode(0,0,NULL);
			temp->tokenPtr = (struct Token*)malloc(sizeof(struct Token));
			temp->tokenPtr->lineNumber = typecheckStack[(*typecheckStackTop)+2]->tokenPtr->lineNumber;
			temp->symbolTablePtr = (SymbolTableParameters*)malloc(sizeof(SymbolTableParameters));
			
			//printf("%d %d\n",isFor1,isFor2);
			if(op==ASSIGNOP && t1==t2)
			{
				//nothing
			}

			if(op == ASSIGNOP && inFor == 1){
				semanticError = 1;
				printf("ERROR: for loop variable can not be assigned within the loop at line no. %d\n",head->tokenPtr->lineNumber);
			}
			
			else if(op==ASSIGNOP && t1!=t2 && t1 != -1 && t2 != -1 && t1 != 5 && t2 != 5)
				printf("ERROR_20: Type Error (In assignment) : Line number %d\n",head->tokenPtr->lineNumber);
			
			else if(t1==INTEGER && t2==INTEGER && op==ARITHMETICOP)
			{
				temp->symbolTablePtr->type = INTEGER;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type integer\n");
			}
			
			else if(t1==REAL && t2==REAL && op==ARITHMETICOP)
			{
				temp->symbolTablePtr->type = REAL;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type REAL\n");
			}
			
			else if(t1==INTEGER && t2==INTEGER && op==RELATIONALOP)
			{
				temp->symbolTablePtr->type = BOOLEAN;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			
			else if(t1==REAL && t2==REAL && op==RELATIONALOP)
			{
				temp->symbolTablePtr->type = BOOLEAN;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			
			else if(t1==BOOLEAN && t2==BOOLEAN && op==LOGICALOP)
			{
				temp->symbolTablePtr->type = BOOLEAN;
				typecheckStack[++(*typecheckStackTop)] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			
			else if(op != ASSIGNOP)
			{
				semanticError = 1;
				
				if(t1 != -1 && t2 != -1 && t1 != 5 && t2 != 5){
					printf("ERROR_19: Type Error (Invalid operation) : Line number %d\n",head->tokenPtr->lineNumber);
					//printf("%d %d %d\n", t1, t2, op);
				}

				temp->symbolTablePtr->type = ERROR;
				typecheckStack[++(*typecheckStackTop)] = temp;
			}
		}
	}
	else if(head->parent!=NULL && head->parent->isNonterminal==1 && strcmp(nonterminalHashTable[head->parent->hashValue],"<iterativeStmt")==0 && strcmp(head->parent->child->tokenPtr->tokenName,"WHILE")==0)
	{
		if(head->parent->child->next==head)
		{
			int t1;
			if(typecheckStack[*typecheckStackTop]->symbolTablePtr==NULL)
				t1 = assign_type(typecheckStack[*typecheckStackTop]->tokenPtr->tokenName);
			else
			{
				t1 = typecheckStack[*typecheckStackTop]->symbolTablePtr->type;
				
				TreeNode* operand1 = typecheckStack[*typecheckStackTop];
				if(t1 == ARRAY && (operand1->next != NULL && operand1->next->child != NULL && strcmp(terminalHashTable[operand1->next->child->hashValue],"SQBO")==0))
					t1 = typecheckStack[*typecheckStackTop]->symbolTablePtr->subtype;
			}
			if(t1!=BOOLEAN)
			{
				semanticError = 1;
				printf("ERROR_21: WHILE expression must evaluate to BOOLEAN : Line Number %d\n",head->parent->child->tokenPtr->lineNumber);
			}
		}
	}
}