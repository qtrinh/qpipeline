//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#ifndef MY_TABIX_H
#define MY_TABIX_H 


#include "tabix.h"

#include "input_data.h"
#include "output_data.h"

#define MODE_TABIX_ANNOTATE_VCF_WITH_BED 2000
#define MODE_TABIX_ANNOTATE_VCF_WITH_PILEUP 2005
#define MODE_TABIX_ANNOTATE_VCF_WITH_VCF 2020
#define MODE_TABIX_ANNOTATE_TXT_WITH_BED 2040


// a structure represent a tabix object 
struct my_tabix {

	char inputFileName[1024];
	char inputFilePrefix[1024];
	FILE *inputFile;
	file_format_t inputFileFormat ;

	// indicate whether to consider mismatctched ALT entries 
	int considerMisMatchedALT;

	// tabix  structure 
	tabix_t *t;

	int tid, beg, end;
	ti_iter_t iter;
	int len;

	// region to extract 
	char region[1024];
	// include data from input database or not 
	int includeDataFromDatabase;

	// data returned from querying the database
	const char *data;
	char datacpy[MAX_CHAR_PER_LINE];



	// number of columns of data returned 
	int numberOfColumns;
	char **columns;
	int n;
	
	
	// output data after annotation 
	char outputData[MAX_CHAR_PER_LINE];
	char outputDataDescription[MAX_CHAR_PER_LINE];

	// number of matches from querying the database
	int matches;
	int entryFound;
};


struct my_tabix * tabix_init() ;
void tabix_loadFile(struct my_tabix *tabix);

void tabix_MODE_TABIX_ANNOTATE_VCF_WITH_BED(struct input_data *id, struct output_data *od, struct my_tabix *tabix) ;

void my_tabix_concatAnnotationData(char in[], char out[]) ;
void my_tabix_tab2InternalColumnSeparator(char in[], char out[]);

void my_tabix_formatAnnotation(char output[], char prefix[], int matches, char data[]); 

void tabix_MODE_TABIX_ANNOTATE_VCF_WITH_VCF(struct input_data *id, struct output_data *od, struct my_tabix *tabix);


#endif 
