

## USAGE
**_qpipeline_** is designed to work with different file formats.  Run **_qpipeline_** by itself to see what file formats currently supported
```
A collection of utils for Next Gen Sequencing (NGS) data.

Usage:
        qpipeline txt 
        qpipeline fasta
        qpipeline fastq
        qpipeline tabix
        qpipeline vcf 
```

Each file format has multiple modes supporting different tasks.  For example, **_qpipeline tabix_** currently supported two tasks: (1) annotate a VCF file against a BED database file; and (2) annotate a VCF file against a VCF database file.
```
Utils for annotating VCF files using tabix.

        qpipeline tabix [ -m mode ]  [ options ]

        -m 2000 annotate a VCF file against a BED database file.
        -m 2020 annotate a VCF file against a VCF database file.
```

Use -m argument to get usage for a particular task.  For example, **_qpipeline tabix -m 2000_** gives usage and example on how to annotate a VCF file against a BED database file:
```
Utils for annotating VCF files using tabix.

        qpipeline tabix [ -m mode ]  [ options ]

        -m 2000 annotate a VCF file against a BED database file.
                -d TABIX_DATABASE_FILE  database file indexed by tabix ( .gz ).
                -q STR  prefix string to annotate.  e.g., ON_TARGET.
                -i FILE input VCF file.

                Example:
                        qpipeline tabix -m 2000 -d test_data/vcf/target.bed.sorted.gz -i test_data/vcf/sample.vcf -q ON_TARGET

        Missing input file (-i).  Please see usage above!
```
Typically, databases are zipped and indexed by **tabix** - see [instructions on how to index databases for tabix](INDEX_DATABASES.md)
