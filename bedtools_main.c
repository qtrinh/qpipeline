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
#include "bedtools.h"
#include "common.h"

#define MAX_READ_DEPTH 1000

/**
  bedtools usage 
  **/
void bedtools_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing BEDTools files.");
	printf( "\n\n\t%s bedtools [ -m mode ] [ options  ]",
			argv[0]);
	printf("\n");

	
	if ((id->mode == MODE_PARSE_ALL_FOR_1X_2X) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse 'all' to get %% of bases in target at 1x, 2x, etc.",MODE_PARSE_ALL_FOR_1X_2X);
		if ((id->mode == MODE_PARSE_ALL_FOR_1X_2X) )  {
			printf("\n\t\t-i FILE\tinput file.");
			printf("\n\t\t-s INT\tmaximum read depth.  Default is %d ( any read deph > %d will be counted as %d ).", MAX_READ_DEPTH, MAX_READ_DEPTH);
			// bedtools coverage -abam $i -b target.bed -hist
		}
	}

	

	printf("\n\n");
}


/**
	bedtools main
**/
void bedtools_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();

	int maxReadDepth = MAX_READ_DEPTH;

	while ((c = getopt(argc, argv, "m:s:i:v")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 's':
			maxReadDepth = atoi(optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			bedtools_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\ni");
			exit(0);
		}
	}

	if (id->mode == 0) {
		bedtools_main_Usage(argc,argv,id);
		exit(1);
	}


	        
	if (strlen(id->inputFileName)==0)  {
		bedtools_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	}

	if (id->mode == MODE_PARSE_ALL_FOR_1X_2X) {
		bedtools_MODE_PARSE_ALL_FOR_1X_2X(id,od,maxReadDepth);
	}
}

