

## Index database files with **tabix**:
* [Index a BED database file](#Index-a-BED-database-file)
* [Index a VCF database file](#Index-a-VCF-database-file)


### Index a BED database file
As an example, we use the file _target.txt_ provided in ${QPIPELINE_HOME}/test_data/vcf 

Sort the BED file by chromosomes, start, and end
```
cat target.txt  | sort -k1,1 -k2,2n -k3,3n > target.txt.sorted
```



### Index a VCF database file 
To be updated
