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
echo "Extracting Makefile (with variable substitutions)"

$spitshell >Makefile <<!GROK!THIS!
#
# Makefile for CGIWrap
# Don't change this file, it will be replaced by Makefile.SH
#

#
# Other Compiler Flags
#
CC= $cc
CCFLAGS= $ccflags
CPPFLAGS= $cppflags
LDFLAGS= $ldflags
LIBS= $libs
AFSCCOPTS= $afsccopts
AFSLDOPTS= $afsldopts
AFSLIBS= $afslibs

#
# Dependencies and rules
#
all: cgiwrap

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(AFSCCOPTS) $<

cgiwrap: debug.o cgiwrap.o util.o
	$(CC) cgiwrap.o debug.o util.o $(LDFLAGS) $(AFSLDOPTS) \
		$(AFSLIBS) $(LIBS) -o cgiwrap

clean:
	rm -f *.o *.tar cgiwrap.cat cgiwrap core

realclean:
	rm -f Makefile cppstdin Wanted config.h 

#
# Dependencies to get it to rebuild if you reconfigure
#
debug.o: cgiwrap.h config.h Makefile
cgiwrap.o: cgiwrap.h config.h Makefile
util.o: cgiwrap.h config.h Makefile

!GROK!THIS!

chmod 755 Makefile
$eunicefix Makefile
