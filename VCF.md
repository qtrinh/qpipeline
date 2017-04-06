

## Using **_qpipeline_** with input VCF files:
* [Annotate input VCF files against a BED database file](#Annotate-input-VCF-files-against-a-BED-database-file)
* [Annotate input VCF files with a VCF database file such as COSMIC or dbSNP](#Annotate-input-VCF-file-with-VCF-database-file)  


### Annotate input VCF files against a BED database file
Use **tabix** to index the BED database file - see [Index a BED database file](INDEX_DATABASES.md#index-a-bed-database-file) 

```
qpipeline tabix -m 2000 qpipeline tabix -m 2000 -d test_data/vcf/target.txt.sorted.gz -i test_data/vcf/sample.vcf -q ON_TARGET  | less
```
### Annotate input VCF files against a VCF database file such as COSMIC or dbSNP
To be updated
