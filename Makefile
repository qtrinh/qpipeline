
################################
#
# other external libraries/tools 
#
# 
EXTERNAL_TOOLS=external_tools

# where samtools is installed 
SAMTOOLS=$(EXTERNAL_TOOLS)/samtools/samtools-0.1.18

# where tabix is installed 
TABIX=$(EXTERNAL_TOOLS)/tabix/tabix-0.2.5

# where Gnu Scientific Library is installed 
GSL=$(EXTERNAL_TOOLS)/gsl/gsl-1.9


################################

CC=gcc
CFLAGS=-c -Wall -O2

LDFLAGS=-D_IOLIB=2 -D_FILE_OFFSET_BITS=64 #-D_NO_RAZF #-D_NO_CURSES


INCLUDES=-I. -I $(SAMTOOLS) -I $(TABIX) -I $(GSL)
LIBDIRS=-L. -L$(SAMTOOLS) -L$(TABIX) -L$(GSL)
LIBS= -lm -lbam -lz -ltabix 

SOURCES=utils.c log.c acgtn.c pileup.c input_data.c output_data.c my_string.c txt.c txt_main.c bedtools.c bedtools_main.c snpeff.c fasta.c fasta_main.c vcf.c vcf_main.c my_tabix.c tabix_main.c fastq.c fastq_main.c main.c 

OBJECTS=$(SOURCES:.c=.o)
HEADERS=$(SOURCES:.c=.h)
EXECUTABLE=qpipeline
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):$(OBJECTS)
	$(CC)  $(LDFLAGS) $(INCLUDES) $(LIBDIRS) $(OBJECTS) -o $@ $(LIBS)

.c.o:$(SAMTOOLS_DIR)/libbam.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBDIRS) $(LIBS)  $< -o $@

clean: 
	rm *.o $(EXECUTABLE)

