/**
 **  File: debug.c
 **  Purpose: Debugging output routines
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/*
 * Global flag to turn debugging output on/off
 */
int CONF_DEBUG;


/*
 * Print out a debugging message if CONF_DEBUG is true
 */
void DEBUG_Msg (char *msg)
{
	if (CONF_DEBUG)
	{
		printf("%s\n", NullCheck(msg));
		fflush(stdout);
	}
}

void DEBUG_Str (char *msg, char *var)
{
	if (CONF_DEBUG)
	{
		printf("%s '%s'\n", NullCheck(msg), NullCheck(var) );
		fflush(stdout);
	}
}

void DEBUG_StrStr (char *msg, char *var, char *var2)
{
	if (CONF_DEBUG)
	{
		printf("%s '%s'-'%s'\n", NullCheck(msg), 
		    NullCheck(var), NullCheck(var2) );
		fflush(stdout);
	}
}

void DEBUG_Int (char *msg, int var)
{
	if (CONF_DEBUG)
	{
		printf("%s '%d'\n", NullCheck(msg), var );
		fflush(stdout);
	}
}


