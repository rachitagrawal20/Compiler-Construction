// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symbolTable.h"

int num_relop = 0;
int num_switch = 0;
int pr_switch = 0;
int num_for = 0;
int num_while = 0;
int index_fg = 0;
int typecheckStackTop;
int labels = 0;
int newOffset = 0;

FILE* code_generator_assignment(TreeNode* head, FILE* fp)
{
	if(head==NULL)
		return fp;
	
	TreeNode *temp = head->child;

	while(temp!=NULL)
	{
		fp = code_generator_assignment(temp, fp);
		temp = temp->next;
	}

	int op; 
	//current TreeNode
	if(head->isNonterminal == 0)
	{
		if(strcmp(head->tokenPtr->tokenName,"ID")==0)
		{
			//fprintf(fp, "in ID\n");
			typecheckStack[++typecheckStackTop] = head;
			fprintf(fp, "\t\tmov ax, word[MEM + %d]\n", head->symbolTablePtr->offset);
			fprintf(fp, "\t\tpush ax\n");
		}
		else if(strcmp(head->tokenPtr->tokenName,"NUM")==0)
		{
			//fprintf(fp, "in NUM\n");
			typecheckStack[++typecheckStackTop] = head;
			fprintf(fp, "\t\tmov ax, %s\n", head->tokenPtr->tokenVal->tokenVal);
			fprintf(fp, "\t\tpush ax\n");	
		}
		else if(strcmp(head->tokenPtr->tokenName,"TRUE")==0)
		{
			//fprintf(fp, "in NUM\n");
			typecheckStack[++typecheckStackTop] = head;
			fprintf(fp, "\t\tmov ax, 1\n");
			fprintf(fp, "\t\tpush ax\n");	
		}
		else if(strcmp(head->tokenPtr->tokenName,"FALSE")==0)
		{
			//fprintf(fp, "in NUM\n");
			typecheckStack[++typecheckStackTop] = head;
			fprintf(fp, "\t\tmov ax,0\n");
			fprintf(fp, "\t\tpush ax\n");	
		}
		else if(check_op(head->tokenPtr->tokenName)==NEGATION)
		{
			//fprintf(fp, "in NEGATION\n");
			fprintf(fp, "\t\tpop ax\n");
			fprintf(fp, "\t\timul -1\n");
			fprintf(fp, "\t\tpush ax\n");
		}
		else if(op = check_op(head->tokenPtr->tokenName))
		{
			//In case of an operator
			fprintf(fp, " ;operator check %s\n", head->tokenPtr->tokenName);
			
			TreeNode* temp = getParseTreeNode(0, 0, NULL);

			fprintf(fp, "\t\tpop bx\n");
			fprintf(fp, "\t\tpop ax\n");
					
			fprintf(fp, " ;bx: %s\n", typecheckStack[typecheckStackTop]->tokenPtr->tokenName);
			fprintf(fp, " ;ax: %s\n", typecheckStack[typecheckStackTop-1]->tokenPtr->tokenName);

			// checking for array for bx
			if(typecheckStack[typecheckStackTop]->parent != NULL && typecheckStack[typecheckStackTop]->parent->child != typecheckStack[typecheckStackTop]) //this is necessary for typecheckStackTop to be array
			{
				// printf("123\n");
				fprintf(fp, " ;can be array for ax operand\n");
				
				// moving back by a sibling in order to get SQBO
				TreeNode* tt = typecheckStack[typecheckStackTop]->parent->child;
				while(tt->next != typecheckStack[typecheckStackTop]){
					tt = tt->next;
				}
				
				if(tt->isNonterminal == 0 && strcmp(tt->tokenPtr->tokenName,"SQBO") == 0)
				{
					fprintf(fp, " ;confirmed! found SQBO\n");

					TreeNode *array_node = typecheckStack[typecheckStackTop-1]; //previous stored one
					int lb = array_node->symbolTablePtr->lb;
					int ub = array_node->symbolTablePtr->ub;
					int width = getWidth(array_node->symbolTablePtr->subtype);
					int type = array_node->symbolTablePtr->subtype;
					
					int base = array_node->symbolTablePtr->offset; 

					if(typecheckStack[typecheckStackTop]->symbolTablePtr == NULL) // the index value is number
					{
						int a = atoi(typecheckStack[typecheckStackTop]->tokenPtr->tokenVal->tokenVal);
						newOffset = base + ((a-lb) * width);

						//todo: //extend for boolean
						fprintf(fp, "\t\tmov bx, word[MEM + %d]\n", newOffset);
						
					}
					else // the index value is a variable
					{
						fprintf(fp, "\t\t; work in progress - bx\n");
						// todo: boolean arrays

						// fprintf(fp, "\t\tpush rcx\n");
						// fprintf(fp, "\t\tpush rdx\n");
						fprintf(fp, "\t\tpush ax\n");
						
						fprintf(fp, "\t\t\tmov rdx, 0\n");	
						fprintf(fp, "\t\t\tmov ax, word[MEM + %d]\n", typecheckStack[typecheckStackTop]->symbolTablePtr->offset);
						
						//runtime error checking-----------------------------------------------------------------------
						fprintf(fp, "\t\t\t; runtime error checking\n");
						fprintf(fp, "\t\t\tpush rsi\n");
						fprintf(fp, "\t\t\tpush rdi\n");
						
						fprintf(fp, "\t\t\t\tmov si, %d\n", lb);
						fprintf(fp, "\t\t\t\tmov di, %d\n", ub);
						fprintf(fp, "\t\t\t\tcmp ax, si\n");
						fprintf(fp, "\t\t\t\tJL RE\n");
						fprintf(fp, "\t\t\t\tcmp ax, di\n");
						fprintf(fp, "\t\t\t\tJG RE\n");

						fprintf(fp, "\t\t\tpop rsi\n");
						fprintf(fp, "\t\t\tpop rdi\n");
						//---------------------------------------------------------------------------------------------

						fprintf(fp, "\t\t\tsub ax, %d\n", lb);
						fprintf(fp, "\t\t\tmov cx, %d\n", width);
						fprintf(fp, "\t\t\timul cx\n");
						fprintf(fp, "\t\t\tadd ax, %d\n", base);
						fprintf(fp, "\t\t\tmov dx, ax\n");
						fprintf(fp, "\t\t\tmov bx, word[MEM + rdx]\n");
						
						fprintf(fp, "\t\tpop ax\n");
						// fprintf(fp, "\t\tpop rdx\n");
						// fprintf(fp, "\t\tpop rcx\n");
						fprintf(fp, "\n");

					}

					fprintf(fp, "\t\tpop ax\n");
					typecheckStackTop--;
				}
				else{
					fprintf(fp, " ; false condn't, couldnt find array!\n");
				}
			}

			// for ax
			typecheckStackTop--;
			
			// checking for array for ax
			if(typecheckStack[typecheckStackTop]->parent != NULL && typecheckStack[typecheckStackTop]->parent->child != typecheckStack[typecheckStackTop]) //this is necessary for typecheckStackTop to be array
			{
				// printf("456\n");
				fprintf(fp, " ;can be array for ax operand\n");
				
				// moving back by a sibling in order to get SQBO
				TreeNode* tt = typecheckStack[typecheckStackTop]->parent->child;
				while(tt->next != typecheckStack[typecheckStackTop]){
					tt = tt->next;
				}
				
				if(tt->isNonterminal == 0 && strcmp(tt->tokenPtr->tokenName,"SQBO") == 0)
				{
					fprintf(fp, " ;confirmed! found SQBO\n");

					TreeNode *array_node = typecheckStack[typecheckStackTop-1]; //previous stored one
					int lb = array_node->symbolTablePtr->lb;
					int ub = array_node->symbolTablePtr->ub;
					int width = getWidth(array_node->symbolTablePtr->subtype);
					int type = array_node->symbolTablePtr->subtype;
					
					int base = array_node->symbolTablePtr->offset; 

					fprintf(fp, "\t\tpop ax\n");

					if(typecheckStack[typecheckStackTop]->symbolTablePtr == NULL) // the index value is number
					{
						int a = atoi(typecheckStack[typecheckStackTop]->tokenPtr->tokenVal->tokenVal);
						newOffset = base + ((a-lb) * width);

						//todo: //extend for boolean
						fprintf(fp, "\t\tmov ax, word[MEM + %d]\n", newOffset);
						index_fg = 2;
					}
					else // the index value is a variable
					{
						fprintf(fp, "\t\t; work in progress - ax\n");
						// todo: boolean arrays

						// fprintf(fp, "\t\tpush rcx\n");
						// fprintf(fp, "\t\tpush rdx\n");
						// fprintf(fp, "\t\tpush ax\n");
						
						fprintf(fp, "\t\t\tmov rdx, 0\n");	
						fprintf(fp, "\t\t\tmov ax, word[MEM + %d]\n", typecheckStack[typecheckStackTop]->symbolTablePtr->offset);

						//runtime error checking-----------------------------------------------------------------------
						fprintf(fp, "\t\t\t; runtime error checking\n");
						fprintf(fp, "\t\t\tpush rsi\n");
						fprintf(fp, "\t\t\tpush rdi\n");
						
						fprintf(fp, "\t\t\t\tmov si, %d\n", lb);
						fprintf(fp, "\t\t\t\tmov di, %d\n", ub);
						fprintf(fp, "\t\t\t\tcmp ax, si\n");
						fprintf(fp, "\t\t\t\tJL RE\n");
						fprintf(fp, "\t\t\t\tcmp ax, di\n");
						fprintf(fp, "\t\t\t\tJG RE\n");

						fprintf(fp, "\t\t\tpop rsi\n");
						fprintf(fp, "\t\t\tpop rdi\n");
						//---------------------------------------------------------------------------------------------

						fprintf(fp, "\t\t\tsub ax, %d\n", lb);
						fprintf(fp, "\t\t\tmov cx, %d\n", width);
						fprintf(fp, "\t\t\timul cx\n");
						fprintf(fp, "\t\t\tadd ax, %d\n", base);
						fprintf(fp, "\t\t\tmov dx, ax\n");
						fprintf(fp, "\t\t\tmov ax, word[MEM + rdx]\n");
						
						// fprintf(fp, "\t\tpop ax\n");
						// fprintf(fp, "\t\tpop rdx\n");
						// fprintf(fp, "\t\tpop rcx\n");
						fprintf(fp, "\n");
						index_fg = 1;
					}
					typecheckStackTop--;
				}
				else{
					fprintf(fp, " ; false condn't, couldnt find array!\n");
				}
			}

			if(op == ASSIGNOP)
			{
				fprintf(fp, " ;ASSIGNOP\n");
				if(typecheckStack[typecheckStackTop]->symbolTablePtr->type == BOOLEAN) 
				{
					if(index_fg == 1)
					{
						fprintf(fp, "; USED\n");
						fprintf(fp, "\t\tmov byte[MEM + rdx], bl\n");
					}
					else if(index_fg == 2){
						fprintf(fp, "; USED\n");
						fprintf(fp, "\t\tmov byte[MEM + %d], bl\n", newOffset);
					}else{
						fprintf(fp, "\t\tmov byte[MEM + %d], bl\n",typecheckStack[typecheckStackTop]->symbolTablePtr->offset);
					}
					index_fg = 0;
				}
				else // integer real
				{
					if(index_fg == 1)
					{
						fprintf(fp, "; USED\n");
						fprintf(fp, "\t\tmov word[MEM + rdx], bx\n");
					}
					else if(index_fg == 2){
						fprintf(fp, "; USED\n");
						fprintf(fp, "\t\tmov word[MEM + %d], bx\n", newOffset);
					}else{
						fprintf(fp, "\t\tmov word[MEM + %d], bx\n",typecheckStack[typecheckStackTop]->symbolTablePtr->offset);
					}	
					index_fg = 0;
				}
				typecheckStackTop--;
			}
			else if(strcmp(head->tokenPtr->tokenName,"PLUS")==0)
			{
				fprintf(fp, " ;PLUS\n");
				fprintf(fp, "\t\tadd ax, bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
			}
			else if(strcmp(head->tokenPtr->tokenName,"MINUS")==0)
			{
				fprintf(fp, " ;MINUS\n");
				fprintf(fp, "\t\tsub ax, bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
			}
			else if(strcmp(head->tokenPtr->tokenName,"MUL")==0)
			{
				fprintf(fp, " ;MUL\n");
				fprintf(fp, "\t\timul bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
			}
			else if(strcmp(head->tokenPtr->tokenName,"DIV")==0)
			{
				fprintf(fp, " ;DIV\n");
				fprintf(fp, "\t\tidiv bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
			}
			else if(strcmp(head->tokenPtr->tokenName,"AND")==0)
			{
				fprintf(fp, " ;AND\n");
				fprintf(fp, "\t\tand ax, bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;	
			}
			else if(strcmp(head->tokenPtr->tokenName,"OR")==0)
			{
				fprintf(fp, " ;OR\n");
				fprintf(fp, "\t\tor ax, bx\n");
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
			}
			else if(strcmp(head->tokenPtr->tokenName,"LT")==0)
			{
				fprintf(fp, " ;LT\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJL TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tjmp FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n", num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			else if(strcmp(head->tokenPtr->tokenName,"LE")==0)
			{
				fprintf(fp, " ;LE\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJLE TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tjmp FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n",num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			else if(strcmp(head->tokenPtr->tokenName,"GT")==0)
			{
				fprintf(fp, " ;GT\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJG TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tjmp FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n",num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			else if(strcmp(head->tokenPtr->tokenName,"GE")==0)
			{
				fprintf(fp, " ;GE\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJGE TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tjmp FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n", num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			else if(strcmp(head->tokenPtr->tokenName,"EQ")==0)
			{
				fprintf(fp, " ;EQ\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJE TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tJMP FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n", num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			else if(strcmp(head->tokenPtr->tokenName,"NQ")==0)
			{
				fprintf(fp, " ;NQ\n");
				fprintf(fp, "\t\tcmp ax, bx\n");
				fprintf(fp, "\t\tJNE TRUE%d\n", num_relop);
				fprintf(fp, "\t\tmov ax, 0\n");
				fprintf(fp, "\t\tJMP FALSE%d\n", num_relop);
				fprintf(fp, "\t\tTRUE%d: mov ax, 1\n", num_relop);
				fprintf(fp, "\t\tFALSE%d:\n", num_relop);
				fprintf(fp, "\t\tpush ax\n");
				typecheckStack[typecheckStackTop] = temp;
				num_relop++;
			}
			index_fg = 0;
		}
	}
	return fp;
}

FILE* code_generator_conditional(TreeNode* head, FILE* fp)
{
	pr_switch = 0;
	TreeNode* curr = head->child->next;
	TreeNode* temp;

	fprintf(fp, "\t\t; conditional statement!\n\n");

	if(curr->symbolTablePtr == NULL)
		fprintf(fp, "\t\tnull in switch!\n");
	else if(curr->symbolTablePtr->type == BOOLEAN)
	{
		fprintf(fp, "\t\tMOV CL, byte[MEM+%d]\n", curr->symbolTablePtr->offset);
		temp = curr->next;
		while(!(temp->isNonterminal == 0 && strcmp(temp->tokenPtr->tokenName, "epsilon") == 0))
		{
			if(strcmp((temp->child->next)->tokenPtr->tokenName,"TRUE") == 0)
				fprintf(fp, "\t\tCMP CL, 1\n");
			else if(strcmp((temp->child->next)->tokenPtr->tokenName,"FALSE") == 0)
				fprintf(fp, "\t\tCMP CL, 0\n");
			else
				fprintf(fp, "\t\tCMP CL,%s\n",(temp->child->next->tokenPtr->tokenVal->tokenVal));
			
			fprintf(fp, "JNZ C%d%d\n",num_switch,pr_switch);
			fp = code_generator(temp->child->next->next, fp);
			fprintf(fp, "JMP SWITCH_EXIT%d\n",num_switch);
			fprintf(fp, "C%d%d:\n",num_switch,pr_switch);
			pr_switch++;
			temp = temp->child->next->next->next;
		}
		fprintf(fp, "SWITCH_EXIT%d:\n",num_switch);
	}
	else if(curr->symbolTablePtr->type == INTEGER)
	{
		fprintf(fp, "MOV CX,word[MEM+%d]\n",curr->symbolTablePtr->offset);
		temp = curr->next;
		while(!(temp->isNonterminal == 0 && strcmp(temp->tokenPtr->tokenName, "epsilon") == 0))
		{
			fprintf(fp, "CMP CX,%s\n",(temp->child->next->tokenPtr->tokenVal->tokenVal));
			fprintf(fp, "JNZ C%d%d\n",num_switch,pr_switch);
			fp = code_generator(temp->child->next->next, fp);
			fprintf(fp, "JMP SWITCH_EXIT%d\n",num_switch);
			fprintf(fp, "C%d%d:\n",num_switch,pr_switch);
			pr_switch++;
			temp = temp->child->next->next->next;
		}
		//default statement
		fp = code_generator(curr->next->next->child->next, fp);
		fprintf(fp, "SWITCH_EXIT%d:\n",num_switch);
	}
	else
		fprintf(fp, "ERROR_22: invalid input in code_generator_conditional\n");
	num_switch++;

	return fp;
}	

FILE* code_generator_iterative(TreeNode* head, FILE* fp)
{
	if(strcmp(head->child->tokenPtr->tokenName,"FOR")==0)
	{
		num_for++;
		TreeNode* curr = head->child->next;
		TreeNode* num1 = curr->next->child;
		TreeNode* num2 = num1->next;
		fprintf(fp, "\n");
		fprintf(fp, "\t\t; Iterative statements\n");
		fprintf(fp, "\t\tmov cx, %s\n", num1->tokenPtr->tokenVal->tokenVal);
		fprintf(fp, "\t\tmov word[MEM + %d], cx\n", curr->symbolTablePtr->offset);
		fprintf(fp, "\t\tFOR_LOOP%d:\n", num_for);

		fp = code_generator(curr->next->next, fp);

		fprintf(fp, "\t\tmov cx, word[MEM + %d]\n", curr->symbolTablePtr->offset);
		fprintf(fp, "\t\tinc cx\n");
		fprintf(fp, "\t\tmov word[MEM + %d], cx\n", curr->symbolTablePtr->offset);
		fprintf(fp, "\t\tmov ax,%s\n", num2->tokenPtr->tokenVal->tokenVal);
		fprintf(fp, "\t\tcmp cx, ax\n");
		fprintf(fp, "\t\tJLE FOR_LOOP%d\n", num_for);
	}
	else if(strcmp(head->child->tokenPtr->tokenName,"WHILE")==0)
	{
		num_while++;
		fp = code_generator(head->child->next, fp);
		fprintf(fp, "POP AX\n");
		fprintf(fp, "CMP AL,1\n");
		fprintf(fp, "JNZ WHILE_END%d\n",num_while);
		fprintf(fp, "WHILE_LOOP%d:\n",num_while);
		fp = code_generator(head->child->next->next, fp);
		fp = code_generator(head->child->next, fp);
		fprintf(fp, "POP AX\n");
		fprintf(fp, "CMP AL,1\n");
		fprintf(fp, "JZ WHILE_LOOP%d\n",num_while);
		fprintf(fp, "WHILE_END%d:\n",num_while);
	}

	return fp;
}

FILE* code_generator_print(TreeNode *head, FILE* fp)
{
	fprintf(fp, "\n");
	fprintf(fp, "\t\t; for print statement\n");
	fprintf(fp, "\t\tmov rax, 0\n");
	fprintf(fp, "\t\tmov rbx, 0xFFFFFFFFFFFFFFFF\n");
	fprintf(fp, "\t\tcmovl rax, rbx\n");

	// printf("%s\n", getTypeName(head->child->next->child->symbolTablePtr->type));

	if(head->child->next->isNonterminal == 0) // for terminals (bool, num)
	{
		// for bools and nums
		// printf("prashant 1\n");
		if(strcmp(head->child->next->tokenPtr->tokenName,"TRUE")==0){
			fprintf(fp, "\t\t\tmov rdi, trueOutput\n");
		}
		else if(strcmp(head->child->next->tokenPtr->tokenName,"FALSE")==0){
			fprintf(fp, "\t\t\tmov rdi, falseOutput\n");
		}
		else{
			fprintf(fp, "\t\tmov ax, %s\n",head->child->next->tokenPtr->tokenVal->tokenVal);
			fprintf(fp, "\t\t\tmov rdi, men\n");
		}
	} // for non terminals
	else if(head->child->next->child->next->isNonterminal == 0 && strcmp(head->child->next->child->next->tokenPtr->tokenName, "epsilon") == 0 && 
		head->child->next->child->symbolTablePtr->type != ARRAY)
	{
		// for a variable (can not be array)
		// printf("prashant 2\n");
		if(head->child->next->child->symbolTablePtr->type == BOOLEAN){
			fprintf(fp, "\t\tmov al, byte[MEM+%d]\n",head->child->next->child->symbolTablePtr->offset);
			fprintf(fp, "\t\tcmp al, 1\n");
			fprintf(fp, "\t\tJE printTrue\n");
			fprintf(fp, "\t\tmov rdi, falseOutput\n");
			fprintf(fp, "\t\tprintTrue:\n");
			fprintf(fp, "\t\tmov rdi, trueOutput\n");
		}else{
			fprintf(fp, "\t\t\tmov rdi, men\n");
			fprintf(fp, "\t\tmov ax, word[MEM+%d]\n",head->child->next->child->symbolTablePtr->offset);
		}
	}
	else if(head->child->next->child->next->isNonterminal == 0 && strcmp(head->child->next->child->next->tokenPtr->tokenName, "epsilon") == 0 && 
		head->child->next->child->symbolTablePtr->type == ARRAY)
	{
		// printf("prashant 3\n");

		TreeNode* curr = head->child->next->child;
		int base = curr->symbolTablePtr->offset;
		int width = getWidth(curr->symbolTablePtr->subtype);
		int len = curr->symbolTablePtr->ub - curr->symbolTablePtr->lb + 1;

		// for printing whole array
		fprintf(fp, "\t\tpush rax\n");
		fprintf(fp, "\t\tpush rcx\n");
		fprintf(fp, "\t\tpush rbx\n");

		fprintf(fp, "\t\t\tmov rdi, arrayOutputLabel\t\t;printing label\n");
		fprintf(fp, "\t\t\tmov rsi, rax\n");
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");
		
		fprintf(fp, "\t\t\tmov rbx, 0\n");
		fprintf(fp, "\t\t\tL%d: \n", ++labels);
		fprintf(fp, "\t\t\tmov rcx, rbx\n");
		fprintf(fp, "\t\t\tadd rcx, %d\n", base);
		fprintf(fp, "\t\t\tmov ax, word[MEM + rcx]\n");

		fprintf(fp, "\t\t\tmov rdi, arrayOutput\n");
		fprintf(fp, "\t\t\tmov rsi, rax\n");
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");

		fprintf(fp, "\t\t\tadd rbx, %d\n", width);
		fprintf(fp, "\t\t\tcmp rbx, %d\n", width*len);
		fprintf(fp, "\t\t\tjnz L%d\n", labels);

		fprintf(fp, "\t\t\tmov rdi, newLine\t\t;printing new line\n");
		fprintf(fp, "\t\t\tmov rsi, rax\n");
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");

		fprintf(fp, "\t\tpop rbx\n");
		fprintf(fp, "\t\tpop rcx\n");
		fprintf(fp, "\t\tpop rax\n");

		fprintf(fp, "\n");
		return fp;
	}
	else
	{
		// for a particular array value  
		// printf("prashant 4\n");
		TreeNode* curr = head->child->next->child;
		TreeNode* temp = curr->next->child->next;
		if(temp->symbolTablePtr == NULL)
		{
			int a = atoi(temp->tokenPtr->tokenVal->tokenVal);
			fprintf(fp, "\t\tmov word[arr],%d\n",a=a*2-2);
		}
		else
		{
			fprintf(fp, "\t\tmov ax, word[MEM+%d]\n",temp->symbolTablePtr->offset);
			if(curr->symbolTablePtr->subtype==INTEGER)
			{
				fprintf(fp, "\t\timul 2\n");
				fprintf(fp, "\t\tsub ax, 2\n");
			}
			else
				fprintf(fp, "\t\tsub ax, 1\n");	
			fprintf(fp, "\t\tmov word[arr], ax\n");
		}
		fprintf(fp, "\t\tmov cx, word[arr]\n");
		fprintf(fp, "\t\tand ecx, 0x00001111\n");
		fprintf(fp, "\t\tmov ax, word[MEM+%d + ecx]\n",curr->symbolTablePtr->offset);
		fprintf(fp, "\t\t\tmov rdi, men\n");
	}

	fprintf(fp, "\t\tpush rax\n");
	fprintf(fp, "\t\tpush rcx\n");
	fprintf(fp, "\t\t\tmov rsi, rax\n");
	fprintf(fp, "\t\t\txor rax, rax\n");
	fprintf(fp, "\t\t\tcall printf\n");
	fprintf(fp, "\t\tpop rcx\n");
	fprintf(fp, "\t\tpop rax\n");

	fprintf(fp, "\n");
	return fp;
}

FILE* code_generator_input(TreeNode* head, FILE* fp)
{
	fprintf(fp, "\n");
	TreeNode* curr = head->child->next;

	if(curr->symbolTablePtr->type == ARRAY){
		fprintf(fp, "\t\t; to print input placeholder for array to console\n");
		fprintf(fp, "\t\tpush rax\n");
		fprintf(fp, "\t\tpush rcx\n");

		if(curr->symbolTablePtr->subtype == INTEGER){
			fprintf(fp, "\t\t\tmov rdi, arrayIntegerInput\n");
		}else if(curr->symbolTablePtr->subtype == REAL){
			fprintf(fp, "\t\t\tmov rdi, arrayRealInput\n");
		}else if(curr->symbolTablePtr->subtype == BOOLEAN){
			fprintf(fp, "\t\t\tmov rdi, arrayBooleanInput\n");
		}

		int len = curr->symbolTablePtr->ub - curr->symbolTablePtr->lb + 1;

		fprintf(fp, "\t\t\tmov rsi, %d\n", len);
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");
		
		fprintf(fp, "\t\t\tmov rdi, arrayLowRangeInput\n");
		fprintf(fp, "\t\t\tmov rsi, %d\n", curr->symbolTablePtr->lb);
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");

		fprintf(fp, "\t\t\tmov rdi, arrayHighRangeInput\n");
		fprintf(fp, "\t\t\tmov rsi, %d\n", curr->symbolTablePtr->ub);
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");

		fprintf(fp, "\t\tpop rcx\n");
		fprintf(fp, "\t\tpop rax\n");
		fprintf(fp, "\n");

		fprintf(fp, "\t\t; to scan input\n");
		fprintf(fp, "\t\tpush rcx\n");
		fprintf(fp, "\t\tpush rax\n");
		fprintf(fp, "\t\tpush rbx\n");

		fprintf(fp, "\t\t\tmov rbx, 0\n");
		fprintf(fp, "\t\t\tL%d:\n", ++labels);

		fprintf(fp, "\t\t\tmov rsi, var+0\n");
		fprintf(fp, "\t\t\tmov rdi, inputFormat\n");
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall scanf\n");
		fprintf(fp, "\t\t\tmov ax, word[var+0]\n");

		int base = curr->symbolTablePtr->offset;
		int width = getWidth(curr->symbolTablePtr->subtype);

		if(curr->symbolTablePtr->subtype == INTEGER){
			fprintf(fp, "\t\t\tmov rcx, rbx\n");
			fprintf(fp, "\t\t\tadd rcx, %d\n", base);
			fprintf(fp, "\t\t\tmov word[MEM+rcx], ax\n");
		}else{
			//later do it 
			fprintf(fp, "\t\t\tmov byte[MEM+%d], al\n",curr->symbolTablePtr->offset);
		}

		fprintf(fp, "\t\t\tadd rbx, %d\n", width);
		fprintf(fp, "\t\t\tcmp rbx, %d\n", len * width);
		fprintf(fp, "\t\t\tjnz L%d\n", labels);

		fprintf(fp, "\t\tpop rbx\n");
		fprintf(fp, "\t\tpop rax\n");
		fprintf(fp, "\t\tpop rcx\n");

	} else {
		fprintf(fp, "\t\t; to print input placeholder to console\n");
		fprintf(fp, "\t\tpush rax\n");
		fprintf(fp, "\t\tpush rcx\n");
		fprintf(fp, "\t\t\tmov rdi, integerInput\n");
		fprintf(fp, "\t\t\tmov rsi, rax\n");
		fprintf(fp, "\t\t\txor rax, rax\n");
		fprintf(fp, "\t\t\tcall printf\n");
		fprintf(fp, "\t\tpop rcx\n");
		fprintf(fp, "\t\tpop rax\n");
		fprintf(fp, "\n");

		fprintf(fp, "\t\t; to scan input\n");
		fprintf(fp, "\t\tmov rsi, var+0\n");
		fprintf(fp, "\t\tmov rdi, inputFormat\n");
		fprintf(fp, "\t\txor rax, rax\n");
		fprintf(fp, "\n");
		fprintf(fp, "\t\t; to align stack to 16B before calling rsp\n");
		fprintf(fp, "\t\tsub rsp, 8\n");
		fprintf(fp, "\t\tcall scanf\n");
		fprintf(fp, "\t\tadd rsp, 8\n");
		fprintf(fp, "\n");
		fprintf(fp, "\t\tmov ax, word[var+0]\n");
		
		if(curr->symbolTablePtr->type == INTEGER)
			fprintf(fp, "\t\tmov word[MEM+%d], ax\n",curr->symbolTablePtr->offset);
		else
			fprintf(fp, "\t\tmov byte[MEM+%d], al\n",curr->symbolTablePtr->offset);
	}

	fprintf(fp, "\n");

	return fp;
}

//DO THIS WHEN NO ERRORS
FILE* code_generator(TreeNode* head, FILE *fp)
{	
	if(head==NULL)
		return fp;

	if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]," assignmentStmt")==0)
	{
		typecheckStackTop = -1;

		fp = code_generator_assignment(head, fp);
		return fp;
	}
	else if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]," conditionalStmt")==0)
	{
		fp = code_generator_conditional(head, fp);
		return fp;
	}
	else if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]," iterativeStmt")==0)
	{
		fp = code_generator_iterative(head, fp);
		return fp;
	}
	else if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]," ioStmt")==0 && strcmp(head->child->tokenPtr->tokenName,"PRINT")==0)
	{
		fp = code_generator_print(head, fp);
		return fp;
	}
	else if(head->isNonterminal && strcmp(nonterminalHashTable[head->hashValue]," ioStmt")==0 && strcmp(head->child->tokenPtr->tokenName,"GET_VALUE")==0)
	{
		fp = code_generator_input(head, fp);
		return fp;
	}

	TreeNode *temp = head->child;
	while(temp!=NULL)
	{
		fp = code_generator(temp, fp);
		temp = temp->next;
	}
	return fp;
}

// Enter 6 array elements of integer type for range 10 to 15

void codeGenerator(TreeNode * head, char *outFile){
	FILE *fp = fopen(outFile, "w");
	if(fp == NULL){
		puts("File can't be opened!");
		return;
	}
	fprintf(fp, "global main\n");
	fprintf(fp, "extern printf\n");
	fprintf(fp, "extern scanf\n");
	fprintf(fp, "\n");
	
	fprintf(fp, "section .data\n");
	fprintf(fp, "\tMEM: times 1000 db 0\n");
	fprintf(fp, "\tarr: times 2 db 0\n");
	fprintf(fp, "\tmen: db \"Output: %%hd \", 10, 0\n");

	fprintf(fp, "\ttrueOutput: db \"Output: true \", 10, 0\n");
	fprintf(fp, "\tfalseOutput: db \"Output: false \", 10, 0\n");

	fprintf(fp, "\tarrayOutputLabel: db \"Output:\", 0\n");
	fprintf(fp, "\tarrayOutput: db \" %%hd\", 0\n");

	fprintf(fp, "\tnewLine: db 10, 0\n");

	fprintf(fp, "\tinputFormat: db \"%%d\", 0\n");
	fprintf(fp, "\tvar: times 8 db 0\n");

	fprintf(fp, "\tintegerInput: db \"Input: Enter an integer value \", 10, 0\n");
	fprintf(fp, "\tarrayIntegerInput: db \"Input: Enter %%d array elements of integer type \", 0\n");
	fprintf(fp, "\tarrayRealInput: db \"Input: Enter %%d array elements of real type \", 0\n");
	fprintf(fp, "\tarrayBooleanInput: db \"Input: Enter %%d array elements of boolean type \", 0\n");
	fprintf(fp, "\tarrayLowRangeInput: db \"for range %%d \", 0\n");
	fprintf(fp, "\tarrayHighRangeInput: db \"to %%d \", 10, 0\n");
	fprintf(fp, "\truntimeError: db \"RUN TIME ERROR:  Array index out of bound\", 10, 0\n");
	  
	
	fprintf(fp, "\n");
	fprintf(fp, "section .text\n");
	fprintf(fp, "\tmain:\n");
	fprintf(fp, "\n");
	
	fp = code_generator(head,fp);

	fprintf(fp, "\n");
	fprintf(fp, "\tmov al, 1 ; function 1: exit()\n");
	fprintf(fp, "\tmov rbx, 0 ; Return code\n");
	fprintf(fp, "\tint 80h\n");
	fprintf(fp, "\n");

	//runtime error -----------------------------------------------------------------------
	fprintf(fp, "\tRE:\n");
	
	fprintf(fp, "\t\t\tmov rdi, runtimeError\n");
	fprintf(fp, "\t\t\txor rax, rax\n");
	fprintf(fp, "\t\t\tcall printf\n");

	fprintf(fp, "\n");
	fprintf(fp, "\t\tmov al, 1 ; function 1: exit()\n");
	fprintf(fp, "\t\tmov rbx, 0 ; Return code\n");
	fprintf(fp, "\t\tint 80h\n");
	fprintf(fp, "\n");

	fclose(fp);
}	
