#!/usr/bin/perl

$f=$ARGV[0];

open(F,"<$f");
read(F,$buf,(-s $f));
close(F);

($decl,$versionstring,$rest) = split('"',$buf);
($version,$stamp) = split(' ',$versionstring);
($major,$minor,$patch) = split('\.',$version);

($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdt) = localtime(time());
$year = 1900 + $year;
$build = sprintf("%02d-%02d-%02d %02d:%02d:%02d", 
	$year, $mon+1, $mday, $hour, $min, $sec);

$out = $decl;
$out .= '"';
$out .= $version;
$out .= ' ';
$out .= $build;
$out .= "\";\n";

open(F,">$f");
print F $out;
close(F);

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
		$newf .= "AppVerName=muzikbrowzer ";
		$newf .= $s1;
	} elsif ($line =~ m/^OutputBaseFilename/) {
		$newf .= "OutputBaseFilename=muzikbrowzer_setup_";
		$newf .= $s2;
	} else {
		$newf .= $line;
	}
	$newf .= "\r\n";
}

open(F,">../Inno/Release.iss");
print F $newf;
close(F);


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
		$newf .= "AppVerName=muzikbrowzer ";
		$newf .= $s1;
	} elsif ($line =~ m/^OutputBaseFilename/) {
		$newf .= "OutputBaseFilename=muzikbrowzer_setup_";
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


