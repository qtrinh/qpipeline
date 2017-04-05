//
// written by Quang Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "acgtn.h"

#ifndef FASTQ_H
#define FASTQ_H 



#define MODE_FASTQ_COUNT_ENTRY 1001

#define MODE_FASTQ_COUNT_N 1002

#define MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM 1005 // tally base quality for each of A,C,G,T, and N

#define MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE 1010
#define MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION 1501

#define MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE 2001


struct fastq {
	// number of lines in input file ( this is necessary for checking if the number of lines is multiple of 4 )
	unsigned long numberOfLines;
	
	// number of reads 
	unsigned long numberOfReads;
	int readLength;

	// number of reads with Ns
	// index is the number of Ns in a read 
	double numberOfReadsWithN[MAX_READ_LENGTH+1];
	// number of Ns per cycle 
	double numberOfNPerCycle[MAX_READ_LENGTH+1];
	
	// average quality per cycle
	double averageQualityPerCycle[MAX_READ_LENGTH+1];

	
	acgtn_t * acgtn;

} ;

struct fastq * fastq_init();
void fastq_MODE_FASTQ_COUNT_ENTRY_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq);

void fastq_MODE_FASTQ_COUNT_N_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq);

void fastq_MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM(struct input_data *id, struct output_data *od, struct fastq *fq) ;

void fastq_MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM(struct input_data *id, struct output_data *od, struct fastq *fq) ;

void fastq_MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) ;

void fastq_MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) ;

void fastq_MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) ;

void fastq_process_FASTQ_File(struct input_data *id, struct output_data *od, struct fastq *fq) ;


#endif 
