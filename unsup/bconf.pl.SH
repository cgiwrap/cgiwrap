case $CONFIG in
'')
	if test -f config.sh; then TOP=.;
	elif test -f ../config.sh; then TOP=..;
	elif test -f ../../config.sh; then TOP=../..;
	elif test -f ../../../config.sh; then TOP=../../..;
	elif test -f ../../../../config.sh; then TOP=../../../..;
	else
		echo "Can't find config.sh."; exit 1
	fi
	. $TOP/config.sh
	;;
esac
: This forces SH files to create target in same directory as SH file.
: This is so that make depend always knows where to find SH derivatives.
case "$0" in
*/*) cd `expr X$0 : 'X\(.*\)/'` ;;
esac
echo "Extracting bconf.pl (with variable substitutions)"
: This section of the file will have variable substitutions done on it.
: Move anything that needs config subs from !NO!SUBS! section to !GROK!THIS!.
: Protect any dollar signs and backticks that you do not want interpreted
: by putting a backslash in front.  You may delete these comments.
$spitshell >bconf.pl <<!GROK!THIS!
#!$perl
!GROK!THIS!

: In the following dollars and backticks do not need the extra backslash.
$spitshell >>bconf.pl <<'!NO!SUBS!'
$| = 1;

print "Bulk Configuration of CGIWrap\n";
print "Reading 'config.sh'... ";
@CONFIG = ();

open(CONFIG, "config.sh") || die "couldn't open config.sh";
while ( $_ = <CONFIG> )
{
	chop $_;
	push(@CONFIG, $_);
}
close(CONFIG);
print "done.\n";

print "Backing up 'config.sh'... ";
system("cp config.sh config.sh.sv");
print "done.\n";

print "\nUserID: ";
chop($conf_userid = <STDIN>);

print "\nServer Port: ";
chop($conf_port = <STDIN>);

print "\nServer Name: ";
chop($conf_hostname = <STDIN>);

print "\nCGI Dir (i.e. public_html/cgi-bin): ";
chop($conf_cgidir = <STDIN>);

print "\n";
print "Writing new 'config.sh'...\n";

open(CONFIG, ">config.sh") || die "couldn't open config.sh";
foreach $f ( @CONFIG )
{
	if ( $f =~ /^httpd_user='/ )
	{
		print CONFIG "httpd_user='$conf_userid'\n";
	}
	elsif ( $f =~ /^conf_log_label='/ )
	{
		print CONFIG "conf_log_label='$conf_hostname-$conf_userid-$conf_port'\n";
	}
	elsif ( $f =~ /^conf_cgidir='/ )
	{
		print CONFIG "conf_cgidir='$conf_cgidir'\n";
	}
	else
	{
		print CONFIG "$f\n";
	}
}
close(CONFIG);

!NO!SUBS!
chmod 755 bconf.pl
$eunicefix bconf.pl
