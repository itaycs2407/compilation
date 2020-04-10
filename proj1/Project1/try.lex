%option noyywrap

%{
#include "Token.h"
#define _CRT_SECURE_NO_WARNINGS
int line_num=1;
int IsFullyCommentedLine(char* line);
%}

DIGIT    [0-9]
ALPHA	 [a-zA-Z]


%%
[ \t] 			{}
[\n ] {line_num++;	}
\/\*([^*]|[\r\n]|(\*+([^*\/]|[\r\n])))*\*\/+      {line_num += IsFullyCommentedLine(yytext); }

int   { create_and_store_token(KEY_INT,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type KEY_INT , lexeme %s\n", line_num, yytext);  
                       return 1;
 	   }
float  { create_and_store_token(KEY_FLOAT,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type KEY_FLOAT , lexeme %s\n", line_num, yytext);  
                       return 1;
 	   }
void   { create_and_store_token(KEY_VOID,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type KEY_VOID , lexeme %s\n", line_num, yytext);  
                       return 1;
 	   }
if     { create_and_store_token(KEY_IF,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type KEY_IF , lexeme %s\n", line_num, yytext);  
                       return 1;
 	    }
return  { create_and_store_token(KEY_RETURN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type KEY_RETURN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	    }
[1-9][0-9]* { create_and_store_token(INT_NUMBER,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type INT_NUMBER , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }		
[0] { create_and_store_token(INT_NUMBER,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type INT_NUMBER , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
{DIGIT}+(\.{DIGIT}+)([e|E][\+\-]{DIGIT}+)?     { create_and_store_token(FLOAT_NUMBER,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type FLOAT_NUMBER , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }	
,     { create_and_store_token(COMMA_SIGN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type COMMA_SIGN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
:     { create_and_store_token(COLON_SIGN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type COLON_SIGN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
;     { create_and_store_token(SEMICOLON_SIGN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type SEMICOLON_SIGN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\(     { create_and_store_token(PARENTHESES_OPEN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type PARENTHESES_OPEN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\)     { create_and_store_token(PARENTHESES_CLOSE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type PARENTHESES_CLOSE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\[     { create_and_store_token(BRACKETS_OPEN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type BRACKETS_OPEN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\]     { create_and_store_token(BRACKETS_CLOSE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type BRACKETS_CLOSE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\{     { create_and_store_token(CURLY_BRACES_OPEN,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type CURLY_BRACES_OPEN , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\}     { create_and_store_token(CURLY_BRACES_CLOSE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type CURLY_BRACES_CLOSE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }	  
\+     { create_and_store_token(ARGUMENT_OPR_PLUS,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type ARGUMENT_OPR_PLUS , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\*     { create_and_store_token(ARGUMENT_OPR_MULTIPLICATION,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type ARGUMENT_OPR_MULTIPLICATION , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\<\=     { create_and_store_token(OP_LE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_LE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\<     { create_and_store_token(OP_L,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_L , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\>\=     { create_and_store_token(OP_GE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_GE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\>     { create_and_store_token(OP_G,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_G , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\=\=     { create_and_store_token(OP_E,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_E , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
\!\=     { create_and_store_token(OP_NE,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type OP_NE , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
=     { create_and_store_token(EQUAL_OP,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type EQUAL_OP , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
[a-z]+(_?[{ALPHA}{DIGIT}]+)*     { create_and_store_token(OTHER_ID,  yytext, line_num); 
                       fprintf(yyout,"Line %d : found token of type ID , lexeme %s\n", line_num, yytext);  
                       return 1;
 	      }
.       			fprintf(yyout ,"Unrecognized character!: %s in line %d\n", yytext, line_num);

%%

int IsFullyCommentedLine(char* line)
{
	int sum=0;
    // For each character in the line
    for (int i = 0; line[i] != 0; i++)
    {
        char c = line[i];
	if(c == '\n'){
		sum++;
	}
	}
	return sum;
}

int CheckFile(char* inputFileName) {
#define NUM_OF_CHARS_FOR_STRING_FOR_OUTPUT_FILENAME  100
	// build the output filename with the prefix of the input name
	int i = 0, j = 0;
	char* addition = "_066524737_302788781_204072524_lex_output.txt";
	char outputFileName[NUM_OF_CHARS_FOR_STRING_FOR_OUTPUT_FILENAME];
	strcpy(outputFileName, inputFileName);
	while (outputFileName[i] != '.') {
		i++;
	}
	while (i < i + strlen(addition) && (i < NUM_OF_CHARS_FOR_STRING_FOR_OUTPUT_FILENAME)) {
		outputFileName[i++] = addition[j++];
	}

	// open file pointer for read
	yyin = fopen(inputFileName, "r");
	// open file for output
	yyout = fopen(outputFileName, "w");
	// check for file pointers
	if (yyin == NULL)
	{
		printf("Error opening input file...");
		return 0;
	}
	else if (yyout == NULL)
	{
		printf("Error opening output file for writing...");
		return 0;
	}
	// start lexing the file
	while (yylex() != 0);

	// closing file pointers
	fclose(yyin);
	fclose(yyout);
	return 1;
}
int PrintMenu() {
	int inputFromUser = 0;
	do {
		printf("Please enter your choise :     \n");
		printf("-------------------------------\n");
		printf("1. c:\\temp\\test1.txt         \n");
		printf("2. c:\\temp\\test2.txt         \n");
		printf("3. run both files              \n");
		printf("4. exit                        \n");
		printf("enter your choise :            \n");
		scanf_s("%d", &inputFromUser);
	} while ((inputFromUser > 4) || (inputFromUser < 0));
	return inputFromUser;
}

int main(int argc, char** argv) {
	// get input from user
	int userInput = PrintMenu();

	// check the user input
	switch (userInput) {
	case 1: {
		printf("Start lexing file: c:\\temp\\test1.txt ..... please wait \n");
		if (CheckFile("c:\\temp\\test1.txt")) {
			printf("The output file is ready, located in c:\\temp....\n");
		}
		else
		{
			printf("Problem lexing.... exit from program \n");
		}
		system("pause");
		break;
	}
	case 2: {
		printf("Start lexing file: c:\\temp\\test2.txt ..... please wait \n");
		if (CheckFile("c:\\temp\\test2.txt")) {
			printf("The output file is ready, located in c:\\temp.  \n");
		}
		else
		{
			printf("Problem lexing.... exit from program \n");
		}
		system("pause");
		break;
	}
	case 3: {
		printf("Start lexing file: c:\\temp\\test1.txt ..... please wait \n");
		if (CheckFile("c:\\temp\\test1.txt")) {
			printf("The output file is ready, located in c:\\temp....\n");
		}
		else
		{
			printf("Problem lexing test1.txt\n");
		}
		printf("Start lexing file: c:\\temp\\test2.txt ..... please wait \n");
		if (CheckFile("c:\\temp\\test2.txt")) {
			printf("The output file is ready, located in c:\\temp.  \n");
		}
		else
		{
			printf("Problem lexing test2.txt\n");
		}
		system("pause");
		break;
	}
	case 4: {
		system("pause");
		exit(0);
	}
	}

}

