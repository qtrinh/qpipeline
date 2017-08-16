
The following sections describe how to use **_qpipeline_** with input FASTA files.  The examples below used data in **_${QPIPELINE_HOME}/test_data/fasta_** folder.

---
[Extract FASTA sequences from a FASTA file](#extract-fasta-sequences-from-fasta-file)

There are two ways of extracting FASTA sequences from a FASTA file:
* use -r argument if there is only one region to extract.  For example,
```
qpipeline fasta -m 1000 -i chr1.fa  -r chr1:20000-20100
```
* use -T argument if there are many targets.  For example,
```
qpipeline fasta -m 1000 -i chr1.fa -T targets.txt
```
where _targets.txt_ is a 3-column (chromosome, start, and end ) text file.

---
Run **_qpipeline fasta_** by itself to see other commands and their usage.
