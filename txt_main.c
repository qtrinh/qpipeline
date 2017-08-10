//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "input_data.h"
#include "output_data.h"
#include "txt.h"


/**
  txt usage 
  **/
void txt_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing txt files.");
	printf( "\n\n\t%s txt [ -m mode ] [ options ]",
			argv[0]);
	printf("\n");
	
	if ((id->mode == MODE_TXT_COUNT_ENTRY) || (id->mode == 0))  {
		printf("\n\t-m %d\tcount number of lines.",MODE_TXT_COUNT_ENTRY);
		if ((id->mode == MODE_TXT_COUNT_ENTRY) )  {
			printf("\n\t\t-i FILE\tinput txt file.");
			printf("\n\t\t-k CHAR\tignore or don't count lines starting with this character.  Default '#'");
			//printf("\n\t\t-v\tturn on verbose for debugging");
			printf("\n\n\t\tExample:\n\t\t\t%s txt -m %d -i test_data/txt/data.txt", argv[0], MODE_TXT_COUNT_ENTRY);
		}
	}
	if ((id->mode == MODE_TXT_EXTRACT_COLUMN_FROM_FILE) || (id->mode == 0))  {
		printf("\n\t-m %d\textract one or more columns from a file.",MODE_TXT_EXTRACT_COLUMN_FROM_FILE);
		if ((id->mode == MODE_TXT_EXTRACT_COLUMN_FROM_FILE) )  {
			printf("\n\t\t-i FILE\tinput txt file.");
			printf("\n\t\t-s STR\tcolumn names to be extracted in double quotes and separated by ','.  For example, \"filename,count\"");
			printf("\n\t\t-A\talso print all columns following columns identified by -s ");
			printf("\n\n\t\tExample:\n\t\t\t%s txt -m %d -i test_data/txt/data.txt", argv[0], MODE_TXT_EXTRACT_COLUMN_FROM_FILE);
		}
	}

	printf("\n\n");
}


/**
	txt main
**/
void txt_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();

	char ignoreLinesStartingWith = '#';

	char secondInputFileName[1024] = "";
	char str[1000000] = "";

	int alsoPrintOtherColumns = 0;

	while ((c = getopt(argc, argv, "s:m:i:j:q:k:vAH")) != -1) {
		switch (c) {
		case 'A':
			alsoPrintOtherColumns = 1;
			break;
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'q':
			strcpy(id->inputFilePrefix, optarg);
			break;
		case 's':
			strcpy(str, optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'j':
			strcpy(secondInputFileName, optarg);
			break;
		case 'k':
			strcpy(str, optarg);
			ignoreLinesStartingWith = str[0];
			break;
		case 'H':
			od->outputOrientation = OUTPUT_ORIENTATION_HORIZONTAL;
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			txt_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above.\n\n");
			exit(0);
		}
	}

	if (id->mode == 0) {
		txt_main_Usage(argc,argv,id);
		exit(1);
	}

	if (strlen(id->inputFileName)==0)  {
		txt_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	}

	if ((id->mode == MODE_TXT_COUNT_ENTRY)) {
		int n,m;
		n = txt_MODE_TXT_COUNT_ENTRY(id, id->inputFileName, ignoreLinesStartingWith);
			
		if (strlen(secondInputFileName) > 0) {
			m = txt_MODE_TXT_COUNT_ENTRY(id, secondInputFileName, ignoreLinesStartingWith);
			printf ("\ndiff\tinput_file_name_1\tnumber_of_lines_1\tinput_file_name_2\tnumber_of_lines_2");
			printf ("\n%d\t%s\t%d\t%s\t%d", m-n,id->inputFileName, n, secondInputFileName, m);
			printf ("\n");
		} else { 
			printf ("\ninput_file_name\tnumber_of_lines");
			if (strstr(id->inputFileName,"/dev/stdin")) {
				printf ("\n%s\t%d", id->inputFilePrefix, n);
			}  else {
				printf ("\n%s\t%d", id->inputFileName, n);
			}
			printf ("\n");
		}
	} else if (id->mode == MODE_TXT_EXTRACT_COLUMN_FROM_FILE) {
		if (strlen(str) == 0) {
			txt_main_Usage(argc,argv,id);
			printf ("\tMissing columns to be extracted (-s).  Please see usage above!\n\n");
			exit(1);
		}
		txt_MODE_TXT_EXTRACT_COLUMN_FROM_FILE(id, id->inputFileName, str, alsoPrintOtherColumns);
	}
}



