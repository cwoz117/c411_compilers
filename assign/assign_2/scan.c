#include <stdio.h>
#include <stdlib.h>

#ifndef GLOBALS
#define GLOBALS
#include "scan.h"
#endif

#define WHITESPACE  ((c == ' ') || (c == '\t') || (c == '\n'))

static char *file = NULL;
static int ptr = 0;
static int init_ptr;

typedef enum {
	START, DONE, NUM, VAR, ASSIGN, BLOCK_COMMENT, INLINE_COMMENT,
	IF, THEN, BEGIN, DO, WHILE, 
}DFA_STATE;

static void set_token(char c){
	switch (c){
		case EOF:
			tok.type = ENDFILE;
			break;
		case '+':
			tok.type = ADD;
			break;
		case '-':
			tok.type = SUB;
			break;
		case '*':
			tok.type = MUL;
			break;
		case '/':
			tok.type = DIV;
			break;
		case '(':
			tok.type = LPAR;
			break;
		case ')':
			tok.type = RPAR;
			break;
		case ';':
			tok.type = SEMICOLON;
			break;
		default:
			tok.type = ERROR;
			break;
		}
	}
	return tok;
}

TOKEN getToken(){
	TOKEN tok;
	DFA_STATE dfa = START;
	while (dfa != DONE){
		char c = file[ptr];
		switch (dfa_state){
			case START:
				if (isDigit(c)){
					state = NUM;
					init_ptr = ptr;
				} else if (c == ':'){
					state = ASSIGN;
				} else if (WHITESPACE){
					// Do nothing
				} else if (c == '/'){
					if (file[ptr+1] == '*')
						state = BLOCK_COMMENT;
				} else if (c == '%'){
					state = INLINE_COMMENT
				} else if (c == 'B') {
					state = BEGIN;
				} else if (c == 'E') {
					if (file[ptr+1] == 'N')
						state = END;
					else if (file[ptr+1] == 'L')
						state = ELSE;
				} else if (c == 'i'){
					state = IF;
				} else if (c == 't'){
					state = THEN;
				} else if (c == 'w') {
					state = WHILE;
				} else if (c == 'd'){
					state = DO;
				} else if (isAlpha(c)){
					state = VAR;
					init_ptr = ptr;
				} else {
					state = DONE;
					set_token(c);
				}
				break;
			case NUM:
				if (!isDigit(c)){
					ptr -=1;
					state = DONE;
					tok.type = NUM;
					char buf[ptr - init_ptr];
					memcpy(buf, file, 
					       init_ptr, (ptr-init_ptr));
				}	tok.attribute.val = atoi(buf);
				break;
			case VAR:
				if (!isAlpha(c)){
					ptr -=1;
					state = DONE;
					tok.type = VAR;
					memcpy(tok.attribute.name, file, 
					       init_ptr, (ptr-init_ptr));
				}
				break;
			case ASSIGN:
				state = DONE;
				if (c == '='){
					tok.type = ASSIGN;
				} else {
					ptr -= 1;
					tok.type = ERROR;
				}
				break;
			case IF:
				state = DONE;
				if (c == 'f'){
					tok.type = IF;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case THEN:
				state = DONE;
				if (c == 'h'){
					tok.type = THEN;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case ELSE:
				state = DONE;
				if (c == 'l'){
					tok.type = ELSE;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case WHILE:
				state = DONE;
				if (c == 'h') {
					tok.type = WHILE;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case DO:
				state = DONE;
				if (c == 'o') {
					tok.type = DO;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case BEGIN:
				state = DONE;
				if (c == 'e'){
					tok.type = BEGIN;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case END:
				state = DONE;
				if (c == 'n'){
					tok.type = END;
				} else {
					tok.type = ERROR;
					ptr -= 1;
				}
				break;
			case BLOCK_COMMENT:
				if (c == '*')
					if (file[ptr+1] == '/')
						state = START;
				break;
			case INLINE_COMMENT:
				if (c == '\n')
					state = START;
				break;
			default:
				state = DONE:
				tok.type = ERROR;
				break;
		}
		ptr += 1;
	}
	return tok;
}

int load_source(char *name){
	FILE *fd = fopen(name, "r");
	if (fd != NULL){
		// Determine the buffer size requirement
		if (fseek(fd, 0L, SEEK_END) == 0){
			long buffer_size = ftell(fd);
			if (buffer_size = -1)
				return 0;
			file = malloc(sizeof(char) * (buffer_size+1));
			if (fseek(fd, 0L, SEEK_SET) != 0)
				return 0;
			
			// Load the file
			int count = fread(fd, sizeof(char), buffer_size, fp);
			if (count == 0)
				return 0;
			else
				source[count++] = '\0';
			fclose(fd);
		}
		
		return 1;
	} else {
		return 0;
	}
}

int delete(){
	free(file);
}