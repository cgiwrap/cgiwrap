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

VERSION=2.7

#
# CGIwrap Configuration Options - Overides config.h
#
LOGFILE=/home/local/testhttp.logs/cgiwrap.log
CGIDIR=public_html/cgi-bin
#CGIDIR=public_html/auth-cgi-bin
HTTPD_USER=testhttp

#
# AFS libs - only needed if you're compiling with AFS pagsh support
#
AFSDIR = /usr/afsws
AFSLDFLAGS = -L$(AFSDIR)/lib -L$(AFSDIR)/lib/afs
AFSLIBS = -lkauth -lprot -lubik -lauth -lrxkad -lsys -ldes -lrx \
	-llwp -lcom_err $(AFSDIR)/lib/afs/util.a -lBSD
AFS_INC_FLAGS = -I$(AFSDIR)/include


#
# Other Compiler Flags
#
CC= gcc
CCOPT= -O3
LDOPT= -O3 #$(AFSLDFLAGS) $(AFSLIBS)


#
# Dependencies and rules
#
all: cgiwrap man

cgiwrap.o: cgiwrap.c config.h Makefile
	$(CC) $(CCOPT) \
		-DCGIDIR=\"$(CGIDIR)\" \
		-DHTTPD_USER=\"$(HTTPD_USER)\" \
		-DLOGFILE=\"$(LOGFILE)\" \
		-c cgiwrap.c 

debug.o: debug.c config.h Makefile
	$(CC) $(CCOPT) -c debug.c

cgiwrap: debug.o cgiwrap.o
	$(CC) cgiwrap.o debug.o $(LDOPT) -o cgiwrap

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

