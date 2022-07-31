%{
#include<stdio.h>
#include <ctype.h>
#include<string.h>
#include<stdlib.h>
#include "functions.c"

int yylex();
int yylineno;


%}

%token<ival> INT FLOAT VOID
%token<str> ID NUM REAL STRING
%token WHILE IF RETURN PREPROC LE  PRINT FUNCTION DO ARRAY ELSE STRUCT STRUCT_VAR FOR GE EQ NE INC DEC AND OR MAIN SCAN
%left LE GE EQ NEQ AND OR '<' '>'
%right '='
%right UMINUS
%left '+' '-'
%left '*' '/'
%type<str> assignment assignment1 consttype '=' '+' '-' '*' '/' E T F
%type<ival> Type
%union {
		int ival;
		char *str;
	}
%%



start : Function start
	| PREPROC start
	| Declaration start
	|
	;

Function : Type MAIN '('')'  CompoundStmt {}
	;

Type : INT
	| FLOAT
	| VOID
	;

CompoundStmt : '{' StmtList '}'
	;

StmtList : StmtList stmt
	|
	;

stmt : Declaration
	| if
	| ID '(' ')' ';'
	| while
	| for
	| ';'
	| PRINT '(' STRING ')' ';' {display_str($3);}
	| PRINT '(' ID '[' ID']'')' ';' {display_arr($3,$5);}
	| PRINT '('ID')'';' {display($3);} 
	| CompoundStmt
	| SCAN '('ID')' ';' {scan($3);}
	;



for	: FOR '(' E {for1();} ';' E {for2();}';' E {for3();} ')' CompoundStmt {for4();}
	;

if : 	 IF '(' E ')' {if1();} CompoundStmt {if2();} else
	;

else : ELSE CompoundStmt {if3();}
	|
	;

while : WHILE {while1();}'(' E ')' {while2();} CompoundStmt {while3();}
	;

assignment : ID '=' consttype
	| ID '+' assignment
	| ID ',' assignment
	| consttype ',' assignment
	| ID
	| consttype
	;

assignment1 : array '='  ID {array2($3,$3);
				
			}
		| array '='  consttype {array2($3,$3);
				
			}
		
		| ID {push($1);} '=' {strcpy(st1[++top],"=");} E {codegen_assign();}
	{
		int sct=returnscope($1,stack[index1-1]);
		int type=returntype($1,sct);
		//printf("%d\n",type);
		//if((!(strspn($5,"0123456789")==strlen($5))) && type==258 && fl==0  )
			//printf("\nError : Type Mismatch : Line %d\n",printline());
		if(!lookup($1))
		{
			int currscope=stack[index1-1];
			int scope=returnscope($1,currscope);
			if((scope<=currscope && end[scope]==0) && !(scope==0))
			{
				check_scope_update($1,$5,currscope);
			}
		}
		}

	| ID ',' assignment1    {
					if(lookup($1))
						printf("\nUndeclared Variable %s : Line %d\n",$1,printline());
				}
	| consttype ',' assignment1
	| ID  {
		if(lookup($1))
			printf("\nUndeclared Variable %s : Line %d\n",$1,printline());
		}
	| consttype
	;

consttype : NUM
	| REAL
	;

Declaration : Type ID {push($2);} '=' {strcpy(st1[++top],"=");} E {codegen_assign();} ';'
	{
		//printf("%s\n",$6);
		if( (!(strspn($6,"0123456789")==strlen($6))) && $1==258 && (fl==0))
		{
			printf("\nError : Type Mismatch : Line %d\n",printline());
			fl=1;
		}
		if(!lookup($2))
		{
			int currscope=stack[index1-1];
			int previous_scope=returnscope($2,currscope);
			if(currscope==previous_scope)
				printf("\nError : Redeclaration of %s : Line %d\n",$2,printline());
			else
			{
				insert_dup($2,$1,currscope);
				check_scope_update($2,$6,stack[index1-1]);
				int sg=returnscope($2,stack[index1-1]);
				g_addr+=4;
			}
		}
		else
		{
			int scope=stack[index1-1];
			insert($2,$1);
			insertscope($2,scope);
			check_scope_update($2,$6,stack[index1-1]);
			g_addr+=4;
		}
	}
	
	|Type ID {push($2);strcpy(st1[++top],"="); push("0");codegen_assign();} ';'{
		
		char a1[10];
		if($1==258)
		strcpy(a1,"0");
		else if($2==259)
		strcpy(a1,"0.0");
		// printf("%s\n",a1);
		if( (!(strspn(a1,"0123456789")==strlen(a1))) && $1==258 && (fl==0))
		{
			printf("\nError : Type Mismatch : Line %d\n",printline());
			fl=1;
		}
		if(!lookup($2))
		{
			int currscope=stack[index1-1];
			int previous_scope=returnscope($2,currscope);
			if(currscope==previous_scope)
				printf("\nError : Redeclaration of %s : Line %d\n",$2,printline());
			else
			{
				insert_dup($2,$1,currscope);
				check_scope_update($2,a1,stack[index1-1]);
				int sg=returnscope($2,stack[index1-1]);
				g_addr+=4;
			}
		}
		else
		{
			int scope=stack[index1-1];
			insert($2,$1);
			insertscope($2,scope);
			check_scope_update($2,a1,stack[index1-1]);
			g_addr+=4;
		}
	}

	| assignment1 ';'


	| Type ID '[' assignment ']' ';' {
			
			array_decl($2,$4);
		
		}
		
	| Type ID '[' assignment ']' '=' '{'{strcpy(arr_dec,"");} dec'}' ';'{
	int itype;
			if(!(strspn($4,"0123456789")==strlen($4))) { itype=259; } else itype = 258;
			if(itype!=258)
			{ printf("\nError : Array index must be of type int : Line %d\n",printline());errc++;}
			if(atoi($4)<=0)
			{ printf("\nError : Array index must be of type int > 0 : Line %d\n",printline());errc++;}
			//printf("Array Declaration recognized, %s\n",$4);
			char a1[50];
			strcpy(a1,arr_dec);
			array_decl2($2,$4,a1);
			if(!lookup($2))
			{
				int currscope=stack[index1-1];
				int previous_scope=returnscope($2,currscope);
				if(currscope==previous_scope)
				{printf("\nError : Redeclaration of %s : Line %d\n",$2,printline());errc++;}
				else
				{
					insert_dup($2,ARRAY,currscope);
				insert_by_scope($2,$1,currscope);	//to insert type to the correct identifier in case of 													multiple entries of the identifier by using scope
					check_scope_update($2,a1,stack[index1-1]);
					
					if (itype==258) 
					{
					insert_index($2,$4);
					}
				}
			}
			else
			{
				int scope=stack[index1-1];
				
				insert($2,$1);
				insert($2,269);
				insertscope($2,scope);
				check_scope_update($2,a1,stack[index1-1]);
				if (itype==258) {insert_index($2,$4);}
			}
			
	}

	| error
	;
dec:     dec ',' consttype{strcat(arr_dec,$3);strcat(arr_dec,", ");}
	   | consttype{strcat(arr_dec,$1);strcat(arr_dec,", ");}
	   | 
	   ;
	   
array : ID {push($1);}'[' E ']'
	;

E : E '+'{strcpy(st1[++top],"+");} T{codegen();}
   | E '-'{strcpy(st1[++top],"-");} T{codegen();}
   | T
   | ID {push($1);} LE {strcpy(st1[++top],"<=");} E {codegen();}
   | ID {push($1);} GE {strcpy(st1[++top],">=");} E {codegen();}
   | ID {push($1);} EQ {strcpy(st1[++top],"==");} E {codegen();}
   | ID {push($1);} NEQ {strcpy(st1[++top],"!=");} E {codegen();}
   | ID {push($1);} AND {strcpy(st1[++top],"&&");} E {codegen();}
   | ID {push($1);} OR {strcpy(st1[++top],"||");} E {codegen();}
   | ID {push($1);} '<' {strcpy(st1[++top],"<");} E {codegen();}
   | ID {push($1);} '>' {strcpy(st1[++top],">");} E {codegen();}
   | ID {push($1);} '=' {strcpy(st1[++top],"=");} E {codegen_assign();}
   | array {array1();}
   ;
T : T '*'{strcpy(st1[++top],"*");} F{codegen();}
   | T '/'{strcpy(st1[++top],"/");} F{codegen();}
   | T '%'{strcpy(st1[++top],"%");} F{codegen();}
   | F
   ;
F : '(' E ')' {$$=$2;}
   | '-'{strcpy(st1[++top],"-");} F{codegen_umin();} %prec UMINUS
   | ID {push($1);fl=1;}
   | consttype {push($1);}
   ;

%%

#include "lex.yy.c"
#include<ctype.h>


int main(int argc, char *argv[])
{
	yyin =fopen(argv[1],"r");
	
	fptr=fopen("out.asm","w");
	fptr1=fopen("temp","w");
	
	fprintf(fptr,".data\n");
	
	yyparse();
	if(!yyparse())
	{
		//printf("Parsing done\n");
		
		//print();
		
		//asm
		fprintf(fptr1,"\tjr $ra\n");
		fprintf(fptr,"\n.text\n\n.globl main\nmain:\n");
		fclose(fptr1);
		fptr1=fopen("temp","r");
		char c;
		while((c=fgetc(fptr1))!=EOF)
		fputc(c,fptr);
		//asm
	}
	else
	{
		printf("Error\n");
	}
	fclose(fptr);
	fclose(yyin);
	int k11=system("spim run out.asm");
	return k11;
}

void yyerror(char *s)
{
	printf("\nLine %d : %s %s\n",yylineno,s,yytext);
}
int printline()
{
	return yylineno;
}
