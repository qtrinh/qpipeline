

The following sections describe how to use **_qpipeline_** with input text (txt) files.  The examples below used data in **_${QPIPELINE_HOME}/test_data/bam_** folder.

---
## Selecting One or More Columns From A Text File
Sometimes it is handy to be able to select one or more columns from a text file regardless of where the column(s) are in the file. Using the Unix command:
```
cut -f N FILE | less
```
will work, however, if any columns in the input file are changed, then _N_ needs to be updated accordingly.  Inspired by the SQL _select_ statement, **_qpipeline_** can be used to select one or more columns from the input file regardless of where the columns are in the file.  For example, select the two columns _input_file_ and _mean_coverage_ from the file _alignment.stats.txt_:
```
qpipeline txt  -m 1010 -i alignment.stats.txt -s "input_file,mean_coverage" | less
```

A more complicated example would be select entries in the file _alignment.stats.txt_ where _mean_coveage_ is greater than 200.    This can be done easily as follows:
```
qpipeline txt  -m 1010 -i alignment.stats.txt -s "mean_coverage" -A | awk '$1>200' | cut -f 2- | less
```
where
```
-A indicates adding all columns in the input file to the right of the selected column "mean_coverage"
awk '$1>200' prints only rows where values in column 1 ( i.e., values of the mean_coverage column ) is greater than 200
cut -f 2- prints all columns starting from column 2
```
---

Run **_qpipeline txt_** by itself to see other commands and their usage.
