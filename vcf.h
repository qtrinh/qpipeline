//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef VCF_H
#define VCF_H 

#include "input_data.h"
#include "output_data.h"
#include "fasta.h"


#define MODE_VCF_FILTER_PASS_FILTERS_ONLY 1000
#define MODE_VCF_FILTER_SNV_OR_INDEL 1005

#define MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY 1500
#define MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY 1600


#define MODE_VCF_PARSE_SNPEFF_ANN 1800 

#define MODE_VCF_PARSE_ANNOTATED_DATABASE 2000 

#define MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT 2500 
#define MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES 2505 
#define MODE_VCF_ANNOTATE_GENOTYPE 2510 

#define MODE_VCF_GENERATE_SUBSTITUTION_MATRIX 3000 

#define MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE  3100 



#define VCF_COLUMN_ALT_SEPARATOR ','
enum VCF_COLUMNS {
	VCF_COLUMN,
	VCF_COLUMN_CHR,
	VCF_COLUMN_POS,
	VCF_COLUMN_ID,
	VCF_COLUMN_REF,
	VCF_COLUMN_ALT,
	VCF_COLUMN_QUAL,
	VCF_COLUMN_FILTER,
	VCF_COLUMN_INFO,
	VCF_COLUMN_FORMAT,
	VCF_COLUMN_FIRST_SAMPLE
};

struct vcf {
	int columnOffSet;
	  
	int numberOfSamples;
	// first sample is at index 0;
	char **sampleNames; 


	// first attribute and value are at index 0 
	char **formatKeys; // array of strings
	char ***formatValues;
	int formatN;

	// 3D int array to store SNV substitution changes
	// first dimention: sample
	// second dimention: reference 
	// third dimention: alt ( i.e., changed to )
	int ***snvSubstitutionMatrix;
	
	// 5D int array to store SNV substitution changes
	// first dimention: ???
	// second dimention: ???
	// third dimention: ???
	int *****snvSubstitutionMatrixNNN;
	
	int *totalNumberOfCallsPerSample;

	// https://www.mun.ca/biology/scarr/Transitions_vs_Transversions.html
	int *Ts, *Tv;

	        
	// text headers
	char **textHeaders;
	int textHeadersN;

};


struct vcf * vcf_init() ;


void vcf_setInputFileToVCFformat(struct input_data *id) ;
vcf_compare_result_t vcf_compareEntry(struct input_data *id, char *a[], int an, char *b[], int bn);
void vcf_MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY(struct input_data *id, struct output_data *od, char key[], double value) ;
void vcf_MODE_VCF_FILTER_PASS_FILTERS_ONLY(struct input_data *id, struct output_data *od) ;
void vcf_parseHEADER(struct input_data * id, struct vcf *vcf) ;
void vcf_parseFORMAT(struct input_data * id, struct vcf *vcf) ;
void vcf_printFORMAT(struct input_data * id, struct vcf *vcf) ;

void vcf_MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY(struct input_data *id, struct output_data *od, char key[], double value, int sampleIndex2Process) ;
void vcf_MODE_VCF_PARSE_ANNOTATED_DATABASE(struct input_data *id, struct output_data *od, char key[], char recordSeparator[], char columnSeparator[]) ;
void vcf_parseAnnotatedDatabase(int verbose, char line[], char infoColumn[], char databasePrefix[], int *inDatabase, int *matches, char recordSeparator[], char columnSeparator[]) ;




void vcf_MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT(struct input_data *id, struct output_data *od, struct fasta *fa, int flanking) ;

void vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;
void vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX_WITH_SEQUENCE_CONTEXT(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample, struct fasta *fa ) ;


void vcf_printTsTv(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;
void vcf_printSubstitutionMatrix(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;
void vcf_printSubstitutionMatrixHorizontal(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;

void vcf_addAnnotation2VCFINFO(struct input_data *id, char *columns[], int n, char data2add[]) ;

void vcf_constructSubstitutionData12(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;
void vcf_constructSubstitutionData96(struct input_data *id, struct output_data *od, struct vcf *myVCF, int bySample) ;

int vcf_isATransitionChange(char ref, char alt) ;
int vcf_isATransversionChange(char ref, char alt) ;

void vcf_MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE(struct input_data *id, struct output_data *od, struct fasta *fa) ;


#endif
