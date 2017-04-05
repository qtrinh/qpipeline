//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#ifndef INPUT_DATA_H
#define INPUT_DATA_H

struct input_data {

	// user input mode 
	int mode;

	// input file 
	char inputFileName[1024];
	FILE *inputFile;
	char inputFilePrefix[1024];
	file_format_t inputFileFormat ;
	

	char targetFileName[1024];
	FILE *targetFile;
	
	// for debugging 
	int verbose ;

	// input line 
	char line[MAX_CHAR_PER_LINE];

	// input line parsed into columns
	char **columns;
	char **headers;
	int n;

	int printRejected;

	// input chr, start, end, ref, variant column numbers  
	int chrColumnNumber, startColumnNumber, endColumnNumber;
	int refColumnNumber, variantColumnNumber;
	int columnOffSet;

} ;

struct input_data * input_data_init() ;
void input_data_freeMem(char **data, int n) ;
char ** input_data_parseLineMem(struct input_data *id, char *line, char separator, int *n) ;
void input_data_printParsedLineMemDebugging(char **data, int n) ;


#endif 
