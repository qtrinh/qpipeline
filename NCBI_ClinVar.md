## NCBI ClinVar VCF File

This page describes how to create the ClinVar VCF database and how to use **_qpipeline_** to annotate VCF files with ClinVar VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/clinvar* to store ClinVar VCF database 
```
cd ${QPIPELINE_HOME}/external_databases/clinvar 
```

Download ClinVar VCF database ( check ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37 for latest version ). As of March 30, 2018, 'clinvar_20180225.vcf.gz' is the latest version. 
```
# Set FILE variable for the ClinVar VCF file to be downloaded ( we will be using this variable a few times )
FILE="clinvar_20180225.vcf.gz"

# use wget to download the ClinVar
wget ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/${FILE} --no-passive-ftp
```
Add the 'chr' prefix and save it as ${FILE}.modified.vcf 
```
zcat $FILE  | grep ^# > ${FILE}.modified.vcf 
zcat $FILE  | grep -v ^# | awk '{ print "chr"$0 }' >>  ${FILE}.modified.vcf 
```
Compress and index 
```
# compress 
bgzip ${FILE}.modified.vcf 

# index 
tabix -p vcf ${FILE}.modified.vcf.gz 
```
Test to see if **_qpipeline_** works with the newly created database
```
# take a few lines from the newly created database as a test file
zcat  ${FILE}.modified.vcf.gz  | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against ${FILE}.modified.vcf.gz database.  
qpipeline tabix -m 2020 -i test.vcf  -d  ${FILE}.modified.vcf.gz  -q clinvar_20180225 | less 
```
All of the entries in test.vcf should be annotated as in ${FILE}.modified.vcf.gz database.
