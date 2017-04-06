

## Using **_qpipeline_** with input VCF files to:
* [Annotate input VCF files against a BED database file](#Annotate-input-VCF-files-against-a-BED-database-file)
* [Annotate input VCF files with a VCF database file such as COSMIC or dbSNP](#Annotate-input-VCF-file-with-VCF-database-file)  


### Annotate input VCF files against a BED database file
Use **tabix** to index the BED database file - see [Index a BED database file](INDEX_DATABASES.md#index-a-bed-database-file) 

Go to _${QPIPELINE_HOME}/test_data/vcf_ directory and annotate _sample.vcf_ with _target.txt.sorted.gz_ 
```
cd ${QPIPELINE_HOME}/test_data/vcf

qpipeline tabix -m 2000 -d target.txt.sorted.gz -i sample.vcf -q ON_TARGET > sample.vcf.out
```
Count number of entries in _sample.vcf.out_
```
cat sample.vcf.out | grep -v ^# | wc -l
10
```
Count number of entries in _sample.vcf.out_ that are on target 
```
cat sample.vcf.out | grep ON_TARGET=1 | wc -l
2
```

### Annotate input VCF files against a VCF database file such as COSMIC or dbSNP
To be updated
