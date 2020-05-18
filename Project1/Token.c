#include "Token.h"

#define TOKEN_ARRAY_SIZE 1000

extern FILE* yyin, * yyout;

Node* headNode = NULL;
Node* currentNode = NULL;
int count = 0;
int currentIndex = 0;


Token* getFirstToken() {
	currentNode = headNode;
	currentIndex = 0;
	return &(currentNode->tokensArray[currentIndex]);
}

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;
	if (headNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
		headNode = currentNode;
	}
	else
	{
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif		
}

void false_create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	int length = strlen(lexeme)+1;
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));
		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->tokensArray = (Token*) calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	else
	{
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));
			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token),TOKEN_ARRAY_SIZE);
			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}
			currentNode->next = NULL;
			currentNode->prev = NULL;
			headNode = currentNode;
		}
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;	
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*length);
	#ifdef _WIN32
		strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
	#else
		strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	#endif		
}

Token *back_token() {
	
	if (headNode == NULL || headNode == currentNode && currentIndex == 0) {
		return NULL;
	}
	else if (currentIndex == 0) {
		currentNode = currentNode->prev;
		currentIndex = TOKEN_ARRAY_SIZE - 1;
	}
	else {
		currentIndex--;
	}
	return &(currentNode->tokensArray[currentIndex]);
}

Token* next_token() {
	if (currentNode == NULL) {
		return NULL;
	}
	if (currentIndex == TOKEN_ARRAY_SIZE - 1) {
		currentNode = currentNode->next;
		currentIndex = 0;
	}
	else {
		currentIndex++;
	}
	return &(currentNode->tokensArray[currentIndex]);
}

void deleteList()
{
	Node* current = headNode->next;
	Node* next;
	while (current != NULL)
	{
		next = current->next;
		free(current->tokensArray);
		free(current);
		current = next;
	}
}