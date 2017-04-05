//
// written by Quang Trinh
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"
#include "acgtn.h"



/**
  initialize a ACGTN structure 
  **/
acgtn_t * acgtn_Init() {

	acgtn_t *acgtn  = (acgtn_t *) malloc (1 * sizeof (acgtn_t));

	acgtn_reset(acgtn);

	return acgtn;	
}   

/**
	reset counts
	**/
void acgtn_reset(acgtn_t * acgtn) {
	int i,j;
	for (i = 0; i < NUMBER_OF_BASES; i++)  {
		acgtn->count[i] = 0;
		acgtn->countFR[0][i] = 0;
		acgtn->countFR[1][i] = 0;
	}
	for (i = 0; i <= MAX_READ_LENGTH; i++)  {
		for (j = 0; j < NUMBER_OF_BASES; j++)  {
			acgtn->perCycleCount[i][j] = 0;
		}
	}
}


/** 
  get index of a base 
  **/
dna_bases_t acgtn_getIndex(char base) {

	char b = toupper(base);
	if (b == 'A')
		return BASE_A;
	else if (b == 'C') 
		return BASE_C;
	else if (b == 'G')
		return BASE_G;
	else if (b == 'T') 
		return BASE_T;
	else if (b == 'N')
		return BASE_N;
	else 
		return BASE_OTHER;
}

/** 
  get base from an index 
  **/
char acgtn_getBase(int index) {

	if (index == BASE_A)
		return 'A';
	else if (index == BASE_C) 
		return 'C';
	else if (index == BASE_G)
		return 'G';
	else if (index == BASE_T) 
		return 'T';
	else if (index == BASE_N)
		return 'N';
	else 
		return '*';
}
/** 
  get complement of a base 
  **/
char acgtn_getBaseComplement(char base) {

	base = toupper(base);
	if (base == 'A')
		return 'T';
	if (base == 'C')
		return 'G';
	if (base == 'G')
		return 'C';
	if (base == 'T')
		return 'A';
	else
		return '0';
}

/**
  check to see if the input dnaString is a valid string
  return 1 if valid
  return 0 if not valid 
  **/
int acgtn_validBases(char dnaString[]) {

	char *p = dnaString;
	while (*p != '\0') {
		if (acgtn_getIndex(*p) == BASE_OTHER)
			return 0;
		p++;
	}
	return 1;
}



void acgtn_printCount(acgtn_t *acgtn) {
	int j;
	int total = 0;

	printf ("\n\n");
	for (j = 0; j < NUMBER_OF_BASES ; j++) {
		printf ("%c\t%.0f:%.0f\n", acgtn_getBase(j), acgtn->countFR[0][j], acgtn->countFR[1][j]);
		total += (acgtn->countFR[0][j] + acgtn->countFR[1][j]);
	}
	printf ("\nTotal\t%d", total);
	printf ("\n\n");
	fflush(stdout);
}


void acgtn_printCount2Str(acgtn_t *acgtn, char outputStr[]) {
	int j;
	char str[MAX_CHAR_PER_LINE];
	strcpy(outputStr,"");

	for (j = 0; j < NUMBER_OF_BASES ; j++) {
		sprintf (str,"(%c,%.0f,%.0f),", acgtn_getBase(j), acgtn->countFR[0][j], acgtn->countFR[1][j]);
		strcat(outputStr,str);
	}
	// remove the last ','
	outputStr[strlen(outputStr)-1] = '\0';
}


