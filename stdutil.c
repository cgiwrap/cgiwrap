static char *rcsid="$Id$";

/* CVS Log: 
test
$Log$
Revision 1.7  1996/07/30 02:25:11  nneul
*** empty log message ***

Revision 1.6  1996/07/30 02:24:38  nneul
*** empty log message ***

*/

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

