
//
// written by Quang M Trinh <quang.trinh@gmail.com>
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bgzf.h"
#include "tabix.h"

#include "my_tabix.h"
#include "pileup.h"
#include "acgtn.h"
#include "common.h"
#include "vcf.h"

/**
  initialize a tabix structure 
  **/
struct my_tabix * tabix_init() {

	struct my_tabix *t = (struct my_tabix *) malloc (1 * sizeof(struct my_tabix));
	t->inputFileFormat = FILE_FORMAT_NOT_DEFINED;

	t->includeDataFromDatabase = 0;

	return t;
}

/**
  load tabix file 
  **/
void tabix_loadFile(struct my_tabix *tabix) {

	if ((tabix->t = ti_open(tabix->inputFileName, 0)) == 0) {
		fprintf(stderr, "\n\n[%s:%d] fail to open tabix data file '%s'.\n\n", __FILE__, __LINE__, tabix->inputFileName);
		exit (1);
	}
			      
	if ( ti_lazy_index_load(tabix->t) ) {
		fprintf (stderr, "\n\n[%s:%d] - failed to load tabix index file '%s'\n\n", __FILE__, __LINE__, tabix->inputFileName);
		exit (1);
	} 
}




/**
  annotate VCF file with tabix index BED file 
  **/
void tabix_MODE_TABIX_ANNOTATE_VCF_WITH_BED(struct input_data *id, struct output_data *od, struct my_tabix *tabix) {

	char str[MAX_CHAR_PER_COLUMN];

	acgtn_t *acgtn = acgtn_Init();

	// open input file 
   id->inputFile = fopen(id->inputFileName, "r");
   if (id->inputFile == NULL) {
      printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
      printf ("\n\n");
      exit (1);
   }   
	
	{ 
		tabix_loadFile(tabix);
		
		// get number of columns in tabix file 
		//tabix_getNumberOfColumns(tabix);

		while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
			// adding info on that target file used
			if (strstr(id->line,"CHROM")) {
				printf ("##%s,BED_FILE=%s\n",tabix->inputFilePrefix, tabix->inputFileName); 
				printf ("%s", id->line);
				continue;
			}

			if (id->line[0] == '#') {
				printf ("%s", id->line);
				continue;
			}


			// remove newline at the end of line 
			if (id->line[strlen(id->line)-1] == '\n')
				id->line[strlen(id->line)-1] = '\0';

			id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));

			sprintf(tabix->region,"%s:%s-%s", id->columns[id->chrColumnNumber], id->columns[id->startColumnNumber], id->columns[id->endColumnNumber]);

			if (id->verbose) {
				printf("\n\n======================");
				printf("\n[%s:%d]\n%s\n'%s'\n", __FILE__, __LINE__, id->line,tabix->region);
				printf("\n");
			}
			
			tabix->matches = 0;
			strcpy(tabix->outputData,"");

			if (ti_parse_region(tabix->t->idx, tabix->region, &tabix->tid, &tabix->beg, &tabix->end) == 0) {
				tabix->iter = ti_queryi(tabix->t, tabix->tid, tabix->beg, tabix->end);

				while ((tabix->data = ti_read(tabix->t, tabix->iter, &tabix->len)) != 0 ) {
					
					tabix->matches++;

					if (id->verbose) {
						printf("[%s:%d] - FOUND entry %d in tabix databse:\t'%s'", __FILE__, __LINE__, tabix->matches, tabix->data);
						printf("\n\n");
					}

					//sprintf (tabix->datacpy,"%s\t%s", "on-target", tabix->data);
					sprintf (tabix->datacpy,".\t%s", tabix->data);
					tabix->columns = input_data_parseLineMem(id, tabix->datacpy, '\t', &(tabix->n));
					if (id->verbose)
						input_data_printParsedLineMemDebugging(tabix->columns, tabix->n);
			
					my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);

					//free(id->tabix->data);
				}
				ti_iter_destroy(tabix->iter);
			}
			if (tabix->matches == 0) {
				if (id->verbose) {
					printf("[%s:%d] - didn't find any entries in tabix database ... ", __FILE__, __LINE__);
					printf("\n\n");
				}
				strcpy(tabix->outputData, ".");
			} else {
				if (id->verbose)
					printf ("\nstring to add to VCF INFO column:\t%s\n\n", tabix->outputData);
			}
				
			my_tabix_formatAnnotation(str, tabix->inputFilePrefix, tabix->matches, tabix->outputData);

			vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str); 

			// free mem allocated by Input_parseLineMem 
			input_data_freeMem(id->columns, id->n);
	}
	ti_close(tabix->t);
	fclose(id->inputFile);
	}
}





/**
  annotate txt file with tabix index BED file 
  **/
void tabix_MODE_TABIX_ANNOTATE_TXT_WITH_BED(struct input_data *id, struct output_data *od, struct my_tabix *tabix, int chrColumn , int posColumn ) {

	char str[MAX_CHAR_PER_COLUMN];

	acgtn_t *acgtn = acgtn_Init();

	// open input file 
   id->inputFile = fopen(id->inputFileName, "r");
   if (id->inputFile == NULL) {
      printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
      printf ("\n\n");
      exit (1);
   }   
	
	{ 
		tabix_loadFile(tabix);
		
		// get number of columns in tabix file 
		//tabix_getNumberOfColumns(tabix);

		while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
			// adding info on that target file used
			if (strstr(id->line,"CHROM")) {
				printf ("##%s,BED_FILE=%s\n",tabix->inputFilePrefix, tabix->inputFileName); 
				printf ("%s", id->line);
				continue;
			}

			if (id->line[0] == '#') {
				printf ("%s", id->line);
				continue;
			}


			// remove newline at the end of line 
			if (id->line[strlen(id->line)-1] == '\n')
				id->line[strlen(id->line)-1] = '\0';

			id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));

			sprintf(tabix->region,"%s:%s-%s", id->columns[id->chrColumnNumber], id->columns[id->startColumnNumber], id->columns[id->endColumnNumber]);

			if (id->verbose) {
				printf("\n\n======================");
				printf("\n[%s:%d]\n%s\n'%s'\n", __FILE__, __LINE__, id->line,tabix->region);
				printf("\n");
			}
			
			tabix->matches = 0;
			strcpy(tabix->outputData,"");

			if (ti_parse_region(tabix->t->idx, tabix->region, &tabix->tid, &tabix->beg, &tabix->end) == 0) {
				tabix->iter = ti_queryi(tabix->t, tabix->tid, tabix->beg, tabix->end);

				while ((tabix->data = ti_read(tabix->t, tabix->iter, &tabix->len)) != 0 ) {
					
					tabix->matches++;

					if (id->verbose) {
						printf("[%s:%d] - FOUND entry %d in tabix databse:\t'%s'", __FILE__, __LINE__, tabix->matches, tabix->data);
						printf("\n\n");
					}

					//sprintf (tabix->datacpy,"%s\t%s", "on-target", tabix->data);
					sprintf (tabix->datacpy,".\t%s", tabix->data);
					tabix->columns = input_data_parseLineMem(id, tabix->datacpy, '\t', &(tabix->n));
					if (id->verbose)
						input_data_printParsedLineMemDebugging(tabix->columns, tabix->n);
			
					my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);

					//free(id->tabix->data);
				}
				ti_iter_destroy(tabix->iter);
			}
			if (tabix->matches == 0) {
				if (id->verbose) {
					printf("[%s:%d] - didn't find any entries in tabix database ... ", __FILE__, __LINE__);
					printf("\n\n");
				}
				strcpy(tabix->outputData, ".");
			} else {
				if (id->verbose)
					printf ("\nstring to add to output:\t%s\n\n", tabix->outputData);
			}
				
			my_tabix_formatAnnotation(str, tabix->inputFilePrefix, tabix->matches, tabix->outputData);

			printf ("%s\t%s\n", id->line, str);
			//vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str); 

			// free mem allocated by Input_parseLineMem 
			input_data_freeMem(id->columns, id->n);
	}
	ti_close(tabix->t);
	fclose(id->inputFile);
	}
}



/**
  annotate VCF file with PILEUP file index by tabix 
  **/
void tabix_MODE_TABIX_ANNOTATE_VCF_WITH_PILEUP(struct input_data *id, struct output_data *od, struct my_tabix *tabix) {

	char str[MAX_CHAR_PER_COLUMN];
	char pileupSummary[MAX_CHAR_PER_COLUMN] = "";

	acgtn_t *acgtn = acgtn_Init();

	// open input file 
   id->inputFile = fopen(id->inputFileName, "r");
   if (id->inputFile == NULL) {
      printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
      printf ("\n\n");
      exit (1);
   }   
	
	{ 
		tabix_loadFile(tabix);
		
		// get number of columns in tabix file 
		//tabix_getNumberOfColumns(tabix);

		while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
			if (id->line[0] == '#') {
				printf ("%s", id->line);
				continue;
			}


			// remove newline at the end of line 
			if (id->line[strlen(id->line)-1] == '\n')
				id->line[strlen(id->line)-1] = '\0';

			id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));

			sprintf(tabix->region,"%s:%s-%s", id->columns[id->chrColumnNumber], id->columns[id->startColumnNumber], id->columns[id->endColumnNumber]);

			if (id->verbose) {
				printf("\n\n======================");
				printf("\n[%s:%d]\n%s\n'%s'\n", __FILE__, __LINE__, id->line,tabix->region);
				printf("\n");
			}
			
			tabix->matches = 0;
			strcpy(tabix->outputData,"");

			if (ti_parse_region(tabix->t->idx, tabix->region, &tabix->tid, &tabix->beg, &tabix->end) == 0) {
				tabix->iter = ti_queryi(tabix->t, tabix->tid, tabix->beg, tabix->end);

				while ((tabix->data = ti_read(tabix->t, tabix->iter, &tabix->len)) != 0 ) {
					
					tabix->matches++;

					if (id->verbose) {
						printf("[%s:%d] - FOUND entry %d in tabix databse:\t'%s'", __FILE__, __LINE__, tabix->matches, tabix->data);
						printf("\n\n");
					}

					//sprintf (tabix->datacpy,"%s\t%s", "on-target", tabix->data);
					sprintf (tabix->datacpy,".\t%s", tabix->data);
					tabix->columns = input_data_parseLineMem(id, tabix->datacpy, '\t', &(tabix->n));
					if (id->verbose)
						input_data_printParsedLineMemDebugging(tabix->columns, tabix->n);
			
					if (tabix->inputFileFormat == FILE_FORMAT_PILEUP) {
						// compute count of ACGTN
						acgtn_reset(acgtn);
						pileup_countACGTN(id, acgtn, tabix->columns[PILEUP_COLUMN_BASES+1], tabix->columns[PILEUP_COLUMN_QUALITY+1], id->columns[VCF_COLUMN_REF][0], 0, 0) ;
						if (id->verbose) {
							acgtn_printCount(acgtn);
							printf ("\nPILEUP SUMMARY\t%s", pileupSummary);
						}
						acgtn_printCount2Str(acgtn, str);
						sprintf (pileupSummary, "%s_SUMMARY=%s;", tabix->inputFilePrefix, str);
					}

					my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);

					//free(id->tabix->data);
				}
				ti_iter_destroy(tabix->iter);
			}
			if (tabix->matches == 0) {
				if (id->verbose) {
					printf("[%s:%d] - didn't find any entries in tabix database ... ", __FILE__, __LINE__);
					printf("\n\n");
				}
				strcpy(tabix->outputData, ".");
			} else {
				if (id->verbose)
					printf ("\nstring to add to VCF INFO column:\t%s\n\n", tabix->outputData);
			}
				
			my_tabix_formatAnnotation(str, tabix->inputFilePrefix, tabix->matches, tabix->outputData);


			if (tabix->inputFileFormat == FILE_FORMAT_PILEUP){
				strcat(pileupSummary, str);
				vcf_addAnnotation2VCFINFO(id, id->columns, id->n, pileupSummary); 
			} else {
				vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str); 
			}

			// free mem allocated by Input_parseLineMem 
			input_data_freeMem(id->columns, id->n);
	}
	ti_close(tabix->t);
	fclose(id->inputFile);
	}
}

/**
  common format to be output: prefix, number of matches, data 
  **/
void my_tabix_formatAnnotation(char output[], char prefix[], int matches, char data[]) {

	if (matches == 0)
		sprintf(output, "%s=0,%d,%s",prefix, matches, data);
	else
		sprintf(output, "%s=1,%d,%s",prefix, matches, data);
}

void my_tabix_concatAnnotationData(char in[], char out[]) {
	char str[MAX_CHAR_PER_COLUMN];

	my_tabix_tab2InternalColumnSeparator(in,str);

	str[strlen(str)+1] = '\0';
	// add record separator 
	str[strlen(str)] = RECORD_SEPARATOR;

	if (strlen(out) == 0) {
		strcpy(out,str);
	} else {
		strcat(out,str);
	}
}

void my_tabix_tab2InternalColumnSeparator(char in[], char out[]) {
	char *ptr;

	strcpy(out, in);
	ptr = out;
	while (*ptr != '\0') {
		if (*ptr == '\t')
			*ptr = COLUMN_SEPARATOR;
		ptr++;
	}
}


/**

  **/
void tabix_MODE_TABIX_ANNOTATE_VCF_WITH_VCF(struct input_data *id, struct output_data *od, struct my_tabix *tabix) {

	char str[MAX_CHAR_PER_LINE];

	// open input file 
   id->inputFile = fopen(id->inputFileName, "r");
   if (id->inputFile == NULL) {
      printf ("\n[%s:%d] - error open file '%s'", __FILE__, __LINE__, id->inputFileName);
      printf ("\n\n");
      exit (1);
   }   
	
	{ 
		tabix_loadFile(tabix);
		
		// get number of columns in tabix file 
		//tabix_getNumberOfColumns(tabix);

		while ((fgets(id->line, MAX_CHAR_PER_LINE, id->inputFile) != NULL)) {
			if (strstr(id->line,"#CHROM")) {
				printf ("##VCF_DATABASE_FILE:%s=%s\n", tabix->inputFilePrefix, tabix->inputFileName);
				printf ("%s", id->line);
				continue;
			}
			
			if (id->line[0] == '#') {
				printf ("%s", id->line);
				continue;
			}

			// remove newline at the end of line 
			if (id->line[strlen(id->line)-1] == '\n')
				id->line[strlen(id->line)-1] = '\0';

			id->columns = input_data_parseLineMem(id, id->line, '\t', &(id->n));

			//sprintf(tabix->region,"%s:%s-%s", id->columns[id->chrColumnNumber], id->columns[id->startColumnNumber], id->columns[id->endColumnNumber]);
			sprintf(tabix->region,"%s:%s-%s", id->columns[VCF_COLUMN_CHR + id->columnOffSet], id->columns[VCF_COLUMN_POS + id->columnOffSet], id->columns[VCF_COLUMN_POS + id->columnOffSet]);

			if (id->verbose) {
				printf ("\n\n%s\n", LINE_DIVIDER_LONG);
				printf("\n[%s:%d]\n'%s'\n'%s'\n", __FILE__, __LINE__, id->line,tabix->region);
				printf("\n");
			}
			
			tabix->matches = 0;
			// found not necessary a match 
			tabix->entryFound = 0;

			strcpy(tabix->outputData,"");


			if (ti_parse_region(tabix->t->idx, tabix->region, &tabix->tid, &tabix->beg, &tabix->end) == 0) {
				tabix->iter = ti_queryi(tabix->t, tabix->tid, tabix->beg, tabix->end);

				while ((tabix->data = ti_read(tabix->t, tabix->iter, &tabix->len)) != 0 ) {
					
					tabix->entryFound++;
					strcpy(tabix->datacpy, tabix->data);
			
					if (id->verbose) {
						printf ("\n%s\n", LINE_DIVIDER_SHORT);
						printf("[%s:%d] - FOUND entry %d in tabix databse:\t'%s'", __FILE__, __LINE__, tabix->entryFound, tabix->datacpy);
						printf("\n\n");
						fflush(stdout);
					}
					tabix->columns = input_data_parseLineMem(id, tabix->datacpy, '\t', &(tabix->n));
					if (id->verbose) 
						input_data_printParsedLineMemDebugging(tabix->columns, tabix->n);

					vcf_compare_result_t r = vcf_compareEntry(id, id->columns, id->n, tabix->columns, tabix->n);

					if (r == VCF_ENTRY_MATCHED) {
						tabix->matches++;
						sprintf (tabix->datacpy, "%s_VARIANT_MATCHED\t%s",tabix->inputFilePrefix, tabix->data);
						my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);
					} else if (r == VCF_VARIANT_CLASS_MISMATCHED ) {
						//tabix->matches++;
						//sprintf (tabix->datacpy, "%s\t%s","VARIANT_MATCHED",tabix->data);
						//my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);
					} else if (r == VCF_REF_MISMATCHED) {
						tabix->matches++;
						sprintf (tabix->datacpy, "%s\t%s","REF_BASE_MISMATCHED",tabix->data);
						my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);
					} else if ((r == VCF_ALT_MISMATCHED) && (tabix->considerMisMatchedALT == 1)){
						tabix->matches++;
						sprintf (tabix->datacpy, "%s_ALT_BASE_MISMATCHED\t%s",tabix->inputFilePrefix,tabix->data);
						my_tabix_concatAnnotationData(tabix->datacpy,tabix->outputData);
					}

					input_data_freeMem(tabix->columns, tabix->n);

					//free(id->tabix->data);
				}
				ti_iter_destroy(tabix->iter);
			}
			if (tabix->matches == 0) {
				if (id->verbose) {
					printf("[%s:%d] - didn't find any entries in tabix database ... no data added", __FILE__, __LINE__);
					printf("\n\n");
					fflush(stdout);
				}
				strcpy(tabix->outputData,".");
			} else {
				if (id->verbose) {
					printf ("\n[%s:%d] - %d matches", __FILE__, __LINE__, tabix->matches);
					if (tabix->matches > 1)
						printf (" ( multiple matches )");
					printf ("\n[%s:%d] - string to add to VCF INFO column ( length %d ) :\t%s\n\n", __FILE__, __LINE__, (int)strlen(tabix->outputData), tabix->outputData);
					fflush(stdout);
				}
			}
				
			my_tabix_formatAnnotation(str, tabix->inputFilePrefix, tabix->matches, tabix->outputData);

			vcf_addAnnotation2VCFINFO(id, id->columns, id->n, str);

			// free mem allocated by Input_parseLineMem 
			input_data_freeMem(id->columns, id->n);
	}
	ti_close(tabix->t);
	fclose(id->inputFile);
	}
}





