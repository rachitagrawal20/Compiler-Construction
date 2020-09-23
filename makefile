compiler: driver.c parser.c lexer.c hashTable.c helper.c ast.c symbolTable.c typeChecker.c semanticanalyzer.c codegen.c
	gcc -o compiler driver.c parser.c lexer.c hashTable.c helper.c ast.c symbolTable.c typeChecker.c semanticanalyzer.c codegen.c
