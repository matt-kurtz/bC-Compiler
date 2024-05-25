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
   //printf("t is: %s", t->attr.string);
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
%token   <tinfo> FIRSTOP
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
%token   <tinfo> SIZEOF CHSIGN
%token   <tinfo> LASTOP
%type <tinfo> relop minmaxop sumop mulop unaryop assignop
%type <tree>  program
%type <tree> precomList declList decl localDecls stmtList funDecl varDecl compoundStmt stmt matched unmatched expStmt returnStmt breakStmt exp simpleExp andExp unaryRelExp relExp parms parmList parmTypeList varDeclList varDeclInit varDeclId minmaxExp sumExp constant scopedVarDecl mulExp unaryExp
%type <tree> mutable immutable factor call parmIdList parmId iterRange argList args

%type <type> typeSpec


%token <tinfo> LASTTERM
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


scopedVarDecl : STATIC typeSpec varDeclList ';'     {/* DRBC Note: Be careful! And setType.*/ $$ = $3; setType($3, $2, 1);}
                | typeSpec varDeclList ';'             {/* DRBC Note: Be careful! And setType.*/ $$ = $2; setType($2, $1, 0); /* IF SEGFAULTS, DELETE THIS */}
              ;


varDeclList  : varDeclList ',' varDeclInit          {/* addSibling */ $$ = addSibling($1, $3); /*Changed this */}
               | varDeclInit                        { $$ = $1; }
             ;

varDeclInit  : varDeclId				{ $$ = $1; }
               | varDeclId ':' simpleExp               { $$ = $1; if ($$ != NULL) $$->child[0] = $3; }
             ;

varDeclId   : ID                                         {/* newDeclNode */ $$ = newDeclNode(VarK, UndefinedType, $1); /* IF SEGFAULT, DELETE THIS */}
             | ID '[' NUMCONST ']'                         {  $$ = newDeclNode(VarK, UndefinedType, $1); $$->isArray = true; $$->size = $3->nvalue + 1; }
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

parmList   : parmList ';' parmTypeList              {/* addSibling */ $$ = addSibling($1, $3); }
             | parmTypeList				{$$ = $1; }
           ;

parmTypeList : typeSpec parmIdList                  {/* DRBC Note: Be careful! And setType.*/ $$ = $2; setType($2, $1, 0);}
             ;

parmIdList   : parmIdList ',' parmId                 {/* addSibling*/ $$ = addSibling($1, $3); }
             | parmId					{$$ = $1;}
             ;

parmId     : ID                                          {/* newDeclNode*/ $$ = newDeclNode(ParamK, UndefinedType, $1);}
             | ID '[' ']'                                   {/* newDeclNode*/ $$ = newDeclNode(ParamK, UndefinedType, $1); $$->isArray = true; }
           ;

stmt       : matched						{$$ = $1;}
             | unmatched					{$$ = $1;}
           ;


matched    : IF simpleExp THEN matched ELSE matched   {/* newStmtNode*/ $$ = newStmtNode(IfK, $1, $2, $4, $6); }
             | WHILE simpleExp DO matched                  {/* newStmtNode*/ $$ = newStmtNode(WhileK, $1, $2, $4); }
             | FOR ID '=' iterRange DO matched             {/* newStmtNode(newDeclNode)*/ TreeNode *tmp = newDeclNode(VarK, Integer, $2); $$ = newStmtNode(ForK, $1, tmp, $4, $6); }
             | expStmt 						{$$ = $1;}
             | compoundStmt					{$$ = $1;}
             | returnStmt					{$$ = $1;}
             | breakStmt					{$$ = $1;}
           ;

iterRange  : simpleExp TO simpleExp                  {/* newStmtNode*/ $$ = newStmtNode(RangeK, $2, $1, $3); }
             | simpleExp TO simpleExp BY simpleExp    {/* newStmtNode*/ $$ = newStmtNode(RangeK, $2, $1, $3, $5);}
           ;

unmatched  : IF simpleExp THEN stmt                     {/* newStmtNode*/ $$ = newStmtNode(IfK, $1, $2, $4); }
             | IF simpleExp THEN matched ELSE unmatched  {/* newStmtNode*/ $$ = newStmtNode(IfK, $1, $2, $4, $6); }
             | WHILE simpleExp DO unmatched                {/* newStmtNode*/ $$ = newStmtNode(WhileK, $1, $2, $4); }
             | FOR ID '=' iterRange DO unmatched           {/* newStmtNode(newDeclNode)*/ TreeNode *tmp = newDeclNode(VarK, Integer, $2); $$ = newStmtNode(ForK, $1, tmp, $4, $6); }
           ;

expStmt    : exp ';' 						{$$ = $1;}
             | ';'                                        {$$ = NULL; }
           ;

compoundStmt : '{' localDecls stmtList '}'         {/* newStmtNode*/ $$ = newStmtNode(CompoundK, $1, $2, $3);}
             ;

localDecls : localDecls scopedVarDecl              {/* addSibling*/ if ($2 != NULL) {$$ = addSibling($1, $2);}}
             | /* empty */                               { $$ = NULL; printDebug("localDecls empty");}
             ;

stmtList   : stmtList stmt                         {/* addSibling*/ printDebug("stmtList stmt"); if ($2 != NULL) { $$ = addSibling($1, $2);} /*else printf("$2 is NULL");*/}
             | /* empty */                               { $$ = NULL; printDebug("stmtList  empty");}
           ;

returnStmt : RETURN ';'                                {/* newStmtNode*/ $$ = newStmtNode(ReturnK, $1); }
             | RETURN exp ';'                           {/* newStmtNode*/ $$ = newStmtNode(ReturnK, $1, $2); }
           ;

breakStmt  : BREAK ';'                                 {/* newStmtNode*/ $$ = newStmtNode(BreakK, $1); }
           ;

exp        : mutable assignop exp                {/* newExpNode*/  $$ = newExpNode(AssignK, $2, $1, $3); printDebug("In the exp rule");}
             | mutable INC                              {/* newExpNode*/ $$ = newExpNode(AssignK, $2, $1); }
             | mutable DEC                              {/* newExpNode*/ $$ = newExpNode(AssignK, $2, $1); }
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

relExp     : minmaxExp relop minmaxExp          {/* newExpNode*/ $$ = newExpNode(OpK, $2, $1, $3);  /* IF SEGFAULT, $$ = NULL; */}
             | minmaxExp			{ /* $$ = $1; */ $$ = $1;  /* IF SEGFAULT, $$ = NULL; */}
           ;

relop      : LEQ				{ $$ = $1; }
             | '<'				{ $$ = $1; }
             | '>'				{ $$ = $1; }
             | GEQ				{ $$ = $1; }
             | EQ				{ $$ = $1; }
             | NEQ				{ $$ = $1; }
           ;

minmaxExp  : minmaxExp minmaxop sumExp              {/* newExpNode*/  $$ = newExpNode(OpK, $2, $1, $3);  /* IF SEGFAULT, CLEAR THIS */}
             | sumExp 					{ $$ = $1; /* IF SEGFAULT, CLEAR THIS */}
           ;

minmaxop   : MAX				{ $$ = $1; }
             | MIN				{ $$ = $1; }
           ;

sumExp     : sumExp sumop mulExp              {/* newExpNode*/ $$ = newExpNode(OpK, $2, $1, $3); }
             | mulExp				{ $$ = $1; }
           ;

sumop      : '+'				{ $$ = $1; }
             | '-'				{ $$ = $1; }
           ;

mulExp     : mulExp mulop unaryExp           {/* newExpNode*/ $$ = newExpNode(OpK, $2, $1, $3); }
             | unaryExp				{ $$ = $1; }
           ;

mulop      : '*'				{ $$ = $1; }
             | '/'				{ $$ = $1; }
             | '%'				{ $$ = $1; }
           ;

unaryExp   : unaryop unaryExp                   {/* newExpNode*/ $$ = newExpNode(OpK, $1, $2); }
             | factor				{ $$ = $1; }
           ;

unaryop    : '-'                                     { /* newExpNode Is this wrong??? $$ = newExpNode(OpK, $1); */ $1->tokenclass = CHSIGN; $$ = $1; }
             | '*'                                      {/* newExpNode Is this wrong??? $$ = newExpNode(OpK, $1); */ $1->tokenclass = SIZEOF; $$ = $1; }
             | '?'                                      { /* newExpNode Is this wrong??? $$ = newExpNode(OpK, $1); */ $$ = $1; }
             
           ;

factor     : immutable					{ $$ = $1; }
             | mutable					{ $$ = $1; }
           ;

mutable    : ID                                       {/* newExpNode*/ $$ = newExpNode(IdK, $1);}
             | ID '[' exp ']'                          {/* newExpNode*/ TreeNode *tmp = newExpNode(IdK, $1); $$ = newExpNode(OpK, $2, tmp, $3); /* This is probably wrong */ }
           ;

immutable  : '(' exp ')'                            {/* DRBC Note: Be careful!*/ $$ = $2; }
             | call					{ $$ = $1; }
             | constant					{ $$ = $1; }
           ;

call       : ID '(' args ')'                        {/*newExpNode*/ $$ = newExpNode(CallK, $1, $3); }
           ;

args       : argList					{ $$ = $1; }
             | /* empty */                                { /* NULL;*/ $$ = NULL; }
           ;

argList    : argList ',' exp                       {/* addSibling*/ if ($3 != NULL) {$$ = addSibling($1, $3); }}
             | exp					{ $$ = $1; }
           ;

constant   : NUMCONST                                   {/* newExpNode*/ $$ = newExpNode(ConstantK, $1);}
             | CHARCONST                                   {/* newExpNode*/ $$ = newExpNode(ConstantK, $1);}
             | STRINGCONST                                 {/* newExpNode*/ $$ = newExpNode(ConstantK, $1);}
             | BOOLCONST                                   {/* newExpNode*/ $$ = newExpNode(ConstantK, $1);}
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

