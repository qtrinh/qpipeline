//
// written by Quang M Trinh <quang.trinh@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "my_string.h"
#include "common.h"
#include "input_data.h"


/**
  convert an array of chars to an array of 'strings'
  return array with first index starting at 0
  **/
char ** my_string_arrayOfChars2arrayOfStrings(struct input_data *id, char input[], char delimeter, int *length) {

	char *ptr = input;
	int i,n;
	char str[MAX_CHAR_PER_LINE];
	char **array ;
	int localVerbose = 0;

	// count the number of strings separated by delimeter
	n = 0;
	while (*ptr) {
		if (*ptr == delimeter)
			n++;
		ptr++;
	}
	// one more at the end 
	n++;

	if (localVerbose && id->verbose)
		printf ("\n[%s:%d] - %d elements in input string '%s'", __FILE__, __LINE__, n,input); fflush(stdout);


	// create array and set it to NULL
	array = (char **) malloc ((n) * sizeof(char *));
	for (i = 0; i < n; i++)
		array[i] = NULL;

	ptr = input;
	i = 0;
	n = 0;
	while (*ptr) {
		if (*ptr == delimeter) {
			str[i] = '\0';
			if (localVerbose && id->verbose)
				printf ("\n[%s:%d] - complete parsing string '%s' ( index %d ) ", __FILE__, __LINE__, str, n); fflush(stdout);
			array[n] = (char *) malloc ((strlen(str) + 1)* sizeof(char));
			strcpy(array[n], str);
			n++;
			i = 0;
		} else {
			str[i] = *ptr;
			i++;
		}
		ptr++;
	}
	// get the last string 
	str[i] = '\0';
	if (localVerbose && id->verbose)
		printf ("\n[%s:%d] - complete parsing string '%s' ( index %d ) ", __FILE__, __LINE__, str, *length); fflush(stdout);
	array[n] = (char *) malloc ((strlen(str)+1) * sizeof(char));
	strcpy(array[n], str);
	*length = n+1;
	return array;
}

void my_string_printArray(char **a, int length) {

	int i;

  	printf ("\n[%s:%d] - ", __FILE__, __LINE__);
  	printf ("\nlength %d", length);
  	for (i = 1; i < length; i++) {
		printf ("\n%d\t'%s'", i, a[i]);
	}
	printf ("\n");
}


void my_string_printArrayHorizontal(char **a, int length) {

	int i;

  	printf ("\n[%s:%d] - ", __FILE__, __LINE__);
  	printf ("\n");
  	for (i = 1; i < length; i++) {
		printf ("%s\t", a[i]);
	}
	printf ("\n");
}



/**
  convert to upper case 
 **/
void my_string_toupper(char *ptr) {

	char *p = ptr;

	while (*p != '\0') {
		*p = toupper(*p);
		p++;
	}
}

void my_string_removeTrailingSpaces(char *ptr) {


}
