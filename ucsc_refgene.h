//
// written by Quang M Trinh <quang.trinh@gmail.com>
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef REFGENE_H
#define REFGENE_H

#include "input_data.h"
#include "output_data.h"



/**
  refgene column definition from UCSC 

1	bin 	970	smallint(5) unsigned 	range 	Indexing field to speed chromosome range queries.
2	name 	NM_213590	varchar(255) 	values 	Name of gene (usually transcript_id from GTF)
3	chrom 	chr13	varchar(255) 	values 	Reference sequence chromosome or scaffold
4	strand 	+	char(1) 	values 	+ or - for strand
5	txStart 	50571142	int(10) unsigned 	range 	Transcription start position
6	txEnd 	50592603	int(10) unsigned 	range 	Transcription end position
7	cdsStart 	50586076	int(10) unsigned 	range 	Coding region start
8	cdsEnd 	50587300	int(10) unsigned 	range 	Coding region end
9	exonCount 	2	int(10) unsigned 	range 	Number of exons
10	exonStarts 	50571142,50586070,	longblob 	  	Exon start positions
11	exonEnds 	50571899,50592603,	longblob 	  	Exon end positions
12	score 	0	int(11) 	range 	score
13	name2 	TRIM13	varchar(255) 	values 	Alternate name (e.g. gene_id from GTF)
14	cdsStartStat 	cmpl	enum('none', 'unk', 'incmpl', 'cmpl') 	values 	enum('none','unk','incmpl','cmpl')
15	cmpl	enum('none', 'unk', 'incmpl', 'cmpl') 	values 	enum('none','unk','incmpl','cmpl')
16	exonFrames 	-1,0,	longblob 	  	Exon frame {0,1,2}, or -1 if no frame for exon

**/


#define REFGENE_COLUMN_NAME 2
#define REFGENE_COLUMN_CHROM 3
#define REFGENE_COLUMN_STRAND 4
#define REFGENE_COLUMN_TX_START 5
#define REFGENE_COLUMN_TX_END 6
#define REFGENE_COLUMN_CDS_START 7
#define REFGENE_COLUMN_CDS_END 8

#define REFGENE_COLUMN_EXON_COUNT 9
#define REFGENE_COLUMN_EXON_STARTS 10
#define REFGENE_COLUMN_EXON_ENDS 11
#define REFGENE_COLUMN_SCORE 12
#define REFGENE_COLUMN_NAME2 13
#define REFGENE_COLUMN_CDS_START_STAT 14
#define REFGENE_COLUMN_CMPL 15
#define REFGENE_COLUMN_EXON_FRAMES 16


#define MODE_PARSE_REFGENE 1001
#define MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH 1002

void ucsc_refgene_MODE_PARSE_REFGENE(struct input_data *id, struct output_data *od) ;
void ucsc_refgene_MODE_REFGENE_PARSE_EXTRACT_CODING_LENGTH(struct input_data *id, struct output_data *od, int exonPadding) ;




#endif 
