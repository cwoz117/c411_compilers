%option noyywrap
%{
	#include <ctype.h>
	#include <stdio.h>
	
	#define FALSE 0
	#define TRUE 1
	
	void strUpper(char *buf, char *ptr);
%}

operations	("+"|":="|"-"|"*"|"/")
reserved	(if|then|while|do|input|else|begin|end|write)
whitespace	(\n|" "|\t)*
digit		[0-9]
letter		[a-zA-Z]
identifier	{letter}({letter}|{digit})*

%%
"("	{printf("LPAR\t(\n");}
")"	{printf("RPAR\t)\n");}
";"	{printf("TERM\t;\n");}
{reserved}	{
	char word[sizeof(yytext)];
	strUpper(&word[0], yytext);
	printf("KEY\t%s\n", yytext);
}
{whitespace}	;
{operations}	{printf("OP\t%s\n", yytext);}
{identifier}	{printf("ID\t%s\n", yytext);}

{digit}+	{printf("NUM\t%d\n",atoi(yytext));}


[%](.)* 	;

"/*"		{
	char c;
	int done = FALSE;
	do {
		while ((c=input()) != '*'){
			input();
		}
		while((c=input()) == '*'){
			input();
		}
		if (c == '/')
			done = TRUE;
	} while (!done);
}

%%
	void strUpper(char *buf, char *ptr){
		for(int i = 0; i < sizeof(yytext); i++){
			buf[i]= toupper(ptr[i]);
		}
	}
	int main(){
		yylex();
	}
