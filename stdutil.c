static char *rcsid="$Id$";

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

