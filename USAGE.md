

## General Usage
The primary function of **_qpipeline_** is to annotate NGS data with multiple databases while allowing users to quickly query the annotated data for new knowledge.  In addition, **_qpipeline_** also contained a set of utils that support the many different common tasks in different file formats.  

### Annotations
Typically, annotations are done in 2 steps: (1) create and index the database to be used; and (2) annotate the input file with the newly created database.  Below are instructions on how to create and index different commonly used databases:
* [ICGC](https://dcc.icgc.org/releases)
  * [simple_somatic_mutations](ICGC_simple_mutations.md) ; [copy number somatic mutations](ICGC_copy_number.md)
* [COSMIC](https://cancer.sanger.ac.uk/cosmic) 
  * [VCF Files (coding and non-coding mutations)](COSMIC_VCF.md) ;  [cancer_gene_census](COSMIC_cancer_gene_census.md)
* NCBI (ftp://ftp.ncbi.nlm.nih.gov/pub/clinvar/vcf_GRCh37)
  * [NCBI ClinVar](NCBI_ClinVar.md)
* [POLYPHEN2-WHESS](POLYPHEN2-WHESS.md)
* More to come soon
### Utils
**_qpipeline_** utils are designed to work with many common file formats associated with NGS. Each file format, for example, txt format) has multiple modes supporting different tasks.  Most of the commands are straight forward to run and their usage information can be used as a guide on how to run these commands.  

* [TXT](TXT.md)
* [FASTA](FASTA.md)
* [FASTQ](FASTQ.md)
* [VCF](VCF.md)
* More to come soon

