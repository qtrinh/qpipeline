

This page describes how to create the ClinVar VCF database and how to use **_qpipeline_** to annotate VCF files with ClinVar VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/clinvar* to store ClinVar VCF database 
```
cd ${QPIPELINE_HOME}/external_databases/clinvar 
```

Download ClinVar VCF database
```
wget ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37/clinvar_20180128.vcf.gz --no-passive-ftp
```
Add the 'chr' prefix and save it as 'clinvar_20180128.modified.vcf'
```
zcat clinvar_20180128.vcf.gz  | grep ^# > clinvar_20180128.modified.vcf
zcat clinvar_20180128.vcf.gz  | grep -v ^# | awk '{ print "chr"$0 }' >> clinvar_20180128.modified.vcf
```
Compress and index 
```
bgzip clinvar_20180128.modified.vcf

tabix -p vcf clinvar_20180128.modified.vcf.gz 
```
Test to see if **_qpipeline_** works with the newly created database
```
# take the first 200 lines from the newly created database as a test file
zcat clinvar_20180128.modified.vcf.gz | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against clinvar_20180128.modified.vcf.g database.  
qpipeline tabix -m 2020 -i test.vcf  -d clinvar_20180128.modified.vcf.gz -q ClinVar_0180128 | less 
```
All of the entries in test.vcf should be annotated as in clinvar_20180128.modified.vcf.gz database.
