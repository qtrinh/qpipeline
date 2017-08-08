//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bedtools.h"



/*
	module load BEDTools; bedtools coverage -abam ../GTS_0001_Li_P_PE_253_TS_1001.bam -b /.mounts/labs/hudsonlab/public/qtrinh/bam.test/target.bed -hist | grep ^all > GTS_0001_Li_P_PE_253_TS_1001.bam


	all     0       2528    1057642 0.0023902
	all     1       135     1057642 0.0001276
	all     2       1335    1057642 0.0012622
	all     3       199     1057642 0.0001882
	all     4       1383    1057642 0.0013076
	all     5       241     1057642 0.0002279
	all     6       1187    1057642 0.0011223
	all     7       420     1057642 0.0003971
	all     8       1240    1057642 0.0011724
	all     9       255     1057642 0.0002411
	all     10      970     1057642 0.0009171
	all     11      158     1057642 0.0001494
	all     12      728     1057642 0.0006883
	all     13      175     1057642 0.0001655
	all     14      522     1057642 0.0004936
	all     15      130     1057642 0.0001229
	all     16      583     1057642 0.0005512
	all     17      149     1057642 0.0001409
	all     18      432     1057642 0.0004085
	all     19      126     1057642 0.0001191
	*/
void bedtools_MODE_PARSE_ALL_FOR_1X_2X(struct input_data *id, struct output_data *od, int maxReadDepth) {

	int DEPTH_COL = 2;
	int BASES_AT_DEPTH_COL = 3; // number of bases at depth indicated in column 2
	int TARGET_LENGTH_COL = 4;
	int PERCENT_AT_READ_DEPTH_COL = 5;

	double d;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}


	double total = 0;
	int done = 0;
	// read input file 
   while ((!done) && (fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
		if (id->verbose) {
      	printf ("\n\n\n[%s:%d] - \t%s", __FILE__, __LINE__, id->line);
		}
		// remove new line at the end 
		id->line[strlen(id->line)-1] = '\0';

		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);

		// stop when we reach max read depth.
		if ( atoi(id->columns[DEPTH_COL]) >= maxReadDepth) {
			printf ("%s\t%4.2f\n", id->line, (1.0-total)*100);
			done = 1;
			continue;
		}

		d = atof(id->columns[PERCENT_AT_READ_DEPTH_COL]);
		total += d;
		printf ("%s\t%4.2f\n", id->line, (1.0-total)*100);
	
		input_data_freeMem(id->columns, id->n);
   }
   fclose(id->inputFile);
}


