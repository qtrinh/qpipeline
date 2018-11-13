

This page describes instructions on how to convert COSMIC databases to use with _**qpipeline**_.  
Follow instructions from the COSMIC website to download the databases needed ( http://cancer.sanger.ac.uk/cosmic/download ) then follow the instructions below on how to convert and use them with _**qpipeline**_.

[COSMIC Cancer Gene Census](#CGC)

## COSMIC Cancer Gene Census
```
# convert cancer_gene_census.csv to BED-like format 
perl ${QPIPELINE_HOME}/scripts/database.converter.pl 2010 cancer_gene_census.csv 1

```
