## NCBI ClinVar VCF File

This page describes how to create the ClinVar VCF database and how to use **_qpipeline_** to annotate VCF files with ClinVar VCF database.


Create 'clinvar' directory in *${QPIPELINE_HOME}/external_databases/ncbi* 
```
cd ${QPIPELINE_HOME}/external_databases/ncbi/clinvar 
```

Download ClinVar VCF database ( check ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37 for latest version ). As of April 10, 2018, 'clinvar_20180401.vcf.gz' is the latest version.  Previous versions of ClinVar are in 'archive' directory under ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37. 
```
# Set FILE_URL variable for the ClinVar VCF file to be downloaded ( we will be using this variable a few times )
FILE_URL="ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/clinvar_20180401.vcf.gz"

# use wget to download the ClinVar VCF file
wget $FILE_URL --no-passive-ftp

# use basename to get just the file name from FILE_URL so we can modify it 
FILE=`basename $FILE_URL`;

# Add the 'chr' prefix and save it as ${FILE} 
zcat $FILE | grep ^# > ${FILE}.modified.vcf 
zcat $FILE | grep -v ^# | awk '{ print "chr"$0 }' >>  ${FILE}.modified.vcf 

# compress using bgzip 
bgzip ${FILE}.modified.vcf 

# index using tabix
tabix -p vcf ${FILE}.modified.vcf.gz 

# Test to see if qpipeline works with the newly created database
# take a few lines from the newly created database as a test file
zcat  ${FILE}.modified.vcf.gz  | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against ${FILE}.modified.vcf.gz database.  
# all of the entries in test.vcf should be annotated as in ClinVar database!
qpipeline tabix -m 2020 -i test.vcf  -d  ${FILE}.modified.vcf.gz  -q `basename $FILE .vcf.gz` | less 

```
