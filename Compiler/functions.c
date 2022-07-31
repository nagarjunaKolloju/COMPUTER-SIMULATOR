#include<stdio.h>
#include <ctype.h>
#include<string.h>
#include<stdlib.h>
#include "symbolTable.c"
/*
int yylex();
int yylineno;*/

int g_addr = 100;

int i=1,lnum1=0;


int stack[100],index1=0,end[100],arr[10],ct,c,b,fl,  top=0, label[20],label_num=0,ltop=0;

char st1[100][10]; // st1 is a stack for pushing 

char temp_count[2]="0";

int plist[100],flist[100],k=-1,errc=0,j=0;

char temp[2]="t";

char null[2]=" ";

char arr_dec[50];

char t_t[2]="0", s_t[2]="0",a_t[2]="0", st_t[2]="0", _txt[2]="0";

char _s[8][20];
char _t[10][20];
char _ar[10][20];
char _st[10][30];


FILE *fptr;

FILE *fptr1;



void yyerror(char *s);
int printline();
extern int yylineno;
void scope_start()
{
	stack[index1]=i;
	i++;
	index1++;
	return;
}

void scope_end()
{
	index1--;
	end[stack[index1]]=1;
	stack[index1]=0;
	return;
}

void if1()
{
	label_num++;
	char a1[20];
	strcpy(a1,st1[top]);
	strcpy(temp,"t");
	strcat(temp,temp_count);
	//printf("\n%s = not %s\n",temp,st1[top]);
	
	//asm
	fprintf(fptr1,"\n\tli $a0, 1\n");
	fprintf(fptr1,"\n\txor $%s, $%s, $a0\n", temp, a1);
	//asm
	
 	//printf("if %s goto L%d\n",temp,label_num);
 	
 	
 	//asm
 	fprintf(fptr1,"\tbnez $%s, L%d\n",temp,label_num);
 	//asm
	//temp_count[0]++;
	label[++ltop]=label_num;

}


void if2()
{
	label_num++;
	//printf("\ngoto L%d\n",label_num);
	//asm
	fprintf(fptr1,"\n\tj L%d\n",label_num);
	//asm
	
	//printf("L%d: \n",label[ltop--]);
	
	//asm
	fprintf(fptr1,"L%d: \n",label[ltop--]);
	//asm
	label[++ltop]=label_num;
}
void if3()
{
	//printf("\nL%d:\n",label[ltop--]);
	//asm
	fprintf(fptr1,"\nL%d:\n",label[ltop--]);
	//asm
}
void while1()
{
	label_num++;
	label[++ltop]=label_num;
	//printf("\nL%d:\n",label_num);
	//asm
	fprintf(fptr1,"\nL%d:\n",label_num);
	//asm
}
void while2()
{
	label_num++;
	char a1[20];
	strcpy(a1,st1[top--]);
	strcpy(temp,"t");
	strcat(temp,temp_count);
	//printf("\n%s = not %s\n",temp,st1[top--]);
	
	//asm
	fprintf(fptr1,"\n\tli $a0, 1\n");
	fprintf(fptr1,"\n\txor $%s, $%s, $a0\n", temp, a1);
	//asm
	
 //	printf("if %s goto L%d\n",temp,label_num);
 	
 	//asm
 	fprintf(fptr1,"\tbnez $%s, L%d\n",temp,label_num);
 	//asm
 	
	//temp_count[0]++;
	label[++ltop]=label_num;
}
void while3()
{
	int y=label[ltop--];
	int y1=label[ltop--];
	//printf("\ngoto L%d\n",y1);
	
	//asm
	fprintf(fptr1,"\n\tj L%d\n",y1);
	//asm
	
	//printf("L%d:\n",y);
	
	//asm
	fprintf(fptr1,"L%d:\n",y);
	//asm
}

void for1()
{
	label_num++;
	label[++ltop]=label_num;
	//printf("\nL%d:\n",label_num);
	//asm
	fprintf(fptr1,"\nL%d:\n",label_num);
	//asm
	
}
void for2()
{
	label_num++;
	char a1[20];
	strcpy(a1,st1[top--]);
	strcpy(temp,"t");
	strcat(temp,temp_count);
	
	//printf("\n%s = not %s\n",temp,a1);
	
	//asm
	fprintf(fptr1,"\n\tli $a0, 1\n");
	fprintf(fptr1,"\n\txor $%s, $%s, $a0\n", temp, a1);
	//asm
	
 	//printf("if %s goto L%d\n",temp,label_num);
 	
 	//asm
 	fprintf(fptr1,"\tbnez $%s, L%d\n",temp,label_num);
 	//asm
 	
	//temp_count[0]++;
	label[++ltop]=label_num;
	label_num++;
	//printf("goto L%d\n",label_num);
	//asm
	fprintf(fptr1,"\tj L%d\n",label_num);	
	//asm
	label[++ltop]=label_num;
	label_num++;
	//printf("L%d:\n",label_num);
	//asm
	fprintf(fptr1,"\tL%d:\n",label_num);
	//asm
	label[++ltop]=label_num;
}
void for3()
{
	//printf("\ngoto L%d\n",label[ltop-3]);
	//asm
	fprintf(fptr1,"\n\tj L%d\n",label[ltop-3]);
	//asm
	
	//printf("L%d:\n",label[ltop-1]);
	//asm
	fprintf(fptr1,"\tL%d:\n",label[ltop-1]);
	//asm	
}

void for4()
{
	//printf("\ngoto L%d\n",label[ltop]);
	//asm
	fprintf(fptr1,"\n\tj L%d\n",label[ltop]);
	
	//asm
	
	
	//printf("L%d:\n",label[ltop-2]);
	//asm
	fprintf(fptr1,"\tL%d:\n",label[ltop-2]);
	//asm
	ltop-=4;
}
void push(char *a)
{
	strcpy(st1[++top],a);
}

void array1()
{
	strcpy(temp,"$a0");
	//strcat(temp,temp_count);
	
	//printf("\n%s = %s*4\n",temp,st1[top]);
	
	//asm
	int i;
	if(st1[top][0]>=48 && st1[top][0]<=57)
	{
	fprintf(fptr1,"\tli $a1, %s\n",st1[top]);
	fprintf(fptr1,"\tli $a2, 4\n");
	fprintf(fptr1,"\tmul $a0, $a2, $a1\n");
	}
	else
	{
		for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],st1[top])==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tli $a2, 4\n");
			fprintf(fptr1,"\tmul $a0, $s%s, $a2\n",a);
			
		}
	}
	}
	
	//asm
	
	strcpy(st1[top],temp);
	//temp_count[0]++;
	strcpy(temp,"t");
	strcat(temp,temp_count);
	//strcpy(temp,"a0");
	//printf("%s = %s [ %s ] \n",temp,st1[top-1],st1[top]);
	
	//asm
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],st1[top-1])==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tadd $a0, $s%s, $a0\n",a);
			fprintf(fptr1,"\tlw $%s, 0($a0)\n",temp);
		}
	}
	
	//asm
	
	top--;
	strcpy(st1[top],temp);
	//temp_count[0]++;
}


void array2(char* a, char* b)
{
	/*strcpy(temp,"t");
	strcat(temp,temp_count);
	printf("\n%s = %s*4\n",temp,st1[top]);
	strcpy(st1[top],temp);
	//temp_count[0]++;
	//strcpy(temp,"t");
	//strcat(temp,temp_count);
	printf("%s [ %s ] =%s\n",st1[top-1],st1[top],temp);
	top--;
	strcpy(st1[top],temp);
	temp_count[0]++;*/
	strcpy(temp,"$a0");
	int i;
	if(st1[top][0]>=48 && st1[top][0]<=57)
	{
	fprintf(fptr1,"\tli $a1, %s\n",st1[top]);
	fprintf(fptr1,"\tli $a2, 4\n");
	fprintf(fptr1,"\tmul $a0, $a2, $a1\n");
	}
	else
	{
		for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],st1[top])==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tli $a2, 4\n");
			fprintf(fptr1,"\tmul $a0, $s%s, $a2\n",a);
			
		}
	}
	}
	
	strcpy(st1[top],temp);
	//temp_count[0]++;
	//printf("%s = %s [ %s ] \n",temp,st1[top-1],st1[top]);
	
	//asm
	int j;
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],st1[top-1])==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tadd $a0, $s%s, $a0\n",a);
			if(b[0]>=48 && b[0]<=57)
			{
			fprintf(fptr1,"\tli $a1, %s\n",b);
			fprintf(fptr1,"\tsw $a1, 0($a0) \n",temp);
			}
			else
			{
				for(j=0;j<atoi(s_t);j++)
				{
				if(strcmp(_s[j],b)==0)
					{
						char a[20];
			sprintf(a,"%d",j);
			fprintf(fptr1,"\tsw $s%s, 0($a0)\n",a);
					}
				}
			
			}
			
			break;
		}
	}
	
	
	top--;
}

void codegen()
{
	strcpy(temp,"t");
	strcat(temp,temp_count);
	//printf("\n%s = %s %s %s\n",temp,st1[top-2],st1[top-1],st1[top]);
	//asm
	char temp2[10];
	char temp3[10];
	int i1=0;
	
	
	if(st1[top-2][0]=='t')
	{
	strcpy(temp2,"$");
	strcat(temp2,st1[top-2]);
	}
	else 
	{
		for(i=0;i<atoi(s_t);i++)
		{
			if(strcmp(_s[i],st1[top-2])==0)
			break;
		}
		if(i==atoi(s_t))
		{
		  i1++;
		  strcpy(temp2,st1[top-2]);
		}
		else
		{
		strcpy(temp2,"$s");
		char a[20];
		sprintf(a,"%d",i);
		strcat(temp2,a);
		}
	}
	if(st1[top][0]=='t' )
	{
	strcpy(temp3,"$");
	strcat(temp3,st1[top]);
	}
	else 
	{
		for(i=0;i<atoi(s_t);i++)
		{
			if(strcmp(_s[i],st1[top])==0)
			break;
		}
		if(i==atoi(s_t))
		{
		  i1++;
		  strcpy(temp3,st1[top]);
		}
		else
		{
		strcpy(temp3,"$s");
		char a[20];
		sprintf(a,"%d",i);
		strcat(temp3,a);
		}
	}
	if(i1==2)
	{
	   fprintf(fptr1,"\n\tli $a0, %s\n",temp2);
	   strcpy(temp2,"$a0");
	}
	fprintf(fptr1,"\n\t");
	if(strcmp(st1[top-1],"+")==0)
	fprintf(fptr1,"add");
	else if(strcmp(st1[top-1],"-")==0)
	fprintf(fptr1,"sub");
	else if(strcmp(st1[top-1],"*")==0)
	fprintf(fptr1,"mul");
	else if(strcmp(st1[top-1],"/")==0)
	fprintf(fptr1,"div");
	else if(strcmp(st1[top-1],"%")==0)
	fprintf(fptr1,"rem");
	else if(strcmp(st1[top-1],"<")==0)
	fprintf(fptr1,"slt");
	else if(strcmp(st1[top-1],"<=")==0)
	fprintf(fptr1,"sle");
	else if(strcmp(st1[top-1],"==")==0)
	fprintf(fptr1,"seq");
	else if(strcmp(st1[top-1],">")==0)
	fprintf(fptr1,"sgt");
	else if(strcmp(st1[top-1],">=")==0)
	fprintf(fptr1,"sge");
	else if(strcmp(st1[top-1],"!=")==0)
	fprintf(fptr1,"sne");
	
	
	
	if(i1==2 || i1==1)
	fprintf(fptr1,"i");
	fprintf(fptr1," ");
	fprintf(fptr1,"$%s, %s, %s\n",temp,temp2,temp3);
	//asm 
	
	top-=2;
	strcpy(st1[top],temp);
	temp_count[0]++;
}


void codegen_umin()
{
	strcpy(temp,"t");
	strcat(temp,temp_count);
	printf("\n%s = -%s\n",temp,st1[top]);
	top--;
	strcpy(st1[top],temp);
	temp_count[0]++;
}


void codegen_assign()
{
	//printf("\n%s = %s\n",st1[top-2],st1[top]);
	
	//asm
	int i;
	char temp[20];
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],st1[top-2])==0)
		break;
	}
	
	if(i==atoi(s_t))
	{
		strcpy(temp,"$s");
		strcat(temp,s_t);
		strcpy(_s[atoi(s_t)], st1[top-2]);
		s_t[0]++;
	}
	else
	{
		strcpy(temp,"$s");
		char a[20];
		sprintf(a,"%d",i);
		strcat(temp,a);
		
	}
	
	if(st1[top][0]=='t')
	fprintf(fptr1,"\n\t add %s, $zero,$%s\n",temp,st1[top]);
	else if(st1[top][0]>=48 && st1[top][0]<=57)
	fprintf(fptr1,"\n\t addi %s, $zero,%s\n",temp,st1[top]);
	else
	{
		for(i=0;i<atoi(s_t);i++)
		{
		if(strcmp(_s[i],st1[top])==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tadd %s, $zero, $s%s\n",temp,a);
			
		}
		}
	}
	
	
	//asm
	
	top-=2;
	
	
}

void codegen_assign1()
{
	strcpy(temp,"t");
	strcat(temp,temp_count);
	fprintf(fptr1,"\n%s = %s\n",temp,st1[top]);
		top-=2;
	temp_count[0]++;	
}

void  display(char *a)
{

	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],a)==0)
		{
			fprintf(fptr1,"\tli $v0,1\n");
			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tmove $a0,$s%s\n\tsyscall\n",a);
			
		}
	}

}

void display_str(char *a)
{

	fprintf(fptr,"\n_text%s:\n\t.asciiz %s\n",_txt,a);
	
	fprintf(fptr1,"\tli $v0, 4\n\tla $a0, _text%s\n\tsyscall\n",_txt);
	
	_txt[0]++;
}

void display_arr(char* a1, char* b)
{
	int i;
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],b)==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tli $a2, 4\n");
			fprintf(fptr1,"\tmul $a0, $s%s, $a2\n",a);
			
		}
	}
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],a1)==0)
		{

			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tadd $a0, $s%s, $a0\n",a);
			fprintf(fptr1,"\tlw $a1, 0($a0)\n");
		}
	}
	fprintf(fptr1,"\tli $v0,1\n");
			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tmove $a0,$a1\n\tsyscall\n");
	
}

void scan(char *a)
{
	for(i=0;i<atoi(s_t);i++)
	{
		if(strcmp(_s[i],a)==0)
		{
			fprintf(fptr1,"\tli $v0,5\n");
			char a[20];
			sprintf(a,"%d",i);
			fprintf(fptr1,"\tsyscall\n\tmove $s%s,$v0\n",a);
			
		}
	}
}

void array_decl(char* a, char* b)
{

	strcpy(_s[atoi(s_t)], a);
	
	fprintf(fptr1,"la $s%s, %s\n",s_t,_s[atoi(s_t)]);
	
	
	fprintf(fptr,"%s:\n\t.word ",a);
	for(int i=0;i<atoi(b)-1;i++)
	fprintf(fptr,"0, ");
	fprintf(fptr,"0");
	
	s_t[0]++;

}

void array_decl2(char* a, char* b,char* c)
{

	strcpy(_s[atoi(s_t)], a);
	
	fprintf(fptr1,"la $s%s, %s\n",s_t,_s[atoi(s_t)]);
	
	
	fprintf(fptr,"%s:\n\t.word ",a);
	int count=0;
	
	for(int i=0;i<strlen(c)-2;i++)
	{
		fprintf(fptr,"%c",c[i]);
		if(c[i]==',')
		count+=1;
	}
	count+=1;
	for(int j=1;j<=atoi(b)-count;j++)
	{
	
	   fprintf(fptr,", 0");
	}
	fprintf(fptr,"\n");
	s_t[0]++;

}






