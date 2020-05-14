#ifndef PARSERS_H
#define PARSERS_H
#include <stdio.h>
#include "Token.h"
#endif

/*Function Defenition*/

//Start//
void parse_PROG(FILE* yyout);
void parse_GLOBAL_VARS(FILE* yyout);
void parse_FUNC_PREDEFS(FILE* yyout);
void parse_FUNC_FULL_DEFS(FILE* yyout);
void parse_VAR_DEC(FILE* yyout);
void parse_GLOBAL_VARS_(FILE* yyout);
void parse_TYPE(yyout);
void parse_VAR_DEC_(yyout);
void parse_DIM_SIZES(yyout);

/* Handler Function Defenition */
void match(eTOKENS, FILE* yyout);
Token* lookAhead ();
void tokenByName(eTOKENS token, char* str[]);
