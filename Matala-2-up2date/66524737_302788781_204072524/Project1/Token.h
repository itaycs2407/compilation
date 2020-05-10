#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	KEY_INT,
	KEY_FLOAT,
	KEY_VOID,
	KEY_IF,
	KEY_RETURN,
	INT_NUMBER,
	INT_NUMBER_Z,
	FLOAT_NUMBER,
	COMMA_SIGN,
	COLON_SIGN,
	SEMICOLON_SIGN,
	PARENTHESES_OPEN,
	PARENTHESES_CLOSE,
	BRACKETS_OPEN,
	BRACKETS_CLOSE,
	CURLY_BRACES_OPEN,
	CURLY_BRACES_CLOSE,
	ARGUMENT_OPR_PLUS,
	ARGUMENT_OPR_MULTIPLICATION,
	OP_LE,
	OP_L,
	OP_GE,
	OP_G,
	OP_E,
	OP_NE,
	EQUAL_OP,
	OTHER_ID,
	EOF_TOK
	/*
		Examples of tokens, add/change according to your needs.
		
		Alternative way to define tokens:
		#define TOKEN_INTEGER 1
		#define TOKEN_IF 2
		...........
	*/
} eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
} Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char *lexeme, int numOfLine);
Token *next_token();
Token *back_token();

#endif