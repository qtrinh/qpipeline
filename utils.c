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


void utils_addTextHeader(char header[], char newColumnHeader[]) {
		
	// if there is '=' at the end the new column to be added the remove it 
	if (newColumnHeader[strlen(newColumnHeader)-1] == '=')
		newColumnHeader[strlen(newColumnHeader)-1] = '\0';

	// if there is no TEXT_HEADER_ROW 
	if (strlen(header) == 0) {
		sprintf (header,"%s%s",TEXT_HEADER_ROW, newColumnHeader);
	} else {
		char oldHeader[10240];
		int i,j;
		j = 0;
        
		// length of ##HEADER-ROW:
		for (i = strlen(TEXT_HEADER_ROW); i < strlen(header); i++) {
			oldHeader[j] = header[i];
			j++;
		}
		oldHeader[j] = '\0';

		sprintf (header,"%s%s\t%s", TEXT_HEADER_ROW, newColumnHeader, oldHeader);
		// if the last character is a new line, remove it 
		if (header[strlen(header)-1] == '\n')
			header[strlen(header)-1] = '\0';
	}
}
/**
  given a tuple (DP=10), separate them out as key (DP) and value (10)
  **/
void utils_getKeyNValue(char tuple[], char key[], char value[]) {
	int i = 0;
	char *ptr = tuple;
	while (*ptr != '=') {
		key[i] = *ptr;
		ptr++; i++;
	}
	key[i] = '\0';
	ptr++;
	
	i=0;
	while (*ptr != '\0') {
		value[i] = *ptr;
		ptr++; i++;
	}
	value[i] = '\0';
}


