#!/usr/bin/perl

# makefont.pl - by <sec@42.org> 05/2011, BSD Licence
#
# This script rasterizes a font at a given size, then compresses it using a
# simple RLE encoding and writes its definition into compilable .c/.h files

# Rasterisation is done via GD which in turn uses freetype
# Compression is done similar to the PK compressed fonts used by LaTex
# For a verbose description see:
# - <http://www.davidsalomon.name/DC4advertis/PKfonts.pdf> or
# - <http://www.tug.org/TUGboat/tb06-3/tb13pk.pdf>

use GD;
use strict;
use warnings;
use Getopt::Long;

$|=1;

###
### Configure me
### 

my $charlist;
for(32..126){
	$charlist.=chr $_;
};

###
### Runtime Options
###

my ($verbose,$raw);
my $size=18;

my $font="../ttf/Ubuntu-Regular.ttf";

GetOptions ("size=i"   => \$size,    # numeric
            "font=s"   => \$font,    # string
            "verbose"  => \$verbose, # flag
			"raw"      => \$raw,     # flag
			"help"     => sub {
			print <<HELP;
Uasge: makefont.pl [-r] [-v] [-f fontfile] [-s size]

Options:
--verbose         Be verbose.
--raw             Create raw/uncompressed font.
--font <filename> Source .ttf file to use. [Default: $font]
--size <size>     Pointsize the font should be rendered at. [Default: $size]
HELP
			exit(-1);}
			);

###
### Code starts here.
###

my $width=2000;
my $height=100;
my $xoff=30;

my $origsize;
my $c1size;
my $c2size;

my ($title,$licence)=getfontname($font);
die "Couldn't get font name?" if !defined $title;

$title.=" ${size}pt";

my $fonts=$title;
$fonts=~s/[ -]//g;
$fonts=~s/Bitstream//;
$fonts=~s/Sans//;
$fonts=~s/Regular//;

my $file=$fonts;
$file=~s/pt$//;
$file=~y/A-Z/a-z/;

$file.="-raw" if($raw);

print "Rasterizing $title into ${file}.c\n";

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

open (C,">",$file.".c")||die;
if(defined $licence){
	$licence=~s/\n/\n * /g;
	$licence="\n/* ".$licence."\n */";
}else{
	$licence="";
};

print C <<EOF
#include "$file.h"

/* Font data for $title */
$licence

/* This file created by makefont.pl by Sec <sec\@42.org> */

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
	print "### Start $char\n" if($verbose);

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
	if($#enc>$#raw+3){
		warn "Compression failure: Encoding char $char raw.\n";
		$raw=1;
	};

	# Generate C source
	if($raw){
		$c2size-=scalar(@enc);
		@enc=(255,$preblank,$postblank,@raw);
		$c2size+=scalar(@enc);
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

	if($verbose){
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

printf C "\n";
printf C "/* Font metadata: \n";
printf C " * Name:          %s\n", $title;
printf C " * Height:        %d px (%d bytes)\n", $bottom-$top+1,$byte;
printf C " * Maximum width: %d px\n",$maxsz;
printf C " * Storage size:  %d bytes (compressed by %2d%%)\n",
	$c2size,(1-$c2size/$origsize)*100;
printf C " */\n";

close(C);

open (H,">",$file.".h")||die;
print H <<EOF;
#include "fonts.h"

extern const struct FONT_DEF Font_$fonts;
EOF
close(H);

print "\ndone.\n" if($verbose);
print "\n";
print "Original size: $origsize\n";
print "Simple compression: $c1size\n";
print "PK compression: $c2size\n";
print "Maximum character size is: $byte*$maxsz bytes\n";


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
			$dl++ while ($idx+$dl<$#echar && $echar[$idx] eq $echar[$idx+$dl]);

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
		}elsif($_ == 0){ # Encoding a 0 will only happen at the start of
                         # character if "first pixel" is 1 instead of 0.
                         # HACK:  We transmit this fact to the decoder
                         #        by encoding a "14"-nibble which would be
                         #        illegal at this point anyway.
			push @enc,14;
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
	print "Input char is $size bytes\n" if $verbose;

	$char=pk_dedup($char);
	
	if($verbose){
		for (@$char){
			print "dedup: $_\n";
		};
	};

	my @rle=pk_rle ($char);

	if($verbose){
		print "RLE: ",join(",",@rle),"\n";
	};

	my @enc=pk_encode (@rle);

	if($verbose){
		print "encoded stream: ",join(",",@enc),"\n";
	};

	return make_bytes(@enc);
};

sub getfontname {
	my $file = shift;
	use constant SEEK_SET => 0;
	use Encode qw(decode);
	my @font;

	open (my $fh,"<",$file) || die "Can't open $font: $!";

	my($buf,$str);

	sysread($fh,$buf,12); # OFFSET_TABLE
	my($maj,$min,$num,undef,undef,undef)=unpack("nnnnnn",$buf);

	die "It's not a truetype font!" if ($maj != 1 || $min != 0);

	for(1..$num){
		sysread($fh,$buf,16); # TABLE_DIRECTORY
		my($name,undef,$off1,$len)=unpack("A4NNN",$buf);
		if ($name eq "name"){
			seek($fh,$off1,SEEK_SET);
			sysread($fh,$buf,6);
			my(undef,$cnt,$off2)=unpack("nnn",$buf);
			sysread($fh,$buf,12*$cnt);
			while(length($buf)){
				my(undef,$enc,undef,$id,$len,$off3)=unpack("nnnnnn",$buf);
				substr($buf,0,12)="";
				seek($fh,$off1+$off2+$off3,SEEK_SET);
				sysread($fh,$str,$len);
				if($enc==1){
					$str=decode("UCS-2",$str);
				};
				# 0 	Copyright notice
				# 1 	Font Family name.
				# 2 	Font Subfamily name.
				# 3 	Unique font identifier.
				# 4 	Full font name.
				# 5 	Version string.
				# 6 	Postscript name for the font.
				# 7 	Trademark
				# 8 	Manufacturer Name.
				# 9 	Designer.
				# 10 	Description.
				# 11 	URL Vendor.
				# 12 	URL Designer.
				# 13 	License description
				# 14 	License information URL.
				$font[$id]=$str;
#				print "- $str\n";
			};
			last;
		};
	};
	my($fontname,$licence);
	$fontname=$font[1];

	if(defined $font[2]){
		$fontname.=" ".$font[2];
	}elsif (defined $font[4]){
		$fontname=$font[4];
	};
	$licence=$font[0]."\n";
	$licence.="\n".$font[13]."\n" if defined $font[13];
	$licence.="\nSee also: ".$font[14]."\n" if defined $font[14];
	if(wantarray()){
		return ($fontname,$licence);
	}else{
		return $fontname;
	};
};
