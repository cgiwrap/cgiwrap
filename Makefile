#
# Simple Makefile for CGIWrap - Nathan Neulinger
#

#
# Installation info
# 
INSTALLDIR=/usr/local/etc/httpd/cgi-bin
INSTALLNAME=cgiwrap
DINSTALLNAME=cgiwrapd
INSTALLPERMS="u=srx,g=x"
INSTALLOWNER=root
INSTALLGROUP=root

#
# Other Compiler Flags
#
CC= gcc

all: cgiwrap cgiwrapd man

cgiwrap: cgiwrap.c config.h Makefile
	$(CC) -O cgiwrap.c -o cgiwrap

cgiwrapd: cgiwrap.c config.h Makefile
	$(CC) -O -DDEBUG cgiwrap.c -o cgiwrapd

man: cgiwrap.1
	nroff -man cgiwrap.1 > cgiwrap.cat

tar: clean
	rm -f cgiwrap.tar
	cd .. ; tar -cvf /tmp/cgiwrap.tar cgiwrap
	mv /tmp/cgiwrap.tar .

install: cgiwrap
	cp cgiwrap $(INSTALLDIR)/$(INSTALLNAME)
	chgrp $(INSTALLGROUP) $(INSTALLDIR)/$(INSTALLNAME)
	chown $(INSTALLOWNER) $(INSTALLDIR)/$(INSTALLNAME)
	chmod $(INSTALLPERMS) $(INSTALLDIR)/$(INSTALLNAME)

installd: cgiwrapd
	cp cgiwrapd $(INSTALLDIR)/$(DINSTALLNAME)
	chgrp $(INSTALLGROUP) $(INSTALLDIR)/$(DINSTALLNAME)
	chown $(INSTALLOWNER) $(INSTALLDIR)/$(DINSTALLNAME)
	chmod $(INSTALLPERMS) $(INSTALLDIR)/$(DINSTALLNAME)

installboth: install installd

clean:
	rm -f *.o cgiwrap cgiwrapd core

