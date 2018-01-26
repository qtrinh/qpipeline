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
#include "common.h"

#define MODE_MATH_LOG_TEN  1000

/**
  math usage 
  **/
void math_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for math calculations.");
	printf( "\n\n\t%s math [ -m mode ] [ options  ]",
			argv[0]);
	printf("\n");

	
	if ((id->mode == MODE_MATH_LOG_TEN) || (id->mode == 0))  {
		printf("\n\t-m %d\tcalculate log10 of a column.",MODE_MATH_LOG_TEN);
		if ((id->mode == MODE_MATH_LOG_TEN) )  {
			printf("\n\t\t-i FILE\tinput file.");
			printf("\n\t\t-s INT\tcolumn number to calculate log10 of.");
		}
	}

	printf("\n\n");
}


/**
	math main
**/
void math_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();

	int col = -1;

	while ((c = getopt(argc, argv, "m:s:i:v")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 's':
			col = atoi(optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			math_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\ni");
			exit(0);
		}
	}

	if (id->mode == 0) {
		math_main_Usage(argc,argv,id);
		exit(1);
	}


	        
	if (strlen(id->inputFileName)==0)  {
		math_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	}

	if (id->mode == MODE_MATH_LOG_TEN) {
		my_math_MODE_MATH_LOG_TEN(id,od,col);
	}
}

