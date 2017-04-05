### INSTALLATION
**_qpipeline_** uses **samtools** ( https://sourceforge.net/projects/samtools/files/samtools ) and **tabix** ( https://sourceforge.net/projects/samtools/files/tabix ).  Both samtools and tabix are available in **_external_tools_** directory.

Go to a directory where to install **_qpipeline_**
```
# clone qpipeline 
git clone https://github.com/qtrinh/qpipeline.git

# change into qpipeline directory 
cd qpipeline

# set QPIPELINE_HOME environment variable
QPIPELINE_HOME=`pwd`
```
Recompile both **samtools** and **tabix** 
```
cd ${QPIPELINE_HOME}/external_tools/samtools/samtools-0.1.18
make

cd ${QPIPELINE_HOME}/external_tools/tabix/tabix-0.2.5
make
```
