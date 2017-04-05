qpipeline
=========

A collection of utils for Next Gen Sequencing (NGS) data.  qpipeline is designed and written for 'super users' to process the many file types generated from NGS data.  The primary goals of qpipeline are:  
1. flexible
2. reusable
3. reproducible
4. fast
5. and equally important, allows users to quickly extract and combine data from a large number of samples for further downstream analysis.

### INSTALLATION
qpipeline uses samtools ( https://sourceforge.net/projects/samtools/files/samtools ) and tabix ( https://sourceforge.net/projects/samtools/files/tabix ).  Both samtools and tabix are available in external_tools directory.

Go to a directory to install qpipeline
```s
# clone qpipeline 
git clone https://github.com/qtrinh/qpipeline.git

# change into qpipeline directory 
cd qpipeline

# set QPIPELINE_HOME environment variable
QPIPELINE_HOME=`pwd`
```
