
The following sections describe how to use **_qpipeline_** with input VCF files. The examples below used data in **_${QPIPELINE_HOME}/test_data/vcf_** folder.

* [Annotate an input VCF file against a BED database file](ANNOTATE_VCF_WITH_BED.md)
* [Annotate an input VCF file against a VCF database file](ANNOTATE_VCF_WITH_VCF.md)

---
## Other miscellaneous tasks
* parse an attribute from the INFO column.  For example, VC (Variant Class)
```
 qpipeline vcf -m 1500 -i common_all_20161122.vcf -k VC= 
```

* parse an attribute from the FORMAT column: 
```
 qpipeline vcf -m 1600  
```
* print only passed filter entriies:
```
 qpipeline vcf -m 1000  
```
---
