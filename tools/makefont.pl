#!/usr/bin/perl -w
use GD;
use strict;
use warnings;

###
### Configure me
### 

my $charlist;
for(32..126){
	$charlist.=chr $_;
};

my $size=shift||17;
my $font="/usr/local/lib/X11/fonts/bitstream-vera/Vera.ttf";
my $title="Bitstream Vera Sans ".$size."pt";

###
### Code starts here.
###

my $width=2000;
my $height=100;
my $xoff=30;

my $fonts=$title;
$fonts=~s/ //g;
$fonts=~s/Bitstream//;
$fonts=~s/Sans//;

my $file=$fonts;
$file=~s/pt$//;
$file=~y/A-Z/a-z/;

### Get & optimize bounding box

my $im = new GD::Image($width,$height);
my $white = $im->colorAllocate(255,255,255);
my $black = $im->colorAllocate(0,0,0);

my @bounds = $im->stringFT(-$black, $font, $size, 0, 0, $xoff,$charlist);

my ($mx,$my)=($bounds[2],$bounds[3]);
my ($top,$bottom)=($bounds[7],$my);

die "Increase width" if $mx>$width;
die "Increase height" if $my>$width;
die "Increase xoff" if $bounds[7]<0;

my $found;

# Cut whitespace at top
do {
	$found=0;
	for my $x (0..$mx){
		if( $im->getPixel($x,$top) == 1){
			$found=1;last;
		}
	};
	$top++;
}while($found==0);
$top--;

# Cut whitespace at bottom.
do {
	$found=0;
	for my $x (0..$mx){
		if( $im->getPixel($x,$bottom) == 1){
			$found=1;last;
		}
	};
	$bottom--;
}while($found==0);
$bottom++;

my $pxsize=$bottom-$top+1;
my $byte=int(($pxsize-1)/8)+1; # Round up

print "Removed ",$top-$bounds[7],"px at top\n";
print "Removed ",$my-$bottom,"px at bottom\n";
print "Chars are ",$bottom-$top+1,"px ($byte bytes) high\n";

#print "x: $bounds[6] - $bounds[2]\n";
#print "y: $bounds[7] - $bounds[3]\n";

open (C,">",$file.".c")||die;
open (H,">",$file.".h")||die;

print C <<EOF
#include "$file.h"

/* Font data for $title */

/* Bitmaps */
const uint8_t ${fonts}Bitmaps[] = {
EOF
;

my $offset=0;
my @offsets;
for (1..length$charlist){
	my $char=substr($charlist,$_-1,1);

	# create a new image
	$im = new GD::Image(2*$height,$height);
	$white = $im->colorAllocate(255,255,255);
	$black = $im->colorAllocate(0,0,0);

	@bounds = $im->stringFT(-$black, $font, $size, 0, 0, $xoff,$char.$charlist);

	my @char;
	for my $y ($top..$bottom){
		for my $x (0..($bounds[2]-$mx)){
			my $px= $im->getPixel($x,$y);
			$char[$x].=$px;
#			$px=~y/01/ */; print $px;
		};
#		print "<\n";
	};
	print C "/* '",$char,"' at $offset */\n";
	for (@char){
		$_=reverse $_;
		my $h= pack("B*",$_).(chr(0).chr(0));
		for my $b (1..$byte){
			printf C "0x%02x,",ord(substr($h,$b-1,1));
		};
		$_=~y/01/ */;
		print C " /* $_ */ \n";
	};
	push @offsets,sprintf " {%2d,%4d}, /* %s */\n",scalar(@char),$offset,$char;
	$offset+=scalar(@char)*$byte;
#	print "-"x80,"\n";
};


print C <<EOF;
};

/* Character descriptors */
const FONT_CHAR_INFO ${fonts}Descriptors[] = {
EOF

print C @offsets;

my($first)=ord substr($charlist,0,1);
my($last)=$first+length($charlist)-1;
print C <<EOF;
};

/* Font info */
const struct FONT_DEF Font_$fonts = {0,$pxsize,$first,$last,
    ${fonts}Bitmaps,
    ${fonts}Descriptors,
};
EOF

print H <<EOF;
#include "fonts.h"

extern const struct FONT_DEF Font_$fonts;
EOF

close(C);
close(H);

