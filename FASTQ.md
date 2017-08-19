


The following sections describe how to use **_qpipeline_** with input FASTQ files.  The examples below used data in **_${QPIPELINE_HOME}/test_data/fastq_** folder.

---
[Count percent of Ns per cycle](#Count-percent-of-Ns-per-cycle)
```
 zcat AB1.gz  | qpipeline fastq  -m 1002 -i /dev/stdin -q "AB1" | less
```

[Count A,C,G,T, and N distributions](#Count-A-C-G-T-and-N-distributions)
```
 zcat AB1.gz  | qpipeline fastq  -m 1501 -i /dev/stdin -q "AB1" | less
```

[Calculate average quality value per cycle](#Calculate-average-quality-value-per-cycle)
```
 zcat AB1.gz  | qpipeline fastq  -m 2001 -i /dev/stdin -q "AB1" | less
```
---

Run **_qpipeline fastq_** by itself to see other available commands and their usage.
