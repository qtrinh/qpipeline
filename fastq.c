//
// written by Quang Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"
#include "input_data.h"
#include "output_data.h"
#include "acgtn.h"
#include "fastq.h"

/**
  initialize a fastq structure 
  **/
struct fastq * fastq_init() {

	struct fastq *fq = (struct fastq *) malloc (sizeof (struct fastq));

	fq->acgtn = acgtn_Init();

	return fq;
}

void fastq_reset(struct fastq *fq) {

	int i;

	fq->numberOfReads = 0;

	for (i = 0; i <= MAX_READ_LENGTH;i++) {
		fq->numberOfReadsWithN[i] = 0;
		fq->numberOfNPerCycle[i] = 0;

		fq->averageQualityPerCycle[i] = 0;
	}

}


/*
	convert fastq count to output 
	*/
void fastq_MODE_FASTQ_COUNT_ENTRY_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) {

	od->n = 1;
	sprintf(od->header[od->n], "#HEADER");
	sprintf(od->data[od->n], "DATA");

	od->n++;
	sprintf(od->header[od->n], "input_file_name");
	// take the prefix if input file is '/dev/stdin
	if (strstr(id->inputFileName,"/dev/stdin")) {
		sprintf(od->data[od->n], "%s",id->inputFilePrefix);
	} else {
		sprintf(od->data[od->n], "%s",id->inputFileName);
	}
	
	od->n++;
	sprintf(od->header[od->n], "number_of_reads");
	sprintf(od->data[od->n], "%ld",fq->numberOfReads);
}



/**
  count the number of entries in input file with 0N, 1N, 2N, and so on 
  **/
void fastq_process_FASTQ_File(struct input_data *id, struct output_data *od, struct fastq *fq) {

	char *ptr;
	int i,n;
	int cycle;
	int baseIndex;

	n = 0;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	fq->numberOfLines = 0;
	fq->numberOfReads = 0;


	acgtn_reset(fq->acgtn);

	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
		fq->numberOfLines++;

		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		if (id->verbose)  {
			if ((fq->numberOfLines % 4) == 1) 
				// only print when reading the first line of the 'read' ( FASTQ read has 4 lines)
      		printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - \t%ld\t%s\n", __FILE__, __LINE__, fq->numberOfLines %4, id->line);
		}	

		if ((fq->numberOfLines % 4) == 1)  {
			// read ID

			// increment read count when reading the first line of the read
			fq->numberOfReads++;
		} else if ((fq->numberOfLines % 4) == 2) {
			// DNA sequence 

			ptr = id->line;
			n = 0;//number of N in a read 
			cycle = 1;
			while (*ptr) {
				baseIndex = acgtn_getIndex((char)*ptr);

				if (baseIndex  == BASE_N) {
					// keep track of now many Ns in a read 
					n++;

					fq->numberOfNPerCycle[cycle]++;
				}

				// count base based on the index
				fq->acgtn->count[baseIndex]++;

				// count base based on cycle 
				fq->acgtn->perCycleCount[cycle][baseIndex]++;

				cycle++;
				ptr++;
			}
			fq->readLength = cycle-1;
			fq->numberOfReadsWithN[n]++;
		} else if ((fq->numberOfLines % 4) == 0) {
			// base quality 
		
			cycle = 1;
			ptr = id->line;
			while (*ptr) {
				// running average  - see http://en.wikipedia.org/wiki/Moving_average
				if (fq->numberOfReads == 1) {
					fq->averageQualityPerCycle[cycle] = *ptr;
				} else {
					fq->averageQualityPerCycle[cycle] = fq->averageQualityPerCycle[cycle] + (*ptr - fq->averageQualityPerCycle[cycle])/(fq->numberOfReads*1.0);
				}
				ptr++;
				cycle++;
			}

		}
			
		// for debugging 
		if (id->verbose && ((fq->numberOfLines % 4) == 0)) {
			// show when 
			if (id->mode == MODE_FASTQ_COUNT_N){
				if (n > 0)
					printf ("\n[%s:%d] - %d Ns ( FOUND one or more Ns )\n\n", __FILE__, __LINE__, n);
				else 
					printf ("\n[%s:%d] - %d Ns\n\n", __FILE__, __LINE__, n);
			}

			if (id->mode == MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE) {
				printf ("\n[%s:%d] - running average quality per cycle:\n", __FILE__, __LINE__);
				for (i = 1; i <= MAX_READ_LENGTH; i++) {
					printf ("%2.f\t", fq->averageQualityPerCycle[i]);
				}
				printf ("\n\n");
			}
			
			if (id->mode == MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION) {
				printf ("\n[%s:%d] - ACGTN distributions with respect to total:\n", __FILE__, __LINE__);
				for (i = 0; i < NUMBER_OF_BASES; i++) {
					printf ("%2.f%c\t", fq->acgtn->count[i], acgtn_getBase(i));
				}
				printf ("\n\n");
				
				/*
				printf ("\n[%s:%d] - ACGTN distributions per cycle:\n", __FILE__, __LINE__);
				for (i = 1; i <= MAX_READ_LENGTH ; i++) {
					printf ("cycle %d\t", i);
					for (j = 0; j < NUMBER_OF_BASES; j++) {
						printf ("%2.f%c\t", fq->acgtn->perCycleCountUpperCase[i][j], acgtn_getBase(j));
					}
					printf ("\n");
				}
				printf ("\n");
				*/
			}

		}
   }   
   fclose(id->inputFile);
}


/*
	convert fastq count N to output 
	*/
void fastq_MODE_FASTQ_COUNT_N_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) {

	od->n = 1;
	sprintf(od->header[od->n], "#HEADER");
	sprintf(od->data[od->n], "DATA");

	od->n++;
	sprintf(od->header[od->n], "input_file_name");
	// take the prefix if input file is '/dev/stdin
	if (strstr(id->inputFileName,"/dev/stdin")) {
		strcpy(id->inputFileName,id->inputFilePrefix);
		sprintf(od->data[od->n], "%s",id->inputFileName);
	} else {

		sprintf(od->data[od->n], "%s",id->inputFileName);
	}
	od->n++;
	sprintf(od->header[od->n], "number_of_reads");
	sprintf(od->data[od->n], "%ld",fq->numberOfReads);

	od->n++;
	sprintf(od->header[od->n], "number_of_Ns");
	sprintf(od->data[od->n], "percent_of_reads_with_Ns");
	
	int i;
	//for (i = 0; i <= MAX_READ_LENGTH; i++) {
	for (i = 0; i <= fq->readLength; i++) {
		od->n++;
		sprintf(od->header[od->n], "%d",i);
		sprintf(od->data[od->n], "%.2f",(100.0*fq->numberOfReadsWithN[i])/fq->numberOfReads);
	}
}


/*
	*/
void fastq_MODE_FASTQ_CALCULATE_AVERAGE_QUALITY_PER_CYCLE_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) {

	od->n = 1;
	sprintf(od->header[od->n], "#HEADER");
	sprintf(od->data[od->n], "DATA");

	od->n++;
	sprintf(od->header[od->n], "input_file_name");
	// take the prefix if input file is '/dev/stdin
	if (strstr(id->inputFileName,"/dev/stdin")) {
		strcpy(id->inputFileName,id->inputFilePrefix);
		sprintf(od->data[od->n], "%s",id->inputFileName);
	} else {

		sprintf(od->data[od->n], "%s",id->inputFileName);
	}
	od->n++;
	sprintf(od->header[od->n], "number_of_reads");
	sprintf(od->data[od->n], "%ld",fq->numberOfReads);

	od->n++;
	sprintf(od->header[od->n], "cycle_number");
	sprintf(od->data[od->n], "average_quality_value");
	
	int i;
	//for (i = 1; i <= MAX_READ_LENGTH; i++) {
	for (i = 1; i <= fq->readLength; i++) {
		od->n++;
		sprintf(od->header[od->n], "%d",i);
		sprintf(od->data[od->n], "%.2f",fq->averageQualityPerCycle[i]);
	}
}






/*
	*/
void fastq_MODE_FASTQ_CALCULATE_PERCENT_OF_N_PER_CYCLE_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) {

	od->n = 1;
	sprintf(od->header[od->n], "#HEADER");
	sprintf(od->data[od->n], "DATA");

	od->n++;
	sprintf(od->header[od->n], "input_file_name");
	// take the prefix if input file is '/dev/stdin
	if (strstr(id->inputFileName,"/dev/stdin")) {
		strcpy(id->inputFileName,id->inputFilePrefix);
		sprintf(od->data[od->n], "%s",id->inputFileName);
	} else {

		sprintf(od->data[od->n], "%s",id->inputFileName);
	}
	od->n++;
	sprintf(od->header[od->n], "number_of_reads");
	sprintf(od->data[od->n], "%ld",fq->numberOfReads);

	od->n++;
	sprintf(od->header[od->n], "cycle_number");
	sprintf(od->data[od->n], "percent_of_reads_with_Ns");
	
	int i;
	//for (i = 1; i <= MAX_READ_LENGTH; i++) {
	for (i = 1; i <= fq->readLength; i++) {
		od->n++;
		sprintf(od->header[od->n], "%d",i);
		sprintf(od->data[od->n], "%.2f",(100.0*fq->numberOfNPerCycle[i])/fq->numberOfReads);
	}
}






/*
	*/
void fastq_MODE_FASTQ_CALCULATE_ACGTN_DISTRIBUTION_2_output_data(struct input_data *id, struct output_data *od, struct fastq *fq) {

	od->n = 1;
	sprintf(od->header[od->n], "#HEADER");
	sprintf(od->data[od->n], "DATA");

	od->n++;
	sprintf(od->header[od->n], "input_file_name");
	// take the prefix if input file is '/dev/stdin
	if (strstr(id->inputFileName,"/dev/stdin")) {
		strcpy(id->inputFileName,id->inputFilePrefix);
		sprintf(od->data[od->n], "%s",id->inputFileName);
	} else {

		sprintf(od->data[od->n], "%s",id->inputFileName);
	}
	od->n++;
	sprintf(od->header[od->n], "number_of_reads");
	sprintf(od->data[od->n], "%ld",fq->numberOfReads);
	
	unsigned long totalBases = 0;
	int i;
	for (i = 0; i < NUMBER_OF_BASES; i++) {
		totalBases += fq->acgtn->count[i];
	}
	
	od->n++;
	sprintf(od->header[od->n], "total_number_of_bases");
	sprintf(od->data[od->n], "%ld",totalBases);
	
	for (i = 0; i < NUMBER_OF_BASES; i++) {
		od->n++;
		if (acgtn_getBase(i) == '*')
			sprintf(od->header[od->n], "other");
		else
			sprintf(od->header[od->n], "%c",acgtn_getBase(i));
		sprintf(od->data[od->n], "%ld",(unsigned long )fq->acgtn->count[i]);
	}

	/*
	od->n++;
	sprintf(od->header[od->n], "base");
	sprintf(od->data[od->n], "percent_composition");
	*/
	
	
	for (i = 0; i < NUMBER_OF_BASES; i++) {
		od->n++;
		if (acgtn_getBase(i) == '*')
			sprintf(od->header[od->n], "%%other");
		else
			sprintf(od->header[od->n], "%%%c",acgtn_getBase(i));
		//sprintf(od->data[od->n], "%ld\t%.2f",(unsigned long )fq->acgtn->countUpperCase[i], (100.0 * fq->acgtn->countUpperCase[i])/ totalBases);
		sprintf(od->data[od->n], "%.2f",(100.0 * fq->acgtn->count[i])/ totalBases);
	}
}



/**
  **/
void fastq_MODE_FASTQ_COUNT_BASE_QUALITY_HISTOGRAM(struct input_data *id, struct output_data *od, struct fastq *fq) {

	char *ptr;
	int i,j,n;
	int cycle;
	int baseIndex;
	int readLength;

	int baseIndices[MAX_READ_LENGTH];
	unsigned long baseQualityCount[MAX_READ_LENGTH];// count how many bases with Q30, Q31, and so on.

	n = 0;

	long qualities[NUMBER_OF_BASES][MAX_BASE_QUALITY+1];
	for (i = 0; i < NUMBER_OF_BASES; i++) {
		for (j = 0; j <= MAX_BASE_QUALITY; j++) {
			qualities[i][j] = 0;
		}
	}

	char fileName[1024];
	if (strstr(id->inputFileName,"/dev/stdin")) 
		strcpy(fileName,id->inputFilePrefix);
	else
		strcpy(fileName,id->inputFileName);


   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	fq->numberOfLines = 0;
	fq->numberOfReads = 0;

	unsigned long totalNumberOfBases = 0;

	acgtn_reset(fq->acgtn);



	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
		fq->numberOfLines++;

		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		if (id->verbose)  {
			if ((fq->numberOfLines % 4) == 1) 
				// only print when reading the first line of the 'read' ( FASTQ read has 4 lines)
      		printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - \t%ld\t%s\n", __FILE__, __LINE__, fq->numberOfLines %4, id->line);
		}	

		if ((fq->numberOfLines % 4) == 1)  {
			// read ID

			// increment read count when reading the first line of the read
			fq->numberOfReads++;
		} else if ((fq->numberOfLines % 4) == 2) {
			// DNA sequence 

			ptr = id->line;
			n = 0;//number of N in a read 
			cycle = 1;
			while (*ptr) {
				baseIndex = acgtn_getIndex((char)*ptr);

				baseIndices[cycle] = baseIndex;

				cycle++;
				ptr++;

				totalNumberOfBases++;
			}
			fq->readLength = cycle-1;
		} else if ((fq->numberOfLines % 4) == 0) {
			// base quality 		
				
			if (id->verbose) {
				printf ("\n");
				for (i = 1; i<= fq->readLength;i++) {
					printf ("%d\t", baseIndices[i]);
				}
				printf ("\n");
			}

			ptr = id->line;
			cycle = 1;
			while (*ptr) {
				int q = (int)*ptr - 33;
				if (id->verbose) {
					printf ("%d\t", q);
				}
				qualities[baseIndices[cycle]][q]++;
				ptr++;
				cycle++;
			}
				
			if (id->verbose) {
				printf ("\n");
			}
		}
   }   
   fclose(id->inputFile);

	printf ("#file_name\tbase\tbase_quality\tcount\tpercent");
	// (NUMBER_OF_BASES-1) because we don't want non A,C,G,T, and N bases 
	for (i = 0; i < (NUMBER_OF_BASES-1); i++) {
		for (j = 1; j <= MAX_BASE_QUALITY; j++) {
			printf ("\n%s\t%c\t%d\t%ld\t%.2f", fileName,acgtn_getBase(i),j, qualities[i][j], (100.0*qualities[i][j])/totalNumberOfBases);;
		}
	}
	printf ("\n");
}


