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
#include "fasta.h"
#include "common.h"

/**
  fasta usage 
  **/
void fasta_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing VCF files.");
	printf( "\n\n\t%s fasta [ -m mode ] [ options  ]",
			argv[0]);
	printf("\n");
	
	if ((id->mode == FASTA_MODE_GET_DNA_SEQUENCE) || (id->mode == 0))  {
		printf("\n\t-m %d\textract fasta sequences from input FASTA file",FASTA_MODE_GET_DNA_SEQUENCE);
		if ((id->mode == FASTA_MODE_GET_DNA_SEQUENCE) )  {
			printf("\n\t\t-i FILE\tinput FASTA file");
			printf("\n\t\t-T FILE\t3-column tab-delimited target file ( in the form: chr	start	end ) ");
			printf("\n\t\t-r STR\tregion to be extracted ( eg, chr1:1000-2000 ) ");
			//printf("\n\t\t-v\t\tturn on verbose for debugging");
			printf("\n\n\t\tNOTE: Only one of -T or -r is needed."); 

			printf ("\n\n\t\tExample:\n\t\t\t%s fasta -m %d -i test_data/fasta/WS220.fasta -T test_data/fasta/targets.txt", argv[0], FASTA_MODE_GET_DNA_SEQUENCE);
		}
	}

	printf("\n\n");
}


/**
	fasta main
**/
void fasta_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	//struct output_data *od = output_data_init();
	struct fasta *fa = fasta_init();

	while ((c = getopt(argc, argv, "r:n:m:i:k:s:z:T:vH")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'i':
			strcpy(fa->inputFileName, optarg);
			break;
		case 'T':
			strcpy(id->targetFileName, optarg);
			break;
		case 'r':
			strcpy(fa->region, optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			fasta_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\ni");
			exit(0);
		}
	}

	if (id->mode == 0) {
		fasta_main_Usage(argc,argv,id);
		exit(1);
	}

	if (strlen(fa->inputFileName)==0)  {
		fasta_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	}
		
	if (id->mode == FASTA_MODE_GET_DNA_SEQUENCE) {
		fasta_FASTA_MODE_GET_DNA_SEQUENCE(id, fa);
	}
}

