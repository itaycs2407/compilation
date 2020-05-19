#define _CRT_SECURE_NO_WARNINGS
#include "Parsers.h"
#include "Token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Token* current_token;
int currentIndex;
Node* currentNode;

// Handler Function

int tokenMatcher(eTOKENS token, FILE* file)
{
	if (token == currentNode->tokensArray[currentIndex].kind) {
		current_token = next_token();
		return 1;
	}
	else if (token == EOF_TOK)
	{
		//do nothing;
	}
	else
	{
		char* currentTokenName, * correctTokenName;
		defineToketToName(token, &correctTokenName);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s at line %d,\n", correctTokenName, current_token->lineNumber);
		fprintf(file, "Actual token of type %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		current_token = next_token();
		return 0;
	}
}
Token* lookAHead(int timesToLook) {

	Token* tempToken = &currentNode->tokensArray[currentIndex];
	while (timesToLook > 0 && tempToken->kind != EOF_TOK) {
		tempToken = next_token();
		timesToLook--;
	}
	return tempToken;
}
void errorHandler(int *followArray, int size)
{
	Token* searchToken = current_token;
	while (followArray[size-1] != EOF_TOK && size > 0) {
		if (followArray[size - 1] != searchToken)
			size--;
		else
		{
			current_token = searchToken;
			break;
		}
	}
}
void defineToketToName(eTOKENS token, char* str[]) {
	char* tokenName = (char*)malloc(sizeof(char) * 20);
	switch (token)
	{
	case KEY_INT:
		tokenName = "int";
		break;
	case KEY_FLOAT:
		tokenName = "float";
		break;
	case KEY_VOID:
		tokenName = "void";
		break;
	case KEY_IF:
		tokenName = "if";
		break;
	case KEY_RETURN:
		tokenName = "return";
		break;
	case INT_NUMBER:
		tokenName = "int number";
		break;
	case INT_NUMBER_Z:
		tokenName = "zero";
		break;
	case FLOAT_NUMBER:
		tokenName = "float number";
			break;
	case COMMA_SIGN:
		tokenName = "sign : \"";
			break;
	case COLON_SIGN:
		tokenName = "sign : \,";
			break;
	case SEMICOLON_SIGN:
		tokenName = "sign : \;";
			break;
	case PARENTHESES_OPEN:
		tokenName = "sign : (";
			break;
	case PARENTHESES_CLOSE:
		tokenName = "sign : )";
			break;
	case BRACKETS_OPEN:
		tokenName = "sign : [";
			break;
	case BRACKETS_CLOSE:
		tokenName = "sign : ]";
			break;
	case CURLY_BRACES_OPEN:
		tokenName = "sign : {";
			break;
		case CURLY_BRACES_CLOSE:
			tokenName = "sign : }";
			break;
	case ARGUMENT_OPR_PLUS:
		tokenName = "sign : +";
			break;
	case ARGUMENT_OPR_MULTIPLICATION:
		tokenName = "sign : *";
			break;
	case OP_LE:
		tokenName = "sign : <=";
			break;
	case OP_L:
		tokenName = "sign : <";
			break;
	case OP_GE:
		tokenName = "sign : =>";
			break;
	case OP_G:
		tokenName = "sign : >";
			break;
	case OP_E:
		tokenName = "sign : ==";
			break;
	case OP_NE:
		tokenName = "sign : !=";
			break;
	case EQUAL_OP:
		tokenName = "sign : =";
			break;
	case OTHER_ID:
		tokenName = "sign : ID";
			break;
	case EOF_TOK:
		tokenName = "sign : END OF FILE";
			break;
	default:
		break;
	}

	*str = tokenName;

}

// Entry point for the parsing process

void startParsing(FILE* file) {
	current_token = getFirstToken();
	if (current_token != NULL) {
		parse_PROG(file);
	}
	tokenMatcher(EOF_TOK, file);
}

// Parsers 

void parse_PROG(FILE* file) {
	fprintf(file, "PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS\n");
	parse_GLOBAL_VARS(file);
	parse_FUNC_PREDEFS(file);
	parse_FUNC_FULL_DEFS(file);

}

void parse_GLOBAL_VARS(FILE* file) {
	fprintf(file, "GLOBAL_VARS -> VAR_DEC GLOBAL_VARS_\n");
	parse_VAR_DEC(file);
	parse_GLOBAL_VARS_(file);
}

void parse_GLOBAL_VARS_(FILE* file) {
	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	Token* dummyToken = NULL;
	char* tokenSemiColonSign, * tokenKeyVoid, * tokenKeyInt, * keyFloatToken, * currentTokenName;
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		dummyToken = lookAHead(2);
		switch (dummyToken->kind)
		{
		case SEMICOLON_SIGN:
		case BRACKETS_OPEN:
			fprintf(file, "GLOBAL_VARS_ -> VAR_DEC GLOBAL_VARS_\n");
			current_token = back_token();
			current_token = back_token();
			parse_VAR_DEC(file);
			parse_GLOBAL_VARS_(file);
			return;
		default:
			fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
			current_token = back_token();
			current_token = back_token();
			return;
		}
		break;
	case KEY_VOID:
		fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &keyFloatToken);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColonSign);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, %s, %s,at line %d,\n", tokenKeyInt, keyFloatToken, tokenSemiColonSign, tokenKeyVoid, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;
	}
}

void parse_VAR_DEC(FILE* file) {
	fprintf(file, "VAR_DEC->TYPE id VAR_DEC_\n");
	parse_TYPE(file);
	tokenMatcher(OTHER_ID,file);
	parse_VAR_DEC_(file);
}

void parse_VAR_DEC_(FILE* file) {
	int followToken[] = { KEY_INT,KEY_FLOAT };
	char* keySemicolon, * keyBracketsOpen, * currentTokenName;
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "VAR_DEC_ -> ; \n");
		current_token = next_token();
		break;
	case BRACKETS_OPEN:
		fprintf(file, "VAR_DEC_ -> [ DIM_SIZES ] ; \n");
		parse_DIM_SIZES(file);
		tokenMatcher(BRACKETS_CLOSE, file);
		tokenMatcher(SEMICOLON_SIGN, file);
		break;
	default:
		defineToketToName(SEMICOLON_SIGN, &keySemicolon);
		defineToketToName(BRACKETS_OPEN, &keyBracketsOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", keySemicolon, keyBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}

void parse_DIM_SIZES(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenCommaSign, tokenIntNumber, * tokenIntNumberZ, * currentTokenName;
	fprintf(file, "parse_DIM_SIZES' -> int_num DIM_SIZES_ \n");
	current_token = next_token();
	switch (current_token->kind)
	{
	case INT_NUMBER:
	case INT_NUMBER_Z:
		parse_DIM_SIZES_(file);
		break;
	defult:
		defineToketToName(INT_NUMBER, &tokenIntNumber);
		defineToketToName(INT_NUMBER_Z, &tokenIntNumberZ);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenIntNumber, tokenIntNumberZ, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
	}
}
		
void parse_DIM_SIZES_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	int followIntegerToken[] = { INT_NUMBER ,INT_NUMBER_Z };
	char* tokenCommaSign, tokenIntNumber,*tokenIntNumberZ, * currentTokenName;
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "DIM_SIZES_ -> , int_num DIM_SIZES_\n");
		current_token = next_token();
		switch (current_token->kind)
		{
		case INT_NUMBER:
		case INT_NUMBER_Z:
			parse_DIM_SIZES_(file);
			return;
		defult:
			defineToketToName(INT_NUMBER, &tokenIntNumber);
			defineToketToName(INT_NUMBER_Z, &tokenIntNumberZ);
			defineToketToName(current_token->kind, &currentTokenName);
			fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenIntNumber, tokenIntNumberZ, current_token->lineNumber);
			fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
			errorHandler(followIntegerToken, 2);
		}
		return;
	case BRACKETS_CLOSE:
		fprintf(file, "DIM_SIZES_ -> epsilon \n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", tokenCommaSign,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_TYPE(FILE* file) {
	int followToken[] = { OTHER_ID };
	char *tokenInt, *tokenFloat, * currentTokenName;
	switch (current_token->kind)
	{
	case KEY_INT:
		current_token = next_token();
		fprintf(file, "TYPE -> int \n");
		break;
	case KEY_FLOAT:
		current_token = next_token();
		fprintf(file, "TYPE -> float \n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_FUNC_PREDEFS(FILE * file) {
	fprintf(file, "FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_\n");
	parse_FUNC_PROTOTYPE(file);
	tokenMatcher(SEMICOLON_SIGN, file);
	parse_FUNC_PREDEFS_(file);
}

void parse_FUNC_PREDEFS_(FILE* file) {
	int numOfTimes = 1, followToken[] = { KEY_VOID, KEY_INT, KEY_FLOAT };
	char* tokenSemiColonSign, * tokenKeyVoid,*tokenCurlyOpen ,* tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	Token* dummyToken = next_token();
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
	case KEY_VOID:
		while (dummyToken->kind != SEMICOLON_SIGN && dummyToken->kind != CURLY_BRACES_OPEN && dummyToken->kind != EOF_TOK)
		{
			numOfTimes++;
			dummyToken = lookAHead(numOfTimes);
			currentIndex = currentIndex - numOfTimes;
		}
		switch (dummyToken->kind)
		{
		case SEMICOLON_SIGN:
		case EOF_TOK:
			fprintf(file, "FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_\n");
			parse_FUNC_PROTOTYPE(file);
			tokenMatcher(SEMICOLON_SIGN, file);
			parse_FUNC_PREDEFS_(file);
			return;
		case CURLY_BRACES_OPEN:
			fprintf(file, "FUNC_PREDEFS -> epsilon\n");
			current_token = back_token();
			return;
		}
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColonSign);
		defineToketToName(CURLY_BRACES_OPEN, &tokenCurlyOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s , %s, at line %d,\n", tokenSemiColonSign, tokenCurlyOpen, tokenKeyFloat, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;
	}
}

void parse_FUNC_PROTOTYPE(FILE* file) {
	fprintf(file, "FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )\n");
	parse_RETURNED_TYPE(file);
	tokenMatcher(OTHER_ID, file);
	tokenMatcher(PARENTHESES_OPEN, file);
	parse_PARAMS(file);
	tokenMatcher(PARENTHESES_CLOSE,file);
}

void parse_FUNC_FULL_DEFS(FILE* file) {
	fprintf(file, "FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS_\n");
	parse_FUNC_WITH_BODY(file);
	parse_FUNC_FULL_DEFS_(file);
}
 
void parse_FUNC_WITH_BODY(FILE* file) {
	fprintf(file, "FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP_STMT\n");
	parse_FUNC_PROTOTYPE(file);
	parse_COMP_STMT(file);
}

void parse_FUNC_FULL_DEFS_(FILE* file) {

	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	char*  tokenKeyVoid,* tokenEOF ,* tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	switch (current_token->kind) {
	case KEY_VOID:
	case KEY_FLOAT:
	case KEY_INT: 		
		fprintf(file, "FUNC_FULL_DEFS_ -> FUNC_WITH_BODY FUNC_FULL_DEFS_\n");
		parse_FUNC_WITH_BODY(file);
		parse_FUNC_FULL_DEFS_(file);
		break;
	case EOF_TOK:
		fprintf(file, "FUNC_FULL_DEFS_ -> epsilon\n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(EOF_TOK, &tokenEOF);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s,  %s ,%s , %s, at line %d,\n", tokenKeyFloat, tokenEOF, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 3);
		break;
	}
}

void parse_RETURNED_TYPE(FILE* file) {
	int followToken[] = { OTHER_ID };
	char* tokenKeyInt, * tokenKeyFloat, * tokenKeyVoid, *currentTokenName;
	switch (current_token->kind) {
	case KEY_INT:
		fprintf(file, "RETURNED_TYPE -> TYPE\n");
		parse_TYPE(file);
		break;
	case KEY_FLOAT:
		fprintf(file, "RETURNED_TYPE -> TYPE \n");
		parse_TYPE(file);
		break;
	case KEY_VOID:
		current_token = next_token();
		fprintf(file, "RETURNED_TYPE -> void\n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenKeyInt);
		defineToketToName(KEY_FLOAT, &tokenKeyFloat);
		defineToketToName(KEY_VOID, &tokenKeyVoid);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenKeyInt, tokenKeyFloat, tokenKeyVoid, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}

}

void parse_PARAMS(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenInt, * tokenFloat, *tokenParentesesClose, * currentTokenName;
	switch (current_token->kind) {
	case KEY_INT:
		fprintf(file, "PARAMS -> PARAM_LIST\n");
		parse_PARAM_LIST(file);
		break;
	case KEY_FLOAT:
		fprintf(file, "PARAMS -> PARAM_LIST\n");
		parse_PARAM_LIST(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "PARAMS -> epsilon\n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesesClose);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, at line %d,\n", tokenInt, tokenFloat, tokenParentesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_PARAM_LIST_(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenCommaSign, * currentTokenName, * tokenParentesesClose;
	switch (current_token->kind) {
	case COMMA_SIGN:
		fprintf(file, "PARAM_LIST_ ->, PARAM PARAM_LIST_ \n");
		current_token = next_token();
		parse_PARAM(file);
		parse_PARAM_LIST_(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "PARAM_LIST_ -> epsilon\n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, at line %d,\n", tokenCommaSign, tokenParentesesClose, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_PARAM_(FILE* file) {
	int followToken[] = { COMMA_SIGN };
	char* tokenCommaSign, * currentTokenName, * tokenBracketsOpen;
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "PARAM_ -> [ DIM_SIZES ]\n");
		tokenMatcher(BRACKETS_OPEN, file);
		parse_DIM_SIZES(file);
		tokenMatcher(BRACKETS_CLOSE, file);
		break;
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:   
		fprintf(file, "PARAM_ -> epsilon \n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(BRACKETS_OPEN, &tokenBracketsOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s  at line %d,\n", tokenCommaSign, tokenBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_COMP_STMT(FILE* file) {
	fprintf(file, "COMP_STMT -> { VAR_DEC_LIST_ STMT_LIST }\n");
	tokenMatcher(CURLY_BRACES_OPEN, file);
	parse_VAR_DEC_LIST_(file);
	parse_STMT_LIST(file);
	tokenMatcher(CURLY_BRACES_CLOSE, file);
}

void parse_VAR_DEC_LIST_(FILE* file) {
	int followToken[] = { OTHER_ID, KEY_INT, KEY_FLOAT, PARENTHESES_CLOSE, KEY_IF, KEY_RETURN };
	char* tokenInt, * tokenFloat, *tokenOtherID, *tokenParentsisClose, *tokenIf, *tokenRetrun,* currentTokenName;
	switch (current_token->kind) {
	case KEY_INT:
	case KEY_FLOAT:
		fprintf(file, "VAR_DEC_LIST_ -> VAR_DEC VAR_DEC_LIST_\n");
		parse_VAR_DEC(file);
		parse_VAR_DEC_LIST_(file);
		break;
	case OTHER_ID:
	case PARENTHESES_CLOSE:
	case KEY_IF:
	case KEY_RETURN:
		fprintf(file, "VAR_DEC_LIST_ -> epsilon\n");
		break;
	default:
		defineToketToName(KEY_INT, &tokenInt);
		defineToketToName(KEY_RETURN, &tokenRetrun);
		defineToketToName(KEY_IF, &tokenIf);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentsisClose);
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(KEY_FLOAT, &tokenFloat);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s,  %s, %s, %s, %s, %s at line %d,\n", tokenOtherID, tokenParentsisClose, tokenIf, tokenRetrun,  tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 6);
		break;
	}
}

void parse_PARAM(FILE* file) {
	fprintf(file, "PARAM -> TYPE id PARAM_\n");
	parse_TYPE(file);
	tokenMatcher(OTHER_ID, file);
	parse_PARAM_(file);
}

void parse_PARAM_LIST(FILE* file) {
	fprintf(file, "PARAM_LIST -> PARAM PARAM_LIST_\n");
	parse_PARAM(file);
	parse_PARAM_LIST_(file);
}

void parse_STMT_LIST(FILE* file) {
	fprintf(file, "STMT_LIST -> STMT STMT_LIST_\n");
	parse_STMT(file);
	parse_STMT_LIST_(file);
}

void parse_IF_STMT(FILE* file) {
	fprintf(file, "IF_STMT -> if ( CONDITION ) STMT\n");
	tokenMatcher(KEY_IF, file);
	tokenMatcher(PARENTHESES_OPEN, file);
	parse_CONDITION(file);
	tokenMatcher(PARENTHESES_CLOSE, file);
	parse_STMT(file);
}

void parse_RETURN_STMT(FILE* file) {
	fprintf(file, "RETURN_STMT -> return RETURN_STMT_\n");
	tokenMatcher(KEY_RETURN, file);
	parse_RETURN_STMT_(file);
}

void parse_STMT_LIST_(FILE* file) {
	int followToken[] = { CURLY_BRACES_CLOSE };
	char* tokenCurlyClose, * tokenSemicolon ,* currentTokenName;
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "STMT_LIST_ -> ; STMT STMT_LIST_ \n");
		tokenMatcher(SEMICOLON_SIGN, file);
		parse_STMT(file);
		parse_STMT_LIST_(file);
		break;
	case CURLY_BRACES_CLOSE:
		fprintf(file, "STMT_LIST_ -> ; epsilon \n");
		break;
	default:
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemicolon);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s,at line %d,\n", tokenCurlyClose, tokenSemicolon, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}
		
void parse_STMT(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenOtherID, * tokenCurlyBraces, * tokenKeyIf, * tokenKeyReturn, * currentTokenName;
	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "STMT -> id STMT_\n");
		current_token = next_token();
		parse_STMT_(file);
		break;
	case CURLY_BRACES_OPEN:
		fprintf(file, "STMT -> COMP_STMT\n");
		current_token = next_token();
		parse_COMP_STMT(file);
		break;
	case KEY_IF:
		fprintf(file, "STMT -> IF_STMT \n");
		parse_IF_STMT(file);
		break;
	case KEY_RETURN:
		fprintf(file, "STMT -> RETURN_STMT \n");
		parse_RETURN_STMT(file);
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(CURLY_BRACES_OPEN, &tokenCurlyBraces);
		defineToketToName(KEY_IF, &tokenKeyIf);
		defineToketToName(KEY_RETURN, &tokenKeyReturn);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s,at line %d,\n", tokenOtherID, tokenCurlyBraces, tokenKeyIf, tokenKeyReturn, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_STMT_(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenBracketsOpen, * tokenParenthesesOpen, * currentTokenName;
	switch (current_token->kind) {
	case BRACKETS_OPEN:
	case EQUAL_OP:
		fprintf(file, "STMT_ -> VAR_ = EXPR \n");
		parse_VAR_(file);
		tokenMatcher(EQUAL_OP, file);
		parse_EXPR(file);
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "STMT_ -> ( ARGS ) \n");
		current_token = next_token();
		parse_ARGS(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		break;
	default:
		defineToketToName(BRACKETS_OPEN, &tokenBracketsOpen);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, at line %d,\n", tokenBracketsOpen, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_FACTOR(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION, PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE , CURLY_BRACES_CLOSE };
	char* tokenOtherID, *tokenIntNum, *tokenFloatNum,* tokenZeroNum, * tokenParenthesesClose, * currentTokenName;
	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "FACTOR -> id MOMO\n");
		current_token = next_token();
		parse_MOMO(file);
		break;
	case INT_NUMBER:
	case INT_NUMBER_Z:
		fprintf(file, "FACTOR -> int_num \n");
		current_token = next_token();
		break;
	case FLOAT_NUMBER:
		fprintf(file, "FACTOR -> float_num\n");
		current_token = next_token();
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "FACTOR -> ( EXPR )\n");
		current_token = next_token();
		parse_EXPR(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		break;			
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s,%s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 11);
		break;
	}
}

void parse_MOMO(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION, ARGUMENT_OPR_PLUS , PARENTHESES_CLOSE, 
		COMMA_SIGN, SEMICOLON_SIGN,BRACKETS_CLOSE,CURLY_BRACES_CLOSE , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE };
	char* tokenARGMulti, * currentTokenName, tokenOpE, tokenOpLE, tokenOpNE, tokenOpL, tokenOpGE, tokenOpG,
		tokenCurlyClose, tokenBracketsClose, tokenSemiColon, tokenComma, tokenParentesisClose;

	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "MOMO-> VAR_\n");
		current_token = next_token();
		parse_VAR_(file);
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "MOMO-> ( ARGS )\n");
		current_token = next_token();
		parse_ARGS(file);
		tokenMatcher(PARENTHESES_CLOSE, file);
		break;
	case ARGUMENT_OPR_MULTIPLICATION:
	case ARGUMENT_OPR_PLUS:
	case CURLY_BRACES_CLOSE:
	case BRACKETS_CLOSE:
	case SEMICOLON_SIGN:
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:
	case OP_NE:
	case OP_LE:
	case OP_L:
	case OP_GE:
	case OP_G:
	case OP_E:
		fprintf(file, "MOMO-> epsilon \n");
		break;
	default:
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenARGMulti);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(BRACKETS_CLOSE, &tokenBracketsClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s, at line %d,\n", tokenARGMulti, tokenBracketsClose, tokenOpL, tokenOpGE, tokenCurlyClose, tokenOpG, tokenOpNE, tokenComma, tokenParentesisClose, tokenSemiColon, tokenOpE, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 13);
		break;
	}
}

void parse_ARG_LIST_(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char*  tokenComma,  * currentTokenName;
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "ARG_LIST_ -> , EXPR ARG_LIST_ \n");
		current_token = next_token();
		parse_EXPR(file);
		parse_ARG_LIST_(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "ARG_LIST_ -> epsilon\n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenComma, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_ARG_LIST(FILE* file) {
	fprintf(file, "ARG_LIST -> EXPR ARG_LIST_\n");
	parse_EXPR(file);
	parse_ARG_LIST_(file);
}

void parse_ARG_LIST_undercover(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesClose, * tokenParenthesesOpen, * currentTokenName;
	switch (current_token->kind) {
	case INT_NUMBER:
	case OTHER_ID:
	case INT_NUMBER_Z:
	case PARENTHESES_OPEN:
	case FLOAT_NUMBER:
		fprintf(file, "ARG_LIST -> EXPR ARG_LIST_\n");
		parse_EXPR(file);
		parse_ARG_LIST_(file);
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenParenthesesOpen, tokenIntNum, tokenZeroNum, tokenFloatNum, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);

		errorHandler(followToken, 1);
		break;
	}
}

void parse_ARGS(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesClose, *tokenParenthesesOpen, * currentTokenName;
	switch (current_token->kind) {
	case OTHER_ID:
	case INT_NUMBER:
	case INT_NUMBER_Z:
	case FLOAT_NUMBER:
	case PARENTHESES_OPEN:
		fprintf(file, "ARGS -> ARG_LIST\n");
		parse_ARG_LIST(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "ARGS -> ARG_LIST | ɛ\n");
		
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenParenthesesOpen, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}
		
void parse_RETURN_STMT_(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN  , CURLY_BRACES_CLOSE};
	char* tokenOtherID, * tokenIntNum,* tokenCurlyClose ,* tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName;

	switch (current_token->kind) {
	case INT_NUMBER:
	case FLOAT_NUMBER:
	case OTHER_ID:
	case PARENTHESES_OPEN:
	case INT_NUMBER_Z:
		fprintf(file, "RETURN_STMT_ -> EXPR\n");
		parse_EXPR(file);
		break;
	case SEMICOLON_SIGN:
	case CURLY_BRACES_CLOSE:
		fprintf(file, "RETURN_STMT_ -> epsilon\n");
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenCurlyClose, tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}
		
void parse_VAR_(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION , EQUAL_OP };
	char* tokenBracketOpen, * tokenOp_e, * tokenArgMulti, * currentTokenName;
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "VAR_ -> [ EXPR_LIST ]\n");
		current_token = next_token();
		parse_EXPR_LIST(file);
		break;
	case ARGUMENT_OPR_MULTIPLICATION:
	case EQUAL_OP:
		fprintf(file, "VAR_ -> epsilon\n");
		break;
	default:
		defineToketToName(BRACKETS_OPEN, &tokenBracketOpen);
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenArgMulti);
		defineToketToName(EQUAL_OP, &tokenOp_e);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s , %s, %s, at line %d,\n", tokenBracketOpen, tokenArgMulti, tokenOp_e, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 2);
		break;
	}
}

void parse_EXPR_LIST(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName;
	switch (current_token->kind) {
	case INT_NUMBER:
	case INT_NUMBER_Z:
	case OTHER_ID:
	case FLOAT_NUMBER:
	case PARENTHESES_OPEN:
		fprintf(file, "EXPR_LIST -> EXPR EXPR_LIST_\n");
		parse_EXPR(file);
		parse_EXPR_LIST_(file);
		break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenZeroNum, tokenParenthesesOpen, tokenFloatNum, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_EXPR_LIST_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenCommaSign, * currentTokenName;
	switch (current_token->kind) {
	case COMMA_SIGN:
		fprintf(file, "EXPR_LIST_ -> , EXPR EXPR_LIST_\n" );
		current_token = next_token();
		parse_EXPR(file);
		parse_EXPR_LIST_(file);
		break;
	case BRACKETS_CLOSE:
		fprintf(file, "EXPR_LIST_ -> epsilon\n");
		break;
	default:
		defineToketToName(COMMA_SIGN, &tokenCommaSign);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, at line %d,\n", tokenCommaSign, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_CONDITION(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName, * tokenOPLE, * tokenOPL, * tokenOPGE, * tokenOPG, * tokenOPE, * tokenOPNE;
	fprintf(file, "CONDITION -> EXPR rel_op EXPR\n");
	parse_EXPR(file);
	switch (current_token->kind)
	{
		case OP_LE:	
		case OP_L:					
		case OP_GE:					
		case OP_G:					
		case OP_E:					
		case OP_NE:
			current_token = next_token();
			parse_EXPR(file);
			break;
	default:
		defineToketToName(OTHER_ID, &tokenOtherID);
		defineToketToName(INT_NUMBER, &tokenIntNum);
		defineToketToName(INT_NUMBER_Z, &tokenZeroNum);
		defineToketToName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		defineToketToName(FLOAT_NUMBER, &tokenFloatNum);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s ,%s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenZeroNum, tokenParenthesesOpen, tokenFloatNum,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 1);
		break;
	}
}

void parse_EXPR(FILE* file) {
	fprintf(file, "EXPR -> TERM EXPR_\n");
	parse_TERM(file);
	parse_EXPR_(file);
}			

void parse_EXPR_(FILE* file) {
	int followToken[] = {  PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE , CURLY_BRACES_CLOSE};
	char * currentTokenName, tokenOpE, tokenOpLE, tokenOpNE, tokenOpL, tokenOpGE, tokenOpG,
		 tokenSemiColon, tokenComma, tokenParentesisClose, tokenCurlyClose;
	switch (current_token->kind) {
	case ARGUMENT_OPR_PLUS:
		fprintf(file, "EXPR_ -> + TERM EXPR_\n");
		current_token = next_token();
		parse_TERM(file);
		parse_EXPR_(file);
		break;
	case PARENTHESES_CLOSE :
	case CURLY_BRACES_CLOSE :
	case COMMA_SIGN:
	case SEMICOLON_SIGN :
	case OP_E:
	case OP_G:
	case OP_GE:
	case OP_L:
	case OP_LE:
	case OP_NE:
		fprintf(file, "EXPR_ -> epsilon\n");
		break;
	default:
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, %s, %s, %s, %s, %s, %s,  at line %d,\n", tokenParentesisClose, tokenCurlyClose, tokenOpGE, tokenOpLE, tokenOpL, tokenOpG, tokenOpE, tokenSemiColon, tokenComma, tokenOpNE, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 10);
	}
}

void parse_TERM_(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_PLUS , PARENTHESES_CLOSE, COMMA_SIGN, SEMICOLON_SIGN,
		BRACKETS_CLOSE,CURLY_BRACES_CLOSE , OP_E,
	OP_G,OP_GE,OP_L,OP_LE,OP_NE};
	char* tokenARGMulti, * currentTokenName , tokenOpE, tokenOpLE, tokenOpNE, tokenOpL, tokenOpGE, tokenOpG, 
			tokenCurlyClose ,tokenBracketsClose , tokenSemiColon, tokenComma, tokenParentesisClose;
	switch (current_token->kind) {
	case ARGUMENT_OPR_MULTIPLICATION:
		fprintf(file, "TERM_ -> * FACTOR TERM_\n");
		current_token = next_token();
		parse_FACTOR(file);
		parse_TERM_(file);
		break;
	case ARGUMENT_OPR_PLUS:
	case CURLY_BRACES_CLOSE:
	case BRACKETS_CLOSE:
	case SEMICOLON_SIGN:
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:
	case OP_NE:
	case OP_LE:
	case OP_L:
	case OP_GE:
	case OP_G:
	case OP_E:
		fprintf(file, "TERM_ -> epsilon\n");
		break;
	default:
		defineToketToName(ARGUMENT_OPR_MULTIPLICATION, &tokenARGMulti);
		defineToketToName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		defineToketToName(BRACKETS_CLOSE, &tokenBracketsClose);
		defineToketToName(SEMICOLON_SIGN, &tokenSemiColon);
		defineToketToName(PARENTHESES_CLOSE, &tokenParentesisClose);
		defineToketToName(COMMA_SIGN, &tokenComma);
		defineToketToName(OP_NE, &tokenOpNE);
		defineToketToName(OP_G, &tokenOpG);
		defineToketToName(OP_GE, &tokenOpGE);
		defineToketToName(OP_L, &tokenOpL);
		defineToketToName(OP_LE, &tokenOpLE);
		defineToketToName(OP_E, &tokenOpE);
		defineToketToName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s,  %s, %s, at line %d,\n", tokenARGMulti, tokenBracketsClose, tokenOpL, tokenOpGE,  tokenCurlyClose, tokenOpG, tokenOpNE, tokenComma, tokenParentesisClose, tokenSemiColon,  tokenOpE,   current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorHandler(followToken, 11);
	}
}

void parse_TERM(FILE* file) {
	fprintf(file, "TERM -> FACTOR TERM_\n");
	parse_FACTOR(file);
	parse_TERM_(file);
}
