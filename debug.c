/**
 **  File: debug.c
 **  Purpose: Debugging output routines
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

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

/*
 * Intended to debug a call to execv by printing the path & argv array.
 */
void DEBUG_Exec (char *path, char **argv)
{
	if ( CONF_DEBUG )
	{
		printf("Executing: '%s'\n", path);
		printf("Arguments:\n");

		if ( argv == NULL )
		{
			printf("  missing argv\n");
		}
		else
		{
			int i = 0;
			while (argv[i] != NULL) 
			{ 
				printf("\t%d: '%s'\n", i, NullCheck(argv[i]));
				i++;
			}
		}
		printf("\n");
		fflush(stdout);
	}
}



