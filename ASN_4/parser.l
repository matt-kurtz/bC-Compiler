%{
#include "scanType.h"
#include "treeNodes.h"
#include "treeUtils.h"
#include "parser.tab.h" // This has to be the last include

using namespace std;

#define YY_DECL extern "C" int yylex()
#define YYSTYPE int

int line=1;
char *lastToken=(char *)"";
//extern int numErrors;

int setValue(int linenum, int tokenClass, char *svalue) {
    yylval.tinfo = new TokenData;
    yylval.tinfo->tokenclass = tokenClass;
    yylval.tinfo->linenum = linenum;
    yylval.tinfo->tokenstr = strdup(svalue);
    yylval.tinfo->cvalue = svalue[0];
    yylval.tinfo->nvalue = atoi(svalue);
    yylval.tinfo->svalue = strdup(svalue);

    yylval.tinfo->tokenstr = lastToken = strdup(svalue);

    switch (tokenClass) {
    
    case STRINGCONST:
	 yylval.tinfo->nvalue = strlen(svalue) - 2;
	 break;

    case NUMCONST:
         //yylval.tinfo.nvalue = 
         break;
    case CHARCONST:
         if (svalue[1] == '\\') {
            switch (svalue[2]) {
               case 'n': yylval.tinfo->cvalue = '\n'; break;
	    }
	 }
         else {
	    yylval.tinfo->cvalue = lastToken[1];
	 }
	 break;
    //case OP:
         // yylval.tinfo.cvalue = hmmm..... What should I set it to?
         //break;
    case BOOLCONST:
	 yylval.tinfo->nvalue = ((lastToken[0] == 't' ? 1 : 0));
	 break;
    }


    return tokenClass;
}

%}

%option noyywrap

letter      [a-zA-Z]
digit    [0-9]
quote    [\']
underbar [_]
letdig      {digit}|{letter}|{underbar}
limitters   [\;\,\(\)\{\}\[\]]
op    [\-\+\*\/\%]
whitespace  [\ \t]
%%
      
[\(\)\>\<\{\}\=\+\-\*\%\;\:\,\/\?\[\]]         { return setValue(line, yytext[0], yytext); } // single char tokens

":<:"          { return setValue(line, MIN, yytext); }

":>:"          { return setValue(line, MAX, yytext); }

"--"           { return setValue(line, DEC, yytext); }

"++"           { return setValue(line, INC, yytext); }

"*="           { return setValue(line, MULASS, yytext); }

"/="           { return setValue(line, DIVASS, yytext); }

"+="           { return setValue(line, ADDASS, yytext); }

"-="           { return setValue(line, SUBASS, yytext); }

"break"        { return setValue(line, BREAK , yytext); }

"by"           { return setValue(line, BY, yytext); }

"do"           { return setValue(line, DO, yytext); }

(true|false)  { return setValue(line, BOOLCONST, yytext); }

"for"          { return setValue(line, FOR, yytext); }

"not"          { return setValue(line, NOT, yytext); }

"or"           { return setValue(line, OR, yytext); }

"to"           { return setValue(line, TO, yytext); }

"while"        { return setValue(line, WHILE, yytext); }

"static"       { return setValue(line, STATIC, yytext); }

"return"       { return setValue(line, RETURN, yytext); }

"=="           { return setValue(line, EQ, yytext); }

"!="           { return setValue(line, NEQ, yytext); }

"<="           { return setValue(line, LEQ, yytext); }

">="           { return setValue(line, GEQ, yytext); }

"and"          { return setValue(line, AND, yytext); }

"if"          { return setValue(line, IF, yytext); }

"then"          { return setValue(line, THEN, yytext); }

"else"         { return setValue(line, ELSE, yytext); }

^"#DRBCRUN ".* { return setValue(line, PRECOMPILER, yytext); }

^"#DRBC ".*    { return setValue(line, PRECOMPILER, yytext); }

"int"         { return setValue(line, INT, yytext); }

"char"        { return setValue(line, CHAR, yytext); }

"bool"        { return setValue(line, BOOL, yytext); }

"//".*        ;


[']\\?.[']     { return setValue(line, CHARCONST, yytext); }

["](\\[^\n]|[^\"\\\n])*["] { return setValue(line, STRINGCONST, yytext); }

{letter}{letdig}* { return setValue(line, ID, yytext); } 

[0-9]+         { return setValue(line, NUMCONST, yytext); }
\n             {  line++; }
"//".*\n       {  line++; }
{whitespace}+  ;
.              { printf("ERROR(%d): Token error, invalid or misplaced input character: \'%c\'. Character Ignored.\n", line, yytext[0]);}


