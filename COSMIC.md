


[COSMIC Cancer Gene Census](#CGC)

# COSMIC Cancer Gene Census
Create a directory to store COSMIC Cancer Gene Census database
```
mkdir -p ${QPIPELINE_HOME}/external_databases/cosmic/cancer_gene_census
```
Follow instructions from the COSMIC website to download the COSMIC Cancer Gene Census - http://cancer.sanger.ac.uk/cosmic/download. User registration is required. 
Once downloaded, the following instructions described how to create the cancer census database and test it with qpipeline

```
perl ${QPIPELINE_HOME}/scripts/database.converter.pl 2010 cancer_gene_census.csv 1


```
