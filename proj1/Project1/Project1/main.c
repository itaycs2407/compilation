#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

const FIRST_TEST_FILE1 = "c:\\temp\\test1.txt";
const FIRST_TEST_FILE2 = "c:\\temp\\test2.txt";

void main() {
	// loading file to FD
	FILE *testFile1 = fopen(FIRST_TEST_FILE1, "r");
	FILE *testFile2 = fopen(FIRST_TEST_FILE2, "r");
	//test for loading succesfull
	if (testFile1 == NULL || testFile2 == NULL) {
		prinf("problem openning test files.... exiting from program");
		exit(0);
	}
	printf("the loading was succesfull.....");
	// read from file number 1:
	
	// read from file number 2:


	// close files pointers
	fclose(testFile1);
	fclose(testFile2);
	system("pause");

}