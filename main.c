//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void txt_main (int argc, char *argv[]);
void fasta_main (int argc, char *argv[]);
void fastq_main (int argc, char *argv[]);
void tabix_main (int argc, char *argv[]);
void vcf_main (int argc, char *argv[]);
void bedtools_main (int argc, char *argv[]);

/** 
  main usage 

 **/
void usage(int argc, char *argv[]) {
	printf("\n\n");
	printf("%s", AUTHORS );
	printf("\nqpipeline is an annotation and a collection of utils for Next Gen Sequencing (NGS) data.");
	printf("\n\nUsage:");
	printf("\n\t%s txt\t", argv[0]);
	printf("\n\t%s fasta\t", argv[0]);
	printf("\n\t%s fastq\t", argv[0]);
	printf("\n\t%s tabix\t", argv[0]);
	printf("\n\t%s vcf\t", argv[0]);
	printf("\n\t%s bedtools\t", argv[0]);
	printf("\n\n");
	exit(0);
}

/**
  main line 

 **/
int main(int argc, char *argv[]) {
		
	if (argc == 1) {
		usage(argc, argv);
	}
		
	if (strstr(argv[1], "txt") ) {
		txt_main(argc, argv);
	} else if (strstr(argv[1], "fasta") ) {
		fasta_main(argc, argv);
	} else if (strstr(argv[1], "fastq") ) {
		fastq_main(argc, argv);
	} else if (strstr(argv[1], "vcf") ) {
		vcf_main(argc, argv);
	} else if (strstr(argv[1], "tabix") ) {
		tabix_main(argc, argv);
	} else if (strstr(argv[1], "bedtools") ) {
		bedtools_main(argc, argv);
	}

	return 0;
}



