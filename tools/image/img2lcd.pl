#!/usr/bin/perl

# img2lcd.pl - by <sec@42.org> 05/2011, BSD Licence
#
# This script converts an image to .lcd format for the r0ket

use strict;
use warnings;
use Getopt::Long;
use Module::Load;

$|=1;

###
### Runtime Options
###

my ($verbose);

GetOptions (
            "verbose"  => \$verbose, # flag
			"help"     => sub {
			print <<HELP;
Uasge: img2lcd.pl [-v]

Options:
--verbose         Be verbose.
HELP
			exit(-1);}
			);


###
### Code starts here.
###

my $in=shift || "i42.gif";

my $out=$in;
$out=~s/\..*/.lcd/;

load GD;
my $image = GD::Image->new($in);


my $w=$image->width;
my $h=$image->height;

if($verbose){
	print STDERR "$in: ${w}x$h\n\n";
};

my @img;
for my $y (0..$h-1){
	for my $x (0..$w-1){
		my $px= $image->getPixel($x,$y);
		$img[$x][($y+4)/8]|=$px<<(7-($y+4)%8);
		if($verbose){
			$px=~y/01/ */; print STDERR $px;
		};
	};
	if ($verbose){
		print STDERR "<\n";
	};
};

open(F,">",$out)||die "open: $!";

$|=1;
my $hb=int(($h-1)/8);
for my $y (0..$hb){
	for my $x (0..$w-1){
		printf F "%c",$img[$w-$x-1][$hb-$y];
	};
};

close(F);
