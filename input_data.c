//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "input_data.h"
#include "log.h"


/**
  initialize input structure
 **/
struct input_data * input_data_init() {

	struct input_data *id =  (struct input_data *) malloc (1 * sizeof (struct input_data));

	id->mode = 0;

	strcpy(id->inputFileName, "");
	id->inputFile = NULL;

	strcpy(id->inputFilePrefix, "");
	id->printRejected = 0;
	
	return id;
}


/**
  parse line into columns 
  first column index is 1 
  **/
char ** input_data_parseLineMem(struct input_data *id, char *line, char separator, int *n) {

	char *startPtr;
	char *ptr = line;
	int len;
	int column ;
	int i;
	// columns to return 
	char **columns;

	// find out how many columns in the input line 
	*n = 0;
	while (*ptr != '\0') {
		if (*ptr == separator) {
			(*n)++;
		}
		ptr++;
	}
	(*n)++;


	columns = (char **)malloc (((*n) + 1) * sizeof(char *));
	if (columns == NULL) {
		log_msg(LOG_ERROR, __FILE__, __LINE__, "", "can't allocate mem for columns!!!");
	}

	column = 1;
	len = 0;
	ptr = line;
	startPtr = ptr;
	while (*ptr != '\0') {
		if (*ptr == separator) {
			columns[column] = (char *)malloc ((len+1)*sizeof(char));
			i = 0;
			while (*startPtr != separator) {
				columns[column][i] = *startPtr;
				startPtr++;
				i++;
			}
			columns[column][i] = '\0';

			startPtr = ptr +1;

			len = 0;
			column++;
		} else {
			len++;
		}
		ptr++;
	}
	columns[column] = (char *)malloc ((len+1)*sizeof(char));
	i = 0;
	while (*startPtr != '\0') {
		columns[column][i] = *startPtr;
		startPtr++;
		i++;
	}
	columns[column][i] = '\0';
	
	return columns;
}

void input_data_printParsedLineMemDebugging(char **data, int n) {

	int i;

	printf("\n");
	printf ("\nnumber of columns %d", n);
	fflush(stdout);
	for (i = 1; i <= n; i++) {
		printf ("\ncolumn %3d\t'%s'", i, data[i]);
	}
	printf("\n\n\n");
	fflush(stdout);
}

void input_data_printParsedLineMem(char **data, int n) {

	int i;
	         
	for (i = 1; i <= n; i++) {
		if (i < n)
			printf ("%s\t", data[i]);
		else 
			printf ("%s\n", data[i]);
	}
}


void input_data_freeMem(char **data, int n) {

	int i;

	if (data == NULL)
		return ;
	         
	// free mem allocated by Input_parseLineMem 
	for (i = 1; i <= n; i++)
		if (data[i])
			free(data[i]);

	free(data);
}


