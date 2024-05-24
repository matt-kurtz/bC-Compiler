%{
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "scanType.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *msg);

void printToken(TokenData myData, string tokenName, int type = 0) {
   cout << "Line: " << myData.linenum << " Type: " << tokenName;
   if(type==0)
     cout << " Token: " << myData.tokenstr;
   if(type==1)
     cout << " Token: " << myData.nvalue;
   if(type==2)
     cout << " Token: " << myData.cvalue;
   cout << endl;
}

%}
%union
{
   struct   TokenData tinfo ;
}
%token   <tinfo>  OP
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
%type <tinfo>  term program
%%
program  :  program term
   |  term  {$$=$1;}
   ;
term  : 
      OP {printToken(yylval.tinfo, "OP");}
   |  EQ {printToken(yylval.tinfo, "EQ");}
   |  GEQ {printToken(yylval.tinfo, "GEQ");}
   |  LEQ {printToken(yylval.tinfo, "LEQ");}
   |  NEQ {printToken(yylval.tinfo, "NEQ");}
   |  AND {printToken(yylval.tinfo, "AND");}
   |  IF {printToken(yylval.tinfo, "IF");}
   |  THEN {printToken(yylval.tinfo, "THEN");}
   |  ELSE {printToken(yylval.tinfo, "ELSE");}
   |  PRECOMPILER {printToken(yylval.tinfo, "PRECOMPILER");}
   |  NUMCONST {printToken(yylval.tinfo, "NUMCONST");}
   |  CHARCONST {printToken(yylval.tinfo, "CHARCONST");}
   |  STRINGCONST {printToken(yylval.tinfo, "STRINGCONST");}
   |  ID {printToken(yylval.tinfo, "ID");}
   |  INT {printToken(yylval.tinfo, "INT");}
   |  CHAR {printToken(yylval.tinfo, "CHAR");}
   |  BOOL {printToken(yylval.tinfo, "BOOL");}
   |  DEC {printToken(yylval.tinfo, "DEC");}
   |  INC {printToken(yylval.tinfo, "INC");}
   |  BREAK {printToken(yylval.tinfo, "BREAK");}
   |  BY {printToken(yylval.tinfo, "BY");}
   |  DO {printToken(yylval.tinfo, "DO");}
   |  BOOLCONST {printToken(yylval.tinfo, "BOOLCONST");}
   |  FOR {printToken(yylval.tinfo, "FOR");}
   |  NOT {printToken(yylval.tinfo, "NOT");}
   |  OR {printToken(yylval.tinfo, "OR");}
   |  RETURN {printToken(yylval.tinfo, "RETURN");}
   |  STATIC {printToken(yylval.tinfo, "STATIC");}
   |  WHILE {printToken(yylval.tinfo, "WHILE");}
   |  TO {printToken(yylval.tinfo, "TO");}
   |  MIN {printToken(yylval.tinfo, "MIN");}
   |  MAX {printToken(yylval.tinfo, "MAX");}
   |  ADDASS {printToken(yylval.tinfo, "ADDASS");}
   |  SUBASS {printToken(yylval.tinfo, "SUBASS");}
   |  MULASS {printToken(yylval.tinfo, "MULASS");}
   |  DIVASS {printToken(yylval.tinfo, "DIVASS");}
   |  ERROR    {cout << "ERROR(SCANNER Line " << yylval.tinfo.linenum << "): Invalid input character " << yylval.tinfo.tokenstr << endl; }
   ;
%%

void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

int main(int argc, char **argv) {
   yylval.tinfo.linenum = 1;
   int option, index;
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
   return 0;
}

