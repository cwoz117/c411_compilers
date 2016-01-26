%option noyywrap
%{
	#include <ctype.h>
	#include <stdio.h>
	#include <stdlib.h>
	void strUpper(char *buf, char *ptr);
	char *word;
%}

reserved	(if|then|while|do|input|else|begin|end|write)
whitespace	(\n|" "|\t)*
digit		[0-9]
letter		[a-zA-Z]
identifier	{letter}({letter}|{digit})*
number		()
%%
{reserved}   {	
		word = malloc(sizeof(yytext));
		strUpper(word, yytext);
		printf("Keyword %s\n",word);
	   	free(word);
	     }
{identifier} {
	    	printf("%s\n", yytext);
      	     }


[%](.)*	     ;
{whitespace} ;


%%
	void strUpper(char *buf, char *ptr){
		for(int i = 0; i < sizeof(yytext); i++){
			buf[i]= toupper(ptr[i]);
		}
	}
	int main(){
		yylex();
	}
