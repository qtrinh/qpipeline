



This page describes how to create the COSMIC VCF database and how to use **_qpipeline_** to annotate VCF files with COSMIC VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/cosmic* to store COSMIC VCF database ( e.g., v83 for version 83 )
```
cd ${QPIPELINE_HOME}/external_databases/cosmic 

mkdir v83 ; cd v83
```

Follow instructions from the COSMIC website to download both coding and non-coding VCF files - http://cancer.sanger.ac.uk/cosmic/download.  User registration is required. The following are instructions on how to download the coding and non-coding VCF files using sftp:
```
sftp "email@somewhere.com"@sftp-cancer.sanger.ac.uk

cd /cosmic/grch37/cosmic/v83/VCF

mget CosmicCodingMuts.vcf.gz CosmicNonCodingVariants.vcf.gz
```
Once downloaded, set CFILE and NCFILE variables to coding and non-coding VCF files.  We will be using these variables a few times
```
CFILE="CosmicCodingMuts.vcf.gz"
NCFILE="CosmicNonCodingVariants.vcf.gz"
```
Get headers of both files so we can include them in the database file
```
zcat $CFILE  | head -300  | grep "^#"  > _${CFILE}.header
zcat $NCFILE  | head -300  | grep "^#" > _${NCFILE}.header

# get the headers
echo "#################################" >_header ; echo "##header for $CFILE " >> _header; 
cat _${CFILE}.header | grep -v CHROM >>_header ; 
echo "#################################" >> _header ; 
echo "##header for $NCFILE " >> _header ; cat _${NCFILE}.header >> _header
```


Add 'chr' and 'CODING=1' to data in coding file.  This is necessary so we know where the entries came from later on
```
zcat $CFILE  | awk '{ if ($1 ~ /^#/) { print $0 } else { print "chr"$0";CODING=1" }}' | grep -v "^#" > _${CFILE}.modified
```

Similarly, add 'chr' and 'NONCODING=1' to data in non-coding file
```
zcat $NCFILE  | awk '{ if ($1 ~ /^#/) { print $0 } else { print "chr"$0";non-coding=1" }}' | grep -v "^#" > _${NCFILE}.modified
```
Combine data from coding and non-coding files
```
cat _${CFILE}.modified _${NCFILE}.modified | sort -k1,1 -k2,2n > _data
```
Set the COSMIC version and create the COSMIC database
```
VER="v83"; 
cat _header _data | sed 's/^chrMT/chrM/' > COSMIC_${VER}.vcf

# compress the newly created database
bgzip COSMIC_${VER}.vcf; 

# index the database using tabix 
tabix -p vcf COSMIC_${VER}.vcf.gz
```
Finally, delete intermediate files.
```
rm _*
```
Test to see if **_qpipeline_** works with the newly created database
```
# take the first 200 lines from newly created COSIMC database as a test file
zcat COSMIC_83.vcf.gz | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against COSMIC_v83 database.  
# all of the entries in test.vcf should be in COSMIC_v83 database
qpipeline tabix -m 2020 -i test.vcf  -d COSMIC_83.vcf.gz -q COSMIC_v83 | less 
```

