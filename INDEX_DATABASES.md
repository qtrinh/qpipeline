

# Index database files with **tabix**:
* [Index a BED database file](#index-a-bed-database-file)
* [Index a VCF database file](#index-a-vcf-database-file)


## Index a BED database file
For simplicity, an example BED file _target.bed_ is provided in _${QPIPELINE_HOME}/test_data/vcf_ 

Do the following to create a BED file database
```
# set FILE so we can cut and paste and reuse all the commands below 
# for different BED database file when needed 
FILE="target.bed"

#sort the file by chromosomes, start, and end
cat $FILE  | sort -k1,1 -k2,2n -k3,3n > ${FILE}.sorted

# Zip the sorted file using **bgzip** 
bgzip ${FILE}.sorted


# Index the zipped file using **tabix** 
tabix -p bed ${FILE}.sorted.gz 
```
**tabix** should generated the index file _${QPIPELINE_HOME}/test_data/vcf/target.bed.sorted.gz.tbi_.


## Index a VCF database file 
Also, for simplicity, a parttial VCF file from NCBI called _common_all_20161122.vcf_ is provided in _${QPIPELINE_HOME}/test_data/vcf_ directory.  This file is a small subset of chromsome 1 downloaded from ftp://ftp.ncbi.nlm.nih.gov/snp/organisms/human_9606/VCF/common_all_20161122.vcf.gz

NOTE: Both the input VCF file and the VCF database file must have the same chromosome format. 
That is, either '1',..., '22', 'X', 'Y', 'M' or 'chr1',..., 'chr22', 'chrX', 'chrY', 'chrM'.  Check to make sure your input VCF file and your VCF database file have the same format before indexing your VCF database file.

The sample input VCF file uses 'chr1',...,'chr22', 'chrX', 'chrY','chrM' so we will add the prefix 'chr' to _common_all_20161122.vcf_ before indexing it.
```
# Again, set FILE so we can cut and paste and reuse all the commands below 
# for different VCF database file when needed 
FILE="common_all_20161122.vcf"

# get just the VCF header
cat $FILE | grep ^# >  ${FILE}.modified.vcf

# add 'chr' to chromosomes
cat $FILE | grep -v ^# | awk '{ print "chr"$0 }' >> ${FILE}.modified.vcf

# Zip and index using **tabix**

bgzip  ${FILE}.modified.vcf

# index using VCF format 
tabix -p vcf  ${FILE}.modified.vcf.gz 
```
**tabix** should generated the index file _${QPIPELINE_HOME}/test_data/vcf/common_all_20161122.vcf.modified.vcf.gz.tbi_.


