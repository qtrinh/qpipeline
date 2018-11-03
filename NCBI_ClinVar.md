## NCBI ClinVar VCF File

This page describes how to create ClinVar VCF database and how to use **_qpipeline_** to annotate user data with it.

Create a directory to store NCBI ClinVar database
```
# create directory
mkdir ${QPIPELINE_HOME}/external_databases/ncbi/clinvar 
# change into it
cd ${QPIPELINE_HOME}/external_databases/ncbi/clinvar 
```

Download and use tabix to index ClinVar VCF database 
```
# Set FILE_URL variable to point to the ClinVar VCF database file to be downloaded
# ( we will be using this variable a few times )
FILE_URL="FILE_URL="ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/clinvar.vcf.gz""

# use wget to download the ClinVar VCF database file
wget $FILE_URL --no-passive-ftp

# use basename to get just the file name from FILE_URL so we can save our modifications to it 
FILE=`basename $FILE_URL`;

# Add the 'chr' prefix and save it as ${FILE} 
zcat $FILE | grep ^# > ${FILE}.modified.vcf 
zcat $FILE | grep -v ^# | awk '{ print "chr"$0 }' >>  ${FILE}.modified.vcf 

# compress using bgzip 
bgzip ${FILE}.modified.vcf 

# index using tabix
tabix -p vcf ${FILE}.modified.vcf.gz 
```
Test the newly created database with **_qpipeline_** 
```
# take a few lines from the newly created database and use it as a test file
zcat  ${FILE}.modified.vcf.gz  | head -200 > test.vcf 

# use qpipeline to annotate test.vcf file against the newly created database (${FILE}.modified.vcf.gz ).  
# all of the entries in test.vcf should be annotated as in ClinVar database!
qpipeline tabix -m 2020 -i test.vcf  -d  ${FILE}.modified.vcf.gz  -q `basename $FILE .vcf.gz` | less 

```
