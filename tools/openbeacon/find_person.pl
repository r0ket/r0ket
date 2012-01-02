use JSON;
use Data::Dumper;

my $content;

#open(F,"<",shift);
open(F,"-|","wget -qO- http://176.99.39.100/tracking.json");
while(<F>){
	$content.=$_;
};


my $json = from_json($content, {ascii => 1});

#print Dumper $json;

my $room;
my $floor;
for (@{$json->{tag}}){
	if($_->{nick} =~ qr/$ARGV[0]/i){
		for my $r (@{$json->{reader}}){
			if($r->{id} eq $_->{reader}){
#				print Dumper $r;
				$room=$r->{name};
				$floor=$r->{floor};
			};
		};
		print $_->{nick}, " is at ",$room," (Level ",qw(A B C)[$floor-1],")\n";
#		print Dumper $_
	}
};
