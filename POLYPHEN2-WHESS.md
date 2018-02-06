

This page describes how to use qpipeline with PolyPhen-2 annotations for whole human exome sequence space ( WHESS ) ( ftp://genetics.bwh.harvard.edu/pph2/whess/ )


If it is not already there, create a directory in *${QPIPELINE_HOME}/external_databases* to store PolyPhen2-WHESS database
```
mkdir cd ${QPIPELINE_HOME}/external_databases/PolyPhen2-WHESS

cd ${QPIPELINE_HOME}/external_databases/PolyPhen2-WHESS
```

Download PolyPhen2-WHESS ( note: this will take a while )
```
wget --no-passive  ftp://genetics.bwh.harvard.edu/pph2/whess/polyphen-2.2.2-whess-2011_12.tab.tar.bz2
```

Uncompress the downloaded file ( note: this will take a while )
```
tar xvfj polyphen-2.2.2-whess-2011_12.tab.tar.bz2
```

Go into the extracted directory ( polyphen-2.2.2-whess-2011_12 ).  There are two files associated with each transcript ( *features.tab and *scores.tab ) so they need to be combined to create the WHESS database.

In the extracted directory, create a directory called 'database' and go into it.
```
mkdir database; cd database
```
Combine all the features.tab and scores.tab files
```
DB="WHESS"

for i in `ls ../*features.tab`; do j=`echo $i | sed 's/features/scores/'`; paste  $i $j ; done  | cut -d$'\t' -f1-10,16-20,56- > $DB


head -1 $DB | cut -f 2- | awk '{ print "#chr\tstart\tend\t"$0 }' > ${DB}.tab

cat $DB | grep -v transcript | sed 's/:/\t/' | awk '{ print $1"\t"$2"\t"$2"\t"$0 }' | cut -f 1-3,6- | sort -k1,1 -k2,2n -k3,3n >> ${DB}.tab

bgzip ${DB}.tab ; tabix -s 1 -b 2 -e 3 ${DB}.tab.gz


```

More to come shortly!


