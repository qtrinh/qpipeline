### INSTALLATION
**_qpipeline_** uses **samtools** ( https://sourceforge.net/projects/samtools/files/samtools ) and **tabix** ( https://sourceforge.net/projects/samtools/files/tabix ) Application Programming Interface (API).  Both **samtools** and **tabix** are available in **_external_tools_** directory.

Change to directory of where to install **_qpipeline_**
```
# clone qpipeline 
git clone https://github.com/qtrinh/qpipeline.git

# change into qpipeline directory 
cd qpipeline
```
Set QPIPELINE_HOME by adding the following line to your .bashrc 
```
export QPIPELINE_HOME=/path/to/where/qpipeline/is/stored
```
Recompile both **samtools** and **tabix** binaries to ensure they work on your system:
```
cd ${QPIPELINE_HOME}/external_tools/samtools/samtools-0.1.18
make

cd ${QPIPELINE_HOME}/external_tools/tabix/tabix-0.2.5
make
```
Add both **samtools** and **tabix** to your path by adding the following line to your .bashrc
```
export PATH=$PATH:${QPIPELINE_HOME}/external_tools/samtools/samtools-0.1.18:${QPIPELINE_HOME}/external_tools/tabix/tabix-0.2.5
```
Compile **_qpipeline_**
```
cd ${QPIPELINE_HOME}
make
```
Check to see if **_qpipeline_** is compiled successfully
```
qpipeline
```
If you see the following, then you have successuflly compiled **_qpipeline_**
```

A collection of utils for Next Gen Sequencing (NGS) data.

Usage:
        qpipeline txt
        ...
```
Now you are ready to use **_qpipeline_**!  See [usage](https://github.com/qtrinh/qpipeline/blob/master/USAGE.md) on how to use **_qpipeline_** 
