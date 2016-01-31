#===============================================================================
# Lex makefile
# Author: Chris Wozniak
#
# Basic makefile for the lexer for assignment 1 of CPSC411
#
#===============================================================================
objects = lex.yy.c

parser: $(objects)
	gcc $(objects) -o parser

$(objects): assign1.lex
	flex assign1.lex

clean:
	rm parser $(objects)
