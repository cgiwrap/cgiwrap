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

INSTALLPERMS="u=srx,g=x"
INSTALLOWNER=root
INSTALLGROUP=root

#
# Other Compiler Flags
#
CC= gcc
CCOPT= -O


#
# Dependencies and rules
#
all: cgiwrap man

cgiwrap: cgiwrap.c config.h Makefile
	$(CC) $(CCOPT) cgiwrap.c -o cgiwrap

man: cgiwrap.1
	nroff -man cgiwrap.1 > cgiwrap.cat

tar: clean
	rm -f cgiwrap-2.0.tar
	cd .. ; tar -cvf /tmp/cgiwrap-2.0.tar cgiwrap-2.0
	mv /tmp/cgiwrap-2.0.tar .

install: cgiwrap
	cp cgiwrap $(INSTALLDIR)/$(INSTALLNAME)
	chgrp $(INSTALLGROUP) $(INSTALLDIR)/$(CGIW_NAME)
	chown $(INSTALLOWNER) $(INSTALLDIR)/$(CGIW_NAME)
	chmod $(INSTALLPERMS) $(INSTALLDIR)/$(CGIW_NAME)
	ln $(CGIW_NAME) $(CGIWD_NAME)
	ln $(CGIW_NAME) $(NPHCGIW_NAME)
	ln $(CGIW_NAME) $(NPGCGIWD_NAME)

clean:
	rm -f *.o cgiwrap core

