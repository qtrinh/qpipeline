
## COSMIC Cancer Gene Census

This page describes how to create the COSMIC Cancer Gene Census database and how to use **_qpipeline_** to annotate VCF files with COSMIC Cancer Gene Census database.


Create a directory to store COSMIC Cancer Gene Census database
```
mkdir -p ${QPIPELINE_HOME}/external_databases/cosmic/cancer_gene_census
```
Follow instructions from the COSMIC website to download the COSMIC Cancer Gene Census - http://cancer.sanger.ac.uk/cosmic/download. User registration is required. 
Once downloaded, the following instructions described how to create the cancer census database and test it with qpipeline

```
# set FILE variable to point to the cancer census database
FILE="cancer_gene_census.csv"

# convert csv to txt 
${QPIPELINE_HOME}/scripts/convert.csv2tab.using.python.sh  < $FILE > ${FILE}.txt

# create the chr, start, end as the first 3 columns and save it as ${FILE}.txt.tmp  
cat ${FILE}.txt | head -1 | awk -F"\t" '{ print "chr\tstart\tend\t"$0 }' > ${FILE}.txt.tmp;
cat ${FILE}.txt | grep -v Gene | awk -F"\t" '{ print "chr"$4"\t"$0 }' | sed 's/:-/:.-./' | sed 's/:/\t/' | sed 's/-/\t/' >> ${FILE}.txt.tmp
# remove entries where start and end are missing ( with values "." );
cat ${FILE}.txt.tmp | awk -F"\t" '($2!=".") || ($3!=".")' > tmp ; mv tmp ${FILE}.txt.tmp ;

# convert to bed 
perl ${QPIPELINE_HOME}/scripts/tab2bed.pl  ${FILE}.txt.tmp  1 2 3 | sort -k1,1 -k2,2n -k3,3 > ${FILE}.bed

# compress and index 
bgzip ${FILE}.bed
tabix -p bed ${FILE}.bed.gz 
```
