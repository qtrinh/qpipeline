//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "input_data.h"
#include "output_data.h"
#include "vcf.h"
#include "my_tabix.h"


/**
  tabix usage 
  **/
void tabix_main_Usage(int argc, char *argv[], struct input_data *id) {
	
	printf("%s", AUTHORS);
	printf("\nUtils for annotating VCF files using tabix.");
	printf( "\n\n\t%s tabix [ -m mode ]  [ options ]",
			argv[0]);
	printf("\n");


	if ((id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_BED) || (id->mode == 0))  {
		printf("\n\t-m %d\tannotate a VCF file against a BED database file.", MODE_TABIX_ANNOTATE_VCF_WITH_BED);
		if (id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_BED)  {
			printf("\n\t\t-d TABIX_DATABASE_FILE\tdatabase file indexed by tabix ( .gz ).");
			printf("\n\t\t-q STR\tprefix string to annotate.  e.g., ON_TARGET.");
			printf("\n\t\t-i FILE\tinput VCF file.");
			//printf("\n\t\t-N\tdo not include data from TABIX_DATABASE_FILE in output.  Default, all columns in TABIX_DATABASE_FILE are included in output.");

			printf ("\n\n\t\tExample:\n\t\t\t%s tabix -m %d -d test_data/vcf/target.bed.gz -i test_data/vcf/sample.vcf -q ON_TARGET", argv[0], MODE_TABIX_ANNOTATE_VCF_WITH_BED);
		}
	}

	if ((id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_VCF) || (id->mode == 0))  {
		printf("\n\t-m %d\tannotate a VCF file against a VCF database file.", MODE_TABIX_ANNOTATE_VCF_WITH_VCF);
		if (id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_VCF)  {
			printf("\n\t\t-d TABIX_DATABASE_FILE\tdatabase file indexed by tabix ( .gz ).");
			printf("\n\t\t-q STR\tVCF database identifier.  e.g., COSMIC_69.");
			printf("\n\t\t-i FILE\tinput VCF file.");
			printf("\n\t\t-R\tinclude matches where the ALT do not match.  Default, both REF and ALT must match.");
			//printf("\n\t\t-N\tdo not include data from TABIX_DATABASE_FILE in output.  Default, all columns in TABIX_DATABASE_FILE are included in output.");

			printf ("\n\n\t\tExample:\n\t\t\t%s tabix -m %d -d test_data/vcf/dbSNP_147.vcf.gz -i test_data/vcf/sample.vcf -q dbSNP_147", argv[0], MODE_TABIX_ANNOTATE_VCF_WITH_VCF);
		}
	}
	if ((id->mode == MODE_TABIX_ANNOTATE_TXT_WITH_BED) || (id->mode == 0))  {
		printf("\n\t-m %d\tannotate a txt file against a txt database file.", MODE_TABIX_ANNOTATE_TXT_WITH_BED);
		if (id->mode == MODE_TABIX_ANNOTATE_TXT_WITH_BED)  {
			printf("\n\t\t-d TABIX_DATABASE_FILE\tdatabase file indexed by tabix ( .gz ). The first three columns must be chr, start, and end.");
			printf("\n\t\t-q STR\tdatabase identifier.  e.g., PANEL_PROBES.");
			printf("\n\t\t-i FILE\tinput txt file.  The first three columns must be chr, start, and end.");
			//printf("\n\t\t-R\tinclude matches where the ALT do not match.  Default, both REF and ALT must match.");

		}
	}
	
	printf("\n\n");
}


/**
	tabix main
**/
void tabix_main(int argc, char *argv[]) {
	char c;
	struct input_data *id = input_data_init();
	struct output_data *od = output_data_init();
	struct my_tabix *tabix = tabix_init();
	tabix->considerMisMatchedALT = 0;
	int chrColumn = -1, posColumn = -1;

	// assume input file will be in VCF format 
	vcf_setInputFileToVCFformat(id);

	while ((c = getopt(argc, argv, "F:m:i:d:q:n:s:b:vNR")) != -1) {
		switch (c) {
		case 'F':
			tabix->inputFileFormat = atoi(optarg);
			break;
		case 'R':
			tabix->considerMisMatchedALT = 1;
			break;
		case 'm':
			id->mode = atoi(optarg);
			break;
		case 'd':
			strcpy(tabix->inputFileName, optarg);
			break;
		case 'q':
			strcpy(tabix->inputFilePrefix, optarg);
			break;
		case 'N':
			tabix->includeDataFromDatabase = 1;
			break;
		case 'i':
			strcpy(id->inputFileName, optarg);
			break;
		case 'n':
			id->columnOffSet = atoi(optarg);
			break;
		case 's':
			chrColumn = atoi(optarg);
			break;
		case 'b':
			posColumn = atoi(optarg);
			break;
		case 'v':
			id->verbose = 1;
			break;
		default:
			//abort();
			tabix_main_Usage(argc, argv, id);
			printf("\tInvalid option.  Please see usage above\n\n");
			exit(0);
		}
	}

	if (id->mode == 0) {
		tabix_main_Usage(argc,argv,id);
		exit(1);
	}

	if (strlen(id->inputFileName)==0)  {
		tabix_main_Usage(argc,argv,id);
		printf ("\tMissing input file (-i).  Please see usage above!\n\n");
		exit(1);
	} else if ((strlen(tabix->inputFileName)==0) || (strlen(tabix->inputFilePrefix)==0)) {
		tabix_main_Usage(argc,argv,id);
		printf ("\tMissing database file (-d) or database file prefix (-q).  Please see usage above!\n\n");
		exit(1);
	}


	if (id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_BED) {
		tabix_MODE_TABIX_ANNOTATE_VCF_WITH_BED(id,od,tabix);
	} else if (id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_PILEUP) {
		tabix_MODE_TABIX_ANNOTATE_VCF_WITH_PILEUP(id,od,tabix);
	} else if (id->mode == MODE_TABIX_ANNOTATE_VCF_WITH_VCF) {
		tabix_MODE_TABIX_ANNOTATE_VCF_WITH_VCF(id,od,tabix);
	} else if (id->mode == MODE_TABIX_ANNOTATE_TXT_WITH_BED) {
		id->chrColumnNumber = 1;
		id->startColumnNumber = 2;
		id->endColumnNumber = 3;
		tabix_MODE_TABIX_ANNOTATE_TXT_WITH_BED(id,od,tabix,chrColumn,posColumn);
	}
}



