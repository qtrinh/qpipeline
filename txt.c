//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "input_data.h"
#include "utils.h"
#include "my_string.h"



/**
  count number of lines in input file 
  **/
int txt_MODE_TXT_COUNT_ENTRY(struct input_data *id, char inputFileName[], char ignoreLinesStartingWith) { 

	int n = 0;

   id->inputFile = fopen(inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		//if (strstr(id->line,"#")) {
		if (id->line[0] == ignoreLinesStartingWith){
			continue;
		}
		n++;
   }   
   fclose(id->inputFile);
	return n;
}



/**
  extract columns from file 
  **/
int txt_MODE_TXT_EXTRACT_COLUMN_FROM_FILE(struct input_data *id, char inputFileName[], char columnsToBeExtracted[], int alsoPrintAllOtherColumns) { 

	// use this to turn on local verbose for debuggin 
	int localVerbose = 0;

	int n = 0;
	int firstLine = 1;
	int i,j;

	// 
	char **headers;
	int headersN;
	
	char **columns2Extract;
	int columns2ExtractN;


	// order of columns to print 
	int *orderOfcolumnsToPrint;
	int indexToPrint;
	

	// split columns to be extracted into array
	columns2Extract = my_string_arrayOfChars2arrayOfStrings(id, columnsToBeExtracted, ',', &columns2ExtractN); 
	if (id->verbose) {
		printf("\n[%s:%d] - %d column to be extracted:", __FILE__, __LINE__, columns2ExtractN);
		for (i = 0; i < columns2ExtractN; i++) {
			printf ("\n%d - '%s'", i+1, columns2Extract[i]);
			fflush(stdout);
		}
		printf ("\n");
	}

	// this is the order of columns to print 
	orderOfcolumnsToPrint = (int *)malloc (columns2ExtractN * sizeof(int));
	for (i = 0; i < columns2ExtractN; i++) {
		orderOfcolumnsToPrint[i] = 0;
	}


   id->inputFile = fopen(inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (id->verbose)  {
			printf ("\n[%s:%d] - line read from input file:\n'%s'", __FILE__, __LINE__, id->line);
			fflush(stdout);
		}

		// remove the new line if it is there 
		if (id->line[strlen(id->line)-1] =='\n')
			id->line[strlen(id->line)-1] = '\0';

		if (firstLine) {
			// first line in input file is the column headers 
			// parse the column headers 
			firstLine = 0;
			// break string into fields 
			headers = input_data_parseLineMem(id, id->line, '\t', &headersN);
			if (id->verbose) {
				input_data_printParsedLineMemDebugging(headers, headersN);
			}

			// look for columns to print
			// indexToPrint is order of columns to be printed 
			indexToPrint = 0;
			for (i = 0; i < columns2ExtractN; i++) {
				for (j = 1; j <= headersN; j++) {
					if (localVerbose && id->verbose)
						printf ("\ncomparing '%s' '%s'", columns2Extract[i],headers[j]);
					if (strstr(columns2Extract[i],headers[j]) && (strlen(columns2Extract[i])==strlen(headers[j]))) {
						orderOfcolumnsToPrint[indexToPrint] = j;
						indexToPrint++;
						if (localVerbose && id->verbose)
							printf ("\tfound column ...");
					}
						
					if (localVerbose && id->verbose)
						printf ("\n");

					fflush(stdout);
				}
			}

			if (id->verbose)
				printf ("\n[%s:%d] - found %d columns to extract\n", __FILE__, __LINE__, indexToPrint);

			if (indexToPrint < columns2ExtractN) {
				printf ("\n");

				printf("\n[%s:%d] - ERROR: Found only %d out of %d columns.  Missing the following columns:", __FILE__, __LINE__, indexToPrint, columns2ExtractN);
				for (i = 0; i < columns2ExtractN; i++) {
					if (orderOfcolumnsToPrint[i] == 0)
						printf ("\n'%s'", columns2Extract[i] );
					fflush(stdout);
				}
				printf ("\n\n");


				exit (1);
			}
			
			// print column headers 
			// subtract 1 so we can use it to check for whether to print the tab character or not
			// when printing out the columns 
			indexToPrint--;
			for (i = 0; i <= indexToPrint; i++) {
				printf ("%s", headers[orderOfcolumnsToPrint[i]]);
				if (i < indexToPrint)
					printf ("\t");

			}
			if (alsoPrintAllOtherColumns) {
				// sometimes is handy to also print out all other columns 
				for (i = 1; i <= headersN; i++) {
					printf ("\t%s", headers[i]);
				}
			}

			printf ("\n");

			continue;
		}


		  
		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);

		for (i = 0; i <= indexToPrint; i++) {
			printf ("%s", id->columns[orderOfcolumnsToPrint[i]]);
			if (i < indexToPrint)
				printf ("\t");
		}
		if (alsoPrintAllOtherColumns) {
			// sometimes is handy to also print out all other columns 
			for (i = 1; i <= id->n; i++) {
				printf ("\t%s", id->columns[i]);
			}
		}
		printf ("\n");
		        
		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
	return n;
}

