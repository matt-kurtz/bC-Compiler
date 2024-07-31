%{
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "scanType.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *msg);

int numErrors;
int numWarnings;
extern int line;
extern int yylex();

%}
%union {
	TokenData* tokenData;
}
%token <tokenData> INT ID NUMCONST IF ELSE THEN TO DO FOR RETURN ERROR PRECOMPILER
%token <tokenData> BOOLCONST STATIC OR BOOL BREAK BY CHAR AND CHARCONST COMMENT NOT WHILE
%token <tokenData> EQ GEQ LEQ DEC DIVASS SUBASS ADDASS INC MULASS NEQ MAX MIN STRINGCONST
%token <tokenData> '*' '+' '{' '}' '[' ']' ';' '-' '>' '<' '=' ':' ',' '/' '(' ')' '%' '?'
%%
program  :  precomList declList {}
	;

precomList   : precomList PRECOMPILER                      {}
             | PRECOMPILER                                 {}
             | /* empty */                                 {}
             ;

declList : declList decl {}
	| decl {}
	;

decl : varDecl {}
	| funDecl {}
	;

varDecl : typeSpec varDeclList ';' {}
;

scopedVarDecl : STATIC typeSpec varDeclList ';' {}
	| typeSpec varDeclList ';' {}
	;

varDeclList : varDeclList ',' varDeclInit {}
	| varDeclInit {}
	;

varDeclInit : varDeclId {}
	| varDeclId ':' simpleExp {}
	;

varDeclId : ID {}
	| ID '[' NUMCONST ']' {}
	;

typeSpec : INT {}
	| BOOL {}
	| CHAR {}
	;

funDecl : typeSpec ID '(' parms ')' stmt {}
	| ID '(' parms ')' stmt {}
	;

parms : parmList {}
	| /*empty*/ {}
	;

parmList : parmList ';' parmTypeList {}
	| parmTypeList {}
	;

parmTypeList : typeSpec parmIdList {}
	;

parmIdList : parmIdList ',' parmId {}
	| parmId {}
	;

parmId : ID {}
	| ID '[' ']' {}
	;

stmt : matched {}
	| unmatched {}
	;

matched : IF simpleExp THEN matched ELSE matched {}
	| WHILE simpleExp DO matched {}
	| FOR ID '=' iterRange DO matched {}
	| expstmt {}
	| compoundstmt {}
	| returnstmt {}
	| breakstmt {}
	;

iterRange : simpleExp TO simpleExp {}
	| simpleExp TO simpleExp BY simpleExp {}
	;

unmatched : IF simpleExp THEN stmt {}
	| IF simpleExp THEN matched ELSE unmatched {}
	| WHILE simpleExp DO unmatched {}
	| FOR ID '=' iterRange DO unmatched {}
	;

expstmt : exp ';' {}
	| ';' {}
	;

compoundstmt : '{' localDecls stmtList '}' {}
	;

localDecls : localDecls scopedVarDecl {}
	| /* empty */ {}
	;

stmtList : stmtList stmt {}
	| /* empty */ {}
	;

returnstmt : RETURN ';' {}
	| RETURN exp ';' {}
	;

breakstmt : BREAK ';' {}
	;

exp : mutable assignop exp {}
	| mutable INC {}
	| mutable DEC {}
	| simpleExp {}
	;

assignop : '=' {}
	| ADDASS {}
	| SUBASS {}
	| MULASS {}
	| DIVASS {}
	;

simpleExp : simpleExp OR andExp {}
	| andExp {}
	;

andExp : andExp AND unaryRelExp {}
	| unaryRelExp {}
	;

unaryRelExp : NOT unaryRelExp {}
	| relExp {}
	;

relExp : minmaxExp relop minmaxExp {}
	| minmaxExp {}
	;

relop : LEQ {}
	| '>' {}
	| '<' {}
	| GEQ {}
	| EQ {}
	| NEQ {}
	;

minmaxExp : minmaxExp minmaxop sumExp {}
	| sumExp {}
	;

minmaxop : MAX {}
	| MIN {}
	;

sumExp : sumExp sumop mulExp {}
	| mulExp {}
	;

sumop : '+' {}
	| '-' {}
	;

mulExp : mulExp mulop unaryExp {}
	| unaryExp {}
	;

mulop : '*' {}
	| '?' {}
	| '%' {}
	| '/' {}
	;

unaryExp : unaryop unaryExp {}
	| factor {}
	;

unaryop : '-' {}
	| '*' {}
	| '?' {}
	;

factor : immutable {}
	| mutable {}
	;

mutable : ID {}
	| ID '[' exp ']' {}
	;

immutable : '(' exp ')' {}
	| call {}
	| constant {}
	;

call : ID '(' args ')' {}
	;

args : argList {}
	| /* empty */ {}
	;

argList : argList ',' exp {}
	| exp {}
	;

constant : NUMCONST {}
	| CHARCONST {}
	| STRINGCONST {}
	| BOOLCONST {}
	;

%%
void yyerror (const char *msg)
{ 
	cout << "Error: " <<  msg << endl;
}

int main(int argc, char **argv) {
	yylval.tokenData = (TokenData*)malloc(sizeof(TokenData)); ///////////////
//	yylval.tree = (TreeNode*)malloc(sizeof(TreeNode)); ///////////////
//	yylval.tokenData->linenum = 1; //////////////////// tinfo. must change to tokenData-> in parser.l as well
	int index;
	char *file = NULL;
	bool dotAST = false;             // make dot file of AST
	extern FILE *yyin;


	if ( optind == argc ) yyparse();


	for (index = optind; index < argc; index++) {
		yyin = fopen (argv[index], "r");
		yyparse();
		fclose (yyin);
	}
	printf("Done\n"); //////////////////
	return 0;
}

