#!/usr/bin/perl 
use strict;
use warnings;
use File::Basename;

#
#
#
#


if ( @ARGV != 1 ) {
   print "\n";
   print "\nconvert the combined PolyPhen2 feature and tlod to VCF";
   print "\n\nusage: perl " . basename($0) . " [ FILE ] ";
   print "\n\n";
   exit (0);
}

my $INPUT_FILE =  $ARGV[0];
my @headers = ();
my @fields;
my $line;
open FHI, "<" , $INPUT_FILE || die ;

print "##fileformat=VCFv4.1";
print "\n#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO";
print "\n";
while (<FHI>) {
   chomp;
	$line = $_;
	
	if ($line =~ /chr_pos/) {
		@headers = split /\t/, $line;
		next;
	}

	@fields = split /\t/, $line;
	my @ff= split /:/, $fields[0];
	# chr and pos		
	print $ff[0] ;
	print "\t" . $ff[1] ;
	
	print "\t."; 
	
	# ref and alt 
	@ff= split /\//, $fields[1];
	print $ff[0] ;
	print "\t" . $ff[1] ;
	
	print "\t."; 
	print "\t.\t"; 

	foreach my $i (0 .. $#fields) {
		my $h = $headers[$i] ;
		my $d = $fields[$i] ;
		$h =~ s/^\s+|\s+$//g;
		$h =~ s/^#//;
		$d =~ s/^\s+|\s+$//g;
		print $h . "=" . $d . ";";
	}

	print "\n";
}
close FHI;

