//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "snpeff.h"
#include "vcf.h"
#include "input_data.h"
#include "output_data.h"


/**

##INFO=<ID=ANN,Number=.,Type=String,Description="Functional annotations: 'Allele | Annotation | Annotation_Impact | Gene_Name | Gene_ID | Feature_Type | Feature_ID | Transcript_BioType | Rank | HGVS.c | HGVS.p | cDNA.pos / cDNA.length | CDS.pos / CDS.length | AA.pos / AA.length | Distance | ERRORS / WARNINGS / INFO' ">


  **/
void snpeff_parseANN(struct input_data *id, struct output_data *od, char **snpeffHeader, int snpeffLength, char outputData[][1024], int *outputDataN) {

	int MAX_ANNOTATION = 100;
	char data[MAX_ANNOTATION][1024];
	int dataN;

	int numberOfAnnotations;

	char str[1024];
	int i, fieldCounter;

	char *ptr;

	int moreData = 1;

	// keep track of how many annotations we have 
	*outputDataN = 0;
	
	ptr = strstr(id->columns[VCF_COLUMN_INFO + id->columnOffSet],"ANN=");
	if (*ptr) {
		if (id->verbose)
			printf ("\n[%s:%d] - parsing '%s' ... \n", __FILE__, __LINE__, ptr);

		ptr = ptr + 4; // point to data after 'ANN='

		numberOfAnnotations = 0;
		while ((moreData) && (*ptr != '\0') && (*ptr != ';')){
			if (id->verbose)
				printf ("\n[%s:%d] - ptr points to '%s' ... \n", __FILE__, __LINE__, ptr);

			numberOfAnnotations++;

			// expect 16 fields 
			//int FIELDS = 16;
			fieldCounter = 0;
			i = 0; 
			while ((fieldCounter < snpeffLength) && (*ptr != '\0')) {
				if (*ptr == ';') {
					// *ptr == ';' means ptr has no nore EFF annotation 
					fieldCounter = snpeffLength;
					continue;
				}
				if (*ptr == ',') {
					// *ptr == ',' means the last field is emptied ( i.e., |,)// annotations are separated by ','
					fieldCounter = snpeffLength;
					ptr++;
					continue;
				}

				if (*ptr == '|') {
					str[i] = '\0';
					//strcpy(outputData[*outputDataN],str);
					//printf ("\nAAA %d '%s' '%s'", fieldCounter+1, outputData[*outputDataN], ptr);
					strcpy(data[fieldCounter],str);
					if (id->verbose) {
						printf ("\n%d '%s' '%s'", fieldCounter, data[fieldCounter], ptr);
						fflush(stdout);
					}

					i = 0;
					fieldCounter++;
					ptr++; // advance beyond '|'

				} else {
					str[i] = *ptr;
					i++;
					ptr++;
				}
			}

			if (id->verbose) {
				printf ("\n\n");
				for (i = 0; i < snpeffLength; i++) {
					//printf ("%d-'%s'\t", i+1, outputData[*outputDataN]);
					printf ("%d-'%s'\t", i+1, data[i]);
				}
				printf ("\n\n");
			}
				

			printf ("%d-", numberOfAnnotations);
			for (i = 0; i < snpeffLength; i++) {
				if (strlen(data[i])==0)
					printf (".\t");
				else
					printf ("%s=%s\t", snpeffHeader[i], data[i]);
			}
			printf ("%s\n", id->line);

			*outputDataN++;
		}
	} else {
		if (id->verbose)
			printf ("[%s:%d] - no SNPEFF ANN annotation ... ", __FILE__, __LINE__);
		exit(1);
	}
}

