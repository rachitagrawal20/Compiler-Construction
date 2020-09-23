// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semanticanalyzer.h"
#include "parser.h"

//Inorder Traversal
void semanticAnalyzer(TreeNode* head)
{
	if(head==NULL)
		return;
	TreeNode* temp = head->child;
	
	//Visiting children
	while(temp!=NULL)
	{
		semanticAnalyzer(temp);
		temp = temp->next;
	}

	//naman A:=B type handling
	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]+1,"assignmentStmt")==0){
		// printf("%s \t %s\n", head->child->tokenPtr->tokenVal->tokenVal, head->child->next->tokenPtr->tokenName);
		int t1 = head->child->symbolTablePtr->type;
		if(t1==ARRAY && strcmp(head->child->next->tokenPtr->tokenName, "ASSIGNOP")==0){
			if(head->child->next->child!=NULL && strcmp(nonterminalHashTable[head->child->next->child->hashValue]+1,"arithmeticOrBooleanExpr")==0){
				// printf("%s \t %s \t %d\n",head->child->tokenPtr->tokenVal->tokenVal,head->child->next->child->child->child->child->child->child->tokenPtr->tokenVal->tokenVal, head->child->tokenPtr->lineNumber);
				int t2 = head->child->next->child->child->child->child->child->child->symbolTablePtr->type;
				if(t2==ARRAY){
					int t1_asize = head->child->symbolTablePtr->ub - head->child->symbolTablePtr->lb + 1;
					int t2_asize = head->child->next->child->child->child->child->child->child->symbolTablePtr->ub - head->child->next->child->child->child->child->child->child->symbolTablePtr->lb +1;
					if(t1_asize!=t2_asize)
						printf("ERROR: Type Mismatch Error : Line Number %d\n", head->child->tokenPtr->lineNumber);
				}
			}
		}
		//return;
	}

	//rachit A[100]:= x+y type out of bounds handling
	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]+1,"assignmentStmt")==0){
		int t = head->child->symbolTablePtr->type;
		// if(t== ARRAY){
		// 	printf("-- %s %d\n", head->child->tokenPtr->tokenVal->tokenVal, (head->child->next)->tokenPtr->lineNumber);
		// }

		if(t == ARRAY && head->child->next->child != NULL && strcmp(terminalHashTable[head->child->next->child->hashValue],"SQBO")==0){
			//printf("%s\n", head->child);
			int low = head->child->symbolTablePtr->lb;
			int high = head->child->symbolTablePtr->ub;
			int index = atoi(head->child->next->child->next->tokenPtr->tokenVal->tokenVal);
			//printf("%d\n", atoi(head->child->next->child->next->tokenPtr->tokenVal->tokenVal));
			// printf("low high index %d %d %d\n",low,high,index);
			if((index < low || index > high) && strcmp(head->child->next->child->next->tokenPtr->tokenName, "ID")!= 0)
				printf("ERROR: Array index out of bounds : Line Number %d\n",(head->child->next)->tokenPtr->lineNumber);
		}
	}

	//A:= B[100] type out of bounds handling
	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]+1,"var_id_num")==0){
		int t = head->child->symbolTablePtr->type;
		// if(t== ARRAY){
		// 	printf("-- %s\n", head->child->tokenPtr->tokenVal->tokenVal);
		// }

		if(t == ARRAY && head->child->next!= NULL && head->child->next->child != NULL && strcmp(terminalHashTable[head->child->next->child->hashValue],"SQBO")==0){
			//printf("%s\n", head->child);
			int low = head->child->symbolTablePtr->lb;
			int high = head->child->symbolTablePtr->ub;
			int index = atoi(head->child->next->child->next->tokenPtr->tokenVal->tokenVal);
			//printf("%d\n", atoi(head->child->next->child->next->tokenPtr->tokenVal->tokenVal));
			// printf("low high index %d %d %d\n",low,high,index);
			if((index < low || index > high) && strcmp(head->child->next->child->next->tokenPtr->tokenName, "ID")!= 0)
				printf("ERROR: Array index out of bounds : Line Number %d\n",(head->child->next)->tokenPtr->lineNumber);
		}
	}
	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]+1,"conditionalStmt")==0)
	{
		//printf("found switch.................\n");
		int t = (head->child->next)->symbolTablePtr->type;
		//int t = BOOLEAN;
		int line_no = (head->child->next)->tokenPtr->lineNumber;

		if(t==INTEGER)
		{
			if(head->child->next->next->next->isNonterminal==0)
			{
				semanticError = 1;
				printf("ERROR_11: Switch with INTEGER datatype should have default statement : line_no %d\n",line_no);
			}
			TreeNode* temp = head->child->next->next;
			while(!(temp->isNonterminal==0 && strcmp(temp->tokenPtr->tokenName,"epsilon")==0))
			{
				if(strcmp(temp->child->next->tokenPtr->tokenName,"NUM")!=0)
				{
					semanticError = 1;
					printf("ERROR_12: Switch with INTEGER datatype can only have NUM in CASE statement : line_no %d\n",temp->child->next->tokenPtr->lineNumber);
				}
				temp = temp->child->next->next->next;	
			}
		}
		else if(t==BOOLEAN)
		{
			if(head->child->next->next->next->isNonterminal!=0)
			{
				semanticError = 1;
				printf("ERROR_13: Switch with BOOLEAN datatype can not have default statement : line_no %d\n",head->child->next->next->next->child->tokenPtr->lineNumber);
			}
			TreeNode* temp = head->child->next->next;
			while(!(temp->isNonterminal==0 && strcmp(temp->tokenPtr->tokenName,"epsilon")==0))
			{
				if(!(strcmp(temp->child->next->tokenPtr->tokenName,"TRUE")==0 || strcmp(temp->child->next->tokenPtr->tokenName,"FALSE")==0))
				{
					semanticError = 1;
					printf("ERROR_14: Switch with BOOLEAN datatype can only have TRUE or FALSE in CASE statement : line_no %d\n",temp->child->next->tokenPtr->lineNumber);
				}
				temp = temp->child->next->next->next;
			}
		}
		else if(t==REAL)
		{
			semanticError = 1;
			printf("ERROR_15: Switch ID can not have REAL datatype : line no- %d\n",line_no);
		}
		else
		{
			semanticError = 1;
			printf("ERROR_16: Switch ID invalid datatype : line no- %d\n",line_no);
		}
		return;
	}
}


