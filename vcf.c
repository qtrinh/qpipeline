//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "input_data.h"
#include "vcf.h"
#include "utils.h"
#include "my_string.h"
#include "my_tabix.h"


struct vcf * vcf_init() {
	struct vcf *v = (struct vcf *) malloc (sizeof (struct vcf));
	return v;
}

void vcf_setInputFileToVCFformat(struct input_data *id) {
         
	id->chrColumnNumber = VCF_COLUMN_CHR;
	id->startColumnNumber = VCF_COLUMN_POS; 
	id->endColumnNumber = VCF_COLUMN_POS;
	id->refColumnNumber = VCF_COLUMN_REF; 
	id->variantColumnNumber = VCF_COLUMN_ALT;
	id->columnOffSet = 0;
}


/**
  parse VCF header 
  **/
void vcf_parseHEADER(struct input_data * id, struct vcf *vcf) {

	int i,j,k;
	char **headers;
	int n;

	// remove the new line if it is there 
	if (id->line[strlen(id->line)-1] =='\n')
		id->line[strlen(id->line)-1] = '\0';

	if (id->verbose)
		printf ("\n[%s:%d] - processing header string '%s'\n", __FILE__, __LINE__, id->line);


	// break string into fields 
	headers = input_data_parseLineMem(id, id->line, '\t', &n);
	if (id->verbose) {
		input_data_printParsedLineMemDebugging(headers, n);
	}

	// first sample ID start from column 10 
	vcf->numberOfSamples = n - VCF_COLUMN_FIRST_SAMPLE + 1;

	vcf->sampleNames = (char **) malloc (vcf->numberOfSamples * sizeof(char *));
	
	vcf->totalNumberOfCallsPerSample = (int *) malloc (vcf->numberOfSamples * sizeof(int *));

	vcf->Ts = (int *) malloc (vcf->numberOfSamples * sizeof(int *));
	vcf->Tv = (int *) malloc (vcf->numberOfSamples * sizeof(int *));

	j = 0;
	for (i = VCF_COLUMN_FIRST_SAMPLE; i <= n; i++) {
		// allocate space to store sample names 
		vcf->sampleNames[j] = (char *) malloc ((strlen(headers[i])+1) * sizeof(char));
		strcpy(vcf->sampleNames[j], headers[i]);

		vcf->totalNumberOfCallsPerSample[j] = 0;
		vcf->Ts[j] = 0;
		vcf->Tv[j] = 0;
		j++;
	}
	

	if (id->verbose) {
		printf ("\n[%s:%d] - number of samples:\t%d", __FILE__, __LINE__, vcf->numberOfSamples);
		for (i = 0; i < vcf->numberOfSamples; i++) {
			printf ("\n%d\t'%s'", i, vcf->sampleNames[i]);
		}
		printf ("\n");
		fflush(stdout);
	}


	// first dim index: sample name 
	vcf->snvSubstitutionMatrix = (int ***) malloc (vcf->numberOfSamples * sizeof(int));
	for (i = 0; i < vcf->numberOfSamples; i++) {
		// second dim index:  ref base
		vcf->snvSubstitutionMatrix [i] = (int **) malloc(NUMBER_OF_BASES * sizeof(int *));
		for (j = 0; j < NUMBER_OF_BASES; j++) {
			// third dim index:  alt base
			vcf->snvSubstitutionMatrix [i][j] = (int *) malloc(NUMBER_OF_BASES * sizeof(int *));
			for (k = 0; k < NUMBER_OF_BASES; k++) {
				vcf->snvSubstitutionMatrix [i][j][k] = 0; 
			}
		}
	}

	// first dim index: sample name 
	vcf->snvSubstitutionMatrixNNN = (int *****) malloc (vcf->numberOfSamples * sizeof(int));
	for (i = 0; i < vcf->numberOfSamples; i++) {
		// second dim index:  ref base
		vcf->snvSubstitutionMatrixNNN[i] = (int ****) malloc(NUMBER_OF_BASES * sizeof(int *));
		for (j = 0; j < NUMBER_OF_BASES; j++) {
			// third dim index:  alt base
			vcf->snvSubstitutionMatrixNNN[i][j] = (int ***) malloc(NUMBER_OF_BASES * sizeof(int *));
			for (k = 0; k < NUMBER_OF_BASES; k++) {
				vcf->snvSubstitutionMatrixNNN[i][j][k] = (int **) malloc(NUMBER_OF_BASES * sizeof(int *)); 


				int p,q;
				for (p = 0; p < NUMBER_OF_BASES; p++) {
					vcf->snvSubstitutionMatrixNNN[i][j][k][p] = (int *) malloc(NUMBER_OF_BASES * sizeof(int *));
					for (q = 0; q < NUMBER_OF_BASES; q++) {
						vcf->snvSubstitutionMatrixNNN[i][j][k][p][q] = 0; 
					}
				}
			}
		}
	}

	input_data_freeMem(headers, n);
}

void vcf_parseFORMAT(struct input_data * id, struct vcf * myVCF) {

	int i;
	char str[1024];
	int n;

	// 
	myVCF->formatKeys = my_string_arrayOfChars2arrayOfStrings(id, id->columns[VCF_COLUMN_FORMAT + id->columnOffSet], ':', &myVCF->formatN);

	myVCF->formatValues = (char ***) malloc (myVCF->numberOfSamples * sizeof(char **));

	if (id->verbose)
		printf ("\n\n"); fflush(stdout);

	i = VCF_COLUMN_FIRST_SAMPLE + id->columnOffSet; 
	int sampleIndex = 0;
	for (sampleIndex = 0; sampleIndex < myVCF->numberOfSamples; sampleIndex++) {
		// 'GT:AD:DP:GQ:PL'
		// '1/1:0,59:59:66.02:597,66,0'
	
		if (1 && id->verbose)
			printf ("\n[%s:%d] - parsing FORMAT data for sample '%s' -> '%s' ", __FILE__, __LINE__, myVCF->sampleNames[sampleIndex], id->columns[i]); fflush(stdout);

		if (strstr(id->columns[i],"./.") && (strlen(id->columns[i]) == 3)) {
			// special case
			// if the FORMAT string is './.' ( no data, then create a emptied data 
			strcpy(str,"");
			for (i = 0 ; i < myVCF->formatN; i++)
				strcat(str,".:");
			str[strlen(str)-1] = '\0';
			myVCF->formatValues[sampleIndex]  = my_string_arrayOfChars2arrayOfStrings(id, str, ':', &n);
		} else 
			myVCF->formatValues[sampleIndex]  = my_string_arrayOfChars2arrayOfStrings(id, id->columns[i], ':', &n);
	
		if (0 && id->verbose)
			printf ("\n\n"); fflush(stdout);

		i++;
	}
}

void vcf_correctFORMAT(struct input_data * id, struct vcf * myVCF) {
	int i;
	int sampleIndex ;

	if (id->verbose)
		printf ("\n\n[%s:%d] - correcting FORMAT ( DP=. -> DP=-1 )\n\n", __FILE__, __LINE__); fflush(stdout);

	i = vcf_getFORMAT_attributeIndex ( myVCF, "DP");

	for (sampleIndex = 0 ; sampleIndex < myVCF->numberOfSamples; sampleIndex++) {
		for (i = 0; i < myVCF->formatN; i++) {
			if (
					(strlen(myVCF->formatValues[sampleIndex][i]) == 1) &&
					(myVCF->formatValues[sampleIndex][i][0] == '.'))
			strcpy(myVCF->formatValues[sampleIndex][i], "-1");
		}
	}

}

void vcf_printFORMAT(struct input_data * id, struct vcf * myVCF) {
	int i;
	int sampleIndex ;

	printf ("\n\n[%s:%d] - printing FORMAT\n\n", __FILE__, __LINE__); fflush(stdout);

	for (i = 0; i < myVCF->formatN; i++) {
		printf ("\t%s", myVCF->formatKeys[i]);
	}
	printf ("\n");

	for (sampleIndex = 0 ; sampleIndex < myVCF->numberOfSamples; sampleIndex++) {
		printf ("\n%s", myVCF->sampleNames[sampleIndex]);
		for (i = 0; i < myVCF->formatN; i++) {
			printf ("\t%s", myVCF->formatValues[sampleIndex][i]);
			fflush(stdout);
		}
	}

	printf ("\n\n");
	fflush(stdout);
}

int vcf_getFORMAT_attributeIndex(struct vcf *myVCF, char key[]) {

	int i;

	for (i = 0; i < myVCF->formatN; i++) {
		if ( strstr(key,myVCF->formatKeys[i]) && (strlen(key) == strlen(myVCF->formatKeys[i])))
			return i;
	}
	return -1;
}
variant_class_t vcf_getVariantClass(char ref[], char alt[], char variantClassText[]) { 

	if (
			(strlen(ref) == 1) && (strlen(alt) == 1)
			) {
		strcpy(variantClassText, "VARIANT_CLASS_SUBSTITUTION_SINGLE_BASE");
		return VARIANT_CLASS_SUBSTITUTION_SINGLE_BASE;
	}
	if (
			(strlen(ref) > 1) && (strlen(ref) == strlen(alt))
			) {
		strcpy(variantClassText, "VARIANT_CLASS_SUBSTITUTION_MULTIPLE_BASES");
		return VARIANT_CLASS_SUBSTITUTION_MULTIPLE_BASES;
	}
	if (
			(strlen(ref) > strlen(alt) )
			) {
		strcpy(variantClassText, "VARIANT_CLASS_DELETION");
		return VARIANT_CLASS_DELETION;
	}	
	if (
			(strlen(ref) < strlen(alt) )
			) {
		strcpy(variantClassText, "VARIANT_CLASS_INSERTION");
		return VARIANT_CLASS_INSERTION;
	}

	strcpy(variantClassText, "VARIANT_CLASS_NOT_DEFINED");
	return VARIANT_CLASS_NOT_DEFINED;

}

/*
	compare two VCF entries a and b ( VCF database )
	*/
vcf_compare_result_t vcf_compareEntry(struct input_data *id, char *a[], int an, char *b[], int bn) {


	variant_class_t aclass, bclass;
	char aclassDescription[1024], bclassDescription[1024];



	if (id->verbose) {
		printf ("\n[%s:%d] - comparing VCF entries \n",__FILE__, __LINE__);
		my_string_printArrayHorizontal(a,an);
		my_string_printArrayHorizontal(b,bn);
	}

	if (!strstr(a[VCF_COLUMN_CHR + id->columnOffSet], b[VCF_COLUMN_CHR])) {
		// this should never happened  but we check anyways 
		if (id->verbose)
			printf ("\n[%s:%d] - chr mismatched\n",__FILE__, __LINE__);
			return VCF_CHR_MISMATCHED;
	}

	aclass = vcf_getVariantClass(a[VCF_COLUMN_REF + id->columnOffSet], a[VCF_COLUMN_ALT + id->columnOffSet], aclassDescription);
	bclass = vcf_getVariantClass(b[VCF_COLUMN_REF], b[VCF_COLUMN_ALT], bclassDescription);
	if (aclass != bclass) {
		if (id->verbose)
			printf ("\n[%s:%d] - variant class mismatched\n",__FILE__, __LINE__);
			return VCF_VARIANT_CLASS_MISMATCHED;
	} else {
		if (id->verbose)
			printf ("\n[%s:%d] - variant class matched\n",__FILE__, __LINE__);
	}

	if (!strstr(a[VCF_COLUMN_POS + id->columnOffSet], b[VCF_COLUMN_POS ])) {
		if (id->verbose)
			printf ("\n[%s:%d] - pos mismatched\n",__FILE__, __LINE__);
			return VCF_POS_MISMATCHED;
	}


	int i,j;
	int matched = 0;

	int aREFLength;
	// split ref column into arrary 
	// this is to deal with multiple values.  For example, CC,CCC	C
	char **aREF = my_string_arrayOfChars2arrayOfStrings(id, a[VCF_COLUMN_REF + id->columnOffSet], ',', &aREFLength);
	int bREFLength;
	char **bREF = my_string_arrayOfChars2arrayOfStrings(id, b[VCF_COLUMN_REF ], ',', &bREFLength);
	if (id->verbose) {
		printf ("\n");
		for (i = 0; i < aREFLength; i++) {
			printf ("\na REF %s", aREF[i]);
		}
		printf ("\n");
		for (i = 0; i < bREFLength; i++) {
			printf ("\nb REF %s", bREF[i]);
		}
	}
		
	matched = 0;
	for (i = 0; i < aREFLength; i++) {
		for (j = 0; j < bREFLength; j++) {
			if (
					(strlen(aREF[i]) == strlen(bREF[j])) && 
					(strstr(aREF[i], bREF[j])) ) {
				if (id->verbose)
					printf ("\n[%s:%d] - REF matched\n",__FILE__, __LINE__);
					//return VCF_ALT_MISMATCHED;
				// if matches one of the values
				matched = 1;
			}
		}
	}
	for (i = 0; i < aREFLength; i++) 
		free(aREF[i]);
	free(aREF);
	for (i = 0; i < bREFLength; i++) 
		free(bREF[i]);
	free(bREF);

	if (!matched)
		return VCF_REF_MISMATCHED;

/*
	if (
			(strlen(a[VCF_COLUMN_REF]) != strlen(b[VCF_COLUMN_REF])) || 
			(!strstr(a[VCF_COLUMN_REF], b[VCF_COLUMN_REF])) ) {
		if (id->verbose)
			printf ("\n[%s:%d] - REF base mismatched\n",__FILE__, __LINE__);
			return VCF_REF_MISMATCHED;
	}*/
	int aALTLength;
	char **aALT = my_string_arrayOfChars2arrayOfStrings(id, a[VCF_COLUMN_ALT + id->columnOffSet], ',', &aALTLength);
	int bALTLength;
	char **bALT = my_string_arrayOfChars2arrayOfStrings(id, b[VCF_COLUMN_ALT ], ',', &bALTLength);
	if (id->verbose) {
		printf ("\n");
		for (i = 0; i < aALTLength; i++) {
			printf ("\na ALT %s", aALT[i]);
		}
		printf ("\n");
		for (i = 0; i < bALTLength; i++) {
			printf ("\nb ALT %s", bALT[i]);
		}
	}
		
	matched = 0;
	for (i = 0; i < aALTLength; i++) {
		for (j = 0; j < bALTLength; j++) {
			if (
					(strlen(aALT[i]) == strlen(bALT[j])) && 
					(strstr(aALT[i], bALT[j])) ) {
				if (id->verbose)
					printf ("\n[%s:%d] - ALT matched\n",__FILE__, __LINE__);
					//return VCF_ALT_MISMATCHED;
				matched = 1;
			}
		}
	}
	for (i = 0; i < aALTLength; i++) 
		free(aALT[i]);
	free(aALT);
	for (i = 0; i < bALTLength; i++) 
		free(bALT[i]);
	free(bALT);

	if (!matched)
		return VCF_ALT_MISMATCHED;

/*
	if (
			(strlen(a[VCF_COLUMN_ALT]) != strlen(b[VCF_COLUMN_ALT])) || 
			(!strstr(a[VCF_COLUMN_ALT], b[VCF_COLUMN_ALT])) ) {
		if (id->verbose)
			printf ("\n[%s:%d] - ALT base mismatched\n",__FILE__, __LINE__);
			return VCF_ALT_MISMATCHED;
	}*/

	if (id->verbose)
		printf ("\n[%s:%d] - matched \n",__FILE__, __LINE__); fflush(stdout);
	return VCF_ENTRY_MATCHED;
}






/**
  parse INFO for a particular key 
  **/
void vcf_MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY(struct input_data *id, struct output_data *od, char key[], double minValue) {

	struct vcf * myVCF = vcf_init();

	char str[MAX_CHAR_PER_LINE];
	char *ptr;
	char *pptr;


	char localKey[MAX_CHAR_PER_LINE], localValue[MAX_CHAR_PER_LINE];

	char textHeaderStr[MAX_CHAR_PER_LINE] = "";
	
	
/*
	// if key does not contain '=' then add '='
	if (strstr(key, "\="))
		; 
	else { 
		//strcat(key,"=");
		if (id->verbose)  {
      	printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - adding '=' to key.  key is now '%s'", __FILE__, __LINE__,key);
		}	
	}
*/

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
		
		if (id->verbose)  {
      	printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - %s", __FILE__, __LINE__, id->line);
		}	
		
		// make copy of TEXT_HEADER_ROW if it is there 
		if (strstr(id->line,TEXT_HEADER_ROW)) {
			strcpy(textHeaderStr, id->line);
			continue;
		}

		if (strstr(id->line,"#CHROM")) {
			if (minValue == INT_MIN)  {
				// print TEXT_HEADER_ROW only when we do parsing
				utils_addTextHeader(textHeaderStr, key);
				printf ("%s\n", textHeaderStr);
			}

			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);

			continue;
		}

		// if comment line 
		if (id->line[0] == '#') {
			printf ("%s", id->line);
			continue;
		}

		         
		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);

		// make a copy because we will make some changes 
		strcpy(str, id->columns[VCF_COLUMN_INFO + id->columnOffSet]);
		ptr = strstr(str,key);
		
		if (key[0] == ';') {
			// if first letter of key is ';', then skip this 
			ptr++;
		}

		if (id->verbose) 
			printf ("\n[%s:%d] - ptr points to '%s' \n\n",__FILE__, __LINE__, ptr);

		if (ptr) {

			// looking for the key 
			pptr = ptr;
			while ((*pptr != ';') && (*pptr != '\0')) {
				pptr++;
			}
			*pptr = '\0';
			if (id->verbose) 
				printf ("\n[%s:%d] - FOUND AA key '%s' ->  '%s' ... \n\n",__FILE__, __LINE__, key, ptr);

			utils_getKeyNValue(ptr,localKey,localValue);

			if (minValue == INT_MIN) {
				// no value entered by user 
				// print key and value pair as first column follow by orginal entry 
					if (id->verbose)
						printf ("\n[%s:%d] - \n", __FILE__, __LINE__);
				printf ("%s\t%s\n", ptr,id->line);
			} else {
				if (atof(localValue) >= minValue) { 
					// print original entry because value of key is greater than
					// user input 
					if (id->printRejected == 0) {
						if (id->verbose)
							printf ("\n[%s:%d] - printing entry because value of key greater than what user input \n", __FILE__, __LINE__);
						printf ("%s\n", id->line);
					}
				} else {
					// value of key is less than value user input 
					if (id->printRejected == 0) {
						if (id->verbose) 
							printf ("\n[%s:%d] - failed printing because %d < %d ( min value entered by user )\n\n",__FILE__, __LINE__, atoi(localValue), (int)(minValue));
					} else {
						// print rejected  entry 
						if (id->verbose) 
							printf ("\n[%s:%d] - printing rejected entry ... \n\n",__FILE__, __LINE__);
						printf ("%s\n", id->line);
					}
				}
			}
		} else {
			if (id->verbose) 
				printf ("\n[%s:%d] - key '%s' does not exist in INFO column - please check input key value (-k) argument\n\n",__FILE__, __LINE__, key);
			//printf ("%s=NO_VALUE\t%s\n", key, id->line);
			exit(1);
		}
   }   
   fclose(id->inputFile);
}




/**
  print PASS filter only 
  **/
void vcf_MODE_VCF_FILTER_PASS_FILTERS_ONLY(struct input_data *id, struct output_data *od) { 

	struct vcf * myVCF = vcf_init();

	int pass;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

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

		pass = 0;
		if (strstr(id->columns[VCF_COLUMN_FILTER + id->columnOffSet], "PASS") && (strlen(id->columns[VCF_COLUMN_FILTER + id->columnOffSet]) == 4))
			pass = 1;

		if (pass) {
			if (id->printRejected == 0) {	
				if (id->verbose) 
					printf ("\n[%s:%d] - FOUND printing PASS entry ...\n\n",__FILE__, __LINE__);
				printf ("%s\n", id->line);
			}
		} else {
			if (id->printRejected == 0) {
				if (id->verbose) 
					printf ("\n[%s:%d] - failed printing because column %d ( '%s' ) does not contained PASS ...\n",__FILE__, __LINE__, VCF_COLUMN_FILTER + id->columnOffSet,  id->columns[VCF_COLUMN_FILTER + id->columnOffSet]);
			} else {
				if (id->verbose) 
					printf ("\n[%s:%d] - printing non-PASS entry ...\n",__FILE__, __LINE__);
				printf ("%s\n", id->line);
			}
		}
		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
}





/**
  print SNV or indels 
  **/
void vcf_MODE_VCF_FILTER_SNV_OR_INDEL(struct input_data *id, int variantClassToPrint, struct output_data *od) { 

	struct vcf * myVCF = vcf_init();
	char tmp[1024];

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

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


		variant_class_t type = vcf_getVariantClass(id->columns[VCF_COLUMN_REF + id->columnOffSet], id->columns[VCF_COLUMN_ALT + id->columnOffSet], tmp);

		if ((variantClassToPrint == VARIANT_CLASS_SUBSTITUTION) &&
				((type == VARIANT_CLASS_SUBSTITUTION_SINGLE_BASE) || (type == VARIANT_CLASS_SUBSTITUTION_MULTIPLE_BASES))
					) {
				if (id->verbose) 
					printf ("\n[%s:%d] - printing substitution entry ...\n",__FILE__, __LINE__);
				printf ("%s\n", id->line);
		} else if ((variantClassToPrint == VARIANT_CLASS_INDELS) &&
				((type == VARIANT_CLASS_INSERTION) || (type == VARIANT_CLASS_DELETION) || (type == VARIANT_CLASS_INDELS))
					) {
				if (id->verbose) 
					printf ("\n[%s:%d] - printing INDEL entry ...\n",__FILE__, __LINE__);
				printf ("%s\n", id->line);
		}
		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
}





/**
  **/
void vcf_MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY(struct input_data *id, struct output_data *od, char key[], double value, int sampleIndex2Process) {

	struct vcf * myVCF = vcf_init();
	int i,sampleIndex;
	char outputStr[MAX_CHAR_PER_LINE];
	// an array to store passes 
	int *passes;
	char **data;
	int j;
	int attributeIndex;
	char textHeaders[1024];

	int text_header_row = 0;


   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {


		if (strstr(id->line,TEXT_HEADER_ROW)) {

			text_header_row = 1;

			// remove '\n' 
			id->line[strlen(id->line)-1] = '\0';

			myVCF->textHeaders = input_data_parseLineMem(id, id->line, '\t', &(myVCF->textHeadersN));
			if (id->verbose)
				input_data_printParsedLineMemDebugging(myVCF->textHeaders,myVCF->textHeadersN);
			continue;
		}

		if (strstr(id->line,"#CHROM")) {
			vcf_parseHEADER(id, myVCF);
			passes = (int *) malloc (sizeof(int) * myVCF->numberOfSamples);
			data = (char **) malloc (sizeof(char) * myVCF->numberOfSamples);
			if (text_header_row == 0) {
				// if there is no TEXT_HEADER_ROW line yet 
				//strcpy(textHeader,TEXT_HEADER_ROW);
			}
			strcpy(textHeaders,"");
			char str[1024];
			for (i = 0; i < myVCF->numberOfSamples; i++) {
				passes[i] = 0;
				data[i] = (char *) malloc (1024 * sizeof(char));
				strcpy(data[i],"");

				//sprintf(str,"%s_SAMPLE_NAME\t%s_%s\t",myVCF->sampleNames[i],myVCF->sampleNames[i],key);
				sprintf(str,"%s_%s\t",myVCF->sampleNames[i],key);
				strcat(textHeaders,str);
			}
			if (text_header_row == 0) {
				textHeaders[strlen(textHeaders)-1] = '\0';
				if (value == INT_MIN)
					// print TEXT_HEADER_ROW only when we do parsing
					printf ("%s%s\n", TEXT_HEADER_ROW, textHeaders);
			} else {
				if (value == INT_MIN)
					// print TEXT_HEADER_ROW only when we do parsing
					printf ("%s%s", TEXT_HEADER_ROW, textHeaders);

				// extract the first header and print 
				for (i = 0; i < strlen(myVCF->textHeaders[1]); i++) {
					int k, j ;
					if ((myVCF->textHeaders[1][i] == ':') && (myVCF->textHeaders[1][i+1] == ':')) {
						k = i+2;
						j = 0;
						for (k = i+2; k < strlen(myVCF->textHeaders[1]); k++) {
							str[j] = myVCF->textHeaders[1][k];
							j++;
						}
						str[j] = '\0';
						// so we can terminate the for loop 
						i = strlen(myVCF->textHeaders[1]);
					}
				}
				printf ("%s", str);
				for (i = 2; i <= myVCF->textHeadersN; i++) {
					printf ("\t%s", myVCF->textHeaders[i]);
				}
				printf ("\n");
			}
			printf ("%s\n", id->line);
			continue;
		}

		if (id->line[0] == '#') {
			printf ("%s", id->line);
			continue;
		}

		if (id->verbose)  {
      	printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - %s", __FILE__, __LINE__, id->line);
			fflush(stdout);
		}	
		         
		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);


		vcf_parseFORMAT(id,myVCF); 
		vcf_correctFORMAT(id,myVCF); 
		if (id->verbose)
			vcf_printFORMAT(id,myVCF);


		attributeIndex = vcf_getFORMAT_attributeIndex ( myVCF, key);
			
		if (attributeIndex == -1) {
      	printf ("\n\n[%s:%d] - %s", __FILE__, __LINE__, id->line);
			printf ("\n\n[%s:%d] - ERROR - Attribute '%s' is not in FORMAT column - see above!!!\n\n", __FILE__, __LINE__, key);
			exit(1);
			fflush(stdout);
		}


		// 
		char tmp[1024];
			if (id->verbose)
				printf ("\n[%s:%d] - process all %d samples .... \n\n", __FILE__, __LINE__, myVCF->numberOfSamples);
			
			for (j = 0; j < myVCF->numberOfSamples; j++) {
				strcpy(data[j],"");
				passes[j] = 0;
			}

			for (sampleIndex = 0 ; sampleIndex < myVCF->numberOfSamples; sampleIndex++) {

				// if attribute value contained ',' then don't know how to compare 
				if ((value != INT_MIN) && strstr(myVCF->formatValues[sampleIndex][attributeIndex], ",")) {
					printf ("\n\n[%s:%d] - %s", __FILE__, __LINE__, id->line);
					printf ("\n\n[%s:%d] - %s", __FILE__, __LINE__, myVCF->formatValues[sampleIndex][attributeIndex]);
					printf ("\n\n[%s:%d] - ERROR - Attribute '%s' contained ',' ... cannot compare  - see above!!!\n\n", __FILE__, __LINE__, key);
					exit(1);

				}

				// for each sample 
				if ((atof(myVCF->formatValues[sampleIndex][attributeIndex]) >= value) || (value == INT_MIN)) {
					if (strlen(data[sampleIndex]) == 0) {
						sprintf (tmp,"%s_%s=%s", myVCF->sampleNames[sampleIndex], myVCF->formatKeys[attributeIndex],myVCF->formatValues[sampleIndex][attributeIndex]);
						strcpy(data[sampleIndex], tmp);
					} else {
						sprintf (tmp,"\t%s_%s=%s", myVCF->sampleNames[sampleIndex], myVCF->formatKeys[attributeIndex],myVCF->formatValues[sampleIndex][attributeIndex]);
						strcat(data[sampleIndex], tmp);
					}

					// this sample passed 
					passes[sampleIndex] = 1;
				}
			}
			int numberOfPassedSamples = 0;
			for (i = 0; i <  myVCF->numberOfSamples; i++)
				numberOfPassedSamples += passes[i];

			if (id->verbose) {
				printf ("\n[%s:%d] - %d out of %d passed ... \n\n", __FILE__, __LINE__, numberOfPassedSamples,  myVCF->numberOfSamples);
			}
			
			if (sampleIndex2Process == PROCESS_ALL_SAMPLES) {
				if (numberOfPassedSamples == myVCF->numberOfSamples) {
					if (id->verbose) {
						printf ("\n[%s:%d] - all samples passed ... printing \n\n", __FILE__, __LINE__);
					}
					if (value == INT_MIN) {
						for (i = 0; i <  myVCF->numberOfSamples; i++)
							printf ("%s\t", data[i]);
					}
					printf ("%s\n", id->line);
				} else {
					if (id->verbose) {
						printf ("\n[%s:%d] - at least 1 sample failed ... skip printing \n\n", __FILE__, __LINE__);
					}
				}
			} else {
				if (passes[sampleIndex2Process] == 1) {
					if (id->verbose) {
						printf ("\n[%s:%d] - sample with index %d passed ... printing ... \n\n", __FILE__, __LINE__, sampleIndex2Process);
					}
					printf ("%s\n", id->line);
				} else {
					if (id->verbose)
						printf ("\n[%s:%d] - sample with index %d failed ... skip printing ... \n\n", __FILE__, __LINE__, sampleIndex2Process);
				}
			}

		input_data_freeMem(id->columns,id->n);
   }   
   fclose(id->inputFile);
}




/**
  annotate sequence context 
  **/
void vcf_MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT(struct input_data *id, struct output_data *od, struct fasta *fa, int flanking) { 

	struct vcf * myVCF = vcf_init();
	char *left, *right;
	char str[1024];

	fasta_loadFile(fa);

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

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
		 
		// left flanking region 
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS])-flanking, atoi(id->columns[VCF_COLUMN_POS])-1);
		left = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(left);

		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS]), atoi(id->columns[VCF_COLUMN_POS]));
		fa->regionData = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(fa->regionData);
		
		// right flanking region 
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS])+1, atoi(id->columns[VCF_COLUMN_POS])+flanking);
		right = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(right);

		sprintf (str,"%s=%s|%s|%s",id->inputFilePrefix,left,fa->regionData,right);

		if (id->verbose) {
      	printf ("[%s:%d] - string to add '%s'\n\n", __FILE__, __LINE__, str);
		}
		vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str);
   }   
   fclose(id->inputFile);
}



/**
  
  **/
void vcf_MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES(struct input_data *id, struct output_data *od, struct fasta *fa) { 

	struct vcf * myVCF = vcf_init();
	int flanking = 1;  // we only want 1 bp flanking 

	char ref, alt;
	char str[1024];
	char *left;
	char *right;
	
	fasta_loadFile(fa);

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

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
		
		
		// left flanking region 
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS])-flanking, atoi(id->columns[VCF_COLUMN_POS])-1);
		left = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(left);

		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS]), atoi(id->columns[VCF_COLUMN_POS]));
		fa->regionData = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(fa->regionData);
		
		// right flanking region 
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR], atoi(id->columns[VCF_COLUMN_POS])+1, atoi(id->columns[VCF_COLUMN_POS])+flanking);
		right = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(right);


		if ((fa->regionData[0] == 'G') || (fa->regionData[0] == 'A')) {
			fa->regionData[0] = acgtn_getBaseComplement(fa->regionData[0]);
		}


		// get ref and alt bases 
		ref = id->columns[VCF_COLUMN_REF + id->columnOffSet][0];
		alt = id->columns[VCF_COLUMN_ALT + id->columnOffSet][0];


		if (
				((ref == 'G') && (alt == 'T')) || 
				((ref == 'G') && (alt == 'C')) ||
				((ref == 'G') && (alt == 'A')) || 
				((ref == 'A') && (alt == 'T')) || 
				((ref == 'A') && (alt == 'G')) || 
				((ref == 'A') && (alt == 'C'))  
				) {
			ref = acgtn_getBaseComplement(ref);
			alt = acgtn_getBaseComplement(alt);
		}

		
		//sprintf (str,"6_SUBSTITUTION_SUB_TYPE=%c->%c",ref, alt);
		sprintf (str,"MUTATION_SIGNATURE=%c%c%c,%c->%c",left[0], fa->regionData[0], right[0], ref, alt);

		if (id->verbose) {
      	printf ("[%s:%d] - string to add '%s'\n\n", __FILE__, __LINE__, str);
		}
		vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str);

		free(left);free(right); free(fa->regionData);

		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
}





/**
  bySample == 0 means just use REF and ALT
  bySample == 1 means use the genotype strings of individual samples
  **/
void vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) { 

	char ref, alt;
	int i;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - ERROR open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			if (id->verbose) 
				printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

		      
		if (id->line[0] == '#') {
			if (id->verbose) 
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

		// get ref and alt bases 
		ref = id->columns[VCF_COLUMN_REF + id->columnOffSet][0];
		alt = id->columns[VCF_COLUMN_ALT + id->columnOffSet][0];

		if (id->verbose) {
			printf ("\n[%s:%d] - %s\nREF and ALT\n%c %d \t%c %d\n", __FILE__, __LINE__, id->line,ref, acgtn_getIndex(ref), alt, acgtn_getIndex(alt));
		}

		if (bySample == 0 ) {
			// use index 0 to keep track of changes for the entire file ( i.e., not using the genotype string from individual )
			myVCF->snvSubstitutionMatrix[0][acgtn_getIndex(ref)][acgtn_getIndex(alt)]++;
			myVCF->totalNumberOfCallsPerSample[0]++;

			if (vcf_isATransitionChange(ref,alt))
				myVCF->Ts[0]++;
			if (vcf_isATransversionChange(ref,alt))
				myVCF->Tv[0]++;
		} else {
			for (i = VCF_COLUMN_FIRST_SAMPLE + id->columnOffSet; i <= id->n; i++) {
				if (id->verbose) 
					printf ("\n[%s:%d] - %s %s", __FILE__, __LINE__, myVCF->sampleNames[i - (VCF_COLUMN_FIRST_SAMPLE + id->columnOffSet)], id->columns[i]);
				if (!strstr(id->columns[i],"./.")) {
					myVCF->snvSubstitutionMatrix[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)][acgtn_getIndex(ref)][acgtn_getIndex(alt)]++;
					myVCF->totalNumberOfCallsPerSample[i - (VCF_COLUMN_FIRST_SAMPLE+id->columnOffSet)]++;
					if (vcf_isATransitionChange(ref,alt))
						myVCF->Ts[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)]++;
					if (vcf_isATransversionChange(ref,alt))
						myVCF->Tv[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)]++;
				}
			}
		}
   }   
   fclose(id->inputFile);
}




/**
  bySample == 0 means just use the REF and ALT columns
  bySample == 1 means use the genotype strings of individual samples
  **/
void vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX_WITH_SEQUENCE_CONTEXT(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample, struct fasta *fa ) { 

	char ref, alt;
	int i;
	char baseBefore[1024];
	char baseAfter[1024];
	int baseBeforeIndex, baseAfterIndex;
	int len;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - ERROR open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// load FASTA file if you provided 
	if (strlen(fa->inputFileName) > 0)
		fasta_loadFile(fa);

	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			if (id->verbose) 
				printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

		if (id->line[0] == '#') {
			if (id->verbose) 
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

		// get ref and alt bases 
		ref = id->columns[VCF_COLUMN_REF + id->columnOffSet][0];
		alt = id->columns[VCF_COLUMN_ALT + id->columnOffSet][0];

		// get base before and after only if users provided FASTA file 
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR+id->columnOffSet], atoi(id->columns[VCF_COLUMN_POS+id->columnOffSet])-1, atoi(id->columns[VCF_COLUMN_POS+id->columnOffSet])-1);
		fa->regionData = fai_fetch(fa->fai,fa->region,&len);
		my_string_toupper(fa->regionData);
		if(fa->regionData == NULL) {
			printf("\n\n[%s:%d] - ERROR: Given region: %s cannot be found.\n", __FILE__, __LINE__, fa->region);
			printf("\n\n");
			exit(1);
		}       
		strcpy(baseBefore, fa->regionData);
		free(fa->regionData);

		
		sprintf(fa->region,"%s:%d-%d", id->columns[VCF_COLUMN_CHR+id->columnOffSet], atoi(id->columns[VCF_COLUMN_POS+id->columnOffSet])+1, atoi(id->columns[VCF_COLUMN_POS+id->columnOffSet])+1);
		fa->regionData = fai_fetch(fa->fai,fa->region,&len);
		my_string_toupper(fa->regionData);
		if(fa->regionData == NULL) {
			printf("\n\n[%s:%d] - ERROR: Given region: %s cannot be found.\n", __FILE__, __LINE__, fa->region);
			printf("\n\n");
			exit(1);
		}       
		strcpy(baseAfter, fa->regionData);
		free(fa->regionData);


		baseBeforeIndex = acgtn_getIndex(baseBefore[0]);
		baseAfterIndex = acgtn_getIndex(baseAfter[0]);

		if (id->verbose) {
			printf ("\n[%s:%d] - %s\nREF and ALT\n%c %d \t%c %d\n", __FILE__, __LINE__, id->line,ref, acgtn_getIndex(ref), alt, acgtn_getIndex(alt));
			printf ("\nSEQUENCE CONTEXT\t%s%c%s", baseBefore, ref, baseAfter);
			fflush(stdout);
		}

		if (bySample == 0 ) {
			// use index 0 to keep track of changes for the entire file ( i.e., not using the genotype string from individual )
			myVCF->snvSubstitutionMatrixNNN[0][acgtn_getIndex(ref)][acgtn_getIndex(alt)][baseBeforeIndex][baseAfterIndex]++;
			myVCF->totalNumberOfCallsPerSample[0]++;
			

			if (vcf_isATransitionChange(ref,alt)) {
				myVCF->Ts[0]++;
			}
			if (vcf_isATransversionChange(ref,alt)) {
				myVCF->Tv[0]++;
			}
		} else {
			for (i = VCF_COLUMN_FIRST_SAMPLE + id->columnOffSet; i <= id->n; i++) {
				if (id->verbose) {
					printf ("\n[%s:%d] - %s %s", __FILE__, __LINE__, myVCF->sampleNames[i - (VCF_COLUMN_FIRST_SAMPLE + id->columnOffSet)], id->columns[i]);
					fflush(stdout);
				}

				if (!strstr(id->columns[i],"./.")) {
					myVCF->snvSubstitutionMatrixNNN[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)][acgtn_getIndex(ref)][acgtn_getIndex(alt)][baseBeforeIndex][baseAfterIndex]++;

					myVCF->totalNumberOfCallsPerSample[i - (VCF_COLUMN_FIRST_SAMPLE+id->columnOffSet)]++;

					if (vcf_isATransitionChange(ref,alt))
						myVCF->Ts[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)]++;
					if (vcf_isATransversionChange(ref,alt))
						myVCF->Tv[i-(VCF_COLUMN_FIRST_SAMPLE+ id->columnOffSet)]++;
				}
			}
		}
   }   
   fclose(id->inputFile);
}


void vcf_printTsTv(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) {
	int n, i;

	if (bySample) 
		n = myVCF->numberOfSamples;
	else {
		n = 1;
		strcpy(myVCF->sampleNames[0],".");
	}

	// print total number of calls for either file or individual samples
	printf ("\n\nTsTv_HEADER");
	printf ("\tfile_name");
	printf ("\tsample_name");
	printf ("\ttotal_number_of_calls");
	printf ("\tTs\tTv\tTs/Tv");

	for (i = 0; i < n; i++) {
		printf ("\nTsTv_DATA");
		printf ("\t%s",id->inputFileName);
		printf ("\t%s",myVCF->sampleNames[i]);
		printf ("\t%d",myVCF->totalNumberOfCallsPerSample[i]);
		printf ("\t%d",myVCF->Ts[i]);
		printf ("\t%d",myVCF->Tv[i]);
		printf ("\t%.2f",(1.0 * myVCF->Ts[i]) / myVCF->Tv[i]);
	}
}


void vcf_constructSubstitutionData12(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) {
	int i,j,k,n;

	if (bySample) 
		n = myVCF->numberOfSamples;
	else {
		n = 1;
		strcpy(myVCF->sampleNames[0],".");
	}
	
	printf("\n\ninput_file\tsample_name");
	for (j = 0; j < (NUMBER_OF_BASES-2); j++) {
		for (k = 0; k < (NUMBER_OF_BASES-2); k++) {
			if (j != k) {
				printf("\t%c->%c", acgtn_getBase(j),acgtn_getBase(k));
			}
		}
	}

	for (i = 0; i < n; i++) {
		printf("\n%s\t%s", id->inputFileName, myVCF->sampleNames[i]);
		for (j = 0; j < (NUMBER_OF_BASES-2); j++) {
			for (k = 0; k < (NUMBER_OF_BASES-2); k++) {
				if (j != k) {
					if (od->outputFormat == OUTPUT_AS_COUNTS)
						printf("\t%d", myVCF->snvSubstitutionMatrix[i][j][k]);
					else if (od->outputFormat == OUTPUT_AS_PERCENTAGES)
						printf("\t%.2f", (100.0*myVCF->snvSubstitutionMatrix[i][j][k])/myVCF->totalNumberOfCallsPerSample[i]);
				}
			}
		}
	}
}



void vcf_constructSubstitutionData96(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) {
	int i,j,k,n,p,q;

	if (bySample) 
		n = myVCF->numberOfSamples;
	else {
		n = 1;
		strcpy(myVCF->sampleNames[0],".");
	}

	printf ("\n\nMATRIX_HEADER\tfile_name\tsample_name");
	for (i = 0; i < n; i++) {
	//	printf ("\n%s", myVCF->sampleNames[i]);
		for (p = 0; p < (NUMBER_OF_BASES-2); p++) {
			for (q = 0; q < (NUMBER_OF_BASES-2); q++) {
				for (j = 0; j < (NUMBER_OF_BASES-2); j++) {
					for (k = 0; k < (NUMBER_OF_BASES-2); k++) {
						if (j != k) {
								printf ("\t%c%c%c:%c->%c", acgtn_getBase(p), acgtn_getBase(j), acgtn_getBase(q), acgtn_getBase(j),acgtn_getBase(k));
								fflush(stdout);
						
						}
					}
				}
			}
		}
	}
		
	printf ("\n");

	for (i = 0; i < n; i++) {
		printf ("MATRIX_DATA\t%s\t%s", id->inputFileName, myVCF->sampleNames[i]);
		for (p = 0; p < (NUMBER_OF_BASES-2); p++) {
			for (q = 0; q < (NUMBER_OF_BASES-2); q++) {
				for (j = 0; j < (NUMBER_OF_BASES-2); j++) {
					for (k = 0; k < (NUMBER_OF_BASES-2); k++) {
						if (j != k) {
							if (od->outputFormat == OUTPUT_AS_COUNTS)
								printf ("\t%d",myVCF->snvSubstitutionMatrixNNN[i][j][k][p][q]);
							else if (od->outputFormat == OUTPUT_AS_PERCENTAGES)
								printf ("\t%.2f",(100.0*myVCF->snvSubstitutionMatrixNNN[i][j][k][p][q])/myVCF->totalNumberOfCallsPerSample[i]);
							fflush(stdout);
						}
					}
				}
			}
		}
		printf ("\n");
	}


}



void vcf_printSubstitutionMatrix(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) {
	int i,j,k,n;

	if (bySample) 
		n = myVCF->numberOfSamples;
	else {
		n = 1;
		strcpy(myVCF->sampleNames[0],".");
	}

	printf ("\n\nMAXTRIX_HEADER\tfile_name\tsample_name\t.");
	for (j = 0; j < NUMBER_OF_BASES; j++) {
		printf ("\t%c", acgtn_getBase(j));
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < NUMBER_OF_BASES; j++) {
			if (n == 1)
				printf ("\nMATRIX_DATA\t%s\t.",id->inputFileName);
			else
				printf ("\nMATRIX_DATA\t%s\t%s",id->inputFileName, myVCF->sampleNames[i]); 
			printf ("\t%c", acgtn_getBase(j));
			for (k = 0; k < NUMBER_OF_BASES; k++) {
				//if (myVCF->snvSubstitutionMatrix [i][j][k] > 0)
				if (od->outputFormat == OUTPUT_AS_COUNTS)
					printf ("\t%d", myVCF->snvSubstitutionMatrix[i][j][k]);
				else {
					if (myVCF->snvSubstitutionMatrix[i][j][k] == 0)
						printf ("\t0");
					else 
						printf ("\t%.2f", (100.0*myVCF->snvSubstitutionMatrix[i][j][k])/myVCF->totalNumberOfCallsPerSample[i]);
				}
			}
		}
		printf ("\n\n");
	}
	printf ("\n");
}


void vcf_printSubstitutionMatrixHorizontal(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) {
	int i,j,k,n;

	if (bySample) 
		n = myVCF->numberOfSamples;
	else 
		n = 1;

	printf ("\n\nMAXTRIX_HEADER\tfile_name\tsample_name\t.");
	for (j = 0; j < NUMBER_OF_BASES; j++) {
		printf ("\t%c", acgtn_getBase(j));
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < NUMBER_OF_BASES; j++) {
			if (n == 1)
				printf ("\nMATRIX_DATA\t%s\t.",id->inputFileName);
			else
				printf ("\nMATRIX_DATA\t%s\t%s",id->inputFileName, myVCF->sampleNames[i]); 
			printf ("\t%c", acgtn_getBase(j));
			for (k = 0; k < NUMBER_OF_BASES; k++) {
				//if (myVCF->snvSubstitutionMatrix [i][j][k] > 0)
				if (od->outputFormat == OUTPUT_AS_COUNTS)
					printf ("\t%d", myVCF->snvSubstitutionMatrix[i][j][k]);
				else {
					if (myVCF->snvSubstitutionMatrix[i][j][k] == 0)
						printf ("\t0");
					else 
						printf ("\t%.2f", (100.0*myVCF->snvSubstitutionMatrix[i][j][k])/myVCF->totalNumberOfCallsPerSample[i]);
				}
			}
		}
		printf ("\n\n");
	}
	printf ("\n");
}


void vcf_addAnnotation2VCFINFO(struct input_data *id, char *columns[], int n, char data2add[]) {

	int i;

	for (i = 1; i <= n; i++) {
		printf ("%s", columns[i]);
		if (i == VCF_COLUMN_INFO + id->columnOffSet) {
			printf (";%s", data2add);
		}
		if (i < n)
			printf ("\t");
	}
	printf ("\n");
}

int vcf_isATransitionChange(char ref, char alt) {
	// https://www.mun.ca/biology/scarr/Transitions_vs_Transversions.html
	char rref = toupper(ref);
	char ralt = toupper(alt);
	if (
			((rref == 'A') && (ralt == 'G')) ||
			((rref == 'G') && (ralt == 'A')) ||
			((rref == 'C') && (ralt == 'T')) ||
			((rref == 'T') && (ralt == 'C')) 
		) 
		return 1;
	else 
		return 0;
}


int vcf_isATransversionChange(char ref, char alt) {
	// https://www.mun.ca/biology/scarr/Transitions_vs_Transversions.html
	char rref = toupper(ref);
	char ralt = toupper(alt);
	if (
			((rref == 'A') && (ralt == 'C')) ||
			((rref == 'C') && (ralt == 'A')) ||

			((rref == 'A') && (ralt == 'T')) ||
			((rref == 'T') && (ralt == 'A')) ||

			((rref == 'C') && (ralt == 'G')) ||
			((rref == 'G') && (ralt == 'C')) ||

			((rref == 'G') && (ralt == 'T')) ||
			((rref == 'T') && (ralt == 'G')) ||
			
			((rref == 'G') && (ralt == 'C')) ||
			((rref == 'C') && (ralt == 'G')) ||
			
			((rref == 'T') && (ralt == 'A')) ||
			((rref == 'A') && (ralt == 'T')) 
		) 
		return 1;
	else 
		return 0;
}





/**

  **/
void vcf_MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE(struct input_data *id, struct output_data *od, struct fasta *fa) { 

	struct vcf * myVCF = vcf_init();
	int printHeader = 1;
	char header[1024] = "\n\nThe following values in REF column do not match with sequences from input FASTA file:\n\n";


	fasta_loadFile(fa);

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - ERROR open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			//printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

		if (id->line[0] == '#') {
			//printf ("%s", id->line);
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
		 

		sprintf(fa->region,"%s:%s-%d", id->columns[VCF_COLUMN_CHR], id->columns[VCF_COLUMN_POS], atoi(id->columns[VCF_COLUMN_POS])+(int)strlen(id->columns[VCF_COLUMN_REF])-1);
		fa->regionData = fai_fetch(fa->fai,fa->region,&(fa->regionDataLength));
		my_string_toupper(fa->regionData);
		if (id->verbose) {
      	printf ("[%s:%d] - region to extract '%s'\n", __FILE__, __LINE__, fa->region);
      	printf ("[%s:%d] - region '%s'\n", __FILE__, __LINE__, fa->regionData);
		}

		// same 
		if (
				(strlen(fa->regionData) == strlen(id->columns[VCF_COLUMN_REF])) &&
				(strstr(fa->regionData,id->columns[VCF_COLUMN_REF]))
				) {
			if (id->verbose) {
				printf ("[%s:%d] - REF string and string extracted from FASTA file is the same\n", __FILE__, __LINE__);
			}
		} else {
				//printf ("[%s:%d] - REF string is not the same as the string extracted from FASTA file:\t%s\t%s\n", __FILE__, __LINE__,id->columns[VCF_COLUMN_REF], fa->regionData);
				if (printHeader) {
					// print header once 
					printf ("%s", header);
					printHeader = 0;
				}
				printf ("\n%s\tFASTA\t%s\tVCF\t%s\t%s",fa->region,fa->regionData, id->columns[VCF_COLUMN_REF], id->line);


		}

		input_data_freeMem(id->columns,id->n);
   }   
   fclose(id->inputFile);
}




/**
  **/
void vcf_MODE_VCF_PARSE_ANNOTATED_DATABASE(struct input_data *id, struct output_data *od, char key[], char recordSeparator[], char columnSeparator[]) {

	struct vcf * myVCF = vcf_init();

	char str[MAX_CHAR_PER_LINE];

	char textHeaderStr[MAX_CHAR_PER_LINE] = "";

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
		
		// make copy of TEXT_HEADER_ROW if it is there 
		if (strstr(id->line,TEXT_HEADER_ROW)) {
			strcpy(textHeaderStr, id->line);
			continue;
		}

		if (strstr(id->line,"#CHROM")) {
			utils_addTextHeader(textHeaderStr, key);
			printf ("%s\n", textHeaderStr);

			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);

			continue;
		}

		      
		if (id->line[0] == '#') {
			printf ("%s", id->line);
			continue;
		}

		if (id->verbose)  {
      	printf ("\n%s\n", LINE_DIVIDER_LONG);
      	printf ("[%s:%d] - %s", __FILE__, __LINE__, id->line);
			fflush(stdout);
		}	
		         
		// remove '\n' 
		id->line[strlen(id->line)-1] = '\0';

		id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));
		if (id->verbose)
			input_data_printParsedLineMemDebugging(id->columns, id->n);
			

		// make a copy because we will make some changes 
		strcpy(str, id->columns[VCF_COLUMN_INFO + id->columnOffSet]);
		
		int inDB, matches; 
		vcf_parseAnnotatedDatabase(id->verbose, id->line, str, key, &inDB, &matches, recordSeparator, columnSeparator);


   }   
   fclose(id->inputFile);
}


/**

data point to the following format
	COSMIC_v71=1,1,VARIANT_MATCHED<D7>chr1<D7>152081034<D7>COSM414263

line
	original line 

  **/

void vcf_parseAnnotatedVCFDatabase(int verbose, char line[], char infoColumn[], char databasePrefix[], int *inDatabase, int *matches, char recordSeparator[], char columnSeparator[]) {
	char *ptr;
	char *p;
	char str[1024];
	int i;

	ptr = strstr(infoColumn, databasePrefix);
	if (ptr) {


		// COSMIC_v73=1;1;VARIANT_MATCHED;COSM564710;[chr19|1220707|COSM564

		 if (verbose) 
			printf ("\n[%s:%d] - FOUND database prefix '%s' ->  '%s' ... ",__FILE__, __LINE__, databasePrefix, ptr);

		p = ptr;

		 while ((*p != '=') && (*p != '\0')) {
			 p++;
		 }
		 p++;
			 
		 *inDatabase = 0;
		 if (*p == '1')
			 *inDatabase = 1;

		 p++; // which pointed to ';'
		 p++;
		 // looking for number of matches 
		 i = 0;
		 while ((*p != ';') && (*p != '\0')) {
			 str[i] = *p;
			 i++;
			 p++;
		 }
		 str[i] = '\0';
		 *matches = atoi(str);
		
		 
		 if (verbose)  {
			if (*inDatabase == 1)
				printf ("\n\n[%s:%d] - IN database  %d matches ... ",__FILE__, __LINE__, *matches) ;
			else 
				printf ("\n\n[%s:%d] - NOT in database  ... ",__FILE__, __LINE__) ;
			printf ("\n\n");
			fflush(stdout);
		 }

		 p++; // move to description of matches 
		 if (*matches > 0) { 
			 int m = 0;
			 while ((m < *matches) && (*p != '\0')) {
				 printf ("%c", *p);
				 //if (*p == recordSeparator) { // record
				 if (*p == -40) { // record
					 m++;
					 //printf ("\t%s\n", line);
				 }
				 p++;
		 	}
			*p = '\0';
				//TESTING PARSING ANNOTION 
			printf ("\nAAAAAAAA %s", ptr);
			//printf ("\t%s\n", line);
		 } else {
			 printf ("not_in_%s\t%s\n", databasePrefix,line);
		 }

	} else {
		if (verbose) 
			printf ("\n[%s:%d] - failed to locate database prefix'%s' in INFO column \n\n",__FILE__, __LINE__, databasePrefix);
	}
}


/**

data point to the following format

MUTATION_TUMOR_PILEUP=1,1,chr1<D7>11576094<D7>G<D7>230<D7>...T

line
	original line 

  **/

void vcf_parseAnnotatedDatabase(int verbose, char line[], char infoColumn[], char databasePrefix[], int *inDatabase, int *matches, char recordSeparator[], char columnSeparator[]) {
	char *ptr;
	char *p;
	char str[1024];
	int i;

	// looking for database prefix 
	ptr = strstr(infoColumn, databasePrefix);
	if (ptr) {
		// AMPLICON=1,1,.<D7>chr1<D7>11562873<D7>11562989<D8>
		 if (verbose) 
			printf ("\n[%s:%d] - FOUND database prefix '%s' ->  '%s' ... ",__FILE__, __LINE__, databasePrefix, ptr);

		p = ptr;

		 while ((*p != '=') && (*p != '\0')) {
			 p++;
		 }
		 p++;
			 
		 *inDatabase = 0;
		 if (*p == '1')
			 *inDatabase = 1;

		 p++; // which pointed to ','
		 p++;
		 // looking for number of matches 
		 i = 0;
		 while ((*p != ',') && (*p != '\0')) {
			 str[i] = *p;
			 i++;
			 p++;
		 }
		 str[i] = '\0';
		 *matches = atoi(str);
		
		 
		 if (verbose)  {
			if (*inDatabase == 1)
				printf ("\n\n[%s:%d] - IN database  %d matches ... ",__FILE__, __LINE__, *matches) ;
			else 
				printf ("\n\n[%s:%d] - NOT in database  ... ",__FILE__, __LINE__) ;
			printf ("\n\n");
			fflush(stdout);
		 }

		 p++; // move to data themselve
		 if (*matches > 0) { 
			 int m = 0;
			 while ((m < *matches) && (*p != '\0')) {
				 //printf ("%c", *p);
				 if (*p == recordSeparator[0]) { // record
				 //if (*p == -40) { // record
					 m++;
					 //printf ("\t%s\n", line);
				 }
				 p++;
		 	}
			p--; // stop at the record separator 
			*p = '\0';
			printf ("%s\t%s\n", ptr, line);
		 } else {
			 printf ("not_in_%s\t%s\n", databasePrefix,line);
		 }
	} else {
		if (verbose) 
			printf ("\n[%s:%d] - failed to locate database prefix'%s' in INFO column \n\n",__FILE__, __LINE__, databasePrefix);
	}
}



/**
  **/
void vcf_MODE_VCF_PARSE_SNPEFF_ANN(struct input_data *id, struct output_data *od) { 

	struct vcf * myVCF = vcf_init();

	char outputData[100][1024];
	int outputDataN;

	int i;

   id->inputFile = fopen(id->inputFileName, "r");
	if (id->inputFile == NULL) {
		printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
		printf ("\n\n");
		exit (1);
	}
	   
	// read input file 
   while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {

		if (strstr(id->line,"#CHROM")) {
			printf ("%s", id->line);
			vcf_parseHEADER(id, myVCF);
			continue;
		}

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

		snpeff_parseANN(id, od, outputData, &outputDataN);

		input_data_freeMem(id->columns, id->n);
   }   
   fclose(id->inputFile);
}

