#!/usr/bin/perl

$f=$ARGV[0];

open(F,"<$f");
read(F,$buf,(-s $f));
close(F);

sub getver {
	local($buf) = @_;
	local($decl,$versionstring,$rest) = split('"',$buf);
	($version,$stamp) = split(' ',$versionstring);
	($major,$minor,$patch) = split('\.',$version);

	local($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdt) = localtime(time());
	$year = 1900 + $year;
	$build = sprintf("%02d-%02d-%02d %02d:%02d:%02d", 
		$year, $mon+1, $mday, $hour, $min, $sec);

	local($newver) = $decl;
	$newver .= '"';
	$newver .= $version;
	$newver .= ' ';
	$newver .= $build;
	$newver .= "\";";
	return $newver;
}

@lines=split("\n",$buf);
$newf="";
foreach $line(@lines) {
	$line =~ s///g;
	if ($line =~ m/static const char MUZIKBROWZER_VERSION/) {
		$newf .= getver($line);
	} else {
		$newf .= $line;
	}
	$newf .= "\r\n";
}

open(F,">$f");
print F $newf;
close(F);
print "\r\n \r\nmuzikbrowzerVersion.h updated\r\n";

#Inno Release.iss

$s1 = $major . '.' . $minor . '.' . $patch;
$s2 = $major . '_' . $minor . '_' . $patch;

open(F,"<../Inno/Release.iss");
read(F, $buf, (-s "../Inno/Release.iss"));
close(F);
@lines = split("\n", $buf);
$newf = "";
foreach $line(@lines) {
	$line =~ s///g;
	if ($line =~ m/^AppVerName/) {
		$newf .= "AppVerName=Muzikbrowzer ";
		$newf .= $s1;
		$newf .= " (built $build)";
	} elsif ($line =~ m/^OutputBaseFilename/) {
		$newf .= "OutputBaseFilename=Muzikbrowzer_setup_";
		$newf .= $s2;
	} else {
		$newf .= $line;
	}
	$newf .= "\r\n";
}

open(F,">../Inno/Release.iss");
print F $newf;
close(F);
print "Release.iss updated\r\n";


#Inno Debug.iss

$s1 = $major . '.' . $minor . '.' . $patch;
$s2 = $major . '_' . $minor . '_' . $patch;

open(F,"<../Inno/Debug.iss");
read(F, $buf, (-s "../Inno/Debug.iss"));
close(F);
@lines = split("\n", $buf);
$newf = "";
foreach $line(@lines) {
	$line =~ s///g;
	if ($line =~ m/^AppVerName/) {
		$newf .= "AppVerName=Muzikbrowzer ";
		$newf .= $s1;
	} elsif ($line =~ m/^OutputBaseFilename/) {
		$newf .= "OutputBaseFilename=Muzikbrowzer_setup_";
		$newf .= $s2;
		$newf .= "_D";
	} else {
		$newf .= $line;
	}
	$newf .= "\r\n";
}

open(F,">../Inno/Debug.iss");
print F $newf;
close(F);

print "Debug.iss updated\r\n";
