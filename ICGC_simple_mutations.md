
## ICGC Simple Somatic Mutations 

This page describes how to create the ICGC Simple Somatic Mutations database and how to use **_qpipeline_** to annotate VCF files with it

Create a directory in *${QPIPELINE_HOME}/external_databases* to store the ICGC simple somatic mutations database - see https://dcc.icgc.org/releases
```
# using the same structure as the ICGC - https://dcc.icgc.org/releases/release_26/Summary 
cd ${QPIPELINE_HOME}/external_databases/icgc/release_26/Summary
```
Download the simple somatic mutation from  https://dcc.icgc.org/releases/release_26/Summary
```
# in ${QPIPELINE_HOME}/external_databases/icgc/release_26/Summary
# set FILE variable to point to the simple_somatic_mutation.aggregated.vcf.gz
FILE="simple_somatic_mutation.aggregated.vcf.gz"

# add 'chr' prefix, change 'chrMT' to 'chrM'; sort by genomic positions
zcat $FILE | head -100 | grep ^# > ${FILE}.modified.vcf 
zcat $FILE | grep -v ^# | awk '{ print "chr"$0 }' | sed 's/chrMT/chrM/' |  sort -k1,1 -k2,2n >> ${FILE}.modified.vcf 

# compress using bgzip 
bgzip ${FILE}.modified.vcf 
 
 
# index using tabix
tabix -p vcf  ${FILE}.modified.vcf.gz 
 
```
