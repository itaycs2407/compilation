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

int match(eTOKENS token, FILE* file)
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
		tokenByName(token, &correctTokenName);
		tokenByName(current_token->kind, &currentTokenName);
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
void errorRecover(int *followArray, int size)
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
void tokenByName(eTOKENS token, char* str[]) {
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
	match(EOF_TOK, file);
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
	//current_token = next_token();
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
			break;
		default:
			fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
			//current_token = back_token();
			current_token = back_token();
			current_token = back_token();
			break;
		}
		break;
	case KEY_VOID:
		fprintf(file, "GLOBAL_VARS_ -> epsilon \n");
		break;
	default:
		tokenByName(KEY_INT, &tokenKeyInt);
		tokenByName(KEY_FLOAT, &keyFloatToken);
		tokenByName(SEMICOLON_SIGN, &tokenSemiColonSign);
		tokenByName(KEY_VOID, &tokenKeyVoid);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, %s, %s,at line %d,\n", tokenKeyInt, keyFloatToken, tokenSemiColonSign, tokenKeyVoid, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);
		break;
	}
}

void parse_VAR_DEC(FILE* file) {
	fprintf(file, "VAR_DEC->TYPE id VAR_DEC_\n");
	parse_TYPE(file);
	match(OTHER_ID,file);
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
		match(BRACKETS_CLOSE, file);
		match(SEMICOLON_SIGN, file);
		break;
	default:
		tokenByName(SEMICOLON_SIGN, &keySemicolon);
		tokenByName(BRACKETS_OPEN, &keyBracketsOpen);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", keySemicolon, keyBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);
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
		tokenByName(INT_NUMBER, &tokenIntNumber);
		tokenByName(INT_NUMBER_Z, &tokenIntNumberZ);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenIntNumber, tokenIntNumberZ, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
	}
}
		
void parse_DIM_SIZES_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	int followIntegerToken[] = { INT_NUMBER ,INT_NUMBER_Z };
	char* tokenCommaSign, tokenIntNumber,*tokenIntNumberZ, * currentTokenName;
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "DIM_SIZES_ -> , int_num DIM_SIZES_ ");
		current_token = next_token();
		switch (current_token->kind)
		{
		case INT_NUMBER:
		case INT_NUMBER_Z:
			parse_DIM_SIZES_(file);
			break;
		defult:
			tokenByName(INT_NUMBER, &tokenIntNumber);
			tokenByName(INT_NUMBER_Z, &tokenIntNumberZ);
			tokenByName(current_token->kind, &currentTokenName);
			fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenIntNumber, tokenIntNumberZ, current_token->lineNumber);
			fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
			errorRecover(followIntegerToken, 2);
		}
		break;
	case BRACKETS_CLOSE:
		fprintf(file, "DIM_SIZES_ -> epsilon \n");
		break;
	default:
		tokenByName(COMMA_SIGN, &tokenCommaSign);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", tokenCommaSign,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}/////

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
		tokenByName(KEY_INT, &tokenInt);
		tokenByName(KEY_FLOAT, &tokenFloat);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s at line %d,\n", tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_FUNC_PREDEFS(FILE * file) {
	fprintf(file, "FUNC_PREDEFS -> FUNC_PROTOTYPE ; FUNC_PREDEFS_\n");
	parse_FUNC_PROTOTYPE(file);
	match(SEMICOLON_SIGN, file);
	parse_FUNC_PREDEFS_(file);
}

void parse_FUNC_PREDEFS_(FILE* file) {
	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	char* tokenSemiColonSign, * tokenKeyVoid, * tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "FUNC_PREDEFS -> ; PROTOTYPE FUNC_PREDEFS_\n");
		break;
	case KEY_VOID:
	case KEY_INT:
	case KEY_FLOAT:
		fprintf(file, "FUNC_PREDEFS -> epsilon \n");
		break;
	default:
		tokenByName(KEY_INT, &tokenKeyInt);
		tokenByName(KEY_FLOAT, &tokenKeyFloat);
		tokenByName(KEY_VOID, &tokenKeyVoid);
		tokenByName(SEMICOLON_SIGN, &tokenSemiColonSign);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s , %s, at line %d,\n", tokenSemiColonSign, tokenKeyFloat, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 3);
		break;
	}
}

void parse_FUNC_PROTOTYPE(FILE* file) {
	fprintf(file, "FUNC_PROTOTYPE -> RETURNED_TYPE id ( PARAMS )\n");
	parse_RETURNED_TYPE(file);
	match(OTHER_ID, file);
	match(PARENTHESES_OPEN, file);
	parse_PARAMS(file);
	match(PARENTHESES_CLOSE,file);
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
		fprintf(file, "FUNC_FULL_DEFS_ -> FUNC_WITH_BODY FUNC_FULL_DEFS_");
		parse_FUNC_WITH_BODY(file);
		parse_FUNC_FULL_DEFS_(file);
		break;
	case EOF_TOK:
		fprintf(file, "FUNC_FULL_DEFS_ -> epsilon");
		break;
	default:
		tokenByName(KEY_INT, &tokenKeyInt);
		tokenByName(KEY_FLOAT, &tokenKeyFloat);
		tokenByName(KEY_VOID, &tokenKeyVoid);
		tokenByName(EOF_TOK, &tokenEOF);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s,  %s ,%s , %s, at line %d,\n", tokenKeyFloat, tokenEOF, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 3);
		break;
	}
}

void parse_RETURNED_TYPE(FILE* file) {
	int followToken[] = { OTHER_ID };
	char* tokenKeyInt, * tokenKeyFloat, * tokenKeyVoid, *currentTokenName;
	//current_token = next_token();
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
		fprintf(file, "RETURNED_TYPE -> void\n ");
		break;
	default:
		tokenByName(KEY_INT, &tokenKeyInt);
		tokenByName(KEY_FLOAT, &tokenKeyFloat);
		tokenByName(KEY_VOID, &tokenKeyVoid);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s at line %d,\n", tokenKeyInt, tokenKeyFloat, tokenKeyVoid, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}

}

void parse_PARAMS(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenInt, * tokenFloat, *tokenParentesesClose, * currentTokenName;
//	current_token = next_token();
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
		
		//current_token = back_token();
		break;
	default:
		tokenByName(KEY_INT, &tokenInt);
		tokenByName(PARENTHESES_CLOSE, &tokenParentesesClose);
		tokenByName(KEY_FLOAT, &tokenFloat);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, %s, at line %d,\n", tokenInt, tokenFloat, tokenParentesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_PARAM_LIST_(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenCommaSign, * currentTokenName, * tokenParentesesClose;
//	current_token = next_token();
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
		tokenByName(COMMA_SIGN, &tokenCommaSign);
		tokenByName(PARENTHESES_CLOSE, &tokenParentesesClose);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, at line %d,\n", tokenCommaSign, tokenParentesesClose, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_PARAM_(FILE* file) {
	int followToken[] = { COMMA_SIGN };
	char* tokenCommaSign, * currentTokenName, * tokenBracketsOpen;
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "PARAM_ -> [ DIM_SIZES ]\n ");
		match(BRACKETS_OPEN, file);
		parse_DIM_SIZES(file);
		match(BRACKETS_CLOSE, file);
		break;
	case COMMA_SIGN:
	case PARENTHESES_CLOSE:   //*/*/*/**/*/*/*//**/*//*/**/*/
		fprintf(file, "PARAM_ -> epsilon ");
		break;
	default:
		tokenByName(COMMA_SIGN, &tokenCommaSign);
		tokenByName(BRACKETS_OPEN, &tokenBracketsOpen);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s  at line %d,\n", tokenCommaSign, tokenBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_COMP_STMT(FILE* file) {
	fprintf(file, "COMP_STMT -> { VAR_DEC_LIST_ STMT_LIST }\n");
	match(CURLY_BRACES_OPEN, file);
	parse_VAR_DEC_LIST_(file);
	parse_STMT_LIST(file);
	match(CURLY_BRACES_CLOSE, file);
}

void parse_VAR_DEC_LIST_(FILE* file) {
	int followToken[] = { OTHER_ID, PARENTHESES_CLOSE, KEY_IF, KEY_RETURN };
	char* tokenInt, * tokenFloat, * currentTokenName;
	//current_token = next_token();
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
		fprintf(file, "VAR_DEC_LIST_ -> epsilon");
		break;
	default:
		tokenByName(KEY_INT, &tokenInt);
		tokenByName(KEY_INT, &tokenFloat);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s,  %s, %s, %s, %s, %s,at line %d,\n", tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 4);
		break;
	}
}

void parse_PARAM(FILE* file) {
	fprintf(file, "PARAM -> TYPE id PARAM_\n");
	//current_token = next_token();
	parse_TYPE(file);
	match(OTHER_ID, file);
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
	match(KEY_IF, file);
	match(PARENTHESES_OPEN, file);
	parse_CONDITION(file);
	match(PARENTHESES_CLOSE, file);
	parse_STMT(file);
}

void parse_RETURN_STMT(FILE* file) {
	fprintf(file, "RETURN_STMT -> return RETURN_STMT_\n");
	match(KEY_RETURN, file);
	parse_RETURN_STMT_(file);
}

void parse_STMT_LIST_(FILE* file) {
	int followToken[] = { CURLY_BRACES_CLOSE };
	char* tokenCurlyClose, * tokenSemicolon ,* currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case SEMICOLON_SIGN:
		fprintf(file, "STMT_LIST_ -> ; STMT STMT_LIST_ \n");
		match(SEMICOLON_SIGN, file);
		parse_STMT(file);
		parse_STMT_LIST_(file);
		break;
	case CURLY_BRACES_CLOSE:
		fprintf(file, "STMT_LIST_ -> ; epsilon ");
		break;
	default:
		tokenByName(CURLY_BRACES_CLOSE, &tokenCurlyClose);
		tokenByName(SEMICOLON_SIGN, &tokenSemicolon);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s,at line %d,\n", tokenCurlyClose, tokenSemicolon, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		
		errorRecover(followToken, 1);
		break;
	}
}
		
void parse_STMT(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenOtherID, * tokenCurlyBraces, * tokenKeyIf, * tokenKeyReturn, * currentTokenName;
	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "STMT -> id STMT_");
		current_token = next_token();
		parse_STMT_(file);
		break;
	case CURLY_BRACES_OPEN:
		fprintf(file, "STMT -> COMP_STMT ");
		current_token = next_token();
		parse_COMP_STMT(file);
		break;
	case KEY_IF:
		fprintf(file, "STMT -> IF_STMT ");
		parse_IF_STMT(file);
		break;
	case KEY_RETURN:
		fprintf(file, "STMT -> RETURN_STMT ");
		parse_RETURN_STMT(file);
		break;
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(CURLY_BRACES_OPEN, &tokenCurlyBraces);
		tokenByName(KEY_IF, &tokenKeyIf);
		tokenByName(KEY_RETURN, &tokenKeyReturn);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s,at line %d,\n", tokenOtherID, tokenCurlyBraces, currentTokenName, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 1);
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
		match(EQUAL_OP, file);
		parse_EXPR(file);
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "STMT_ -> ( ARGS ) \n");
		current_token = next_token();
		parse_ARGS(file);
		match(PARENTHESES_CLOSE, file);
		break;
	default:
		tokenByName(BRACKETS_OPEN, &tokenBracketsOpen);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, at line %d,\n", tokenBracketsOpen, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_FACTOR(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION };
	char* tokenOtherID, *tokenIntNum, *tokenFloatNum,* tokenZeroNum, * tokenParenthesesClose, * currentTokenName;

	switch (current_token->kind) {
	case OTHER_ID:
		fprintf(file, "FACTOR -> id MOMO  ");
		current_token = next_token();
		parse_MOMO(file);
		break;
	case INT_NUMBER:
	case INT_NUMBER_Z:
		fprintf(file, "FACTOR -> int_num  ");
		current_token = next_token();
		break;
	case FLOAT_NUMBER:
		fprintf(file, "FACTOR -> float_num");
		current_token = next_token();
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "FACTOR -> ( EXPR )");
		current_token = next_token();
		parse_EXPR(file);
		match(PARENTHESES_CLOSE, file);
		break;			
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s,%s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_MOMO(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_MULTIPLICATION };
	char* tokenParentesisOpen, * tokenBracketsOpen, * currentTokenName;
	switch (current_token->kind) {
	case BRACKETS_OPEN:
		fprintf(file, "MOMO-> VAR_ ");
		current_token = next_token();
		parse_VAR_(file);
		break;
	case PARENTHESES_OPEN:
		fprintf(file, "MOMO-> ( ARGS )");
		current_token = next_token();
		parse_ARGS(file);
		match(PARENTHESES_CLOSE, file);
		break;
	case ARGUMENT_OPR_MULTIPLICATION:
		fprintf(file, "MOMO-> epsilon ");
		break;
	default:
		tokenByName(PARENTHESES_OPEN, &tokenParentesisOpen);
		tokenByName(BRACKETS_OPEN, &tokenBracketsOpen);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s at line %d,\n", tokenParentesisOpen, tokenBracketsOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 1);
		break;
	}
}

void parse_ARG_LIST_(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char*  tokenComma,  * currentTokenName;
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(file, "ARG_LIST_ -> , EXPR ARG_LIST_ ");
		current_token = next_token();
		parse_EXPR(file);
		parse_ARG_LIST_(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "ARG_LIST_ -> epsilon ");
		break;
	default:
		tokenByName(COMMA_SIGN, &tokenComma);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenComma, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_ARG_LIST(FILE* file) {
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesClose, * tokenParenthesesOpen, * currentTokenName;
	
	switch (current_token->kind) {
	case INT_NUMBER:
	case OTHER_ID:
	case INT_NUMBER_Z:
	case PARENTHESES_OPEN:
	case FLOAT_NUMBER:
		fprintf(file, "ARG_LIST -> EXPR ARG_LIST_");
		parse_EXPR(file);
		parse_ARG_LIST_(file);
		break;
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenParenthesesOpen, tokenIntNum, tokenZeroNum, tokenFloatNum, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);

		errorRecover(followToken, 1);
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
		fprintf(file, "ARGS -> ARG_LIST\n ");
		current_token = next_token();
		parse_ARG_LIST(file);
		break;
	case PARENTHESES_CLOSE:
		fprintf(file, "ARGS -> ARG_LIST | ɛ\n");
		
		break;
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(PARENTHESES_CLOSE, &tokenParenthesesClose);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenParenthesesOpen, tokenZeroNum, tokenParenthesesClose, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}
		
void parse_RETURN_STMT_(FILE* file) {
	int followToken[] = { SEMICOLON_SIGN };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName;

	switch (current_token->kind) {
	case INT_NUMBER:
	case FLOAT_NUMBER:
	case OTHER_ID:
	case PARENTHESES_OPEN:
	case INT_NUMBER_Z:
		fprintf(file, "RETURN_STMT_ -> EXPR");
		parse_EXPR(file);
		break;
	case SEMICOLON_SIGN:
		fprintf(file, "RETURN_STMT_ -> epsilon");
		break;
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenFloatNum, tokenZeroNum, tokenParenthesesOpen, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
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
		tokenByName(BRACKETS_OPEN, &tokenBracketOpen);
		tokenByName(ARGUMENT_OPR_MULTIPLICATION, &tokenArgMulti);
		tokenByName(EQUAL_OP, &tokenOp_e);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s , %s, %s, at line %d,\n", tokenBracketOpen, tokenArgMulti, tokenOp_e, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
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
		fprintf(file, "EXPR_LIST -> EXPR EXPR_LIST_");
		parse_EXPR(file);
		parse_EXPR_LIST_(file);
		break;
	default:
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type   %s, %s, %s, %s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenZeroNum, tokenParenthesesOpen, tokenFloatNum, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_EXPR_LIST_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenCommaSign, * currentTokenName;
	switch (current_token->kind) {
	case COMMA_SIGN:
		fprintf(file, "EXPR_LIST_ -> , EXPR EXPR_LIST_\n " );
		current_token = next_token();
		parse_EXPR(file);
		parse_EXPR_LIST_(file);
		break;
	case BRACKETS_CLOSE:
		fprintf(file, "EXPR_LIST_ -> epsilon\n");
		break;
	default:
		tokenByName(COMMA_SIGN, &tokenCommaSign);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, at line %d,\n", tokenCommaSign, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_CONDITION(FILE* file) {
	
	int followToken[] = { PARENTHESES_CLOSE };
	char* tokenOtherID, * tokenIntNum, * tokenFloatNum, * tokenZeroNum, * tokenParenthesesOpen, * currentTokenName, * tokenOPLE, * tokenOPL, * tokenOPGE, * tokenOPG, * tokenOPE, * tokenOPNE;
	fprintf(file, "CONDITION -> EXPR rel_op EXPR");
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
		tokenByName(OTHER_ID, &tokenOtherID);
		tokenByName(INT_NUMBER, &tokenIntNum);
		tokenByName(INT_NUMBER_Z, &tokenZeroNum);
		tokenByName(PARENTHESES_OPEN, &tokenParenthesesOpen);
		tokenByName(FLOAT_NUMBER, &tokenFloatNum);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, %s, %s ,%s, %s, at line %d,\n", tokenOtherID, tokenIntNum, tokenZeroNum, tokenParenthesesOpen, tokenFloatNum,  current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
		break;
	}
}

void parse_EXPR(FILE* file) {
	fprintf(file, "EXPR -> TERM EXPR_\n");
	parse_TERM(file);
	parse_EXPR_(file);
}			

void parse_EXPR_(FILE* file) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenARG_Plus, * currentTokenName;
	switch (current_token->kind) {
	case ARGUMENT_OPR_PLUS:
		fprintf(file, "EXPR_ -> + TERM EXPR_\n");
		current_token = next_token();
		parse_TERM(file);
		parse_EXPR_(file);
		break;
	case BRACKETS_CLOSE:
		fprintf(file, "EXPR_ -> epsilon\n");
		break;
	default:
		tokenByName(ARGUMENT_OPR_PLUS, &tokenARG_Plus);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type %s, %s, at line %d,\n", tokenARG_Plus, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
	}
}

void parse_TERM_(FILE* file) {
	int followToken[] = { ARGUMENT_OPR_PLUS };
	char* tokenARGMulti, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case ARGUMENT_OPR_MULTIPLICATION:
		fprintf(file, "TERM_ -> * FACTOR TERM_\n");
		current_token = next_token();
		parse_FACTOR(file);
		parse_TERM_(file);
		break;
	case ARGUMENT_OPR_PLUS:
		fprintf(file, "TERM_ -> epsilon\n");
		break;
	default:
		tokenByName(ARGUMENT_OPR_MULTIPLICATION, &tokenARGMulti);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(file, "Expected token of type  %s, %s, at line %d,\n", tokenARGMulti, current_token->lineNumber);
		fprintf(file, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 1);
	}
}

void parse_TERM(FILE* file) {
	fprintf(file, "TERM -> FACTOR TERM_\n");
	parse_FACTOR(file);
	parse_TERM_(file);
}
