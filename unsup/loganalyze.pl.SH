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
echo "Extracting loganalyze.pl (with variable substitutions)"
: This section of the file will have variable substitutions done on it.
: Move anything that needs config subs from !NO!SUBS! section to !GROK!THIS!.
: Protect any dollar signs and backticks that you do not want interpreted
: by putting a backslash in front.  You may delete these comments.
$spitshell >loganalyze.pl <<!GROK!THIS!
#!$perl -s
# cgiwrap_anal.pl - log analysis routine for cgiwrap logs with html output
#       should be run regularly (usually with log rotation)
# usage: cgiwrap_anal [-t] [logfile]
#       -t      check and display the type of each script
#       logfile to be analysed if not the default
#
# written by: Lawrie.brown@adfa.oz.au - Dec 95

# configurable parameters
\$log_file="$log_file"; # location of default cgiwrap logfile
\$conf_cgidir="WWW/cgi-bin";       # location of users cgi-bin under home
\$title="cgiwrap log analysis";          # title used on html document created

!GROK!THIS!

: In the following dollars and backticks do not need the extra backslash.
$spitshell >>loganalyze.pl <<'!NO!SUBS!'

# need ctime package for &ctime
require "ctime.pl";

# check for commandline args
$log_file = $ARGV[0]    if ($#ARGV >= 0);       # grab named logfile

# do some sanity checks
die "cgiwrap_anal: Unable to read logfile: $log_file\n" unless -T "$log_file";

# get date for today
$date = &ctime(time);                   # get current time

# now read and summarise contents of the logfile
open(IN, "<$log_file");
while (<IN>) {
    @in = split(/\t/,$_);               # split entry into tab separated fields
    $name = "~$in[0]/$conf_cgidir/$in[1]";      # construct script name
    $cnt{$name}++;                      # increment calls on this script
}
close(IN);

# now construct html reply showing the log summary
print "<html><head>\n<title> $title </title>\n";
print "</head><body>\n<h1> $title </h1>\n";
print "<h2>Analysis of $log_file</h2>\n<h3>on $date</h3>\n";
print "<table border>\n";
print "<tr><th>Script</th><th># Calls</th>";
print "<th>Type</th>"   if ($t);        # extra col if want script types
print "</tr>\n";

# now summarise calls on the scripts
foreach $name ( sort keys (%cnt) ) {
    $total += $cnt{$name};              # increment total count
    print "<tr><td>$name</td><td>$cnt{$name}</td>";     # display row for script
    &CheckType($name) if ($t);          # check and display script type
    print "</tr>\n";
}

# finish up html output
print "<tr><th>TOTAL</th><th>$total</th>";      # display row of totals
print "<th></th>"       if ($t);        # extra col if want script types
print "</tr>\n";
print "</table>\n";
print "<hr>\n</body></html>\n";
exit(0);

#############################################################################
# CheckType($name) - check and display script type
sub CheckType
{
    local($name) = @_[0];               # script name

    # expand ~user into full home directory (hack from perl FAQ Q2.31)
    $name =~ s#^~(\w+)(/.*)?$#(getpwnam($1))[7].$2#e;   # expand ~user

    print "<td>";
    if (-r "$name") {                   # can access script
        if (-x "$name") { print "executable "; }
        else            { print "non-executable "; }

        if (-B "$name") { print "binary ";}     # binary file
        else {                          # text file or script
            open(SC, "<$name");         # check first line for type
            $shell = <SC>;
            if ($shell =~ "^#!") {      # is a script
                $shell =~ s:^#! *\S+/(\S+).*$:\1:;      # extract shell name
                print "$shell script "; # and display it
            } else {
                print "text file ";
            }
            close(SC);
        }
    } else {
        print "unreadable file";        # no access to script
    }
    print "</td>";
}
!NO!SUBS!
chmod 755 loganalyze.pl
$eunicefix loganalyze.pl
