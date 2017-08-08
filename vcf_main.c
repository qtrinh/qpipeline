//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>

#include "input_data.h"
#include "output_data.h"
#include "common.h"

#include "vcf.h"
#include "my_tabix.h"

/**
  vcf usage 
  **/
void vcf_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for processing VCF files.");
	printf( "\n\n\t%s vcf [ -m mode ] [ options ]",
			argv[0]);
	printf("\n");
	
	if ((id->mode == MODE_VCF_FILTER_PASS_FILTERS_ONLY) || (id->mode == 0))  {
		printf("\n\t-m %d\tprint PASS filter entries only.",MODE_VCF_FILTER_PASS_FILTERS_ONLY);
		if (id->mode == MODE_VCF_FILTER_PASS_FILTERS_ONLY) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-R\tprint only those entries that failed PASS filter.");
			
			printf ("\n\n\t\tExample:\n\t\t\t%s vcf -m %d -i test_data/vcf/test.vcf", argv[0], MODE_VCF_FILTER_PASS_FILTERS_ONLY);     
		}
	}
	if ((id->mode == MODE_VCF_FILTER_SNV_OR_INDEL) || (id->mode == 0))  {
		printf("\n\t-m %d\tprint substitutions or indels .",MODE_VCF_FILTER_SNV_OR_INDEL);
		if (id->mode == MODE_VCF_FILTER_SNV_OR_INDEL) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-t\tINT\tvariant class");
			printf("\n\t\t\t%d\tsubstitutions ( single base or mutliple bases )", VARIANT_CLASS_SUBSTITUTION);
			printf("\n\t\t\t%d\tindels", VARIANT_CLASS_INDELS );
		}
	}

	if (id->mode == 0)
		printf ("\n\nparse");

	if ((id->mode == MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse INFO column for a particular attribute.",MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY);
		if (id->mode == MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-k STR\tattribute in INFO column to parse or to look for.");
			printf("\n\t\t-s INT\tminimum value for attribute indicated by -k to consider for printing.  Output is VCF format. ( OPTIONAL )");
			printf("\n\t\t-R\tprint only entries where attribute values indicated by -k are less than value indicated by -s INT .  Output is VCF format. ( OPTIONAL )");
			         
			printf ("\n\n\t\tExample:\n\t\t\t%s vcf -m %d -i test_data/vcf/test.vcf -k AC", argv[0], MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY);     

		}
	}
	
	if ((id->mode == MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse FORMAT column for a particular attribute.",MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY);
		if (id->mode == MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-k STR\tattribute in FORMAT column to parse or to look for.");
			printf("\n\t\t-s STR\tprint only entries where values of attribute indicated by -k are greater than or equal this value for all samples.  Output is in VCF format. ( OPTIONAL )");
			printf("\n\t\t-z INT\tprocess this sample index only instead of all samples ( 0 for first sample, 1 for second sample, and so on ).");
		}
	}

	if ((id->mode == MODE_VCF_PARSE_SNPEFF_ANN) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse SNPEFF ANN.", MODE_VCF_PARSE_SNPEFF_ANN);
		if (id->mode == MODE_VCF_PARSE_SNPEFF_ANN)  {
			printf("\n\t\t-i FILE\tinput VCF file.");
		}
	}

	  
	if ((id->mode == MODE_VCF_PARSE_ANNOTATED_DATABASE) || (id->mode == 0))  {
		printf("\n\t-m %d\tparse annotated database ( %s tabix -m %d ).", MODE_VCF_PARSE_ANNOTATED_DATABASE, argv[0], MODE_TABIX_ANNOTATE_VCF_WITH_VCF);
		if (id->mode == MODE_VCF_PARSE_ANNOTATED_DATABASE)  {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-k STR\tannnotated database identifier.  e.g., COSMIC_v69.");
			//printf("\n\t\t-r STR\tmatching record separator (OPTIONAL)");
			//printf("\n\t\t-c STR\tmatching column separator (OPTIONAL)");
		}
	}



	if (id->mode == 0)
		printf ("\n\nannotate");


	if ((id->mode == MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT) || (id->mode == 0))  {
		printf("\n\t-m %d\tannotate sequence context.",MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT);
		if (id->mode == MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-f FILE\tinput FASTA file.");
			printf("\n\t\t-l INT\tflanking length - default is 1 ( i.e., 1 base to the left and right of the variant site ).");
			printf("\n\t\t-p STR\tprefix to use in output annotation ( default is SEQUENCE_CONTEXT )");
			
			printf ("\n\n\t\tExample:\n\t\t\t%s vcf -m %d -i test_data/vcf/test.vcf -f test_data/fasta/chrM.fa ", argv[0], MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT);     
		}
	}
	if ((id->mode == MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES) || (id->mode == 0))  {
		printf("\n\t-m %d\tannotate 6 substitution sub-types ( C->A, C->T, etc. )",MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES);
		if (id->mode == MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-f FILE\tinput FASTA file.");
		}
	}
	
	if (id->mode == 0)
		printf ("\n\ncalculate");
	if ((id->mode == MODE_VCF_GENERATE_SUBSTITUTION_MATRIX) || (id->mode == 0))  {
		printf("\n\t-m %d\tcalculate SNV substitution matrix.",MODE_VCF_GENERATE_SUBSTITUTION_MATRIX);
		if (id->mode == MODE_VCF_GENERATE_SUBSTITUTION_MATRIX) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-f FILE\tFASTA file.");
			printf("\n\t\t-S\tuse the genotype string from the individual samples to calculate substitution matrix instead of using just REF and ALT bases.");
			printf("\n\t\t-P\toutput substituion matrix as percentages");
			
			printf ("\n\n\t\tExample:\n\t\t\t%s vcf -m %d -i test_data/vcf/test.vcf", argv[0], MODE_VCF_GENERATE_SUBSTITUTION_MATRIX);     
		}
	}

	if (id->mode == 0)
		printf ("\n\nvalidate or checking ");
	if ((id->mode == MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE) || (id->mode == 0))  {
		printf("\n\t-m %d\tcheck to ensure values in reference (REF) column match with FASTA file.",MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE);
		if (id->mode == MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE) {
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-f FILE\tFASTA file.");
			
			printf ("\n\n\t\tExample:\n\t\t\t%s vcf -m %d -i test_data/vcf/test.vcf -f test_data/fasta/chrM.fa ", argv[0], MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE);     
		}
	}

	printf("\n\n");
}


/**
	vcf main
**/
void vcf_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();
	struct fasta *fa = fasta_init();
	struct vcf * myVCF = vcf_init();

	int flanking = 1;

	// for subsitution matrix calculation, 
	// bySample == 1 means use genotype string
	// bySample == 0 means just use REF and ALT to calculate 
	int bySample = 0;

	char key[1024] = "";
	double value = INT_MIN;
	// default, process all samples
	int sampleIndex2Proces = PROCESS_ALL_SAMPLES;

	int variantClassToPrint = -1;

	char recordSeparator[1024]="", columnSeparator[1024] = "";

	od->outputFormat = OUTPUT_AS_COUNTS;

	while ((c = getopt(argc, argv, "r:c:l:f:t:n:m:i:p:k:s:z:RvHSP")) != -1) {
		switch (c) {
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'p':
			strcpy(id->inputFilePrefix, optarg);
			break;
		case 'l':
			flanking = atoi(optarg);
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'f':
			strcpy(fa->inputFileName, optarg);
			break;
		case 'k':
			strcpy(key, optarg);
			break;
		case 'r':
			strcpy(recordSeparator, optarg);
			break;
		case 'c':
			strcpy(columnSeparator, optarg);
			break;
		case 's':
			value = atof(optarg);
			break;
		case 'z':
			sampleIndex2Proces = atoi(optarg);
			break;
		case 't':
			variantClassToPrint = atof(optarg);
			break;
		case 'H':
			od->outputOrientation = OUTPUT_ORIENTATION_HORIZONTAL;
			break;
		case 'P':
			od->outputFormat = OUTPUT_AS_PERCENTAGES;
			break;
		case 'S':
			bySample = 1;
			break;
		case 'n':
			id->columnOffSet = atoi(optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		case 'R':
			id->printRejected = 1;
			break;
		default:
			//abort();
			vcf_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\n");
			exit(0);
		}
	}

	if (id->mode == 0) {
		vcf_main_Usage(argc,argv,id);
		exit(1);
	}

	if (strlen(id->inputFileName)==0)  {
		vcf_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	/*} else if (strstr(id->inputFileName,"/dev/stdin") && (strlen(id->inputFilePrefix)==0))  {
		vcf_main_Usage(argc,argv,id);
		printf ("\tMissing input file prefix (-q).  Please see usage above!\n\n");
		exit(1); */
	}

	if (id->mode == MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY) {
		if (strlen(key) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing key value (-k).  Please see usage above!\n\n");
			exit(1);
		}
		vcf_MODE_VCF_PARSE_INFO_COLUMN_FOR_KEY(id,od,key,value);
	} else if (id->mode == MODE_VCF_PARSE_ANNOTATED_DATABASE) {
		if (strlen(key) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing key value (-k).  Please see usage above!\n\n");
			exit(1);
		}
		vcf_MODE_VCF_PARSE_ANNOTATED_DATABASE(id,od,key,recordSeparator,columnSeparator);
	} else if (id->mode == MODE_VCF_PARSE_SNPEFF_ANN) {
		vcf_MODE_VCF_PARSE_SNPEFF_ANN(id, od);
	} else if (id->mode == MODE_VCF_FILTER_PASS_FILTERS_ONLY) {
		vcf_MODE_VCF_FILTER_PASS_FILTERS_ONLY(id, od);
	} else if (id->mode == MODE_VCF_FILTER_SNV_OR_INDEL) {
		if (variantClassToPrint == -1) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing variant type to print (-t).  Please see usage above!\n\n");
			exit(1);
		}
		vcf_MODE_VCF_FILTER_SNV_OR_INDEL(id, variantClassToPrint, od);
	} else if (id->mode == MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY) {
		if (strlen(key) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing key value (-k).  Please see usage above!\n\n");
			exit(1);
		}
		/*if (sampleIndex2Proces == -1) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing index of sample (-z).  Please see usage above!\n\n");
			exit(1);
		}*/
		vcf_MODE_VCF_PARSE_FORMAT_COLUMN_FOR_KEY(id, od,key,value, sampleIndex2Proces);
	} else if (id->mode == MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT) {
		if (strlen(id->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing input file (-i).  Please see usage above!\n\n");
			exit(1);
		}
		if (strlen(fa->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing input FASTA file (-f).  Please see usage above!\n\n");
			exit(1);
		}
		if (strlen(id->inputFilePrefix)==0)
			strcpy(id->inputFilePrefix,"SEQUENCE_CONTEXT");
		vcf_MODE_VCF_ANNOTATE_SEQUENCE_CONTEXT(id,od,fa, flanking);
	} else if (id->mode == MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES) {
		if (strlen(id->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing input file (-i).  Please see usage above!\n\n");
			exit(1);
		}
		if (strlen(fa->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing input FASTA file (-f).  Please see usage above!\n\n");
			exit(1);
		}
		vcf_MODE_VCF_ANNOTATE_SUBSTITUTION_SUB_TYPES(id,od, fa);

	} else if (id->mode == MODE_VCF_ANNOTATE_GENOTYPE) {
		//vcf_MODE_VCF_ANNOTATE_GENOTYPE(id,od);

	} else if (id->mode == MODE_VCF_GENERATE_SUBSTITUTION_MATRIX) {
	
		// if there is no input FASTA file 
		if (strlen(fa->inputFileName) > 0) {
			vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX_WITH_SEQUENCE_CONTEXT(id, od, myVCF, bySample,fa);
			
			vcf_printTsTv(id, od, myVCF, bySample) ;
			vcf_constructSubstitutionData96(id, od, myVCF, bySample);
			printf ("\n\n");
		} else  {
			vcf_MODE_VCF_GENERATE_SUBSTITUTION_MATRIX(id, od, myVCF, bySample);

			vcf_printTsTv(id, od, myVCF, bySample) ;
			vcf_constructSubstitutionData12(id, od, myVCF, bySample);
			printf ("\n\n");
		}


		/*if (od->outputOrientation == OUTPUT_ORIENTATION_HORIZONTAL)
			vcf_printSubstitutionMatrixHorizontal(id, od, myVCF, bySample);
		else 
			vcf_printSubstitutionMatrix(id, od, myVCF, bySample);
			*/
	} else if (id->mode == MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE) {
		if (strlen(id->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing input file (-i).  Please see usage above!\n\n");
			exit(1);
		}
		if (strlen(fa->inputFileName) == 0) {
			vcf_main_Usage(argc,argv,id);
			printf ("\tMissing fasta file (-f).  Please see usage above!\n\n");
			exit(1);
		}
		vcf_MODE_VCF_CHECK_REFERENCE_BASE_AGAINST_FASTA_FILE(id,od,fa);
	}
}



