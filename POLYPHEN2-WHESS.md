

This page describes how to use **_qpipeline_** with **_PolyPhen2_** annotations for **_whole human exome sequence space ( WHESS )_** ( ftp://genetics.bwh.harvard.edu/pph2/whess/ )


Create a directory in *${QPIPELINE_HOME}/external_databases* to store PolyPhen2-WHESS database
```
mkdir ${QPIPELINE_HOME}/external_databases/PolyPhen2-WHESS

cd ${QPIPELINE_HOME}/external_databases/PolyPhen2-WHESS
```

Download PolyPhen2-WHESS ( note: this will take a while as PolyPhen2-WHESS is fairly large )
```
wget --no-passive  ftp://genetics.bwh.harvard.edu/pph2/whess/polyphen-2.2.2-whess-2011_12.tab.tar.bz2
```

Uncompress the downloaded file ( note: this will take a while )
```
tar xvfj polyphen-2.2.2-whess-2011_12.tab.tar.bz2
```

There are two files associated with each transcript ( *features.tab and *scores.tab ) and they need to be combined to create the WHESS database.  

```
# go into the extracted directory
cd polyphen-2.2.2-whess-2011_12

#  create a directory called 'database' to store PolyPhen2-WHESS database
mkdir database; cd database
```
Combine all the features.tab and scores.tab files in the extracted directory.  There is a total of ~45,000 transcripts so the instructions below need to be paralellized.  Running sequentially will take a while!
```
# for each feature.tab file, combine with its score.tab file and generate its equivalent VCF file
# $i is the feature file and $j is the score file
$ ${N}.vcf is the output VCF file
for i in `ls ../*features.tab`; do echo $i; N=`basename $i`; j=`echo $i | sed 's/features/scores/'`;  perl ${QPIPELINE_HOME}/scripts/polyphen2-whess_2_vcf.pl $i $j ${N}.vcf ; done
```
Combine all the VCF files to create the PolyPhen2-WHESS database.  The instructions below do not need to be paralellized.
```
# DB is the name of the PolyPhen2-WHESS database to be created
DB="polyphen-2.2.2-whess-2011_11.vcf";

# create VCF header 
echo "##fileformat=VCFv4.1" > $DB
echo "#CHROM|POS|ID|REF|ALT|QUAL|FILTER|INFO" | tr '|' '\t' >> $DB


# combine all VCF files and sort base on chr and pos
cat *.vcf | grep -v ^# | sort -k1,1 -k2,2n >> $DB

# compress the newly created database using bgzip
bgzip ${DB}

# index the newly created database using tabix
tabix -p vcf ${DB}.gz
```
More to come shortly!


