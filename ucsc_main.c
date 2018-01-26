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
#include "ucsc_refgene.h"
#include "common.h"


/**
  ucsc usage 
  **/
void ucsc_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing UCSC files files.");
	printf( "\n\n\t%s ucsc [ -m mode ] [ options  ]",
			argv[0]);
	printf("\n");

	
	if ((id->mode == MODE_PARSE_REFGENE) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse refgene file ",MODE_PARSE_REFGENE);
		if ((id->mode == MODE_PARSE_REFGENE) )  {
			printf("\n\t\t-i FILE\tinput file.");
		}
	}
	if ((id->mode == MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse refgene and tally up coding regions for each gene",MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH);
		if ((id->mode == MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH) )  {
			printf("\n\t\t-i FILE\tinput file.");
			printf("\n\t\t-p INT\tpadding bases to each exon (OPTIONAL).");
		}
	}


	printf("\n\n");
}


/**
	ucsc main
**/
void ucsc_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();
	int padding = 0;

	while ((c = getopt(argc, argv, "p:m:s:i:v")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'p':
			padding = atoi(optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			ucsc_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\ni");
			exit(0);
		}
	}

	if (id->mode == 0) {
		ucsc_main_Usage(argc,argv,id);
		exit(1);
	}


	        
	if (strlen(id->inputFileName)==0)  {
		ucsc_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	}

	if (id->mode == MODE_PARSE_REFGENE) {
		ucsc_refgene_MODE_PARSE_REFGENE(id,od);
	} else if (id->mode == MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH) {
		ucsc_refgene_MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH(id,od,padding);
	}
}

