// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#ifndef symbolTable
#define symbolTable
#include <stdio.h>
#include <stdlib.h>
#include "symbolTableDef.h"

extern int getWidth(int x);

extern int getType(char* str);

extern char* getTypeName(int x);

extern SymbolTableParameters* getSymbolTableParametersNode(int x);

extern int hashId(TreeNode* head, int typ);// returns -1 if error (redeclartion) else 0;

extern int hashArray(TreeNode* head, int typ, int lo, int hi);// returns -1 if error (redeclartion) else 0;

extern int hashArrayDynamic(TreeNode* head, int type);

extern int hashInputList(TreeNode* head, int hash_val);

extern int hashOutputList(TreeNode* head, int hash_val);// cant return arrays rest is same as above

extern int matchModuleParameters(int x, TreeNode* head, int c_scope);

extern int matchModuleReturnList(int x, TreeNode* head, int c_scope); // note that relist cannot have arrays in decleration

extern void all_assigned(int idx, TreeNode* head);

extern void traverse(TreeNode* head);

extern void buildSymbolTable(TreeNode* pt_head);

extern void printSymbolTableUtil(TreeNode* head);

extern void printSymbolTable2(TreeNode* head);

extern void deficit_errors();

extern void printSymbolTableErrors();

extern void printScope(TreeNode *head);

extern void printScopeUtil(TreeNode *head);

extern void printArrayInfo(TreeNode *head);

extern void printArrayInfoUtil(TreeNode *head);

#endif