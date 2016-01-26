%option noyywrap
%{
	/*
		Lex program adds line numbers to the file before printing
		output.
	*/
	#include <stdio.h>
	int line_no = 1;
%}

line	.*\n


%%



{line}	{printf("%5d %s", line_no++, yytext);}



%%

	int main(int argc, char *argv[]){
		yylex();
	}
