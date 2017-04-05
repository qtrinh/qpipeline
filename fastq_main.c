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

#include "fastq.h"

/**
  fastq usage 
  **/
void fastq_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing FASTQ files.");
	printf( "\n\n\t%s fastq [ -m mode ] [ options ]",
			argv[0]);
	printf("\n");
	
	if ((id->mode == MODE_FASTQ_COUNT_ENTRY) || (id->mode == 0))  {
		printf("\n\t-m %d\tcount the number of reads.",MODE_FASTQ_COUNT_ENTRY);
		if ((id->mode == MODE_FASTQ_COUNT_ENTRY) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			//printf("\n\t\t-v\tturn on verbose for debugging");
			printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq", argv[0], MODE_FASTQ_COUNT_ENTRY);
		}
	}

	if ((id->mode == MODE_FASTQ_COUNT_N) || (id->mode == 0))  {
		printf("\n\t-m %d\tcount number of reads containing N bases (e.g., zero N, one N, two Ns, and so on )",MODE_FASTQ_COUNT_N);
		if ((id->mode == MODE_FASTQ_COUNT_N) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq ", argv[0], MODE_FASTQ_COUNT_N);
		}
	}
	if ((id->mode == MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM) || (id->mode == 0))  {
		printf("\n\t-m %d\tcount base quality histograms for each of A,C,G,T,N .",MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM);
		if ((id->mode == MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			//printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq ", argv[0], MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM);
		}
	}
	if ((id->mode == MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE) || (id->mode == 0))  {
		printf("\n\t-m %d\tcalculate percentages of Ns per cycle",MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE);
		if ((id->mode == MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq ",argv[0], MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE);
		}
	}
	
	if ((id->mode == MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION) || (id->mode == 0))  {
		printf("\n\t-m %d\tcalculate A,C,G,T,N distributions.",MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION);
		if ((id->mode == MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq ", argv[0], MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION);
		}
	}

	
	if ((id->mode == MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE) || (id->mode == 0))  {
		printf("\n\t-m %d\tcalculate average quality per cycle.",MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE);
		if ((id->mode == MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE) )  {
			printf("\n\t\t-i FILE\tinput FASTQ file.");
			printf("\n\t\t-H\tprints outputs in horizontal format.  Default, outputs are printed in vertical format.");
			printf("\n\n\t\tExample:\n\t\t\t%s fastq -m %d -i test_data/fastq/AB1.fastq ", argv[0], MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE);
		}
	}

	printf("\n\n");
}


/**
	fastq main
**/
void fastq_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();
	struct fastq *fq = fastq_init();

	while ((c = getopt(argc, argv, "m:i:q:vH")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'q':
			strcpy(id->inputFilePrefix, optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'H':
			od->outputOrientation = OUTPUT_ORIENTATION_HORIZONTAL;
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			fastq_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above.\n\n");
			exit(0);
		}
	}

	if (id->mode == 0) {
		fastq_main_Usage(argc,argv,id);
		exit(1);
	}

	if (strlen(id->inputFileName)==0)  {
		fastq_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	} else if (strstr(id->inputFileName,"/dev/stdin") && (strlen(id->inputFilePrefix)==0))  {
		fastq_main_Usage(argc,argv,id);
		printf ("\tMissing input file prefix (-q).  Please see usage above!\n\n");
		exit(1);
	}

	if ((id->mode == MODE_FASTQ_COUNT_ENTRY)) {
		fastq_process_FASTQ_File(id, od, fq);
		fastq_MODE_FASTQ_COUNT_ENTRY_2_output_data(id,od,fq);
		output_data_print(od);
	} else if ((id->mode == MODE_FASTQ_COUNT_N)) {
		fastq_process_FASTQ_File(id, od, fq);
		fastq_MODE_FASTQ_COUNT_N_2_output_data(id,od,fq);
		output_data_print(od);
	} else if ((id->mode == MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE)) {
		fastq_process_FASTQ_File(id, od, fq);
		fastq_MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE_2_output_data(id,od,fq);
		output_data_print(od);
	} else if ((id->mode == MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM)) {
		fastq_MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM(id, od, fq);
	} else if ((id->mode == MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION)) {
		fastq_process_FASTQ_File(id, od, fq);
		fastq_MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION_2_output_data(id,od,fq);
		output_data_print(od);
	} else if ((id->mode == MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE)) {
		fastq_process_FASTQ_File(id, od, fq);
		fastq_MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE_2_output_data(id,od,fq);
		output_data_print(od);
	}
}



