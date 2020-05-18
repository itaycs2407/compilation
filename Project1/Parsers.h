#ifndef PARSERS_H
#define PARSERS_H
#include <stdio.h>
#include "Token.h"
#endif


// Handler Function Defenition 

int match(eTOKENS token, FILE* yyout);
Token* lookAHead(int num);
void tokenByName(eTOKENS token, char* str[]);
Token* getFirstToken();
void errorRecover(int* followArray, int size);

// Entry point for the parsing process

void startParsing(FILE* yyout);

// Function Defenition

void parse_GLOBAL_VARS(FILE* yyout);
void parse_PROG(FILE* yyout);
void parse_DIM_SIZES_(FILE* yyout);
void parse_DIM_SIZES(FILE* yyout);
void parse_VAR_DEC_(FILE* yyout);
void parse_VAR_DEC(FILE* yyout);
void parse_GLOBAL_VARS_(FILE* yyout);
void parse_PARAM_LIST_(FILE* yyout);
void parse_PARAMS(FILE* yyout);
void parse_RETURNED_TYPE(FILE* yyout);
void parse_FUNC_FULL_DEFS_(FILE* yyout);
void parse_FUNC_FULL_DEFS(FILE* yyout);
void parse_FUNC_WITH_BODY(FILE* yyout);
void parse_TYPE(FILE* yyout);
void parse_FUNC_PREDEFS(FILE* yyout);
void parse_FUNC_PREDEFS_(FILE* yyout);
void parse_FUNC_PROTOTYPE(FILE* yyout);
void parse_STMT_LIST(FILE* yyout);
void parse_PARAM_LIST(FILE* yyout);
void parse_PARAM(FILE* yyout);
void parse_VAR_DEC_LIST_(FILE* yyout);
void parse_COMP_STMT(FILE* yyout);
void parse_ARGS(FILE* yyout);
void parse_MOMO(FILE* yyout);
void parse_FACTOR(FILE* yyout);
void parse_STMT_(FILE* yyout);
void parse_STMT(FILE* yyout);
void parse_ARG_LIST(FILE* yyout);
void parse_ARG_LIST_(FILE* yyout);
void parse_STMT_LIST_(FILE* yyout);
void parse_RETURN_STMT(FILE* yyout);
void parse_RETURN_STMT_(FILE* yyout);
void parse_IF_STMT(FILE* yyout);
void parse_TERM(FILE* yyout);
void parse_TERM_(FILE* yyout);
void parse_EXPR_(FILE* yyout);
void parse_EXPR(FILE* yyout);
void parse_CONDITION(FILE* yyout);
void parse_EXPR_LIST_(FILE* yyout);
void parse_EXPR_LIST(FILE* yyout);
void parse_VAR_(FILE* yyout);
