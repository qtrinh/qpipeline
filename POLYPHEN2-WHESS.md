

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

More to come shortly!


