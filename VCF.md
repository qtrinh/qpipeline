

## Using **_qpipeline_** with input VCF files:
* [Annotate input VCF files against a BED database file](#Annotate-input-VCF-files-against-a-BED-database-file)
* [Annotate input VCF files with a VCF database file such as COSMIC or dbSNP](#Annotate-input-VCF-file-with-VCF-database-file)  


### Annotate input VCF files against a BED database file
Use **tabix** to index the BED database file - see [Index a BED database file](INDEX_DATABASES.md#index-a-bed-database-file) 

Go to _${QPIPELINE_HOME}/test_data/vcf_ directory and annotate _sample.vcf_ with _target.txt.sorted.gz_ 
```
cd ${QPIPELINE_HOME}/test_data/vcf

qpipeline tabix -m 2000 -d target.txt.sorted.gz -i sample.vcf -q ON_TARGET > sample.vcf.out
```
### Annotate input VCF files against a VCF database file such as COSMIC or dbSNP
To be updated
