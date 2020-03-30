#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout;

typedef enum eTOKENS
{
	COMMA,
	COLON,
	SEMICOLON,
	PARENTHESES_OPEN,
	PARENTHESES_CLOSE,
	BRACKETS_OPEN,
	BRACKETS_CLOSE,
	CURLY_BRACES_OPEN,
	CURLY_BRACES_CLOSE,
	KEYWORD_INT,
	KEYWORD_FLOAT,
	KEYWORD_VOID,
	KEYWORD_IF,
	KEYWORD_RETURN,
	INT_NUM,
	FLOAT_NUM,
	AR_OP_PLUS,
	AR_OP_MULTIPLICATION,
	REL_OP_LE,
	REL_OP_L,
	REL_OP_GE,
	REL_OP_G,
	REL_OP_E,
	REL_OP_NE,
	ASSIGNMENT_OP,
	ID
	/*
		Examples of tokens, add/change according to your needs.
		
		Alternative way to define tokens:
		#define TOKEN_INTEGER 1
		#define TOKEN_IF 2
		...........
	*/
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();

#endif