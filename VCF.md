
This page describes how to use **_qpipeline_** to:

* [Annotate an input VCF file against a BED database file](ANNOTATE_VCF_WITH_BED.md)
* [Annotate an input VCF file against a VCF database file](ANNOTATE_VCF_WITH_VCF.md) ( general ) or against
  * [NCBI ClinVar](NCBI_ClinVar.md)
  * [PolyPhen2-WHESS](POLYPHEN2-WHESS.md)
  * [COSMIC](COSMIC_VCF.md)
  

---
## Other miscellaneous commands 

The examples below used data in **_${QPIPELINE_HOME}/test_data/vcf_** folder.

* parse an attribute from the INFO column.  For example, VC (Variant Class)
```
 qpipeline vcf -m 1500 -i common_all_20161122.vcf -k VC= | less
```

* parse an attribute from the FORMAT column: 
```
 qpipeline vcf -m 1600 -i 
```

Run **_qpipeline vcf_** by itself to see other available commands and their usage.

---
