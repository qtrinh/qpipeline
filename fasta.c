//
// written by Quang M Trinh <quang.trinh@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "input_data.h"
#include "my_string.h"
#include "fasta.h"
#include "target.h"


/**
  initialize an fasta structure
 **/
struct fasta * fasta_init() {

	struct fasta *fa =  (struct fasta *) malloc (1 * sizeof (struct fasta));

	strcpy(fa->inputFileName, "");

	return fa;
}


/**
  load a fasta file 
  **/
void fasta_loadFile(struct fasta *fa) {

	// load fasta index 
	fa->fai = fai_load(fa->inputFileName);
	if (fa->fai == NULL) {
		printf ("\n\n[%s:%d] - failed to load FASTA file '%s'\n\n", __FILE__, __LINE__, fa->inputFileName);
		exit (1);
	}
}



/**
  get DNA regions from a FASTA file or from an input region 
  **/
void fasta_FASTA_MODE_GET_DNA_SEQUENCE(struct input_data *id, struct fasta *fa ) {

	char tmp[1024];
	int len ;

	fasta_loadFile(fa);

	if (strlen(fa->region) > 0)  {
		// a single target is entered by the user 
		if (id->verbose) {
			printf ("\n[%s:%d] - input region '%s' ", __FILE__, __LINE__, fa->region);
			printf ("\n\n");
			fflush(stdout);
		}

		// fetch it
		fa->regionData = fai_fetch(fa->fai,fa->region,&len);
		my_string_toupper(fa->regionData);
		if(fa->regionData == NULL) {
			printf("\n\n[%s:%d] - ERROR: Given region: %s cannot be found.\n", __FILE__, __LINE__, fa->region);
			printf("\n\n");
			exit(1);
		}	
		// print it out as 60 characters per line 
		fasta_printNN(fa->region, fa->regionData, 60);
		free(fa->regionData);
	} else {
		// if targets are from an input file ( i.e., targetFileName )
		id->targetFile = fopen(id->targetFileName, "r");

		if (id->targetFile == NULL) {
			fprintf(stderr, "\n\n[%s:%d] - ERROR: error open target file '%s'\n\n", __FILE__, __LINE__, id->targetFileName);
			exit(1);
		}
		while (fgets(id->line, MAX_CHAR_PER_LINE, id->targetFile) != NULL) {

			// remove newline 
			id->line[strlen(id->line)-1] = '\0';

			id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));

   		sprintf(fa->region,"%s:%s-%s", id->columns[TARGET_COLUMN_CHR], id->columns[TARGET_COLUMN_START], id->columns[TARGET_COLUMN_END]);
			if (id->verbose) {
				printf ("\n[%s:%d] - region from target file '%s' ( length %d ) ", __FILE__, __LINE__, fa->region, atoi(id->columns[TARGET_COLUMN_END])- atoi(id->columns[TARGET_COLUMN_START]));
				printf ("\n\n");
				fflush(stdout);
			}

			fa->regionData = fai_fetch(fa->fai,fa->region,&len);
			my_string_toupper(fa->regionData);

			//sprintf (tmp,"%s_%s", id->targetFileName,region);
			sprintf (tmp,"%s", fa->region);
			fasta_printNN(tmp, fa->regionData, 60);

			free(fa->regionData);
			input_data_freeMem(id->columns,id->n);
		}
		fclose (id->targetFile);
	}
}


/**
  prints in FASTA format with NN characters per column
 **/
void fasta_printNN(char fastaID[], char *data, int NN) {
	char *ptr = data;
	unsigned long i = 0;

	printf (">%s\n", fastaID);
	while (*ptr) {
		printf("%c",*ptr);
		ptr++;
		i++;
		if ((i % NN) == 0)
			printf("\n");
	}
	printf ("\n");
}




/**
  complement a DNA string 
  **/
void fasta_complement(char dnaString[]) {
	char *ptr = dnaString;
	while (*ptr) {
		if (*ptr == 'a')
			*ptr = 't';
		else if (*ptr == 'c')
			*ptr = 'g';
		else if (*ptr == 'g')
			*ptr = 'c';
		else if (*ptr == 't')
			*ptr = 'a';
		else if (*ptr == 'A')
			*ptr = 'T';
		else if (*ptr == 'C')
			*ptr = 'G';
		else if (*ptr == 'G')
			*ptr = 'C';
		else if (*ptr == 'T')
			*ptr = 'A';
		ptr++;
	}
}

