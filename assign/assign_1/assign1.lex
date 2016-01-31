	/*======================================================================
 Author: Chris Wozniak
 Course: CPSC411

 This program parses the M- instruction set, producing a list of tokens
 on each newline as it moves through the input code.

 =============================================================================*/
%option noyywrap

	/*======================================================================

 C boilerplate code for the lexer to properly parse operations and keywords
 as uppercase letters, as per assignment requirements.

 =============================================================================*/
%{
	#include <ctype.h>
	#include <stdio.h>
	
	#define FALSE 0
	#define TRUE 1
	
	void strUpper(char *buf, char *ptr);
%}
	/*======================================================================

 Basic regular definitions used in the M- language, used to help make
 the rules much more organized down the file.

 =============================================================================*/
operations	("+"|":="|"-"|"*"|"/")
reserved	(if|then|while|do|input|else|begin|end|write)
whitespace	(\n|" "|\t)*
digit		[0-9]
letter		[a-zA-Z]
identifier	{letter}({letter}|{digit})*

%%
	/*======================================================================

 Basic rules, outputting a format as requested by the assignment:
 each lexeme will output its type, followed by the value associated
 with it.

 =============================================================================*/
"("	{printf("LPAR\t(\n");}
")"	{printf("RPAR\t)\n");}
";"	{printf("TERM\t;\n");}
{reserved}	{
	char word[sizeof(yytext)];
	strUpper(&word[0], yytext);
	printf("KEY\t%s\n", yytext);
}
{whitespace}	;
[%](.)*		;
{operations}	{printf("OP\t%s\n", yytext);}
{identifier}	{printf("ID\t%s\n", yytext);}
{digit}+	{printf("NUM\t%d\n",atoi(yytext));}
"/*"		{
	char c;
	int done = FALSE;
	//Run until we find the end of the comment.
	do {
		// until we find a *, consume and continue.
		while ((c=input()) != '*'){
			input();
		}
		// Once we found a *, consume and check ahead.
		while((c=input()) == '*'){
			input();
		}
		// if the next character ends the comment, 
		// continue.
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
