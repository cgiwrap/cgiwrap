/**
 **  File: stdutil.c
 **  Purpose: Home grown alternatives for routines not in standard library
 **/
 
#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

/*
 * strdup() - copy a string into a newly allocated block of memory
 */
#if !defined(HAS_STRDUP)
char *strdup(char *str)
{
	char *temp;

	temp = (char *) SafeMalloc ( strlen(str) + 1, "strdup" );
	strcpy(temp,str);

	return temp;
}
#endif

