//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef COMMON_H
#define COMMON_H 


#define AUTHORS "\nWritten by Quang M Trinh <quang.trinh@gmail.com>. \n"

#define TEXT_HEADER_ROW "##TEXT_HEADER_ROW::"

#define LINE_DIVIDER_LONG  "###########################################"
#define LINE_DIVIDER_SHORT "======================"

// max number of characters per line 
#define MAX_CHAR_PER_LINE 5000000

// max number of columns
#define MAX_NUMBER_OF_COLUMNS 100
// max number of characters per column 
#define MAX_CHAR_PER_COLUMN 500000

// max read length
#define MAX_READ_LENGTH 150
// max read quality value
#define MAX_READ_QUALITY_VALUE 150 

#define MAX_CHROMOSOME_LENGTH 2500000000 

// different file formats supported
typedef enum FILE_FORMAT { FILE_FORMAT_NOT_DEFINED, FILE_FORMAT_FASTA, FILE_FORMAT_VCF, FILE_FORMAT_UCSC_dbSNP, FILE_FORMAT_NCBI_VCF_dbSNP, FILE_FORMAT_TCGA_MAF, FILE_FORMAT_PILEUP, FILE_FORMAT_BED} file_format_t ;

typedef enum LOG_INDEX { LOG_ERROR, LOG_WARNING, LOG_INFO } log_index_t ;


typedef enum OUTPUT_FORMAT { OUTPUT_AS_COUNTS, OUTPUT_AS_PERCENTAGES} output_format_t ;
// output horizontal or vertical  
typedef enum OUTPUT_ORIENTATION { OUTPUT_ORIENTATION_HORIZONTAL, OUTPUT_ORIENTATION_VERTICAL } output_orientation_t;
// output left or right of input data
typedef enum OUTPUT_POSITION { OUTPUT_ORIENTATION_LEFT, OUTPUT_ORIENTATION_RIGHT } output_position_t;

typedef enum VCF_COMPARE_RESULT { VCF_ENTRY_MATCHED, VCF_CHR_MISMATCHED, VCF_POS_MISMATCHED, VCF_REF_MISMATCHED, VCF_ALT_MISMATCHED, VCF_VARIANT_CLASS_MISMATCHED } vcf_compare_result_t;

typedef enum VARIANT_CLASS { 
        VARIANT_CLASS_NOT_DEFINED, 
        VARIANT_CLASS_SUBSTITUTION, 
        VARIANT_CLASS_INDELS, 
        VARIANT_CLASS_SUBSTITUTION_SINGLE_BASE, 
        VARIANT_CLASS_SUBSTITUTION_MULTIPLE_BASES, 
        VARIANT_CLASS_INSERTION, 
        VARIANT_CLASS_DELETION, 
        VARIANT_CLASS_NOT_ABLE_TO_DERIVE_FROM_REF_AND_ALT,
        VARIANT_CLASS_INVALID_REF_STRING,
        VARIANT_CLASS_INVALID_ALT_STRING
}variant_class_t ;


// http://www.alanwood.net/demos/ansi.html
#define COLUMN_SEPARATOR 0xD7  // -41 decimal 
#define RECORD_SEPARATOR 0xD8   // -40 decimal 


#define PROCESS_ALL_SAMPLES 999999
#define MAX_BASE_QUALITY 40 



#endif 
