static char *rcsid="$Id$";

/* CVS Log: $Log$
/* CVS Log: Revision 1.5  1996/07/30 02:23:24  nneul
/* CVS Log: Added cvs log to source.
/* CVS Log: */


/**
 **  File: stdutil.c
 **  Purpose: Home grown alternatives for routines not in standard library
 **/
 
#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/*
 * strdup() - copy a string into a newly allocated block of memory
 */
#if !defined(HAS_STRDUP)
char *strdup(char *str)
{
	char *temp;

	temp = (char *) malloc ( strlen(str) + 1 );
	if (!temp)
	{
		DoPError("Couldn't malloc memory for string!");
	}
	strcpy(temp,str);

	return temp;
}
#endif

