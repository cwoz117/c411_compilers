%{=============================================================================
								DEFINITIONS
  ===========================================================================%}
%option noyywrap
%{
	#include <ctype.h>
	#include <stdio.h>
	#include <stdlib.h>
	
	#define FALSE 1
	#define TRUE 0
	
	void strUpper(char *buf, char *ptr);
	char *word;
	int nest_count = 0;
%}

operations	("+"|":="|"-"|"*"|"/")
reserved	(if|then|while|do|input|else|begin|end|write)
whitespace	(\n|" "|\t)*
digit		[0-9]
letter		[a-zA-Z]
identifier	{letter}({letter}|{digit})*

%%
%{=============================================================================
									RULES
  ===========================================================================%}
"("	{printf("LPAR\t(\n");}
")"	{printf("RPAR\t)\n");}
";"	{printf("TERM\t;\n");}
{whitespace}	;
{operations}	{printf("OP\t%s\n", yytext);}
{identifier}	{printf("ID\t%s\n", yytext);}
{digit}+	{printf("NUM\t%d\n",atoi(yytext));}
{reserved} {	
	word = malloc(sizeof(yytext));
	strUpper(word, yytext);
	printf("KEY\t%s\n",word);
	free(word);
}
[%](.)* ;
"/*"	{
	char c;
	int done = FALSE;
	ECHO;
	do {
		while ((c=input()) != '*')
			putchar(c);
		putchar(c);
		while((c=input()) == '*')
			putchar(c);
		putchar(c);
		if (c == '/')
			done = TRUE;
	} while (!done);
}

%%
%{=============================================================================
							AUXILIARY ROUTINES
  ===========================================================================%}
	void strUpper(char *buf, char *ptr){
		for(int i = 0; i < sizeof(yytext); i++){
			buf[i]= toupper(ptr[i]);
		}
	}
	int main(){
		yylex();
	}
