//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>

#ifndef ACGTN_H
#define ACGTN_H

#include "common.h"

#define NUMBER_OF_BASES 6
typedef enum DNA_BASES { BASE_A, BASE_C, BASE_G, BASE_T, BASE_N, BASE_OTHER } dna_bases_t;

typedef struct acgtn {
	// lower case ACGTN 
	double count[NUMBER_OF_BASES];
	// index 0 is forward
	// index 1 is reverse 
	double countFR[2][NUMBER_OF_BASES];
	
	// per cycle lower case ACGTN 
	double perCycleCount[MAX_READ_LENGTH+1][NUMBER_OF_BASES];
} acgtn_t ; 

acgtn_t * acgtn_Init() ;
void acgtn_reset(acgtn_t *acgtn) ;

dna_bases_t acgtn_getIndex(char base);
char acgtn_getBase(int index);
char acgtn_getBaseComplement(char base);
int acgtn_validBases(char dnaString[]);
void acgtn_printCount(acgtn_t *acgtn) ;


#endif 
