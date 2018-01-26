//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "my_math.h"




/**
  
  **/
void my_math_MODE_MATH_LOG_TEN(struct input_data *id, struct output_data *od, int col) { 

	struct vcf * myVCF = vcf_init();

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (id->line[0] == '#') {
			printf ("%s", id->line);
			continue;
		}

		if (id->verbose)  {
      	printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - %s", __FILE__, __LINE__, id->line);
		}	
		         
		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);



		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
}



