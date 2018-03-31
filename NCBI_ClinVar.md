## NCBI ClinVar VCF File

This page describes how to create the ClinVar VCF database and how to use **_qpipeline_** to annotate VCF files with ClinVar VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/clinvar* to store ClinVar VCF database 
```
cd ${QPIPELINE_HOME}/external_databases/clinvar 
```

Download ClinVar VCF database ( check ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37 for latest version ) 
```
wget ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/clinvar_20180128.vcf.gz --no-passive-ftp
```
Set FILE variable to the downloaded ClinVar VCF file ( we will be using this a few times )
```
FILE="clinvar_20180128.vcf.gz"
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

# use qpipeline to annotate the test.vcf file against clinvar_20180128.modified.vcf.g database.  
qpipeline tabix -m 2020 -i test.vcf  -d  ${FILE}.modified.vcf.gz  -q ClinVar_20180128 | less 
```
All of the entries in test.vcf should be annotated as in clinvar_20180128.vcf.gz.modified.vcf.gz database.
