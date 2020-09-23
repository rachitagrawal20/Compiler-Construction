// BATCH NUMBER 6
// PRASHANT KHANDELWAL 2016B4A70930P
// NAMAN DEEP SRIVASTAVA 2016B4A70891P
// RACHIT AGRAWAL 2016B2A70901P
// MOHIT KRIPLANI 2016B1A70870P
// UTKARSH AGARWAL 2016B3A70581P

#ifndef codegen
#define codegen
#include <stdio.h>
#include <stdlib.h>
#include "typeChecker.h"

extern FILE* code_generator_assignment(TreeNode* head, FILE * fp);

extern FILE* code_generator_conditional(TreeNode* head, FILE * fp);

extern FILE* code_generator_iterative(TreeNode* head, FILE * fp);

extern FILE* code_generator_print(TreeNode* head, FILE * fp);

extern FILE* code_generator_input(TreeNode* head, FILE * fp);

extern FILE* code_generator(TreeNode* head, FILE * fp);

extern void codeGenerator(TreeNode* head, char *outFile);

#endif