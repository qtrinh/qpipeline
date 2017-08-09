

## USAGE
**_qpipeline_** is designed to work with different file formats.  Run **_qpipeline_** by itself to see what file formats currently supported:
```
A collection of utils for Next Gen Sequencing (NGS) data.

Usage:
        qpipeline txt 
        qpipeline fasta
        qpipeline fastq
        qpipeline tabix
        qpipeline vcf 
```

Each file format (txt, fasta, fastq, _etc._ ) has multiple modes supporting different tasks.  For example, **_qpipeline tabix_** currently supported two tasks: (1) annotate a VCF file against a BED database file; and (2) annotate a VCF file against a VCF database file.
```
Utils for annotating VCF files using tabix.

        qpipeline tabix [ -m mode ]  [ options ]

        -m 2000 annotate a VCF file against a BED database file.
        -m 2020 annotate a VCF file against a VCF database file.
```

Use -m argument to get usage information for a particular task.  For example, **_qpipeline tabix -m 2000_** gives usage and example of how to annotate a VCF file against a BED database file:
```
Utils for annotating VCF files using tabix.

        qpipeline tabix [ -m mode ]  [ options ]

        -m 2000 annotate a VCF file against a BED database file.
                -d TABIX_DATABASE_FILE  database file indexed by tabix ( .gz ).
                -q STR  target name.  e.g., BREAST_CANCER_PANEL.
                -i FILE input VCF file.

                Example:
                        qpipeline tabix -m 2000 -d test_data/vcf/target.bed.sorted.gz -i test_data/vcf/sample.vcf -q BREAST_CANCER_PANEL

        Missing input file (-i).  Please see usage above!
```

Most tasks for each supported file format are straight forward and their usage information can be used as a guide on how to run them.  


