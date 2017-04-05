//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#include "acgtn.h"
#include "pileup.h"
#include "input_data.h"



void pileup_countACGTN(struct input_data *id, acgtn_t *acgtn, char bases[], char qualities[], char referenceBase, double minBaseQuality, double minMappingQuality) {

	// base and base quality ptr 
	char *ptr, *qptr;
	int pos;
	int num_to_skip;
	int i,j;
	char str[1024], tmpStr[1024];
	int acgtn_index = 0;

	int qcounter = 0; // for QC; just to make sure we count the base qualities correctly 

	ptr = bases;
	qptr = qualities;
	pos = 0;

	while (*ptr ) {
		pos++;

		if (0 && id->verbose)
			printf ("\n[%s:%d] - reading character '%c' at position %4d in string %s\n", __FILE__, __LINE__, *ptr, pos, bases);

		if (*ptr == '$') {
			// end of reads 
			ptr++;
		} else if ((*ptr == '+') || (*ptr == '-')) {
			// indels 
			num_to_skip = 0;
			ptr++;
			i = 0;
			while ((*ptr >= '0') && (*ptr <= '9')) {
				str[i] = *ptr;
				ptr++;

				i++;
			}
			str[i] = '\0';
			num_to_skip = atoi(str);
			j = 0;
			i = 1;
			while (i <= num_to_skip) {
				str[j] = *ptr;
				ptr++;
				i++;
				j++;
			}
			str[j] = '\0';
		} else if (*ptr == '^') {
			// begin of reads , skip 1 character 
			ptr += 2;
		}else if (*ptr == '.' || *ptr == ',') {
			// single base forward or reverse 
			acgtn_index = acgtn_getIndex(referenceBase);
			if (*ptr == '.') {
				if (*qptr >= minBaseQuality)
					acgtn->countFR[0][acgtn_index]++;	
			} else {
				if (*qptr >= minBaseQuality)
					acgtn->countFR[1][acgtn_index]++;	
			}
			ptr++;
			qptr++;
			qcounter++;
		} else if (*ptr == '*') {
			// deletion 
			ptr++;
			if (id->verbose)
				printf ("\n[%s:%d] - DELETION\n", __FILE__, __LINE__);
		} else {
			// mismatches 
			acgtn_index = acgtn_getIndex(*ptr);

			// anything bigger than 90 (character 'Z' ) means they are in lower cases 
			if (*ptr > 90) {
				if (*qptr >= minBaseQuality)
					acgtn->countFR[1][acgtn_index]++;
			} else {
				if (*qptr >= minBaseQuality)
					acgtn->countFR[0][acgtn_index]++;
			}

			ptr++;
			qptr++;
			qcounter++;
		} 
	}
}
