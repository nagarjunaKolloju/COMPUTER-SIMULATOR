%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int yylineno;
%}

alpha [A-Za-z]
digit [0-9]


%%
[ \t] ;
\n	{yylineno++;}
"{"	{scope_start(); return '{';}
"}"	{scope_end(); return '}';}
";"	     {   return(';'); }
","	     {   return(','); }
":"	     {   return(':'); }
"="	     {   return('='); }
"("	     {   return('('); }
")"	     {   return(')'); }
("[") 	     {   return('['); }
("]")        {   return(']'); }
"."	     {   return('.'); }
"&"	     {   return('&'); }
"!"	     {   return('!'); }
"~"	     {   return('~'); }
"-"	     {   return('-'); }
"+"	     {   return('+'); }
"*"	     {   return('*'); }
"/"	     {   return('/'); }
"%"	     {   return('%'); }
"<"	     {   return('<'); }
">"	     {   return('>'); }
"^"	     {   return('^'); }
"|"	     {   return('|'); }
"?"	     {   return('?'); }

varF	{yylval.ival = 259; return FLOAT;}

varZ	{yylval.ival = 258; return INT;}


blank	{yylval.ival = 260; return VOID;}


else	{ return ELSE;}
do	return DO;
if	return IF;
struct 	return STRUCT;
^"#include ".+	return PREPROC;
while	return WHILE;
for	return FOR;
return 	return RETURN;
display	return PRINT;
scanZ	return SCAN;
main	return MAIN;
{alpha}({alpha}|{digit})* {yylval.str=strdup(yytext); return ID;}
{digit}+	{yylval.str=strdup(yytext);return NUM;}
{digit}+\.{digit}+ 	{yylval.str=strdup(yytext); return REAL;}
"<="		return LE;
">="		return GE;
"=="		return EQ;
"!="		return NEQ;
"&&"		return AND;
"||"		return OR;

\/\/.* ;
\/\*(.*\n)*.*\*\/ ;

\".*\"	{yylval.str=strdup(yytext);return STRING;}
.	return yytext[0];
%%
