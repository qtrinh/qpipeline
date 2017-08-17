

The following sections describe how to use **_qpipeline_** with input text (txt) files.  The examples below used data in **_${QPIPELINE_HOME}/test_data/bam_** folder.

---
[Select a column](#select-a-column).  For example, select _mean_coverage_ column from the file _alignment.stats.txt_:
```

qpipeline txt  -m 1010 -i alignment.stats.txt -s "mean_coverage" | less
```

---

Run **_qpipeline txt_** by itself to see other commands and their usage.
