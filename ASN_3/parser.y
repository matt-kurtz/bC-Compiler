%{
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "treeUtils.h"
#include "treeNodes.h"
#include "scanType.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

int numErrors;
int numWarnings;
extern int line;


TreeNode *addSibling(TreeNode *t, TreeNode *s) {
   TreeNode *newNode;
   if (s == NULL) {
      printf("Stop! S can't be null");
      exit(1);
   }
   else if (t == NULL) {
      return s;
   }
   else {
      newNode = t;
      while (newNode->sibling != NULL) {
         newNode = newNode->sibling;
      }
      newNode->sibling = s;
      return t;
   }
}

void setType(TreeNode *t, ExpType type, bool isStatic) {
      while (t != NULL) {
         t->type = type;
         t->isStatic = isStatic;
         t = t->sibling;
      }
}


TreeNode *syntaxTree;

void yyerror(const char *msg);

void printDebug(const char *msg) {
   if (1==0) {
      printf("%s\n", msg);
   }
}

%}
%union
{
   TokenData *tinfo;
   TreeNode *tree;
   ExpType type;
}
%token   <tinfo>  '*' '+' '-' '/' '<' '=' '>' '%' '?' '(' ')' ',' ';' '[' '{' '}' ']' ':'
%token   <tinfo>  NEQ EQ LEQ GEQ
%token   <tinfo>  AND OR NOT
%token   <tinfo>  PRECOMPILER
%token   <tinfo>  NUMCONST CHARCONST STRINGCONST BOOLCONST
%token   <tinfo>  ERROR
%token   <tinfo> ID
%token   <tinfo> IF THEN ELSE
%token   <tinfo> INT CHAR BOOL
%token   <tinfo> DEC INC
%token   <tinfo> WHILE FOR TO BY DO
%token   <tinfo> RETURN
%token   <tinfo> BREAK
%token   <tinfo> STATIC
%token   <tinfo> MIN MAX
%token   <tinfo> MULASS DIVASS ADDASS SUBASS
%type <tree>  program
%type <tree> precomList declList decl localDecls stmtList funDecl varDecl compoundStmt stmt matched unmatched expStmt returnStmt breakStmt exp simpleExp andExp unaryRelExp relExp parms parmList parmTypeList varDeclList varDeclInit varDeclId minmaxExp sumExp constant scopedVarDecl
%type <tree> mutable

%type <tinfo> relop minmaxop sumop mulop unaryop assignop
%type <type> typeSpec
%%
program    : precomList declList                    {syntaxTree = $2; }
           ;

precomList : precomList PRECOMPILER                   {$$ = NULL; printf("%s\n", yylval.tinfo->tokenstr); }
             | PRECOMPILER                                 {$$ = NULL; printf("%s\n", yylval.tinfo->tokenstr); }
             | /* empty */                                 { $$ = NULL; }
           ;

declList   : declList decl                          {/* addSibling*/ $$ = addSibling($1, $2);}
             | decl					{ $$ = $1; }
           ;

decl       : varDecl					{ $$ = $1; }
             | funDecl					{ $$ = $1; }
           ;

varDecl    : typeSpec varDeclList ';'               {/* DRBC Note: Be careful! And setType.*/  $$ = $2; setType($2, $1, 0); yyerrok;}
           ;


scopedVarDecl : STATIC typeSpec varDeclList ';'     {/* DRBC Note: Be careful! And setType.*/}
                | typeSpec varDeclList ';'             {/* DRBC Note: Be careful! And setType.*/ $$ = $2; setType($2, $1, 0); /* IF SEGFAULTS, DELETE THIS */}
              ;


varDeclList  : varDeclList ',' varDeclInit          {/* addSibling */  $$ = addSibling($$, $3);}
               | varDeclInit                        { $$ = $1; }
             ;

varDeclInit  : varDeclId				{ $$ = $1; }
               | varDeclId ':' simpleExp               {/* DRBC Note: $$ = $1; if ($$ != NULL) $$->child[0] = $3; */ $$ = $1; if ($$ != NULL) $$->child[0] = $3; }
             ;

varDeclId   : ID                                         {/* newDeclNode */ $$ = newDeclNode(VarK, Void, $1); /* IF SEGFAULT, DELETE THIS */}
             | ID '[' NUMCONST ']'                         {/* newDeclNode  $$ = newDeclNode(VarK, Void, $1); $$->isArray = true; */ }
             ;

typeSpec   : INT                                        { $$ = Integer;}
             | BOOL                                        { $$ = Boolean;}
             | CHAR                                        {$$ = Char;}
           ;

funDecl    : typeSpec ID '(' parms ')' stmt       {/* newDeclNode */ $$ = newDeclNode(FuncK, $1, $2, $4, $6);}
             | ID '(' parms ')' stmt                   {/* newDeclNode */ $$ = newDeclNode(FuncK, Void, $1, $3, $5);}
           ;

parms      : parmList 					{ $$ = $1; }	
             | /* empty */                                { $$ = NULL;}
           ;

parmList   : parmList ';' parmTypeList              {/* addSibling */}
             | parmTypeList
           ;

parmTypeList : typeSpec parmIdList                  {/* DRBC Note: Be careful! And setType.*/}
             ;

parmIdList   : parmIdList ',' parmId                 {/* addSibling*/}
             | parmId
             ;

parmId     : ID                                          {/* newDeclNode*/}
             | ID '[' ']'                                   {/* newDeclNode*/}
           ;

stmt       : matched						{$$ = $1;}
             | unmatched					{$$ = $1;}
           ;


matched    : IF simpleExp THEN matched ELSE matched   {/* newStmtNode*/}
             | WHILE simpleExp DO matched                  {/* newStmtNode*/}
             | FOR ID '=' iterRange DO matched             {/* newStmtNode(newDeclNode)*/}
             | expStmt 						{$$ = $1;}
             | compoundStmt					{$$ = $1;}
             | returnStmt					{$$ = $1;}
             | breakStmt					{$$ = $1;}
           ;

iterRange  : simpleExp TO simpleExp                  {/* newStmtNode*/}
             | simpleExp TO simpleExp BY simpleExp    {/* newStmtNode*/}
           ;

unmatched  : IF simpleExp THEN stmt                     {/* newStmtNode*/}
             | IF simpleExp THEN matched ELSE unmatched  {/* newStmtNode*/}
             | WHILE simpleExp DO unmatched                {/* newStmtNode*/}
             | FOR ID '=' iterRange DO unmatched           {/* newStmtNode(newDeclNode)*/}
           ;

expStmt    : exp ';' 						{$$ = $1;}
             | ';'                                        {$$ = NULL; }
           ;

compoundStmt : '{' localDecls stmtList '}'         {/* newStmtNode*/ $$ = newStmtNode(CompoundK, $1, $2, $3);}
             ;

localDecls : localDecls scopedVarDecl              {/* addSibling*/ $$ = addSibling($1, $2);}
             | /* empty */                               { $$ = NULL; printDebug("localDecls empty");}
             ;

stmtList   : stmtList stmt                         {/* addSibling*/ $$ = addSibling($1, $2);}
             | /* empty */                               { $$ = NULL; printDebug("stmtList  empty");}
           ;

returnStmt : RETURN ';'                                {/* newStmtNode*/}
             | RETURN exp ';'                           {/* newStmtNode*/}
           ;

breakStmt  : BREAK ';'                                 {/* newStmtNode*/}
           ;

exp        : mutable assignop exp                {/* newExpNode*/  $$ = newExpNode(AssignK, $2, $1, $3); printDebug("In the exp rule");}
             | mutable INC                              {/* newExpNode*/}
             | mutable DEC                              {/* newExpNode*/}
             | simpleExp				{ $$ = $1; }
           ;

assignop  : '=' 					{ $$ = $1; printDebug("Top assignop");}
            | ADDASS					{ $$ = $1; }
            | SUBASS					{ $$ = $1; }
            | MULASS					{ $$ = $1; }
            | DIVASS					{ $$ = $1; }
          ;

simpleExp  : simpleExp OR andExp                 { $$ = newExpNode(OpK, $2, $1, $3);}
             | andExp				{ $$ = $1; }
           ;

andExp     : andExp AND unaryRelExp              {/* newExpNode*/ $$ = newExpNode(OpK, $2, $1, $3);}
             | unaryRelExp			{ $$ = $1;}
           ;

unaryRelExp : NOT unaryRelExp                      {$$ = newExpNode(OpK, $1, $2);}
              | relExp					{$$ = $1;}
            ;

relExp     : minmaxExp relop minmaxExp          {/* newExpNode*/ $$ = newExpNode(AssignK, $2, $1, $3);  /* IF SEGFAULT, $$ = NULL; */}
             | minmaxExp			{ /* $$ = $1; */ $$ = $1;  /* IF SEGFAULT, $$ = NULL; */}
           ;

relop      : LEQ
             | '<'
             | '>'
             | GEQ
             | EQ
             | NEQ
           ;

minmaxExp  : minmaxExp minmaxop sumExp              {/* newExpNode*/  $$ = newExpNode(AssignK, $2, $1, $3);  /* IF SEGFAULT, CLEAR THIS */}
             | sumExp 					{ $$ = $1; /* IF SEGFAULT, CLEAR THIS */}
           ;

minmaxop   : MAX
             | MIN
           ;

sumExp     : sumExp sumop mulExp              {/* newExpNode*/}
             | mulExp
           ;

sumop      : '+'
             | '-'
           ;

mulExp     : mulExp mulop unaryExp           {/* newExpNode*/}
             | unaryExp
           ;

mulop      : '*'
             | '/'
             | '%'
           ;

unaryExp   : unaryop unaryExp                   {/* newExpNode*/}
             | factor
           ;

unaryop    : '-'                                     {/* newExpNode*/}
             | '*'                                      {/* newExpNode*/}
             | '?'                                      {/* newExpNode*/}
             ;
           ;

factor     : immutable
             | mutable
           ;

mutable    : ID                                       {/* newExpNode*/ $$ = newExpNode(IdK, $1);}
             | ID '[' exp ']'                          {/* newExpNode*/}
           ;

immutable  : '(' exp ')'                            {/* DRBC Note: Be careful!*/}
             | call
             | constant
           ;

call       : ID '(' args ')'                        {/*newExpNode*/}
           ;

args       : argList
             | /* empty */                                { /* NULL;*/}
           ;

argList    : argList ',' exp                       {/* addSibling*/}
             | exp
           ;

constant   : NUMCONST                                   {/* newExpNode*/ $$ = newExpNode(ConstantK, $1);}
             | CHARCONST                                   {/* newExpNode*/}
             | STRINGCONST                                 {/* newExpNode*/}
             | BOOLCONST                                   {/* newExpNode*/}
           ;

%%

void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

int main(int argc, char **argv) {
   int option, index;
   numErrors = 0;
   char *file = NULL;
   extern FILE *yyin;
   while ((option = getopt (argc, argv, "")) != -1)
      switch (option)
      {
      default:
         ;
      }
   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++) 
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }
   if (numErrors == 0) {
      printTree(stdout, syntaxTree);
   }
   printf("Number of warnings: %d\n", numWarnings);
   printf("Number of errors: %d\n", numErrors);
   return 0;
}

