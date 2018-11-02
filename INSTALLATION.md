### INSTALLATION
**_qpipeline_** uses **samtools** ( https://sourceforge.net/projects/samtools/files/samtools ) and **tabix** ( https://sourceforge.net/projects/samtools/files/tabix ) Application Programming Interface (API).  Both **samtools** and **tabix** are bundled in the **_external_tools_** directory.

Change to a directory where to install **_qpipeline_**
```
# clone qpipeline 
git clone https://github.com/qtrinh/qpipeline.git

# change into qpipeline directory 
cd qpipeline

# set QPIPELINE_HOME to current directory.  This is home directory of qpipeline
QPIPELINE_HOME=`pwd`
```

Recompile both **samtools** and **tabix** binaries to ensure they work on your system:
```
cd ${QPIPELINE_HOME}/external_tools/samtools/samtools-0.1.18
make

cd ${QPIPELINE_HOME}/external_tools/tabix/tabix-0.2.5
make
```
Compile **_qpipeline_**
```
cd ${QPIPELINE_HOME}
make
```
Run **_qpipeline_** by itself to see if it is compiled successfully
```
./qpipeline
```
If you see the following, then you have successuflly compiled **_qpipeline_**
```
...
qpipeline is an annotation and a collection of utils for Next Gen Sequencing (NGS) data.

Usage:
        qpipeline txt
        ...
```
Add **${QPIPELINE_HOME}**, **samtools** and **tabix** to your path by adding the output from running the bash command below to your ~/.bashrc file:
```
cd ${QPIPELINE_HOME}; scripts/env.sh ${QPIPELINE_HOME}

```

See [usage instructions](https://github.com/qtrinh/qpipeline/blob/master/USAGE.md) on how to use **_qpipeline_** 
