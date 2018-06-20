
## ICGC Copy Number Somatic Mutation

This page describes how to create the ICGC Copy Number Somatic Mutation database and how to use **_qpipeline_** to annotate VCF files with it

Create a directory in *${QPIPELINE_HOME}/external_databases* to store the ICGC copy number somatic mutation database - see https://dcc.icgc.org/releases
```
# using the same structure as the ICGC - https://dcc.icgc.org/releases/release_26/Summary 
cd ${QPIPELINE_HOME}/external_databases/icgc/release_26/Projects/BRCA-US/
```
Download the copy number somatic mutation from the ICGC, for example BRCA-US https://dcc.icgc.org/releases/release_26/Projects/BRCA-US
```
# in  ${QPIPELINE_HOME}/external_databases/icgc/release_26/Projects/BRCA-US/ 
# set FILE variable to point to the downloaded file
FILE="copy_number_somatic_mutation.BRCA-US.tsv.gz"

# add 'chr', 'start', and 'end' as the first 3 columns; sort by genomic positions
zcat $FILE | head -1 | awk '{ print "chr\tstart\tend\t"$0 }' > ${FILE}.modified.tsv
zcat $FILE | grep -v icgc_donor |  awk -F"\t" '{ print "chr"$12"\t"$13"\t"$14"\t"$0 }' | sort -k1,1 -k2,2n -k3,3n >> ${FILE}.modified.tsv  


# compress using bgzip 
bgzip ${FILE}.modified.tsv 
 
 
# index using tabix
tabix -p bed  ${FILE}.modified.tsv.gz 


# Use the file '${QPIPELINE_HOME}/test_data/vcf/test.vcf' to test qpipeline with the newly created database
qpipeline tabix -m 2000 -i ${QPIPELINE_HOME}/test_data/vcf/test.vcf  -d copy_number_somatic_mutation.BRCA-US.tsv.gz.modified.tsv.gz  -q BRCA-US | less
```
