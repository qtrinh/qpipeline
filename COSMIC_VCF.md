
## COSMIC Coding and Non-Coding VCF Files 

This page describes how to create the COSMIC VCF database and how to use **_qpipeline_** to annotate VCF files with COSMIC VCF database.


Create a directory in *${QPIPELINE_HOME}/external_databases/cosmic* to store COSMIC VCF database
```
cd ${QPIPELINE_HOME}/external_databases/cosmic

# create a directory to store version 83 of COSMIC VCF database 
mkdir -p v83/VCF ; cd v83/VCF
```

Follow instructions from the COSMIC website to download both coding and non-coding VCF files - http://cancer.sanger.ac.uk/cosmic/download.  User registration is required. The following are instructions on how to download the coding and non-coding VCF files using sftp:
```
sftp "email@somewhere.com"@sftp-cancer.sanger.ac.uk

mget /cosmic/grch37/cosmic/v83/VCF/Cosmic*gz
 
# Once downloaded, set the CFILE and NCFILE variables to the coding and non-coding VCF files ( we will be using these variables a few times )
CFILE="CosmicCodingMuts.vcf.gz"
NCFILE="CosmicNonCodingVariants.vcf.gz"

# Get headers of both files so we can include them in the new database file
zcat $CFILE  | head -300  | grep "^#"  > _${CFILE}.tmp.header
zcat $NCFILE | head -300  | grep "^#" > _${NCFILE}.tmp.header

# get the headers
echo "#################################" > _.tmp.header ; echo "##header for $CFILE " >> _.tmp.header; 
cat _${CFILE}.tmp.header | grep -v CHROM >> _.tmp.header ; 
echo "#################################" >> _.tmp.header ; 
echo "##header for $NCFILE " >> _.tmp.header ; cat _${NCFILE}.tmp.header >> _.tmp.header


# Add 'chr' and 'CODING=1' to data in coding file.  This is necessary so we know where entries came from later on
zcat $CFILE  | awk '{ if ($1 ~ /^#/) { print $0 } else { print "chr"$0";CODING=1" }}' | grep -v "^#" > _${CFILE}.tmp.modified

# Similarly, add 'chr' and 'NONCODING=1' to data in non-coding file
zcat $NCFILE  | awk '{ if ($1 ~ /^#/) { print $0 } else { print "chr"$0";non-coding=1" }}' | grep -v "^#" > _${NCFILE}.tmp.modified

# Combine coding and non-coding files, sort by chromosomes and positions 
cat _${CFILE}.tmp.modified _${NCFILE}.tmp.modified | sort -k1,1 -k2,2n > _.tmp.data

# Set the COSMIC version number and create the COSMIC database
VER="v83"; 
cat _.tmp.header _.tmp.data | sed 's/^chrMT/chrM/' > COSMIC_${VER}.vcf

# compress the newly created database
bgzip COSMIC_${VER}.vcf; 

# index the database using tabix 
tabix -p vcf COSMIC_${VER}.vcf.gz


# Test to see if qpipeline works with the newly created database
# take the first 200 lines from newly created database as a test file
zcat COSMIC_${VER}.vcf.gz | head -200 > test.vcf 

# use qpipeline to annotate the test.vcf file against COSMIC_v83 database.  
# All of the entries in test.vcf should be annotated as in COSMIC_${VER}.vcf.gz database.

qpipeline tabix -m 2020 -i test.vcf -d COSMIC_${VER}.vcf.gz -q COSMIC_${VER} | less 
```

Finally, delete all intermediate files.
```
rm _*.tmp.*
```
