/******************************************************************/
/* CGI Wrapper Program to allow access to scripts on HTTPD server */
/* Written by:  Nathan Neulinger  <nneul@umr.edu>                 */
/* Includes many fixes and improvements as suggested by users     */
/*----------------------------------------------------------------*/
/* Provides various levels of security depending on which options */
/* are chosen in config.h                                         */
/******************************************************************/

/* SCRIPT DEBUGGING ROUTINES */

#include <stdio.h>     /* For passing data into called script */
#include <string.h>     /* String stuff */
#include <strings.h>    /* String stuff */

#include "config.h"    /* Configuration Options */

/* Global Variables */
extern int DEBUG;


/***/
/**   Macro for safely using null strings */
/***/
#define NullCheck(s) ( (s) ? (s) : ("<NULL>") )


/***/
/**   Print a debugging string if DEBUG is defined */
/***/
void DEBUG_Msg (char *msg)
{
	if (DEBUG)
	{
		printf("%s\n", NullCheck(msg));
		fflush(stdout);
	}
}

void DEBUG_Str (char *msg, char *var)
{
	if (DEBUG)
	{
		printf("%s '%s'\n", NullCheck(msg), NullCheck(var) );
		fflush(stdout);
	}
}

void DEBUG_StrStr (char *msg, char *var, char *var2)
{
	if (DEBUG)
	{
		printf("%s '%s'-'%s'\n", NullCheck(msg), 
			NullCheck(var), NullCheck(var2) );
		fflush(stdout);
	}
}

void DEBUG_Int (char *msg, int var)
{
	if (DEBUG)
	{
		printf("%s '%d'\n", NullCheck(msg), var );
		fflush(stdout);
	}
}


