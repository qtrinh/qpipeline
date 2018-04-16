

## General Usage
The primary function of **_qpipeline_** is to annotate NGS data with multiple databases while allowing users to quickly query the annotated data for new knowledge.  In addition, **_qpipeline_** also contained a set of utils that support the many different common tasks on different file formats.  

### Annotations
Typically, annotations are done in 2 steps: (1) create and index the database to be used; and (2) annotate the input file with the newly created database.  Below are instructions on how to create and index different commonly used databases:
* ICGC
  * [simple_somatic_mutations](ICGC_simple_mutations.md)
* COSMIC 
  * [VCF Files (coding and non-coding mutations)](COSMIC_VCF.md) ;  [cancer_gene_census](COSMIC_cancer_gene_census.md)
  
* NCBI
  * [NCBI ClinVar](NCBI_ClinVar.md)
* [POLYPHEN2-WHESS](POLYPHEN2-WHESS.md)
* More to come shortly
