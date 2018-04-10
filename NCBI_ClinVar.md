## NCBI ClinVar VCF File

This page describes how to create the ClinVar VCF database and how to use **_qpipeline_** to annotate VCF files with ClinVar VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/clinvar* to store ClinVar VCF database 
```
cd ${QPIPELINE_HOME}/external_databases/clinvar 
```

Download ClinVar VCF database ( check ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37 for latest version ). As of April 10, 2018, 'clinvar_20180401.vcf.gz' is the latest version.  Previous versions of ClinVar are in 'archive' directory under ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37. 
```
# Set FILE_URL variable for the ClinVar VCF file to be downloaded ( we will be using this variable a few times )
FILE_URL="ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/clinvar_20180401.vcf.gz"

# use wget to download the ClinVar VCF file
wget $FILE_URL --no-passive-ftp

# use basename to get just the file name from FILE_URL so we can modify it 
FILE_NAME=`basename $FILE_URL`;

# Add the 'chr' prefix and save it as ${FILE_NAME} 
zcat $FILE_NAME | grep ^# > ${FILE_NAME}.modified.vcf 
zcat $FILE_NAME | grep -v ^# | awk '{ print "chr"$0 }' >>  ${FILE_NAME}.modified.vcf 

# compress using bgzip 
bgzip ${FILE_NAME}.modified.vcf 

# index using tabix
tabix -p vcf ${FILE_NAME}.modified.vcf.gz 

# Test to see if qpipeline works with the newly created database
# take a few lines from the newly created database as a test file
zcat  ${FILE_NAME}.modified.vcf.gz  | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against ${FILE_NAME}.modified.vcf.gz database.  
# All of the entries in test.vcf should be annotated as in ClinVar database!

qpipeline tabix -m 2020 -i test.vcf  -d  ${FILE_NAME}.modified.vcf.gz  -q $FILE_NAME | less 

```
