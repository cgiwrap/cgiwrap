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
CCDEBUG= $optimize
CCFLAGS= $ccflags $afsccopts
LDFLAGS= $ldflags $afsldopts
LIBS= $afslibs $libs

#
# Object Files
#
OBJS=cgiwrap.o debug.o util.o fetch.o stdutil.o

#
# Dependencies and rules
#
all: cgiwrap

.c.o: cgiwrap.h config.h Makefile
	\$(CC) \$(CCDEBUG) -c \$(CCFLAGS) \$<

cgiwrap: \$(OBJS)
	\$(CC) \$(OBJS) \$(LDFLAGS) \$(LIBS) -o cgiwrap

clean:
	rm -f *.o *.tar cgiwrap.cat cgiwrap core

realclean:
	rm -f Makefile cppstdin Wanted config.h 

reconf: clean
	./Configure -S

remake: reconf all

!GROK!THIS!

chmod 755 Makefile
$eunicefix Makefile
