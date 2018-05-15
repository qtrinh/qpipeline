
## COSMIC Cancer Gene Census

This page describes how to create the COSMIC Cancer Gene Census database and how to use **_qpipeline_** to annotate VCF files with COSMIC Cancer Gene Census database.


Create a directory in *${QPIPELINE_HOME}/external_databases/cosmic* to store COSMIC Cancer Gene Census database
```
cd ${QPIPELINE_HOME}/external_databases/cosmic

# create a directory to store Cancer Gene Census database
mkdir cancer_gene_census
```
Follow instructions from the COSMIC website to download the COSMIC Cancer Gene Census - http://cancer.sanger.ac.uk/cosmic/download. User registration is required. 
Once downloaded, the following instructions described how to create the cancer census database and test it with qpipeline

```
# convert csv to txt 
${QPIPELINE_HOME}/scripts/convert.csv2tab.using.python.sh  < cancer_gene_census.csv > cancer_gene_census.txt

# create the chr, start, end as the first 3 columns and save it as tmp 
cat cancer_gene_census.txt | head -1 | awk -F"\t" '{ print "chr\tstart\tend\t"$0 }' > tmp;
cat cancer_gene_census.txt | grep -v Gene | awk -F"\t" '{ print "chr"$4"\t"$0 }' | sed 's/:-/:.-./' | sed 's/:/\t/' | sed 's/-/\t/' >> tmp
# remove entries where start and end are missing ( with values "." );
cat tmp | awk -F"\t" '($2!=".") || ($3!=".")' > _tmp ; mv _tmp tmp ;

# convert tmp to bed 
perl ${QPIPELINE_HOME}/scripts/tab2bed.pl  tmp 1 2 3 | sort -k1,1 -k2,2n -k3,3 > cancer_gene_census.txt.db.bed

# compress and index 
bgzip cancer_gene_census.txt.db.bed
tabix -p bed cancer_gene_census.txt.db.bed.gz
```
