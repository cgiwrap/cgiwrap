/* 
 *  CGIWrap is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CGIWrap is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CGIWrap; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Copyright 2003-2005, Nathan Neulinger <nneul@umr.edu>
 *
 */

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

