//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "input_data.h"
#include "utils.h"
#include "my_string.h"



/**
  count number of lines in input file 
  **/
int txt_MODE_TXT_COUNT_ENTRY(struct input_data *id, char inputFileName[], char ignoreLinesStartingWith) { 

	int n = 0;

   id->inputFile = fopen(inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		//if (strstr(id->line,"#")) {
		if (id->line[0] == ignoreLinesStartingWith){
			continue;
		}
		n++;
   }   
   fclose(id->inputFile);
	return n;
}

