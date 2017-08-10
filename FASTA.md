
The following sections describe how to use **_qpipeline_** to FASTA input file.  Data for the below examples are in ${QPIPELINE_HOME/test_data/fasta folder.


[Extract FASTA sequences from FASTA file](#extract-fasta-sequences-from-fasta-file)

There are two ways of extracting FASTA sequences from a FASTA file:
* use -r argument if there is only one region to extract.  For example,
```
qpipeline fasta -m 1000 -i chr1.fa  -r chr1:20000-20100
```
* use -T argument if there are many targets.  For example,
```
qpipeline fasta -m 1000 -i chr1.fa -T targets.txt
```
More to come!
