
## ICGC Copy Number Somatic Mutation

This page describes how to create the ICGC Copy Number Somatic Mutation database and how to use **_qpipeline_** to annotate user data with it.

Create a directory to store the ICGC copy number database - for example, BRCA-US ( https://dcc.icgc.org/releases/release_26/Projects/BRCA-US )
```
# using similar structure as the ICGC
mkdir -p ${QPIPELINE_HOME}/external_databases/icgc/releases/release_26/Projects/BRCA-US/
```
Download the copy number somatic mutation database from ICGC, for example BRCA-US ( copy_number_somatic_mutation.BRCA-US.tsv.gz from https://dcc.icgc.org/releases/release_26/Projects/BRCA-US ) and save it to _${QPIPELINE_HOME}/external_databases/icgc/releases/release_26/Projects/BRCA-US/_ 
```
# set FILE variable to point to the downloaded file
FILE="copy_number_somatic_mutation.BRCA-US.tsv.gz"

# add 'chr', 'start', and 'end' as the first 3 columns; sort by genomic positions
zcat $FILE | head -1 | awk '{ print "chr\tstart\tend\t"$0 }' > ${FILE}.tmp
zcat $FILE | grep -v icgc_donor |  awk -F"\t" '{ print "chr"$12"\t"$13"\t"$14"\t"$0 }' | sort -k1,1 -k2,2n -k3,3n >> ${FILE}.tmp  

# make columns 4 and on as key-value pairs
qpipeline txt -m 1500 -i ${FILE}.tmp | sed 's/\xD7/;/g' | sed 's/chr=//' | sed 's/start=//' | sed 's/end=//' | sed 's/;/\t/2' | sed 's/;/\t/' | sed 's/;/\t/' > ${FILE}.bed 

# compress using bgzip 
bgzip ${FILE}.bed  
  
# index using tabix
tabix -p bed  ${FILE}.bed.gz 

# remove intermediate file 
rm  ${FILE}.tmp
```
Testing the newly created database with **_qpipeline_**
```
# Use the file '${QPIPELINE_HOME}/test_data/vcf/test.vcf' to test qpipeline with the newly created database
qpipeline tabix -m 2000 -i ${QPIPELINE_HOME}/test_data/vcf/test.vcf  -d copy_number_somatic_mutation.BRCA-US.tsv.gz.bed.gz  -q BRCA-US | less
```
