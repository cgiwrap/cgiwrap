#
# Simple Makefile for CGIWrap - Nathan Neulinger
#

#
# Installation info
# 
INSTALLDIR=/usr/local/etc/httpd/cgi-bin

CGIW_NAME=cgiwrap
CGIWD_NAME=cgiwrapd
NPHCGIW_NAME=nph-cgiwrap
NPHCGIWD_NAME=nph-cgiwrapd

INSTALLPERMS=4750
INSTALLOWNER=root
INSTALLGROUP=root

VERSION=2.4

#
# Other Compiler Flags
#
CC= gcc
CCOPT= -O

#
# Some options suggestion by users
#
# SunOS 4.1.4
# 	CC=gcc
# 	CCOPT= -O2 -fomit-frame-pointer -pipe
#
# CDC 4680 EP/IX 2.1.1ad
#	CC=cc
#	CCOPT= -systype sysv -I/usr/include/bsd -lbsd
#

#
# Dependencies and rules
#
all: cgiwrap man

cgiwrap.o: cgiwrap.c config.h Makefile
	$(CC) $(CCOPT) -c cgiwrap.c

debug.o: debug.c config.h Makefile
	$(CC) $(CCOPT) -c debug.c

cgiwrap: debug.o cgiwrap.o
	$(CC) $(CCOPT) cgiwrap.o debug.o -o cgiwrap

man: cgiwrap.1
	nroff -man cgiwrap.1 > cgiwrap.cat

tar: clean
	rm -f cgiwrap-$(VERSION).tar
	cd .. ; tar -cvf /tmp/cgiwrap-$(VERSION).tar cgiwrap-$(VERSION)
	mv /tmp/cgiwrap-$(VERSION).tar .

install: cgiwrap
	cp cgiwrap $(INSTALLDIR)/$(INSTALLNAME)
	chgrp $(INSTALLGROUP) $(INSTALLDIR)/$(CGIW_NAME)
	chown $(INSTALLOWNER) $(INSTALLDIR)/$(CGIW_NAME)
	chmod $(INSTALLPERMS) $(INSTALLDIR)/$(CGIW_NAME)
	cd $(INSTALLDIR)
	ln $(CGIW_NAME) $(CGIWD_NAME)
	ln $(CGIW_NAME) $(NPHCGIW_NAME)
	ln $(CGIW_NAME) $(NPHCGIWD_NAME)

clean:
	rm -f *.o *.tar cgiwrap.cat cgiwrap core

