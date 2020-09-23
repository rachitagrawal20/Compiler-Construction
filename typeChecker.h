// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P


#ifndef typeChecker
#define typeChecker
#include <stdio.h>
#include <stdlib.h>
#include "typeCheckerDef.h"

extern int check_var(char str[50]);

extern int assign_type(char str[50]);

extern void typeCheck(TreeNode *head);

extern void typeCheckUtil(TreeNode *head, int *sqBracketStackTop, int *typecheckStackTop);

extern void semanticAnalyzer(TreeNode *head);

#endif