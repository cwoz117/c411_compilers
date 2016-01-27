%option noyywrap
%{
	#include <ctype.h>
	#include <stdio.h>
	#include <stdlib.h>
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

"("	{printf("LPAR\t(\n");}
")"	{printf("RPAR\t)\n");}
";"	{printf("TERM\t;\n");}
{reserved} {	
	word = malloc(sizeof(yytext));
	strUpper(word, yytext);
	printf("KEY\t%s\n",word);
	free(word);
}
{operations}	{printf("OP\t%s\n", yytext);}
{identifier}	{printf("ID\t%s\n", yytext);}
{digit}+	{printf("NUM\t%d\n",atoi(yytext));}
{whitespace}	;
[%](.)* ;


"/*"	{
	nest_count++;

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
