#
# Makefile for CGIWrap
# Don't change this file, it will be replaced by Makefile.SH
#

#
# Other Compiler Flags
#
CC= gcc
CCFLAGS= -fpcc-struct-return 
LDFLAGS=  
LIBS=   

#
# Object Files
#
OBJS=cgiwrap.o debug.o allowdeny.o util.o

#
# Dependencies and rules
#
all: cgiwrap

.c.o: cgiwrap.h config.h Makefile
	$(CC) -c $(CFLAGS) $<

cgiwrap: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o cgiwrap

clean:
	rm -f *.o *.tar cgiwrap.cat cgiwrap core

realclean:
	rm -f Makefile cppstdin Wanted config.h 

