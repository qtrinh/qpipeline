//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "output_data.h"



/**
  initialize an output structure
 **/
struct output_data * output_data_init() {

	struct output_data *od = (struct output_data *) malloc (1 * sizeof (struct output_data));

	strcpy(od->outputFileName, "");
	od->outputFile = NULL;

	od->outputFileFormat = FILE_FORMAT_NOT_DEFINED;
	
	od->outputFormat = OUTPUT_AS_COUNTS;

	od->outputOrientation = OUTPUT_ORIENTATION_VERTICAL;

	return od;
}


void output_data_print(struct output_data *od) {

	if (od->outputOrientation == OUTPUT_ORIENTATION_VERTICAL) {
		output_data_printVertical(od);
	} else 
		output_data_printHorizontal(od);
}


/**
  output data in rows format 
  **/
void output_data_printHorizontal(struct output_data *od) {

	int j;

   printf ("\n");
   for (j = 1; j <= od->n; j++) {
		if (strstr(od->header[j],"."))
      	printf ("");
		else 
      	printf ("%s",od->header[j]);
		if (j < od->n)
			printf ("\t");
   }
   printf ("\n");
   for (j = 1; j <= od->n; j++) {
		if (strstr(od->header[j],"."))
      	printf ("");
		else 
			printf ("%s",od->data[j]);
		if (j < od->n)
			printf ("\t");
   }
   printf ("\n\n");
}

/**
  output data in columns format 
  **/
void output_data_printVertical(struct output_data *od) {
	int j;
   printf ("\n");
   for (j = 2; j <= od->n; j++) {
		if (strstr(od->header[j],".") && strstr(od->data[j],"."))
      	printf ("\n");
		else 
      	printf ("%s\t%s\n",od->header[j], od->data[j]);
   }
   printf ("\n");
}






