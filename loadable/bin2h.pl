#!/usr/bin/perl
#
# vim:set ts=4 sw=4:

use strict;

sub dwim{
	local $/=undef;
	my $file=shift;

	open(IN,"<:bytes",$file) || die "Can't open $file: $!";
	my @bytes=unpack("C*",<IN>);
	close(IN);

	$file=~s/\.[^.]+$//;

	open(OUT,">","${file}.h") || die "Can't write ${file}.h: $!";

    $file=~s!.*/!!;

	print OUT "const uint16_t loadable_${file}_size = ", scalar @bytes, ";\n";
	print OUT "const uint8_t loadable_${file}[] = {\n";

	my $ctr=0;
	for(@bytes){
		print OUT "\t" if($ctr==0);
		printf OUT "0x%02x, ",$_;
		if(++$ctr==8){
			print OUT "\n";
			$ctr=0;
		};
	};
	print OUT "\n" if($ctr!=0);

	print OUT "};\n";
	close(OUT);
};

for(@ARGV){
	dwim($_);
};
