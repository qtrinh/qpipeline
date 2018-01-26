//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "ucsc_refgene.h" 



/**
  parse refgene file 
  **/
void ucsc_refgene_MODE_PARSE_REFGENE(struct input_data *id, struct output_data *od)  {

   char line[MAX_CHAR_PER_LINE];
	int i;
	        
	char **columns;
	int n;
	char str[1024]; 
	int strIndex ;

	int numberOfExons;
	char *startExonPtr, *endExonPtr, *exonFramesPtr;
	unsigned long cdsStart, cdsEnd, exonStart, exonEnd;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}


	// 14 column headers
	printf ("#name_of_gene");
	printf ("\tchromosome");
	printf ("\tstrand");
	printf ("\ttranscription_start");
	printf ("\ttranscription_end");

	printf ("\tcoding_start");
	printf ("\tcoding_end");
	printf ("\texon_count");

	printf ("\texon_start");
	printf ("\texon_end");
	printf ("\tscore");
	printf ("\tname2_of_gene");
	printf ("\tcdsStartStat");
	printf ("\texon_frame");
	printf ("\n");


	// read input file 
   while ((fgets(line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		// skip lines with comments 
		if ((line[0] == '#') && (line[1] == '#') ) {
			printf("%s", line);
			continue;
		}
		
		if (id->verbose) {
			printf ("\n\n=====================================");
      	printf ("\n[%s:%d] - %s", __FILE__, __LINE__, line);
		}

		// remove new line at the end 
		line[strlen(line)-1] = '\0';

		columns = input_data_parseLineMem(id, line, '\t', &n);
		if (id->verbose) {
			printf ("\n");
			input_data_printParsedLineMem(columns, n);
			printf ("\ntranscript start/end\t%s:%s-%s", columns[REFGENE_COLUMN_CHROM],columns[REFGENE_COLUMN_TX_START],columns[REFGENE_COLUMN_TX_END]);
			printf ("\ncoding start/end\t%s:%s-%s", columns[REFGENE_COLUMN_CHROM],columns[REFGENE_COLUMN_CDS_START],columns[REFGENE_COLUMN_CDS_END]);
			printf ("\n\n");
		}

		numberOfExons = atoi(columns[REFGENE_COLUMN_EXON_COUNT]);
		startExonPtr = columns[REFGENE_COLUMN_EXON_STARTS];
		endExonPtr = columns[REFGENE_COLUMN_EXON_ENDS];
		exonFramesPtr = columns[REFGENE_COLUMN_EXON_FRAMES];

		i = 1;
		while (i <= numberOfExons) {

			printf ("%s", columns[REFGENE_COLUMN_NAME]);
			printf ("\t%s", columns[REFGENE_COLUMN_CHROM]); 
			printf ("\t%s", columns[REFGENE_COLUMN_STRAND]); 

			// Transcription start and end position 
			printf ("\t%s", columns[REFGENE_COLUMN_TX_START]); 
			printf ("\t%s", columns[REFGENE_COLUMN_TX_END]); 
			// Coding region start and end 
			printf ("\t%s", columns[REFGENE_COLUMN_CDS_START]); 
			printf ("\t%s", columns[REFGENE_COLUMN_CDS_END]); 

			printf ("\t%s", columns[REFGENE_COLUMN_EXON_COUNT]); 
			
			printf ("\t");
			strIndex = 0;
			while (*startExonPtr != ',') {
				//printf ("%c", *startExonPtr);
				str[strIndex] = *startExonPtr;
				strIndex++;
				startExonPtr++;
			}
			startExonPtr++;
			str[strIndex] =  '\0';
			exonStart = atol(str);

			printf ("%s\t", str);
			strIndex = 0;
			while (*endExonPtr != ',') {
				//printf ("%c", *endExonPtr);
				str[strIndex] = *endExonPtr;
				strIndex++;
				endExonPtr++;
			}
			endExonPtr++;

			str[strIndex] =  '\0';
			exonEnd = atol(str);

			printf ("%s\t", str);

/*
			if (columns[REFGENE_strand_COLUMN][0] == '+') {
				while (*startExonPtr != ',') {
					printf ("%c", *startExonPtr);
					startExonPtr++;
				}
				startExonPtr++;

				printf ("\t");
				while (*endExonPtr != ',') {
					printf ("%c", *endExonPtr);
					endExonPtr++;
				}
				endExonPtr++;
			} else {
				while (*endExonPtr != ',') {
					printf ("%c", *endExonPtr);
					endExonPtr++;
				}
				endExonPtr++;

				printf ("\t");
				while (*startExonPtr != ',') {
					printf ("%c", *startExonPtr);
					startExonPtr++;
				}
				startExonPtr++;
			}
*/
			printf ("\t%s", columns[REFGENE_COLUMN_SCORE]); 
			printf ("\t%s", columns[REFGENE_COLUMN_NAME2]); 
			printf ("\t%s", columns[REFGENE_COLUMN_CDS_START_STAT]); 
			printf ("\t%s", columns[REFGENE_COLUMN_CMPL]); 

			printf ("\t");
			while (*exonFramesPtr != ',') {
				printf ("%c", *exonFramesPtr);
				exonFramesPtr++;
			}
			exonFramesPtr++;

			printf ("\n");

			i++;
		}
		
		input_data_freeMem(columns, n);
   }   
   fclose(id->inputFile);
}



/**
  parse refgene file for coding region length 
  **/
void ucsc_refgene_MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH(struct input_data *id, struct output_data *od, int exonPadding)  {

   char line[MAX_CHAR_PER_LINE];
	int i,j;
	        
	char **columns;
	int n;
   
	char start[1024];
	char end[1024];

	int numberOfExons;
	char *startExonPtr, *endExonPtr, *exonFramesPtr;
	char *cdStartPtr, *cdEndPtr ;
	char exonFrame[1024];
	unsigned long totalCodingLength;
	unsigned long totalCodingNonCodingLength; // total length of all exons including non-coding parts 
	unsigned long cdStart, cdEnd;
	unsigned long eStart, eEnd; // exon start and exon end 


   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}


	// 14 column headers
	printf ("#name_of_gene");
	printf ("\tname2_of_gene");
	printf ("\texon_count");
	printf ("\texon_count_coding");
	printf ("\ttotal_coding_length");
	printf ("\texon_padding");
	printf ("\n");

	int codingExonCount ;

	// read input file 
   while ((fgets(line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		// skip lines with comments 
		if ((line[0] == '#') && (line[1] == '#') ) {
			printf("%s", line);
			continue;
		}
		
		if (id->verbose) {
			printf ("\n\n=====================================");
      	printf ("\n[%s:%d] - %s", __FILE__, __LINE__, line);
		}

		// remove new line at the end 
		line[strlen(line)-1] = '\0';

		columns = input_data_parseLineMem(id, line, '\t', &n);
		if (id->verbose) {
			input_data_printParsedLineMem(columns, n);
		}

		codingExonCount = 0;
		numberOfExons = atoi(columns[REFGENE_COLUMN_EXON_COUNT]);

		cdStart = atoi(columns[REFGENE_COLUMN_CDS_START]);
		cdEnd = atoi(columns[REFGENE_COLUMN_CDS_END]);


		startExonPtr = columns[REFGENE_COLUMN_EXON_STARTS];
		endExonPtr = columns[REFGENE_COLUMN_EXON_ENDS];
		exonFramesPtr = columns[REFGENE_COLUMN_EXON_FRAMES];

		totalCodingLength = 0;
		totalCodingNonCodingLength = 0;
		i = 1;
		while (i <= numberOfExons) {
			
			j = 0;
			while (*startExonPtr != ',') {
				start[j] = *startExonPtr;
				startExonPtr++;
				j++;
			}
			startExonPtr++;
			start[j] = '\0';

			j = 0;
			while (*endExonPtr != ',') {
				end[j] = *endExonPtr;
				endExonPtr++;
				j++;
			}
			endExonPtr++;
			end[j] = '\0';

			eStart = atoi(start);eEnd=atoi(end);

			// this will include coding and non coding part of the exon
			totalCodingNonCodingLength += ((eEnd + exonPadding) - (eStart-exonPadding));
			
			j = 0;
			while (*exonFramesPtr != ',') {
				exonFrame[j] = *exonFramesPtr;
				exonFramesPtr++;
				j++;
			}
			exonFramesPtr++;
			exonFrame[j] = '\0';

				
			// a flag indicating wither an adjustment is made to the exon or not 
			int adj = 0;
			
			if (atoi(exonFrame) >= 0) {
				// adjust if the exon start and end are outside of the coding regions 
				if (eStart < cdStart) {
					adj = 1;
					eStart = cdStart;
				}
				if (eEnd > cdEnd) {
					adj = 1;
					eEnd = cdEnd;
				}
				eStart -= exonPadding;
				eEnd += exonPadding;
				
				if (adj && id->verbose) {
					printf ("\ncomment: adjustment made to coding parts of this exon");
					printf ("\nbecause eithe exon start or end is outside of the actual");
					printf ("\ncoding region of this gene");
					printf ("\n");
				}

				// don't need to do +1 to be consistent with what are on the UCSC browser 
				totalCodingLength += (eEnd - eStart);
				codingExonCount++;
			}

			if (id->verbose) {
				if (adj) { 
					// if an adjustment was made 
					printf ("\nCD\t%s:%ld-%ld", columns[REFGENE_COLUMN_CHROM],cdStart, cdEnd);
					printf ("\nexon\t%s:%s-%s", columns[REFGENE_COLUMN_CHROM],start,end);
					printf ("\nADJ\t%s", columns[REFGENE_COLUMN_CHROM]);
					printf (":%ld", eStart);
					printf ("-%ld", eEnd);
				} 

				printf ("\n");
				printf ("\nDETAIL\t%s", columns[REFGENE_COLUMN_NAME]);
				printf ("\t%s", columns[REFGENE_COLUMN_NAME2]); 
				printf ("\t%s", columns[REFGENE_COLUMN_EXON_COUNT]);

				printf ("\t%s:%ld-%ld", columns[REFGENE_COLUMN_CHROM],cdStart, cdEnd);
				printf ("\t%s", columns[REFGENE_COLUMN_CHROM]);
				printf (":%ld", eStart);
				printf ("-%ld", eEnd);
				printf ("\t%ld", eEnd - eStart );
				printf ("\t%s", exonFrame);
				if (atoi(exonFrame) >= 0) 
					printf ("\tCODING_EXON");
				else 
					printf ("\tnon_coding_exon");
				printf ("\t%d", exonPadding);
				printf ("\n\n\n");
			}


			i++;
		}

		if (id->verbose)
			printf("\n\n");

		printf ("%s", columns[REFGENE_COLUMN_NAME]);
		printf ("\t%s", columns[REFGENE_COLUMN_NAME2]); 
		printf ("\t%s_exons", columns[REFGENE_COLUMN_EXON_COUNT]); 
		printf ("\t%ld", totalCodingNonCodingLength);
		printf ("\t%d_coding_exons", codingExonCount);
		printf ("\t%ld", totalCodingLength);
		printf ("\n");

		input_data_freeMem(columns, n);
   }   
   fclose(id->inputFile);
}

