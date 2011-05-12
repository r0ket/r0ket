#!/usr/bin/perl
use warnings;

$|=1;

my %chars;

while (<>){
chomp;
/^ENDCHAR/ && do {
	warn "Char $cchar has strange height?\n" if ($#char+1 != $height);
	for (1..$offy){
		push @char,("0"x$width);
	};
	for (@char){
		$_=("0"x$offx).$_;
	};
	$inchar=0;
#	print "Char: $cchar:\n",join("\n",@char),"\n";
	$chars{$cchar}=[@char];
	@char=();
};
if($inchar){
#	print "_: $_\n"; print "H: ",pack("H*",$_),"\n";
	$x=unpack("B*",pack("H*",$_));
	$x=substr($x,0,$width);
	push @char,$x;
#	print $x,"\n";
	next;
};
/^BITMAP/ && do {$inchar=1;};
	
/^PIXEL_SIZE (.*)/ && do { $size=$1;$size+=0;};
/^ENCODING (.*)/ && do {$cchar=$1};
/^BBX (\d+) (\d+) (\d+) ([-\d]+)/ && do {$width=$1;$height=$2;$offx=$3;$offy=$4+$fontd;};
/^FONT_ASCENT (.*)/ && do {$fonta=$1};
/^FONT_DESCENT (\d+)/ && do {$fontd=$1;$byte=int(($fonta+$fontd-1)/8)+1;print "This will be a $byte byte font\n";};
/^DWIDTH (\d+) (\d+)/ && do {$width=$1;die "H-offset?" if $2!=0};
/^FACE_NAME "(.*)"/ && do {$font=$1;};
}

# Beautify Font Name.
$font.=" ${size}pt";

$fonts=$font;
$fonts=~s/ //g;
$fonts=~s/Bitstream//;
$fonts=~s/Sans//;

$file=$fonts;
$file=~s/pt$//;
$file=~y/A-Z/a-z/;

# Fine. Now write it back out...

open (F,">",$file.".c")||die;
select(F);

print <<EOF
#include "$file.h"

/* Font data for $font */

/* Bitmaps */
const uint8_t ${fonts}Bitmaps[] = {
EOF
;

my $offset=0;
for (32..126){
	warn "Char $_ not available" if !defined $chars{$_};
	$char=$chars{$_};

	@tchar=();
	for $l (@$char){
#		print "tt: $l\n";
		for $y (1..length($l)){
#			$tchar[$y-1].=substr($l,$y-1,1);
			$tchar[$y-1]="" if !defined $tchar[$y-1];
			$tchar[$y-1]=substr($l,$y-1,1).$tchar[$y-1];
		};
	};
#	print "Turn: $_:\n",join("\n",@tchar),"\n";
	print "/* '",chr $_,"' at $offset */\n";
	for (@tchar){
		$h=unpack("H*",pack("B*",$_));
		$h= pack("B*",$_).(chr(0).chr(0));
		for $b (1..$byte){
			printf "0x%02x,",ord(substr($h,$b-1,1));
		};
#		print map {"0x".$_.", "} grep {$_ ne ""} split(/(..)/,$h);
		($ll=$_)=~y/01/ */;
		print " /* $ll */ \n";
	};
	push @offsets,sprintf " {%2d,%4d}, /* %c */\n",scalar(@tchar),$offset,$_;
	$offset+=$byte*scalar(@tchar);
};

print <<EOF;
};

/* Character descriptors */
const FONT_CHAR_INFO ${fonts}Descriptors[] = {
EOF

print @offsets;

print <<EOF;
};

/* Font info */
const struct FONT_DEF Font_$fonts = {0,$size,' ','~',
    ${fonts}Bitmaps,
    ${fonts}Descriptors,
};
EOF

open (F,">",$file.".h")||die;
select(F);
print <<EOF;
#include "fonts.h"

extern const struct FONT_DEF Font_$fonts;
EOF
close(F);

