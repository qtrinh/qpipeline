//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>

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
int txt_MODE_TXT_EXTRACT_COLUMN_FROM_FILE(struct input_data *id, char inputFileName[], char columnsToBeExtracted[], double value, int alsoPrintAllOtherColumns) { 

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

	if ((value > DBL_MIN) && (columns2ExtractN != 1)) {
		printf ("\n\n[%s:%d] - ERROR: Only 1 column is allowed when -s is used.!\n\n");
		exit (1);
	}


	// this is the order of columns to print 
	orderOfcolumnsToPrint = (int *)malloc (columns2ExtractN * sizeof(int));
	for (i = 0; i < columns2ExtractN; i++) {
		orderOfcolumnsToPrint[i] = 0;
	}


   id->inputFile = fopen(inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - ERROR: open file '%s'", __FILE__, __LINE__, inputFileName);
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

				printf("\n[%s:%d] - ERROR: The following columns are not in the input file:", __FILE__, __LINE__, indexToPrint, columns2ExtractN);
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
	
	

		int printEntry = 0;
		for (i = 0; i <= indexToPrint; i++) {
			printEntry = 0;
			if ((atof(id->columns[orderOfcolumnsToPrint[i]]) >= value) || (value == DBL_MIN)){
				// print entry if filtering and condition met ( data > value enter by user ) or no filtering at all ( value == DBL_MI )
				printEntry = 1;

				printf ("%s", id->columns[orderOfcolumnsToPrint[i]]);
				if (i < indexToPrint)
					printf ("\t");
				}
		}

		if (printEntry) {
			if (alsoPrintAllOtherColumns){
				// sometimes is handy to also print out all other columns 
				for (i = 1; i <= id->n; i++) {
					printf ("\t%s", id->columns[i]);
				}
			} 
			printf ("\n");
		}
		
		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
	return n;
}




/**
  delete columns from file 

  **/
int txt_MODE_TXT_DELETE_COLUMN_FROM_FILE(struct input_data *id, char inputFileName[], char columnsToBeDeleted[]) { 

	// use this to turn on local verbose for debuggin 
	int localVerbose = 0;

	int n = 0;
	int firstLine = 1;
	int i,j;

	// 
	char **headers;
	int headersN;
	
	char **columns2Delete;
	int columns2DeleteN;


	// print column 
	// 1 means print 
	// 0 means do not print 
	int *printColumn;


	// split columns to be extracted into array
	columns2Delete = my_string_arrayOfChars2arrayOfStrings(id, columnsToBeDeleted, ',', &columns2DeleteN); 
	if (id->verbose) {
		printf("\n[%s:%d] - %d column to be deleted:", __FILE__, __LINE__, columns2DeleteN);
		for (i = 0; i < columns2DeleteN; i++) {
			printf ("\n%d - '%s'", i+1, columns2Delete[i]);
			fflush(stdout);
		}
		printf ("\n");
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

			// set to 1 ( assume we will print all columns )
			printColumn = (int *)malloc (headersN* sizeof(int));
			for (i = 0; i <= headersN; i++) {
				printColumn[i] = 1;
			}


			// look for columns to delete
			// indexToPrint is order of columns to be printed 
			int numberOfColumns2Delete = 0;
			for (i = 0; i < columns2DeleteN; i++) {
				for (j = 1; j <= headersN; j++) {
					if (localVerbose && id->verbose)
						printf ("\ncomparing '%s' '%s'", columns2Delete[i],headers[j]);
					if (strstr(columns2Delete[i],headers[j]) && (strlen(columns2Delete[i])==strlen(headers[j]))) {
						printColumn[j] = 0;
						numberOfColumns2Delete++;
						if (localVerbose && id->verbose)
							printf ("\tfound column to delete ...");
					}
						
					if (localVerbose && id->verbose)
						printf ("\n");

					fflush(stdout);
				}
			}

			if (id->verbose)
				printf ("\n[%s:%d] - found %d columns to delete\n", __FILE__, __LINE__, numberOfColumns2Delete);

			if (numberOfColumns2Delete < columns2DeleteN) {
				printf ("\n");

				printf("\n[%s:%d] - ERROR: Found only %d out of %d columns.  Missing some of the columns ", __FILE__, __LINE__, numberOfColumns2Delete, columns2DeleteN);
				printf ("\n\n");

				exit (1);
			}
			
			for (i = 1; i <= headersN; i++) {
				if (printColumn[i] == 1) {
					printf("%s", headers[i]);
					if (i < headersN)
						printf ("\t");
				}
			}

			printf ("\n");
			continue;
		}


		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);
	
		for (i = 1; i <= id->n; i++) {
			if (printColumn[i] == 1) {
				printf("%s", id->columns[i]);
				if (i < id->n)
					printf ("\t");
			}
		}

		printf ("\n");
	

		
		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
	return n;
}






/**
  delete rows from file where values of column n matches with values in dataFileName 

  **/
int txt_MODE_TXT_DELETE_ROW_FROM_FILE(struct input_data *id, char inputFileName[], char dataFileName[], int n, int deleteRow) { 

	int i,j;

	// data from the 1-column file 
	int dataIndex = 0;
	char data[100][200];


   id->inputFile = fopen(dataFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, dataFileName);
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

		dataIndex++;
		strcpy(data[dataIndex], id->line);

   }   
   fclose(id->inputFile);






   id->inputFile = fopen(inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (id->verbose)  {
			printf ("\n======================\n[%s:%d] - line read from input file:\n'%s'", __FILE__, __LINE__, id->line);
			fflush(stdout);
		}


		if (strstr(id->line,"#")) {
			if (id->verbose)  {
				printf ("\n[%s:%d] - print header line\n", __FILE__, __LINE__);;
				fflush(stdout);
			}
			printf ("%s", id->line);
			continue;
		}

		// remove the new line if it is there 
		if (id->line[strlen(id->line)-1] =='\n')
			id->line[strlen(id->line)-1] = '\0';

   	id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);

		int found = 0;
		for (i = 1; i <= dataIndex; i++) {
			if (strstr(data[i],id->columns[n]) && (strlen(data[i]) == strlen(id->columns[n]))) {
				found = 1;
				i = dataIndex+1;
			}
		}
	 

		if (found == 1) {
			if (id->verbose)  {
				printf ("\n[%s:%d] - matched ... printing\n", __FILE__, __LINE__);
				fflush(stdout);
			}
			printf ("%s\n", id->line);
		}
	 
		input_data_freeMem(id->columns, id->n);
	}	
		
	fclose(id->inputFile);

	return 1;
}

