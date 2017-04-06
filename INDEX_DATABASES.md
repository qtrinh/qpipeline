

## Index database files with **tabix**:
* [Index a BED database file](#index-a-bed-database-file)
* [Index a VCF database file](#index-a-vcf-database-file)


### Index a BED database file
As an example, we use the file _target.txt_ provided in ${QPIPELINE_HOME}/test_data/vcf 

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
**tabix** should generated the index file _target.txt.sorted.gz.tbi_.


### Index a VCF database file 
To be updated
