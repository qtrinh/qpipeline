

# Index database files with **tabix**:
* [Index a BED database file](#index-a-bed-database-file)
* [Index a VCF database file](#index-a-vcf-database-file)


## Index a BED database file
As an example, we use the file _target.txt_ provided in _${QPIPELINE_HOME}/test_data/vcf_ 

Sort the BED file by chromosomes, start, and end
```
cat target.txt  | sort -k1,1 -k2,2n -k3,3n > target.txt.sorted
```
Zip the sorted file using **bgzip** 
```
bgzip target.txt.sorted
```
Index the zipped file using **tabix** 
```
tabix -p bed target.txt.sorted.gz
```
**tabix** should generated the index file _${QPIPELINE_HOME}/test_data/vcf/target.txt.sorted.gz.tbi_.


## Index a VCF database file 
As an example, we use the file _common_all_20161122.vcf_ in _${QPIPELINE_HOME}/test_data/vcf_.  This file is a small subset of chromsome 1 downloaded from ftp://ftp.ncbi.nlm.nih.gov/snp/organisms/human_9606/VCF/common_all_20161122.vcf.gz

NOTE: Both the input VCF file and the VCF database file must have the same chromosome format. 
That is, either '1',..., '22', 'X', 'Y', 'M' or 'chr1',..., 'chr22', 'chrX', 'chrY', 'chrM'.  

The sample input VCF file uses 'chr1',...,'chr22', 'chrX', 'chrY','chrM' so we will add the prefix 'chr' to _common_all_20161122.vcf_ before indexing it.
```
# get VCF header
cat common_all_20161122.vcf | grep ^# > common_all_20161122.vcf.modified.vcf

# add 'chr' to chromosomes
cat common_all_20161122.vcf | grep -v ^# | awk '{ print "chr"$0 }' >> common_all_20161122.vcf.modified.vcf
```
Zip and index using **tabix**
```
# zip 
bgzip common_all_20161122.vcf.modified.vcf 

# index using VCF format 
tabix -p vcf common_all_20161122.vcf.modified.vcf.gz 
```
**tabix** should generated the index file _${QPIPELINE_HOME}/test_data/vcf/common_all_20161122.vcf.modified.vcf.gz.tbi_.


