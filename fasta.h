//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "faidx.h"

#ifndef FASTA_H
#define FASTA_H 

#include "input_data.h"
#include "output_data.h"

#define FASTA_MODE_GET_DNA_SEQUENCE 1000


struct fasta {

	char inputFileName[1024];
	FILE *inputFile;
	
	char inputFAIFileName[1024];
	FILE *inputFAIFile;

	// from samtools 
	faidx_t *fai;
	
	char region[1024];
	char *regionData;
	int regionDataLength;
} ;


struct fasta * fasta_init();
void fasta_FASTA_MODE_GET_DNA_SEQUENCE(struct input_data *id, struct fasta *fa);
void fasta_printNN(char fastaID[], char *data, int NN) ;
void fasta_loadFileName(struct fasta *fa, char fastaFileName[]) ;
void fasta_loadFile(struct fasta *fa) ;


#endif  

