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
// to be fixed.......
void GLOBAL_VARS_(FILE* yyout) {

	int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
	char* tokenSemiColonSign, * tokenKeyVoid, * tokenKeyInt, * tokenKeyFloat, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind) {
	case  KEY_VOID:
		//fprintf....GLOBAL_VARS_ -> ɛ
		back_token();
		break;
		parse_VAR_DEC(yyout);
		parse_GLOBAL_VARS_(yyout);
	}
}

void parse_VAR_DEC(FILE* yyout) {
	parse_TYPE(yyout);
	match(OTHER_ID,yyout);
	parse_VAR_DEC_(yyout);
}

void parse_VAR_DEC_(FILE* yyout) {
	//
	int followToken[] = { KEY_INT,KEY_FLOAT };
	char* keyIntToken, * keyFloatToken, * currentTokenName;
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
		tokenByName(KEY_INT, &keyIntToken);
		tokenByName(KEY_FLOAT, &keyFloatToken);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout, "Expected token of type %s, %s at line %d,\n", keyIntToken, keyFloatToken, current_token->lineNumber);
		fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		errorRecover(followToken, 2);
		break;

	}
}

void parse_DIM_SIZES(FILE* yyout) {
	fprintf(yyout, "parse_DIM_SIZES' -> int_num DIM_SIZES_ \n");
	match(KEY_INT, yyout);
	parse_DIM_SIZES_(yyout);
}

void parse_DIM_SIZES_(FILE* yyout) {
	int followToken[] = { BRACKETS_CLOSE };
	char* tokenCommaSign, * tokenBracketsClose, * currentTokenName;
	switch (current_token->kind)
	{
	case COMMA_SIGN:
		fprintf(yyout, "DIM_SIZES_ -> , int_num DIM_SIZES_ ");
		match(KEY_INT, yyout);
		parse_DIM_SIZES_(yyout);
		break;
	case BRACKETS_CLOSE:
		fprintf(yyout, "DIM_SIZES_ -> epsilon \n");
		break;
	default:
		tokenByName(COMMA_SIGN, &tokenCommaSign);
		tokenByName(BRACKETS_CLOSE, &tokenBracketsClose);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout, "Expected token of type %s, %s at line %d,\n", tokenCommaSign, tokenBracketsClose, current_token->lineNumber);
		fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 1);
		break;
	}
}
void parse_TYPE(FILE* yyout) {
	int followToken[] = { OTHER_ID };
	char* tokenInt, * tokenFloat, * currentTokenName;
	current_token = next_token();
	switch (current_token->kind)
	{
	case KEY_INT:
		current_token = next_token();
		fprintf(yyout, "TYPE -> int ");
		break;
	case KEY_FLOAT:
		current_token = next_token();
		fprintf(yyout, "TYPE -> float ");
		break;
	default:
		tokenByName(KEY_INT, &tokenInt);
		tokenByName(KEY_FLOAT, &tokenFloat);
		tokenByName(current_token->kind, &currentTokenName);
		fprintf(yyout, "Expected token of type %s, %s at line %d,\n", tokenInt, tokenFloat, current_token->lineNumber);
		fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
		//need to implement//
		errorRecover(followToken, 1);
		break;
	}
}

		void parse_FUNC_PREDEFS(FILE * yyout) {
			parse_PROTOTYPE(yyout);
			match(SEMICOLON_SIGN, yyout);
			parse_FUNC_PREDEFS_(yyout);
		}

		void parse_FUNC_PREDEFS_(FILE* yyout) {


			int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
			char* tokenSemiColonSign, * tokenKeyVoid, * tokenKeyInt, * tokenKeyFloat, * currentTokenName;
			switch (current_token->kind) {

			case SEMICOLON_SIGN:
				fprintf(yyout, "FUNC_PREDEFS -> ; PROTOTYPE FUNC_PREDEFS_");
				break;
			case KEY_VOID:
			case KEY_INT:
			case KEY_FLOAT:
				fprintf(yyout, "FUNC_PREDEFS -> epsilon \n");
				break;

			default:
				tokenByName(KEY_INT, &tokenKeyInt);
				tokenByName(KEY_FLOAT, &tokenKeyFloat);
				tokenByName(KEY_VOID, &tokenKeyVoid);
				tokenByName(SEMICOLON_SIGN, &tokenSemiColonSign);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type %s, %s, %s , %s, at line %d,\n", tokenSemiColonSign, tokenKeyFloat, tokenKeyVoid, tokenKeyInt current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 3);
				break;
			}
		}

		void parse_FUNC_PROTOTYPE(FILE* yyout) {
			parse_RETURNED_TYPE(yyout);
			match(OTHER_ID, yyout);
			match(PARENTHESES_OPEN, yyout);
			parse_PARAMS(yyout);
			match(PARENTHESES_CLOSE,yyout);
		}


		void parse_FUNC_FULL_DEFS(FILE* yyout) {
			parse_FUNC_WITH_BODY(yyout);
			parse_FUNC_FULL_DEFS_(yyout);
		}
		 
		void parse_FUNC_WITH_BODY(FILE* yyout) {
			parse_FUNC_PROTOTYPE(yyout);
			parse_COMP_STMT(yyout);
		}

		void FUNC_FULL_DEFS_(FILE* yyout) {

			int followToken[] = { KEY_VOID,KEY_INT,KEY_FLOAT };
			char*  tokenKeyVoid,* tokenEOF ,* tokenKeyInt, * tokenKeyFloat, * currentTokenName;
			switch (current_token->kind) {
			case KEY_VOID:
			case KEY_FLOAT:
			case KEY_INT: 		
				fprintf(yyout, "FUNC_FULL_DEFS_ -> FUNC_WITH_BODY FUNC_FULL_DEFS_");
				parse_FUNC_WITH_BODY(yyout);
				parse_FUNC_FULL_DEFS_(yyout);
				break;
			case EOF_TOK:
				fprintf(yyout, "FUNC_FULL_DEFS_ -> epsilon");
				break;
			default:
				tokenByName(KEY_INT, &tokenKeyInt);
				tokenByName(KEY_FLOAT, &tokenKeyFloat);
				tokenByName(KEY_VOID, &tokenKeyVoid);
				tokenByName(EOF_TOK, &tokenEOF);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type %s,  %s ,%s , %s, at line %d,\n", tokenKeyFloat, tokenEOF, tokenKeyVoid, tokenKeyInt ,current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 3);
				break;
			}
		}
		
		
		void parse_RETURNED_TYPE(FILE* yyout) {
			int followToken[] = { OTHER_ID };
			char* tokenKeyInt, * tokenKeyFloat, * tokenKeyVoid, *currentTokenName;
			current_token = next_token();
			switch (current_token->kind) {
			case KEY_INT:
				fprintf(yyout, "RETURNED_TYPE -> TYPE ");
				match(KEY_INT, yyout);
				break;
			case KEY_FLOAT:
				fprintf(yyout, "RETURNED_TYPE -> TYPE ");
				match(KEY_FLOAT, yyout);
				break;
			case KEY_VOID:
				fprintf(yyout, "RETURNED_TYPE -> void ");
				match(KEY_VOID, yyout);
				break;
			default:
				tokenByName(KEY_INT, &tokenKeyInt);
				tokenByName(KEY_FLOAT, &tokenKeyFloat);
				tokenByName(KEY_VOID, &tokenKeyVoid);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type %s, at line %d,\n", tokenKeyInt, tokenKeyFloat, tokenKeyVoid, current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 1);
				break;
			}

		}

		void parse_PARAMS(FILE* yyout) {
			int followToken[] = { PARENTHESES_CLOSE };
			char* tokenInt, * tokenFloat, *tokenParentesesClose, * currentTokenName;
			current_token = next_token();
			switch (current_token->kind) {
			case KEY_INT:
				fprintf(yyout, "PARAMS -> PARAM_LIST");
				match(KEY_INT, yyout);
				break;
			case KEY_FLOAT:
				fprintf(yyout, "PARAMS -> PARAM_LIST");
				match(KEY_INT, yyout);
				break;
			case PARENTHESES_CLOSE:
				fprintf(yyout, "PARAMS -> epsilon");
				break;
			default:
				tokenByName(KEY_INT, &tokenInt);
				tokenByName(PARENTHESES_CLOSE, &tokenParentesesClose);
				tokenByName(KEY_FLOAT, &tokenFloat);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type %s, %s, %s, at line %d,\n", tokenInt, tokenFloat, tokenParentesesClose, current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 1);
				break;
			}
		}


		void parse_PARAM_LIST_(FILE* yyout) {
			int followToken[] = { PARENTHESES_CLOSE };
			char* tokenCommaSign, * currentTokenName, * tokenParentesesClose;
			current_token = next_token();
			switch (current_token->kind) {
			case COMMA_SIGN:
				fprintf(yyout, "PARAM_LIST_ ->, PARAM PARAM_LIST_ ");
				match(COMMA_SIGN, yyout);
				parse_PARAM(yyout);
				parse_PARAM_LIST_(yyout);
				break;
			case PARENTHESES_CLOSE:
				fprintf(yyout, "PARAM_LIST_ -> epsilon");
				break;
			default:
				tokenByName(COMMA_SIGN, &tokenCommaSign);
				tokenByName(PARENTHESES_CLOSE, &tokenParentesesClose);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type  %s, %s, %s, at line %d,\n", tokenCommaSign, tokenParentesesClose, currentTokenName, current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 1);
				break;
			}
		}

		void parse_PARAM_(FILE* yyout) {
			int followToken[] = { COMMA_SIGN };
			char* tokenCommaSign, * currentTokenName, * tokenBracketsOpen;
			current_token = next_token();
			switch (current_token->kind) {
			case BRACKETS_OPEN:
				fprintf(yyout, "PARAM_ -> [ DIM_SIZES ] ");
				match(BRACKETS_OPEN, yyout);
				parse_DIM_SIZES(yyout);
				match(BRACKETS_CLOSE, yyout);
				break;
			case COMMA_SIGN:
				fprintf(yyout, "PARAM_ -> epsilon ");
				break;
			default:
				tokenByName(COMMA_SIGN, &tokenCommaSign);
				tokenByName(BRACKETS_OPEN, &tokenBracketsOpen);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type  %s, %s  at line %d,\n", tokenCommaSign, tokenBracketsOpen, current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 1);
				break;
			}
		}

		void parse_COMP_STMT(FILE* yyout) {
			match(PARENTHESES_OPEN, yyout);
			parse_VAR_DEC_LIST_(yyout);
			parse_STMT_LIST(yyout);
			match(PARENTHESES_CLOSE, yyout);
		}

		void parse_VAR_DEC_LIST_(FILE* yyout) {
			int followToken[] = { OTHER_ID, PARENTHESES_CLOSE, KEY_IF, KEY_RETURN };
			char* tokenInt, * tokenFloat, * tokenOtherID, * tokenKeyIf, * tokenKeyReturn, * tokenParenthsesClose, * currentTokenName;
			current_token = next_token();
			switch (current_token->kind) {
			case KEY_INT:
			case KEY_FLOAT:
				fprintf(yyout, "VAR_DEC_LIST_ -> VAR_DEC VAR_DEC_LIST_");
				parse_VAR_DEC(yyout);
				parse_VAR_DEC_LIST_(yyout);
				break;
			case OTHER_ID:
			case PARENTHESES_CLOSE:
			case KEY_IF:
			case KEY_RETURN:
				fprintf(yyout, "VAR_DEC_LIST_ -> epsilon");
				break;
			default:
				tokenByName(KEY_INT, &tokenInt);
				tokenByName(KEY_INT, &tokenFloat);
				tokenByName(KEY_INT, &tokenOtherID);
				tokenByName(KEY_INT, &tokenKeyReturn);
				tokenByName(KEY_INT, &tokenParenthsesClose);
				tokenByName(KEY_INT, &tokenKeyIf);
				tokenByName(current_token->kind, &currentTokenName);
				fprintf(yyout, "Expected token of type  %s,  %s, %s, %s, %s, %s,at line %d,\n", tokenInt, tokenFloat, tokenOtherID, tokenKeyReturn, tokenParenthsesClose, tokenKeyIf, current_token->lineNumber);
				fprintf(yyout, "Actual token: %s, lexeme:\'\'%s\'\'\n", currentTokenName, current_token->lexeme);
				//need to implement//
				errorRecover(followToken, 4);
				break;
			}
		}



		void parse_PARAM(FILE* yyout) {
			parse_TYPE(yyout);
			match(OTHER_ID, yyout);
			PARAM_(yyout);
		}

		void parse_PARAM_LIST(FILE* yyout) {
			parse_PARAM(yyout);
			parse_PARAM_LIST_(yyout);
		}

		void parse_STMT_LIST(FILE* yyout) {
			parse_STMT(yyout);
			parse_STMT_LIST_(yyout);
		}
		
		void parse_STMT_LIST(FILE* yyout) {
			parse_STMT(yyout);
			parse_STMT_LIST_(yyout);
		}
		void parse_IF_STMT(FILE* yyout) {
			match(KEY_IF, yyout);
			match(PARENTHESES_OPEN, yyout);
			parse_CONDITION(yyout);
			match(PARENTHESES_CLOSE, yyout);
			parse_STMT(yyout);
		}
		void parse_RETURN_STMT(FILE* yyout) {
			match(KEY_RETURN, yyout);
			parse_RETURN_STMT_(yyout);
		}
	
		void parse_VAR(FILE* yyout) {
			match(OTHER_ID, yyout);
			parse_VAR_(yyout);
		}

		

void tokenByName(eTOKENS token, char* str[]) {
	switch (token)
	{
	case KEY_INT:
		str = "int";
		break;
	case KEY_FLOAT:
		str = "float";
		break;
	case KEY_VOID:
		str = "void";
		break;
	case KEY_IF:
		str = "if";
		break;
	case KEY_RETURN:
		str = "return";
		break;
	case INT_NUMBER:
		str = "int number";
		break;
	case INT_NUMBER_Z:
		str = "float";
		break;
	case FLOAT_NUMBER:
		str = "float number";
			break;
	case COMMA_SIGN:
		str = "sign : \"";
			break;
	case COLON_SIGN:
		str = "sign : \,";
			break;
	case SEMICOLON_SIGN:
		str = "sign : \;";
			break;
	case PARENTHESES_OPEN:
		str = "sign : (";
			break;
	case PARENTHESES_CLOSE:
		str = "sign : )";
			break;
	case BRACKETS_OPEN:
		str = "sign : [";
			break;
	case BRACKETS_CLOSE:
		str = "sign : ]";
			break;
	case CURLY_BRACES_OPEN:
		str = "sign : {";
			break;
		case CURLY_BRACES_CLOSE:
			str = "sign : }";
			break;
	case ARGUMENT_OPR_PLUS:
		str = "sign : +";
			break;
	case ARGUMENT_OPR_MULTIPLICATION:
		str = "sign : *";
			break;
	case OP_LE:
		str = "sign : <=";
			break;
	case OP_L:
		str = "sign : <";
			break;
	case OP_GE:
		str = "sign : =>";
			break;
	case OP_G:
		str = "sign : >";
			break;
	case OP_E:
		str = "sign : ==";
			break;
	case OP_NE:
		str = "sign : !=";
			break;
	case EQUAL_OP:
		str = "sign : =";
			break;
	case OTHER_ID:
		str = "sign : ID";
			break;
	case EOF_TOK:
		str = "sign : END OF FILE";
			break;


	default:
		break;
	}



}

