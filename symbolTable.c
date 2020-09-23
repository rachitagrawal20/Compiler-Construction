// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

int getWidth(int x)
{
	if(x == INTEGER)
		return 2;
	else if(x == BOOLEAN)
		return 1;
	else if(x == REAL)
		return 4;

	return -1;// semanticError 
}

int getType(char* str)
{
	if(strcmp(str,"integer") == 0)
		return INTEGER;
	else if(strcmp(str,"real") == 0)
		return REAL;
	else if(strcmp(str,"boolean") == 0)
		return BOOLEAN;
	else
		return ARRAY;
}

char* getTypeName(int x)
{
	if(x == INTEGER)
		strcpy(ret,"integer");
	if(x == REAL)
		strcpy(ret,"real");
	if(x == BOOLEAN)
		strcpy(ret,"boolean");
	if(x == ARRAY)
		strcpy(ret,"array");
	return ret;
}

SymbolTableParameters* getSymbolTableParametersNode(int x)
{
	SymbolTableParameters* newNode = (SymbolTableParameters*)malloc(sizeof(SymbolTableParameters));
	newNode->sc = x;
	return newNode;
}

int hashId(TreeNode* head, int typ)// returns -1 if semanticError (redeclartion) else 0;
{
	int ret = -1;
	int iter = 0;

	TreeNode* ch;

	while(head != NULL)
	{
		iter++;
		
		if(iter == 1)
			ch = head->child;
		else
			ch = head->child->next;
		
		if(ch == NULL)
			break;
		
		int x = get_hash_value(ch->tokenPtr->tokenVal->tokenVal);
		x = put_hash(symbolTableHashTable, x, ch->tokenPtr->tokenVal->tokenVal);

		if(scope[c_scope][x].sc != -1 && scope[c_scope][x].inInputList == 0)// no redecleration
		{
			ch->symbolTablePtr =  (SymbolTableParameters*)malloc(sizeof (SymbolTableParameters));
			ch->symbolTablePtr->dec = -1;
			ret = x;
		}
		else
		{
			scope[c_scope][x].sc = c_scope;
			scope[c_scope][x].type = typ;
			scope[c_scope][x].dec = 0;
			scope[c_scope][x].inInputList = 0;
			scope[c_scope][x].inOutputList = 0;
			scope[c_scope][x].width = getWidth(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			strcpy(scope[c_scope][x].module_name, cur_scope_name);
			
			ch->symbolTablePtr = &scope[c_scope][x];

			cur_offset[current_module] += scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		head = ch->next;
	}
	return ret;
} 

int hashArrayDynamic(TreeNode* head, int type)// returns -1 if semanticError (redeclartion) else 0;
{
	int ret = -1;
	int iter = 0;

	TreeNode* ch;
	while(head != NULL)
	{
		iter++;

		if(iter == 1)
			ch = head->child;
		else
			ch = head->child->next;
		
		if(ch == NULL)
			break;

		int x = get_hash_value(ch->tokenPtr->tokenVal->tokenVal);
		x = put_hash(symbolTableHashTable, x,ch->tokenPtr->tokenVal->tokenVal);
		if(scope[c_scope][x].sc!=-1 && scope[c_scope][x].inInputList == 0)// no redecleration
		{
			// dont scope it 
			ch->symbolTablePtr =  (SymbolTableParameters*)malloc(sizeof (SymbolTableParameters));
			ch->symbolTablePtr->dec = -1;
			ret = x;
		}
		else
		{
			scope[c_scope][x].dec = 0;
			scope[c_scope][x].sc = c_scope;
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = type;
			scope[c_scope][x].isDynamic = 1;
			scope[c_scope][x].inInputList = 0;
			scope[c_scope][x].inOutputList = 0;

			scope[c_scope][x].width = 1;
			scope[c_scope][x].offset = cur_offset[current_module];
			strcpy(scope[c_scope][x].module_name, cur_scope_name);
			ch->symbolTablePtr = &scope[c_scope][x];
			
			cur_offset[current_module] += scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		head = ch->next;
	}
	return ret;
} 

int hashArray(TreeNode* head, int type, int lo, int hi)// returns -1 if semanticError (redeclartion) else 0;
{
	int ret = -1;
	int iter = 0;

	TreeNode* ch;
	while(head != NULL)
	{
		iter++;

		if(iter == 1)
			ch = head->child;
		else
			ch = head->child->next;
		
		if(ch == NULL)
			break;

		int x = get_hash_value(ch->tokenPtr->tokenVal->tokenVal);
		x = put_hash(symbolTableHashTable, x,ch->tokenPtr->tokenVal->tokenVal);
		if(scope[c_scope][x].sc!=-1 && scope[c_scope][x].inInputList == 0)// no redecleration
		{
			// dont scope it 
			ch->symbolTablePtr =  (SymbolTableParameters*)malloc(sizeof (SymbolTableParameters));
			ch->symbolTablePtr->dec = -1;
			ret = x;
		}
		else
		{
			scope[c_scope][x].dec = 0;
			scope[c_scope][x].sc = c_scope;
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = type;
			scope[c_scope][x].lb = lo;
			scope[c_scope][x].ub = hi;
			scope[c_scope][x].inInputList = 0;
			scope[c_scope][x].inOutputList = 0;
			scope[c_scope][x].isDynamic = 0;
			
			if(lo > hi)
			{
				sprintf(error_log[err_cnt++],"Error_18: Arrays can have lower index greater than higher index at line_no:%d",ch->tokenPtr->lineNumber);
				semanticError = 1;
			}

			scope[c_scope][x].width =  1 + getWidth(scope[c_scope][x].subtype)*(hi-lo+1);
			scope[c_scope][x].offset = cur_offset[current_module];
			strcpy(scope[c_scope][x].module_name, cur_scope_name);
			ch->symbolTablePtr = &scope[c_scope][x];
			
			cur_offset[current_module] += scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		head = ch->next;
	}
	return ret;
} 

int hashInputList(TreeNode* head, int hashValue)
{

	int iter = 0;
	int ok = 0;
	while(head != NULL && head->child != NULL)
	{
		iter++;
		
		TreeNode* var;
		
		if(iter == 1) // the corresponding parameter list will be  NULL
			var = head->child;
		else
			var = head->child->next;
		
		if(var == NULL)
			break;
		
		TreeNode*  d_type = var->next->next;

		int x = get_hash_value(var->tokenPtr->tokenVal->tokenVal);
		x = put_hash(symbolTableHashTable, x,var->tokenPtr->tokenVal->tokenVal);
		
		int datatype = getType(d_type->child->tokenPtr->tokenVal->tokenVal);
		
		scope[c_scope][x].sc = c_scope;
		scope[c_scope][x].dec = 0;
		scope[c_scope][x].inInputList = 1;
		scope[c_scope][x].inOutputList = 0;
		strcpy(scope[c_scope][x].module_name,cur_scope_name);
		
		if(datatype!=ARRAY)
		{
			scope[c_scope][x].type = datatype;
			scope[c_scope][x].width = getWidth(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		else
		{
			TreeNode* d_type_child = d_type->child;
			d_type_child = d_type_child->next->next;
			
			if(strcmp(terminalHashTable[d_type_child->child->child->hashValue], "NUM") == 0){
				scope[c_scope][x].lb= atoi(d_type_child->child->child->tokenPtr->tokenVal->tokenVal);
				scope[c_scope][x].ub= atoi(d_type_child->child->next->next->child->tokenPtr->tokenVal->tokenVal);
				scope[c_scope][x].isDynamic = 0;
			}else{
				scope[c_scope][x].isDynamic = 1;
			}
			
			d_type_child = d_type_child->next->next->next;

			scope[c_scope][x].type = ARRAY;

			scope[c_scope][x].subtype = getType(d_type_child->child->tokenPtr->tokenVal->tokenVal);// yahan dikkat hai
			
			//--new changes
			// if(d_type_child->child->tokenPtr){
			// 	scope[c_scope][x].subtype = getType(d_type_child->child->tokenPtr->tokenVal->tokenVal);
			// }else{
			// 	scope[c_scope][x].subtype = getType(d_type_child->next->next->next->child->tokenPtr->tokenVal->tokenVal);
			// }

			scope[c_scope][x].width = getWidth(INTEGER)*2 + 1; //(scope[c_scope][x].ub-scope[c_scope][x].lb+1);
			scope[c_scope][x].offset = cur_offset[current_module];
			
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}

		var->symbolTablePtr = &scope[c_scope][x];
		
		if(iter == 1)
		{
			in_list[hashValue] = (idlist*)malloc(sizeof(idlist));
			in_list[hashValue]->sc = scope[c_scope][x].sc;
			in_list[hashValue]->type = scope[c_scope][x].type;
			in_list[hashValue]->subtype = scope[c_scope][x].subtype;
			in_list[hashValue]->ub = scope[c_scope][x].ub;
			in_list[hashValue]->next = NULL;
		}
		else
		{	
			idlist* local = in_list[hashValue];
			
			while(local->next!=NULL)
				local = local->next;
			
			local->next = (idlist*)malloc(sizeof(idlist));
			local =  local->next;
			local->sc = scope[c_scope][x].sc;
			local->type = scope[c_scope][x].type;
			local->subtype = scope[c_scope][x].subtype;
			local->ub = scope[c_scope][x].ub;
			local->next = NULL;
		}
		head = d_type->next;
	}
	return ok;
}

int hashOutputList(TreeNode* head, int hashValue)// can't return arrays rest is same as above
{
	int iter = 0;
	int ok = 0;
	while(head != NULL && head->child != NULL)
	{
		iter++;

		TreeNode* var;
		
		if(iter == 1) // the corresponding parameter list will be  NULL
			var = head->child;
		else
			var = head->child->next;
		
		if(var == NULL)
			break;
		
		TreeNode*  d_type = var->next->next;
		
		int x = get_hash_value(var->tokenPtr->tokenVal->tokenVal);
		x = put_hash(symbolTableHashTable, x,var->tokenPtr->tokenVal->tokenVal);
		
		int datatype = getType(d_type->child->tokenPtr->tokenVal->tokenVal);
		
		scope[c_scope][x].sc = c_scope;
		scope[c_scope][x].dec = 0;
		scope[c_scope][x].inInputList = 0;
		scope[c_scope][x].inOutputList = 1;
		strcpy(scope[c_scope][x].module_name,cur_scope_name);
		
		if(datatype!=ARRAY)
		{
			scope[c_scope][x].type = datatype;
			scope[c_scope][x].width = getWidth(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		else
		{
			sprintf(error_log[err_cnt++],"ERROR_10: Can't have arrays as return type of modules for module %s:",symbolTableHashTable[hashValue]);
			semanticError = 1;
			TreeNode* d_type_child = d_type->child;
			d_type_child = d_type_child->next->next;
			scope[c_scope][x].lb= atoi(d_type_child->child->tokenPtr->tokenVal->tokenVal);
			scope[c_scope][x].ub= atoi(d_type_child->child->next->next->tokenPtr->tokenVal->tokenVal);
			d_type_child = d_type_child->next->next;
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = getType(d_type_child->tokenPtr->tokenVal->tokenVal);
		}

		var->symbolTablePtr = &scope[c_scope][x];
		
		if(iter == 1)
		{
			out_list[hashValue] = (idlist*)malloc(sizeof(idlist));
			out_list[hashValue]->sc = scope[c_scope][x].sc;
			out_list[hashValue]->type = scope[c_scope][x].type;
			out_list[hashValue]->subtype = scope[c_scope][x].subtype;
			out_list[hashValue]->ub = scope[c_scope][x].ub;
			out_list[hashValue]->next = NULL;
			out_list[hashValue]->h_val = x;
		}
		else
		{	
			idlist* local = out_list[hashValue];
			while(local->next!=NULL)
				local = local->next;

			local->next = (idlist*)malloc(sizeof(idlist));
			local =  local->next;
			local->sc = scope[c_scope][x].sc;
			local->type = scope[c_scope][x].type;
			local->subtype = scope[c_scope][x].subtype;
			local->ub = scope[c_scope][x].ub;
			local->next = NULL;
			local->h_val = x;
		}
		head = d_type->next;
	}
	return ok;
}

int matchModuleParameters(int x, TreeNode* head, int c_scope)
{
	int iter = 0;
	idlist* trav = in_list[x];
	while(trav!=NULL && head!=NULL)
	{
		iter++;
		TreeNode* ch;
		
		if(iter == 1)
			ch = head->child;
		else
			ch = head->child->next;
		
		int x = put_hash(symbolTableHashTable, get_hash_value(ch->tokenPtr->tokenVal->tokenVal),ch->tokenPtr->tokenVal->tokenVal);
		
		if(dec[x] == 0 && def[x] == 0)
		{
			int scope_trav = c_scope;
			while(scope_trav != 0) // no global declaration assumption
			{
				if(scope[scope_trav][x].sc!=-1)
					break;
				else
					scope_trav = parent[scope_trav];
			}

			if(scope_trav == 0)// no semantic error
			{
				sprintf(error_log[err_cnt++],"ERROR_1:%s Not declared at line no: %d\n",ch->tokenPtr->tokenVal->tokenVal,ch->tokenPtr->lineNumber);
				semanticError = 1;
				ch->symbolTablePtr = &emptyNode;
			}
			else // update in parse tree pointer
			{
				ch->symbolTablePtr = &scope[scope_trav][x];
			}
		}

		if(ch->symbolTablePtr->type == trav->type)
		{
			if(trav->type == ARRAY)
			{
				if(trav->subtype!=ch->symbolTablePtr->subtype)
					return 0;
				else if(trav->ub!=ch->symbolTablePtr->ub)
					return 0;
			}
		}
		else
			return 0;
		
		head = ch->next;
		trav = trav->next;
	}

	if(head == NULL || head->child == NULL)
		return 0;
	
	if(trav == NULL && strcmp(terminalHashTable[head->child->hashValue],"e") == 0)
		return 1;
	else
		return 0;
}

int matchModuleReturnList(int x, TreeNode* head, int c_scope) // note that return list cannot have arrays in decleration
{
	int iter = 0;
	idlist* trav = out_list[x];

	// ********************
	int cur_line=head->child->tokenPtr->lineNumber;
	// ********************
	
	while(trav != NULL && head != NULL)
	{

		// ********************
		if(strcmp(head->child->tokenPtr->tokenName,"epsilon")==0){
			sprintf(error_log[err_cnt++],"ERROR: Number of output parametters does not match with that of formal parameters at line no: %d\n",cur_line);
			return 1;
		}
		// ********************

		iter++;
		TreeNode* ch;
		
		if(iter == 1)
			ch = head->child;
		else
			ch = head->child->next;
		
		int x = put_hash(symbolTableHashTable, get_hash_value(ch->tokenPtr->tokenVal->tokenVal),ch->tokenPtr->tokenVal->tokenVal);
		
		if(dec[x] == 0 && def[x] == 0)
		{
			int scope_trav = c_scope;
			while(scope_trav != 0) // no global declaration assumption
			{
				if(scope[scope_trav][x].sc != -1)
					break;
				else
					scope_trav = parent[scope_trav];
			}
			if(scope_trav == 0)// no semantic error
			{
				sprintf(error_log[err_cnt++],"ERROR_1: %s Not declared at line no: %d\n",ch->tokenPtr->tokenVal->tokenVal,ch->tokenPtr->lineNumber);
				semanticError = 1;
				ch->symbolTablePtr = &emptyNode;
			}
			else // update in parse tree pointer
			{
				ch->symbolTablePtr = &scope[scope_trav][x];
			}
		}

		if(ch->symbolTablePtr->type == trav->type)
		{
			if(trav->type == ARRAY)
			{
				if(trav->subtype!=ch->symbolTablePtr->subtype)
					return 0;
				else if(trav->ub!=ch->symbolTablePtr->ub)
					return 0;
			}
		}
		else
			return 0;
		
		head = ch->next;
		trav = trav->next;
	}

	if(head == NULL || head->child == NULL)
		return 0;
	
	if(trav == NULL && strcmp(terminalHashTable[head->child->hashValue],"e") == 0)
		return 1;
	else
		return 0;
}

void all_assigned(int idx, TreeNode* head)
{
	if(head == NULL)
		return;
	
	if(head->isNonterminal == 1 && strcmp(nonterminalHashTable[head->hashValue]," assignmentStmt") == 0)
	{
		idlist* trav = out_list[idx];
		
		while(trav != NULL 
			&& trav->h_val != (put_hash(symbolTableHashTable, get_hash_value(head->child->tokenPtr->tokenVal->tokenVal), head->child->tokenPtr->tokenVal->tokenVal)))
		{
			trav = trav->next;
		}

		if(trav != NULL)
			trav->mark = 1;
	}
	TreeNode* temp = head->child;
	while(temp!=NULL)
	{
		all_assigned(idx, temp);
		temp = temp->next;
	}
}

void traverse(TreeNode* head)
{
	if(head == NULL)
		return;

	int currentHashValue = head->hashValue;
	
	if(head->isNonterminal == 1)// chance of getting declareStmt
	{
		// if the current non-terminal is declareStmt 
		if(strcmp(nonterminalHashTable[currentHashValue]," declareStmt") == 0)
		{
			// printf("Success traverse\n");
			TreeNode* t = head->child;
			t = t->next;// idlist
			
			TreeNode* typee = t->next->next;// datatype

			int type = getType(typee->child->tokenPtr->tokenVal->tokenVal);
			if(type != ARRAY)
			{
				int chk = hashId(t, type);
				if(chk != -1) // -1 here means all went well since symbolTableHashTable val can't be -1
				{
					sprintf(error_log[err_cnt++],"ERROR_2: %s was declared more than once at line no: %d\n",symbolTableHashTable[chk],typee->child->tokenPtr->lineNumber);
					semanticError = 1;
				}
			}
			else // array
			{
				TreeNode* rng_ptr = typee->child->next->next;
				TreeNode* sub_type_ptr = rng_ptr->next->next->next;
				int sub_type = getType(sub_type_ptr->child->tokenPtr->tokenVal->tokenVal);
				
				int chk;

				if(strcmp(terminalHashTable[rng_ptr->child->child->hashValue], "NUM") == 0){
					int lo = atoi(rng_ptr->child->child->tokenPtr->tokenVal->tokenVal);
					int hi = atoi(rng_ptr->child->next->next->child->tokenPtr->tokenVal->tokenVal);
					chk = hashArray(t,sub_type,lo,hi);
				}else{
					chk = hashArrayDynamic(t,sub_type);
				}

				if(chk != -1) // -1 here means all went well since symbolTableHashTable val can't be -1
				{
					sprintf(error_log[err_cnt++],"ERROR_2: %s was declared more than once at line no: %d\n",symbolTableHashTable[chk],typee->child->tokenPtr->lineNumber);
					semanticError = 1;
				}
			}
		}
		else 
		if(strcmp(nonterminalHashTable[currentHashValue]," moduleDeclaration") == 0)
		{
			TreeNode* module = head->child->next->next;
			//puts(module->tokenPtr->tokenVal->tokenVal);
			int hashValue = put_hash(symbolTableHashTable, get_hash_value(module->tokenPtr->tokenVal->tokenVal), module->tokenPtr->tokenVal->tokenVal);
			if(dec[hashValue])
			{
				sprintf(error_log[err_cnt++],"ERROR_3: %s is being redeclared at line no: %d\n", module->tokenPtr->tokenVal->tokenVal, module->tokenPtr->lineNumber);
				semanticError = 1;
			}
			dec[hashValue] = 1;
		}
		else
		if(strcmp(nonterminalHashTable[currentHashValue]," moduleNt") == 0)
		{
			int hashValue;
			already_scoped = 1;
			
			int new_scope = ++global_scope_ctr;
			
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			current_module = c_scope;
			
			TreeNode* inp_list = head->child;
			int ctr = 0;
			while(ctr != 7)
			{
				inp_list = inp_list->next;
				ctr++;
				if(ctr == 2)
				{
					hashValue = put_hash(symbolTableHashTable, get_hash_value(inp_list->tokenPtr->tokenVal->tokenVal),inp_list->tokenPtr->tokenVal->tokenVal);
					if(def[hashValue] == 1)
					{
						sprintf(error_log[err_cnt++],"ERROR_4: %s is being redefind at line no: %d\n",inp_list->tokenPtr->tokenVal->tokenVal,inp_list->tokenPtr->lineNumber);
						semanticError = 1;
					}
					def[hashValue] = 1;
					strcpy(cur_scope_name,inp_list->tokenPtr->tokenVal->tokenVal);
				}
			}

			int ret_val = hashInputList(inp_list,hashValue);// symbolTableHashTable vars into new scope + fill def list variables for this module
			
			if(ret_val)
			{
				sprintf(error_log[err_cnt++],"ERROR_5: Some anomaly in parameter list of module %s",symbolTableHashTable[hashValue]);
				semanticError = 1;
			}
			
			ret_val = 0;
			TreeNode* outp_list = inp_list;
			outp_list = outp_list->next->next->next;
			
			if(strcmp(terminalHashTable[outp_list->child->hashValue], "e") != 0)
			{
				outp_list = outp_list->child->next->next;
				ret_val = hashOutputList(outp_list,hashValue);
				
				all_assigned(hashValue,head);
				
				idlist* s_trav = out_list[hashValue];
				while(s_trav!=NULL)
				{
					if(s_trav->mark!=1)
					{
						sprintf(error_log[err_cnt++],"ERROR_18: variable %s in module %s return list not assigned a value in module\n",symbolTableHashTable[s_trav->h_val],symbolTableHashTable[hashValue]);
						semanticError = 1;
					}
					s_trav = s_trav->next;
				}
			}
		}
		else if(strcmp(nonterminalHashTable[currentHashValue], " driverModule") == 0)
		{
			already_scoped = 1;
			int new_scope = ++global_scope_ctr;
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			current_module = c_scope;
			strcpy(cur_scope_name,"driver");
		}
		else if(strcmp(nonterminalHashTable[currentHashValue], " moduleReuseStmt") == 0) // calling modules handling functions defined above
		{
			TreeNode* to_call = head->child->next->next->next;
			int hash_call = put_hash(symbolTableHashTable, get_hash_value(to_call->tokenPtr->tokenVal->tokenVal),to_call->tokenPtr->tokenVal->tokenVal);
			
			if(def[hash_call] && dec[hash_call] && strcmp(cur_scope_name,to_call->tokenPtr->tokenVal->tokenVal) != 0){
				to_call->symbolTablePtr = &emptyNode;

				sprintf(errorDecnDef[errorDecnDef_count++].error_msg,"ERROR: The function %s currentHashValue has been both defined and declared earlier : Line number %d\n",symbolTableHashTable[hash_call],to_call->tokenPtr->lineNumber);
				errorDecnDef[errorDecnDef_count-1].x = hash_call;

			}
			
			else if(def[hash_call])	
			{
				if(strcmp(cur_scope_name,to_call->tokenPtr->tokenVal->tokenVal) == 0)
				{	
					sprintf(error_log[err_cnt++],"ERROR_6 Recursion not allowed at line no: %d\n",to_call->tokenPtr->lineNumber);
					semanticError = 1;
				}
				TreeNode* list  =  to_call->next->next->next;
				int input_match = matchModuleParameters(hash_call,list,c_scope);
				if(input_match == 0)
				{	
					sprintf(error_log[err_cnt++],"ERROR_7 %s parameter list doesn't match the call at line no: %d\n",to_call->tokenPtr->tokenVal->tokenVal,to_call->tokenPtr->lineNumber);
					semanticError = 1;
				}
				int output_match = 1;
				if(strcmp(terminalHashTable[head->child->child->hashValue],"SQBO") == 0) // there's a return list
					output_match = matchModuleReturnList(hash_call,head->child->child->next,c_scope);
				if(output_match == 0)
				{
					sprintf(error_log[err_cnt++],"ERROR_8 %s return list doesn't match the call at line no: %d\n",to_call->tokenPtr->tokenVal->tokenVal,to_call->tokenPtr->lineNumber);
					semanticError = 1;
				}
			}
			else if(dec[hash_call])
			{
				TreeNode* list  =  to_call->next->next->next;
				todo[lzyptr].in = list;
				todo[lzyptr].scope = c_scope;
				todo[lzyptr].lineno = to_call->tokenPtr->lineNumber;
				if(strcmp(terminalHashTable[head->child->child->hashValue],"SQBO") == 0)
					todo[lzyptr].out = head->child->child->next;
				else
					todo[lzyptr].out = NULL;
				todo[lzyptr].x = hash_call;

				lzyptr++;
				// handle case of declaration w/o definition
			}
			else
			{
				to_call->symbolTablePtr = &emptyNode;
				sprintf(error_log[err_cnt++],"ERROR_9: The function %s currentHashValue neither been defined nor been declared earlier : Line number %d\n",symbolTableHashTable[hash_call],to_call->tokenPtr->lineNumber);
				semanticError = 1;
			}
		}
	}
	// declrations checked !
	if(head->isNonterminal == 0) // starts and ends
	{
		if(strcmp(terminalHashTable[head->hashValue], "START") == 0)
		{
			if(!already_scoped)
			{
				int new_scope = ++global_scope_ctr;
				parent[new_scope] = c_scope;
				c_scope = new_scope;
			}
			else
			{	
				already_scoped = 0;
			}
			start[c_scope] = head->tokenPtr->lineNumber;;
		}
		else if(strcmp(terminalHashTable[head->hashValue], "END") == 0)
		{
			end[c_scope] = head->tokenPtr->lineNumber;
			c_scope = parent[c_scope];
		}
		else if(strcmp(terminalHashTable[head->hashValue], "FOR") == 0)
		{
			already_scoped = 1;
			int new_scope = ++global_scope_ctr;
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			head->symbolTablePtr = getSymbolTableParametersNode(c_scope);
		}
		else if(strcmp(terminalHashTable[head->hashValue], "ID") == 0)
		{
			int x = put_hash(symbolTableHashTable, get_hash_value(head->tokenPtr->tokenVal->tokenVal),head->tokenPtr->tokenVal->tokenVal);
			if(dec[x] == 0 && def[x] == 0)
			{
				int scope_trav = c_scope;
				while(scope_trav != 0) // no global declaration assumption
				{
					if(scope[scope_trav][x].sc != -1)
						break;
					else
						scope_trav = parent[scope_trav];
				}
				if(scope_trav == 0 && head->symbolTablePtr == NULL)// no semantic error
				{
					sprintf(error_log[err_cnt++],"ERROR_1: %s Not declared : Line number: %d\n",head->tokenPtr->tokenVal->tokenVal,head->tokenPtr->lineNumber);
					semanticError = 1;
					head->symbolTablePtr = &emptyNode;
				}
				else // update in parse tree pointer
				{
					head->symbolTablePtr = &scope[scope_trav][x];
				}
			}
		}
	}
	TreeNode* temp = head->child;
	while(temp!=NULL)
	{
		traverse(temp);
		temp = temp->next;
	}	
}

void buildSymbolTable(TreeNode* head)
{
	// initializing parameters
	emptyNode.sc = -1;
	emptyNode.type = -1;
	emptyNode.subtype = -1;
	emptyNode.lb = -1;
	emptyNode.ub = -1;
	emptyNode.offset = -1;
	emptyNode.dec = -1;
	memset(parent, -1, sizeof parent);
	memset(scope, -1, sizeof scope);
	memset(dec, 0, sizeof dec);
	memset(def, 0, sizeof def);
	memset(symbolTableHashTable, 0, sizeof symbolTableHashTable);
	memset(in_list, -1, sizeof in_list);
	memset(out_list, -1, sizeof out_list);

	//hashing all the variables to symbolTableHashTable "symbolTableHashTable"
	SymbolTable* trav = symbolTableListHead;
	while(trav != NULL)
	{
		if(strcmp(trav->tokenName, "ID") == 0)
		{
			int x = get_hash_value(trav->tokenVal);
			x = put_hash(symbolTableHashTable, x,trav->tokenVal);
		}
		trav = trav->next;
	}

	traverse(head);// start scopes from 1 so 0 is unassigned
	
	for(int i = 0; i <=1000 ; ++i)
	{
		int iter = 1;
		int cur = i;  
		while(parent[cur] != 0)
		{
			++iter;
			cur = parent[cur];
		}
		scope_map[i] = iter;
	}

}

void printSymbolTableUtil(TreeNode* head)
{
	
	int m;
	if(head == NULL)
		return;

	if(strcmp(terminalHashTable[head->hashValue],"ID") == 0)
	{	
			
		int x = put_hash(symbolTableHashTable, get_hash_value(head->tokenPtr->tokenVal->tokenVal),head->tokenPtr->tokenVal->tokenVal);

		if(dec[x]==0 && def[x]==0 && head->symbolTablePtr->dec == 0)
		{
			head->symbolTablePtr->dec = 1;
			//m = strlen(head->tokenPtr->tokenVal->tokenVal);
			//m = 12-m;

			// identifier name
			printf("%-12s",head->tokenPtr->tokenVal->tokenVal);
			
			// module name
			int y = head->symbolTablePtr->sc;
			printf("%-13s    	",head->symbolTablePtr->module_name);

			// line number 
			int it = 0;
			it = 0;
			it+=printf("%d ",start[y]);
			it+=printf("to ");
			it+=printf("%d ",end[y]);
			while(it!=14)
			{
				it++;
				printf(" ");
			}

			//width
			printf("%-7d     ",head->symbolTablePtr->width);

			//isArray
			if(head->symbolTablePtr->type == ARRAY){	
				printf("yes   ");
			}else{
				printf("no    ");
			}

			//static_or_dynamic
			if(head->symbolTablePtr->type == ARRAY){
				if(head->symbolTablePtr->isDynamic){
					printf("dynamic 	");
				}else{
					printf("static 	");
				}
			}else{
				printf("---  ");
			}

			//range_lexems
			if(head->symbolTablePtr->type == ARRAY && !head->symbolTablePtr->isDynamic){	
				printf("[%d,%d]  ", head->symbolTablePtr->lb, head->symbolTablePtr->ub);
			}else{
				printf("---  ");
			}			
			
			// type
			int ctr;	
			if(head->symbolTablePtr->type == ARRAY){	
				ctr = printf("%s", getTypeName(head->symbolTablePtr->subtype));
			}else{
				ctr = printf("%s", getTypeName(head->symbolTablePtr->type));
			}

			for(it = 0; it<=22-ctr; it++)
				printf(" ");

			//offset 
			printf("%-7d  ", head->symbolTablePtr->offset);
			
			// nesting level
			if(head->symbolTablePtr->inInputList || head->symbolTablePtr->inOutputList){
				printf("0\n");	
			}else{
				printf("%-9d\n",scope_map[head->symbolTablePtr->sc]);
			}
			var_chk[x] = 1;
		}
	}

	TreeNode* temp = head->child;
	while(temp!=NULL)
	{
		printSymbolTableUtil(temp);
		temp = temp->next;
	}	
}

void printSymbolTable2(TreeNode* head){
	printf("variable_name	scope(module_name)	scope(line_number)	width 	isArray 	static_or_dynamic	range_lexems	type_of_element	offset	nesting_level\n");	
	printSymbolTableUtil(head);						
}

void deficit_errors()
{
	int i;
//	puts("\n\n\n\n");
	for (i = 0; i < lzyptr; ++i)
	{
		//printf("%s\n",symbolTableHashTable[todo[i].x]);
		int input_match = matchModuleParameters(todo[i].x,todo[i].in,todo[i].scope);
		if(input_match == 0)
		{
			sprintf(error_log[err_cnt++],"ERROR_7: %s parameter list doesn't match the call: Line number %d\n",symbolTableHashTable[todo[i].x], todo[i].lineno);
			semanticError = 1;
		}
		int output_match = 1;
		if(todo[i].out!=NULL) // there's a return list
			output_match = matchModuleReturnList(todo[i].x,todo[i].out,todo[i].scope);
		if(output_match == 0)
		{
			sprintf(error_log[err_cnt++],"ERROR_8: %s return list doesn't match the call: Line number %d\n",symbolTableHashTable[todo[i].x], todo[i].lineno);
			semanticError = 1;
		}
		if(input_match == 1 && output_match == 1){
			flag[todo[i].x] = 1;//ie now calls after dec and def are valid
		}
	}

	//To ensure that lzyptr is cleared every time an option is chosen from the driver
	lzyptr = 0;
}

void printSymbolTableErrors(){
	//printf("%d",err_cnt);
	int i;
	for (i = 0; i < err_cnt; ++i)
	{
		printf("%s",error_log[i]);
		// strcpy(error_log[i], '\0');
	}

	for(i = 0; i < errorDecnDef_count; i++){
		if(flag[errorDecnDef[i].x] == 0){
			printf("%s", errorDecnDef[i].error_msg);
			semanticError = 1;
		}
	}

	err_cnt = 0; //To ensure that err_cnt is cleared every time an option is chosen from the driver to avoid re-printing of errors when chosen again
	errorDecnDef_count = 0;
}   

void printScope(TreeNode *head){

	printScopeUtil(head);

	for(int i=0;i<HASHTABLE_LEN;i++){
		if(scopeWidth[i]!=0){
			printf("%s \t %d\n", moduleHashTable[i], scopeWidth[i]);
			scopeWidth[i]=0; //So that entire array becomes initialized to 0 again
		}
	}
}

void printScopeUtil(TreeNode *head){
	int m;
	if(head == NULL)
		return;

	if(strcmp(terminalHashTable[head->hashValue],"ID") == 0)
	{	
			
		int x = put_hash(symbolTableHashTable, get_hash_value(head->tokenPtr->tokenVal->tokenVal),head->tokenPtr->tokenVal->tokenVal);

		if(dec[x]==0 && def[x]==0 && head->symbolTablePtr->dec == 0)
		{
			head->symbolTablePtr->dec = 1;
			//m = strlen(head->tokenPtr->tokenVal->tokenVal);
			//m = 12-m;

			//Extracting activation record size
			int hashval = put_hash(moduleHashTable, get_hash_value(head->symbolTablePtr->module_name),head->symbolTablePtr->module_name);
			scopeWidth[hashval] += head->symbolTablePtr->width;		

			var_chk[x] = 1;
		}
	}

	TreeNode* temp = head->child;
	while(temp!=NULL)
	{
		printScopeUtil(temp);
		temp = temp->next;
	}
}

//Static and Dynamic arrays information: driver point 7 implementation
void printArrayInfo(TreeNode *head){

	printf("Scope(module_name)	Scope(line_number)	Array_variable	StaticOrDynamic 	range_lexems	type_of_element		\n");
	printArrayInfoUtil(head);

}

void printArrayInfoUtil(TreeNode *head){
	int m;
	if(head == NULL)
		return;

	if(strcmp(terminalHashTable[head->hashValue],"ID") == 0)
	{	
			
		int x = put_hash(symbolTableHashTable, get_hash_value(head->tokenPtr->tokenVal->tokenVal),head->tokenPtr->tokenVal->tokenVal);

		if(dec[x]==0 && def[x]==0 && head->symbolTablePtr->dec == 0)
		{
			head->symbolTablePtr->dec = 1;
			//m = strlen(head->tokenPtr->tokenVal->tokenVal);
			//m = 12-m;

			if(head->symbolTablePtr->type == ARRAY){
				// module name
				int y = head->symbolTablePtr->sc;
				printf("%-13s    ",head->symbolTablePtr->module_name);

				// line number 
				int it = 0;
				it = 0;
				printf("\t");
				it+=printf("%d ",start[y]);
				it+=printf("to ");
				it+=printf("%d ",end[y]);
				while(it!=14){
					it++;
					printf(" ");
				}

				// identifier name
				printf("\t\t\t %-12s",head->tokenPtr->tokenVal->tokenVal);

				//static or dynamic
				if(head->symbolTablePtr->isDynamic){
					printf("dynamic 	");
				}else{
					printf("static 	");
				}

				//range_lexems
				printf("\t");
				if(head->symbolTablePtr->type == ARRAY && !head->symbolTablePtr->isDynamic){	
					printf("[%d,%d]  ", head->symbolTablePtr->lb, head->symbolTablePtr->ub);
				}else{
					printf("---  ");
				}


				// type
				int ctr;	
				printf("\t");
				if(head->symbolTablePtr->type == ARRAY){	
					ctr = printf("%s", getTypeName(head->symbolTablePtr->subtype));
				}
				else{
					ctr = printf("%s", getTypeName(head->symbolTablePtr->type));
				}
				printf("\n");
			}
			var_chk[x] = 1;
		}
	}

	TreeNode* temp = head->child;
	while(temp!=NULL)
	{
		printArrayInfoUtil(temp);
		temp = temp->next;
	}
}
