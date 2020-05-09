#define _CRT_SECURE_NO_WARNINGS
#include "Parsers.h"
#include "Token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

Token* current_token;
Token* Another_Token;

int currentIndex;
Node* currentNode;


void match(eTOKENS token, FILE* yyout)
{

	if (token == currentNode->tokensArray[currentIndex].kind) {
		current_token = next_token();
	}
	else if (token == EOF_TOK)
	{
		//do nothing;
	}
	else
	{
		char* currentTokenName, * correctTokenName;

		tokenByName(token, &correctTokenName);
		tokenByName(current_token->kind, &currentTokenName);

		fprintf(yyout, "Expected token of type %s at line %d,\n", correctTokenName, current_token->lineNumber);
		fprintf(yyout, "Actual token of type %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		current_token = next_token();

	}
}











void parse_PROG(FILE* yyout) {
	parse_GLOBAL_VARS(yyout);
	parse_FUNC_PREDEFS(yyout);
	parse_FUNC_FULL_DEFS(yyout);

	match(EOF_TOK,yyout);
}

void parse_GLOBAL_VARS(FILE* yyout) {
	parse_VAR_DEC(yyout);
	parse_GLOBAL_VARS_(yyout);
}

void GLOBAL_VARS_(FILE* yyout) {
	parse_VAR_DEC(yyout);
}

void parse_VAR_DEC(FILE* yyout) {
	parse_TYPE(yyout);
	match(OTHER_ID,yyout);
	parse_VAR_DEC_(yyout);
}

void parse_VAR_DEC_(FILE* yyout) {
	//
	int followToken[] = { KEY_INT,KEY_FLOAT};
	char* SemicolonTokenName, * BeginTokenName, * currentTokenName;


	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(yyout, "VAR_DEC' -> ; \n");
		match(SEMICOLON_SIGN, yyout);
		break;
	case BRACKETS_OPEN:
		fprintf(yyout, "VAR_DEC' -> [ DIM_SIZES ] ; \n");
		match(BRACKETS_OPEN, yyout);
		parse_DIM_SIZES(yyout);
		match(BRACKETS_CLOSE, yyout);
		match(SEMICOLON_SIGN, yyout);
		break;
	default:
		tokenByName(TOKEN_ID, &IDTokenName);
		tokenByName(TOK_KW_TYPE, &TypeTokenName);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout, "Expected token of type %s, %s at line %d,\n", IDTokenName, TypeTokenName, current_token->lineNumber);
		fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);
		break;

}

