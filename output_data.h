//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef OUTPUT_DATA_H
#define OUTPUT_DATA_H

#include "common.h"

// for output 
#define MAX_OUTPUT_COLUMNS 1000
#define MAX_OUTPUT_CHAR_PER_COLUMN 1000


struct output_data {

	//  count or percentages 
	output_format_t outputFormat;
	//  Horizontal or Vertical
	output_orientation_t outputOrientation;
	// left or right of input data 
	output_position_t outputPosition;

	char outputFileName[1024];
	FILE *outputFile;
	output_format_t outputFileFormat;
	
	// output comments
	char outputComments[1024];
	   
	// number of columns
	int n;
	char header[MAX_OUTPUT_COLUMNS][MAX_OUTPUT_CHAR_PER_COLUMN];
	char data[MAX_OUTPUT_COLUMNS][MAX_OUTPUT_CHAR_PER_COLUMN];

} ;

struct output_data * 	output_data_init() ;
void 	output_data_print(struct output_data *od) ;

void output_data_printHorizontal(struct output_data *od) ;
void output_data_printVertical(struct output_data  *od); 


#endif
