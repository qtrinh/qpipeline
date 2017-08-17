

The following sections describe how to use **_qpipeline_** with input text (txt) files.  The examples below used data in **_${QPIPELINE_HOME}/test_data/bam_** folder.

---
## Selecting One or More Columns From A Text File
Sometimes it is handy to be able to select one or more columns from a text file regardless of where the column(s) are in the file. Using the Unix command:
```
cut -f N FILE
```
will work, however, if any columns in the input file are changed, then _N_ needs to be updated accordingly.  Inspired by the SQL _select_ statement, **_qpipeline_** can be used to select one or more columns from the input file regardless of where the columns are in the input file.  For example, select _mean_coverage_ column from the file _alignment.stats.txt_:
```

qpipeline txt  -m 1010 -i alignment.stats.txt -s "mean_coverage" | less
```

---

Run **_qpipeline txt_** by itself to see other commands and their usage.
