#!/usr/bin/perl
use strict;
use warnings;




# we need 2 arguments: alignment stats file and project name 
if ( @ARGV != 2 ) {
   print "\n";
   print "\nPlot alignment stats" ;
   print "\n\nusage: perl " . $0 . " [ ALIGNMENT_STATS_FILE ] [ PROJECT_NAME ]";
   print "\n\n";
   exit (0);
}

my ($ALIGNMENT_STATS_FILE, $PROJECT_NAME) = @ARGV;


# we need the following tools to run the this script 
my $tool_names = "Rscript,qpipeline";  # simple example
my @tools = split /,/, $tool_names;
my $tool_path = '';

# for each tools, check to make sure they are on the path, 
# if not, exit and inform user to install 
foreach my $t (@tools) {
	my $foundTools = 0;
	for my $path ( split /:/, $ENV{PATH} ) {
		 if ( -f "$path/$t" && -x _ ) {
				$foundTools = 1;
			  last;
		}
	}
	# if any one of the above tools is not found, exit and give error
	if ($foundTools == 0) {
		print "\n\n'$t' not found.  Please install and add '$t' your path and try again!\n\n";
		exit (1);
	}
}

my $title;
my $xlab;
my $ylab;
my $outputFile;
my $pdfFiles = "";
my $desc;
my $myRscript;
my $rFile = "_plot.R";
my $QPIPELINE_HOME = $ENV{'QPIPELINE_HOME'}; 
my $myColumn ;







# ===============================================

$myColumn = "total_number_of_reads";
print "\nplotting $myColumn";
$title = $myColumn ; $title =~ s/_/ /g;
$outputFile = "_bam_${myColumn}.data.txt";

# use qpipeline to extract these two columns 
$desc = "$myColumn,category";
system "qpipeline txt -m 1010 -i $ALIGNMENT_STATS_FILE -k $desc > $outputFile";


# plot all samples 
system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot.all"; 
system "cat $outputFile | awk '\$2 ~ /BART/'  | cut -f 1 | grep -v $myColumn | sort -k1,1n | awk '{ print NR\"\\t\"\$1\"\\tTEAM UK Tumors\" }' >>  _data.2.plot.all";  
system "cat $outputFile | awk '\$2 ~ /_P\$/' | cut -f 1 | grep -v $myColumn | sort -k1,1n | awk '{ print NR\"\\t\"\$1\"\\tGECCO Tumors\" }' >>  _data.2.plot.all";  
#system "cat $outputFile | awk '\$2 ~ /_R\$/' | cut -f 1 | grep -v $myColumn | sort -k1,1n | awk '{ print NR\"\\t\"\$1\"\\tGECCO Normal\" }' >>  _data.2.plot.all";  
#system "cat $outputFile | cut -f 1 | grep -v $myColumn | sort -k1,1n | awk '{ print NR\"\\t\"\$1\"\\tall-samples\" }' >>  _data.2.plot.all";  
$xlab = "Samples"; $ylab="" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#USE_MILLION_ON_Y_AXIS#//;
$myRscript =~ s/#SET_X_RANGE#//g; $myRscript =~ s/X_RANGE/breaks = seq(0,3000, 500), limits=c(0,3000)/g;
$myRscript =~ s/#SET_Y_RANGE#//g; $myRscript =~ s/Y_RANGE/breaks = seq(0,50, 10), limits=c(0,50)/g;
open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot.all \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.all.pdf";
$pdfFiles = $pdfFiles . " $outputFile.all.pdf";


# get unique types ; and use it to group data together 
system "cat $outputFile | cut -f2  | grep -v category | sort | uniq > _tmp " ;

system "rm _data ; touch _data";
system "for i in `cat _tmp`; do cat $outputFile | grep -w \"\$i\" | sort -k1,1n >> _data; done";
system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot"; 
system "cat _data | awk '{ print NR\"\\t\"\$0 }' >> _data.2.plot";

# =====
$xlab = "Samples"; $ylab="" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#USE_MILLION_ON_Y_AXIS#//;
open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.pdf";
$pdfFiles = $pdfFiles . " $outputFile.pdf";

# =====
# number of reads in BAM file using LOG 10 
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#USE_LOG_10_ON_Y_AXIS#//g;
open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.log10.pdf";
$pdfFiles = $pdfFiles . " $outputFile.log10.pdf";

# ===============================================


plot_percent_on_Y ("percent_of_reads_aligned_to_hg19");
plot_percent_on_Y ("percent_of_reads_aligned_to_target");
plot_percent_on_Y ("1x_or_higher");
plot_percent_on_Y ("25x_or_higher");
plot_log10_on_Y ("mean_coverage");

# ===============================================

$myColumn = "total_number_of_reads,percent_of_reads_aligned_to_target";
$title = "number of reads vs % aligned to target";
print "\n\nplotting $myColumn";
$outputFile = "_bam_${myColumn}.data.txt";

# use qpipeline to extract these two columns 
$desc = "$myColumn,category";
system "qpipeline txt -m 1010 -i $ALIGNMENT_STATS_FILE -k $desc > $outputFile";

system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot"; 
system "cat $outputFile | grep -v category >> _data.2.plot";

# =====
$xlab = "total number of reads"; $ylab="percent of reads aligned to target" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
#$myRscript =~ s/#USE_MILLION_ON_X_AXIS#//;
$myRscript =~ s/#USE_LOG_10_ON_X_AXIS#//g;
$myRscript =~ s/#SET_Y_RANGE#//g; $myRscript =~ s/Y_RANGE/breaks = seq(0,100, 10), limits=c(0,100)/g;

open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.pdf";
$pdfFiles = $pdfFiles . " $outputFile.pdf";

# =====


my $W;# width
my $H;# height


$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_2d.scatter_ggplot.R`;
$myRscript =~ s/#USE_LOG_10_ON_X_AXIS#//g;
$myRscript =~ s/#USE_BIN_WIDTH#//g;
$W = 0.5;
$H = 5;
$myRscript =~ s/WIDTH/$W/g;
$myRscript =~ s/HEIGHT/$H/g;
$myRscript =~ s/TITLE_INFO/$W x $H/g;

$myRscript =~ s/#SET_Y_RANGE#//g; $myRscript =~ s/Y_RANGE/breaks = seq(-10,100, 10), limits=c(-10,100)/g;

open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.2d.log10.on.y.pdf";
$pdfFiles = $pdfFiles . " $outputFile.2d.log10.on.y.pdf";


# ===============================================

$myColumn = "percent_of_reads_aligned_to_hg19,percent_of_reads_aligned_to_target";
$title = "percent of reads aligned to hg19 vs target";
print "\n\nplotting $myColumn";
$outputFile = "_bam_${myColumn}.data.txt";

# use qpipeline to extract these two columns 
$desc = "$myColumn,category";
system "qpipeline txt -m 1010 -i $ALIGNMENT_STATS_FILE -k $desc > $outputFile";

system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot"; 
system "cat $outputFile | grep -v category >> _data.2.plot";

# =====
$xlab = "percent of reads aligned to hg19"; $ylab="percent of reads aligned to target" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#SET_X_RANGE#//g; $myRscript =~ s/X_RANGE/breaks = seq(0,100, 20), limits=c(0,100)/g;
$myRscript =~ s/#SET_Y_RANGE#//g; $myRscript =~ s/Y_RANGE/breaks = seq(0,100, 20), limits=c(0,100)/g;

open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.pdf";
$pdfFiles = $pdfFiles . " $outputFile.pdf";

# =====
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_2d.scatter_ggplot.R`;
$myRscript =~ s/#USE_BIN_WIDTH#//g;
$W = 5;
$H = 5;
$myRscript =~ s/WIDTH/$W/g;
$myRscript =~ s/HEIGHT/$H/g;
$myRscript =~ s/TITLE_INFO/$W x $H/g;

$myRscript =~ s/#SET_X_RANGE#//g; $myRscript =~ s/X_RANGE/breaks = seq(0,100, 20), limits=c(0,101)/g;
$myRscript =~ s/#SET_Y_RANGE#//g; $myRscript =~ s/Y_RANGE/breaks = seq(0,100, 20), limits=c(0,101)/g;

open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.2d.pdf";
$pdfFiles = $pdfFiles . " $outputFile.2d.pdf";





system "gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=Rplots.pdf  $pdfFiles ; cp Rplots.pdf ~/JUNK";

print "\n\nRplots.pdf copied to ~/JUNK/\n\n";

exit (0);



sub plot_log10_on_Y { 

	 ($myColumn) = @_;

$title = $myColumn ; $title =~ s/_/ /g;
print "\n\nplotting $title";
$outputFile = "_bam_${myColumn}.data.txt";

# use qpipeline to extract these two columns 
$desc = "$myColumn,category";
system "qpipeline txt -m 1010 -i $ALIGNMENT_STATS_FILE -k $desc > $outputFile";

# get unique types ; and use it to group data together 
system "cat $outputFile | cut -f2  | grep -v category | sort | uniq > _tmp " ;

system "rm _data ; touch _data";
system "for i in `cat _tmp`; do cat $outputFile | grep -w \"\$i\" | sort -k1,1n >> _data; done";
system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot"; 
system "cat _data | awk '{ print NR\"\\t\"\$0 }' >> _data.2.plot";

# =====
$xlab = "Samples"; $ylab="" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#USE_LOG_10_ON_Y_AXIS#//g;
open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.pdf";
$pdfFiles = $pdfFiles . " $outputFile.pdf";
} 


# ===============================================


sub plot_percent_on_Y { 

	 ($myColumn) = @_;


#$myColumn = "percent_of_reads_aligned_to_hg19";
$title = $myColumn ; $title =~ s/_/ /g;
print "\n\nplotting $title";
$outputFile = "_bam_${myColumn}.data.txt";

# use qpipeline to extract these two columns 
$desc = "$myColumn,category";
system "qpipeline txt -m 1010 -i $ALIGNMENT_STATS_FILE -k $desc > $outputFile";

# get unique types ; and use it to group data together 
system "cat $outputFile | cut -f2  | grep -v category | sort | uniq > _tmp " ;

system "rm _data ; touch _data";
system "for i in `cat _tmp`; do cat $outputFile | grep -w \"\$i\" | sort -k1,1n >> _data; done";
system "echo \"x|y|category\" | tr '|' '\t' > _data.2.plot"; 
system "cat _data | awk '{ print NR\"\\t\"\$0 }' >> _data.2.plot";

# =====
$xlab = "Samples"; $ylab="" ;
$myRscript = `cat ${QPIPELINE_HOME}/scripts/plot_xy_category_dot.ggplot.R`;
$myRscript =~ s/#SET_Y_RANGE#//g;
$myRscript =~ s/Y_RANGE/breaks = seq(0,100, 10), limits=c(0,100)/g;
open FILE, ">$rFile";
print FILE $myRscript;
close FILE;

system "Rscript _plot.R _data.2.plot \"$title\" \"$xlab\" \"$ylab\"; cp Rplots.pdf $outputFile.pdf";
$pdfFiles = $pdfFiles . " $outputFile.pdf";
} 

# ===============================================




