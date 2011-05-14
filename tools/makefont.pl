#!/usr/bin/perl -w
use GD;
use strict;
use warnings;

$|=1;

#for(1..500){
#	printf "%3d | %-10s | %-10s\n",
#		   $_,
#		   join(" ",map {sprintf "%x",$_} (0,pk_encode_long($_))),
#		   join(" ",map {sprintf "%x",$_} pk_encode_long($_+16));
#}
#exit(1);

use constant VERBOSE=>0;

###
### Configure me
### 

my $charlist;
for(32..126){
	$charlist.=chr $_;
};

my $size=shift||17;
my ($font,$title);

$font="/usr/local/lib/X11/fonts/bitstream-vera/Vera.ttf";
$title="Bitstream Vera Sans ".$size."pt";

#$font="./Comic_Sans_MS_KOI8.ttf";
#$title="Comic Sans ".$size."pt";

###
### Code starts here.
###

my $width=2000;
my $height=100;
my $xoff=30;

my $origsize;
my $c1size;
my $c2size;

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
my $maxsz=0;
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
	print "### Start $char\n" if(VERBOSE);

	print C " /* Char ",ord $char," is ",scalar@char,"px wide \@ $offset */\n";

	$maxsz=scalar@char if scalar@char > $maxsz;

	$origsize+=$byte * scalar @char;

	# Whoops. Characters are upside down.
	for (@char){
		$_=reverse $_;
	};

	### PK compression
	my @enc=do_pk(\@char);
	$c2size+=scalar(@enc);

	### Lame compression
	# "compress" blank spaces
	my $preblank=0;
	while(defined $char[1] && $char[0]=~/^0+$/){
		shift@char;
		$preblank++;
	};
	my $postblank=0;
	while($#char>0 && $char[$#char]=~/^0+$/){
		$#char--;
		$postblank++;
	};
	$c1size+=$byte*scalar@char;

	my @raw;
	### Raw character data
	for (@char){
		my $h= pack("B*",$_).(chr(0).chr(0));
		for my $b (1..$byte){
			push @raw,ord(substr($h,$b-1,1));
		};
	};

	# Maintenance overhead
	$origsize+=1;
	$c1size+=3;
	$c2size+=1;

	# If encoding is bigger, fall back to original char
	my $rawpretty=0;
	if($#enc>$#raw+3){
		warn "Compression failure: Encoding char $char raw.\n";
		@enc=(255,$preblank,$postblank,@raw);
		$rawpretty=1;
	};

	# Generate C source
	if($rawpretty){
		my @out=@enc;
		printf C "  0x%02x, %2d, %2d, /* rawmode, preblank, postblank */\n",
			   (shift@out), (shift@out), (shift@out);
		for (@char){
			print C "  ";
			printf C "0x%02x, ",shift@out for(1..$byte);
			$_=~y/01/ */;
			print C " /* $_ */ \n";
		};
	}else{
		for (@char){
			$_=~y/01/ */;
			print C " /* $_ */ \n";
		};
		my $pretty=0;
		for(@enc){
			print C "  " if($pretty==0);
			printf C "0x%02x, ",$_;
			if(++$pretty==8){
				print C "\n" ;
				$pretty=0;
			};
		};
	};

	print C "\n";

	push @offsets,sprintf " {%2d}, /* %s */\n",scalar(@enc),$char;
	print C "\n";

	$offset+=scalar(@enc);

	if(VERBOSE){
		print "-"x80,"\n";
	};
};


print C <<EOF;
};

/* Character descriptors */
const FONT_CHAR_INFO ${fonts}Lengths[] = {
EOF

print C @offsets;

my($first)=ord substr($charlist,0,1);
my($last)=$first+length($charlist)-1;
printf C "};

/* Font info */
const struct FONT_DEF Font_$fonts = {
	%3d,   /* width (1 == comressed) */
	%3d,   /* character height */
	%3d,   /* first char */
	%3d,   /* last char */
    %s, %s
};
",1,$pxsize,$first,$last,"${fonts}Bitmaps","${fonts}Lengths";

print H <<EOF;
#include "fonts.h"

extern const struct FONT_DEF Font_$fonts;
EOF

close(C);
close(H);

print "\ndone.\n\n";
print "Original size: $origsize\n";
print "Simple compression: $c1size\n";
print "PK compression: $c2size\n";
print "Maximum character size: $byte*$maxsz bytes\n";

exit(0);

# subs

sub pk_dedup {
	my $char=shift;
	my @echar=@{$char};

#   for (@echar){ print "dedup_in: $_\n"; };

	my $idx=0;
	while(++$idx<$#echar){
		# dupline code can't deal with all-0 or all-1 dupe lines
		next if ($echar[$idx]=~/^(.)(\1)+$/);

		if($echar[$idx-1] eq $echar[$idx]){
			my $dl=1;
			$dl++ while ($dl<$#echar && $echar[$idx] eq $echar[$idx+$dl]);

#			print "dupline found\n";
			if( $echar[$idx-1]=~ s/01/0[$dl]1/ ){
				$echar[$idx]="";
				@echar[$idx..($idx+$dl-1)]=();
			}elsif ($echar[$idx-1]=~ s/10/1[$dl]0/){
				$echar[$idx]="";
				@echar[$idx..($idx+$dl-1)]=();
			}else{
				die "Shouldn't happen: Can't encode dupline?";
			};
			$idx+=$dl; # Skip deduped lines.
		}
	}
	@echar=grep {defined $_} @echar;
	return \@echar;
};

sub pk_rle {
	my $char=shift;

	my $line=join("",@$char);;

	my $fpx=substr($line,0,1);
	warn "first pixel ==1 - Encoder will do stupid things..." if $fpx ==1 ;

	my @out;
	while($line=~/./){
		$line=~s/^(0*)(\[\d+\])?(1*)(\[\d+\])?//;
		push @out,length($1);
		push @out,$2 if defined $2;
		push @out,length($3);
		push @out,$4 if defined $4;
	};
	pop @out if ($out[$#out]==0); # Remove trailling zero

#	print "rle: ",join(",",@out),"\n";
	return @out;
};

###
### Encode a "long run", i.e. big number
###
sub pk_encode_long {
	my $n=shift;
	my @packed;

	while($n){
		unshift @packed,$n%16;
		$n=$n>>4;
	};

	for my $undef (1..$#packed){
		unshift @packed,0;
	};

	return @packed;
};

###
### Encode RLE data (with included repeat counts) into a nybble stream
###
### PK has "dyn" per-character, but for our font size encoding a dyn per
### character needs more space than it saves, so it's fixed for now.
###
sub pk_encode {
	my @out=@_;
	my $dyn=12;
	my @enc;

	for (@out){
		if($_ =~ /\[(\d+)\]/ ){
			if($1 == 1){
				push @enc,15;
			}else{
				my $n=$1-1; # this deviates from PK spec, i think.
				push @enc,14,pk_encode_long($1-1);
			};
		}elsif($_ == 0){
			warn "Encoder asked to encode a zero?"; # Shouldn't happen.
		}elsif($_ <= $dyn){ # Short length
			push @enc,$_;
		}elsif($_ <= 16*(13-$dyn)+$dyn){ # Medium length
			my $b=($_-$dyn-1)&0x0f;
			my $a=(($_-$dyn-1)>>4)+$dyn+1;
			push @enc,$a,$b; # (16*($a-$dyn-1)+$b+$dyn+1
		}else{ # long length
			my $n=$_- (16*(13-$dyn)+$dyn) + 16;
			push @enc,pk_encode_long($n);
		};
	};
#	print "enc: ",join(",",@enc),"\n";
	return @enc;
};

sub make_bytes{
	my @enc=@_;
	my @out;

	while(@enc){
		push @enc,0 if($#enc==2);
		push @out,16*(shift@enc)+(shift@enc);
	};
	return @out;
};

sub do_pk {
	my $char=shift;
	my $size=scalar @$char * $byte;
	print "Input char is $size bytes\n" if VERBOSE;

	$char=pk_dedup($char);
	
	if(VERBOSE){
		for (@$char){
			print "dedup: $_\n";
		};
	};

	my @rle=pk_rle ($char);

	if(VERBOSE){
		print "RLE: ",join(",",@rle),"\n";
	};

	my @enc=pk_encode (@rle);

	if(VERBOSE){
		print "encoded stream: ",join(",",@enc),"\n";
	};

	return make_bytes(@enc);
};
