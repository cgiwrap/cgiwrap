/**
 **  File: fetch.c
 **  Purpose: Routines to extract the user and script name from request
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

char *FetchUserString(void)
{
	char *pathInfoString;
	char *queryString;
	char *userStr;

	DEBUG_Msg("\n");

	userStr = (char *) 0;
	pathInfoString = getenv("PATH_INFO");
	if ( pathInfoString )  /* use PATH_INFO */
	{
		if ( pathInfoString[0] != 0 )
		{
			DEBUG_Msg("Trying to extract user from PATH_INFO.");

			userStr = GetPathComponents(1, pathInfoString);
		}
		else
		{
			DEBUG_Msg("PATH_INFO is empty, can't check.");
		}
	}

	queryString = getenv("QUERY_STRING");
	if ( !userStr && queryString )  /* or use QUERY_STRING */
	{
		if ( queryString[0] != 0 )
		{
			DEBUG_Msg("Trying to extract user from QUERY_STRING");

			DEBUG_Msg("Read in user keyword value");
			userStr = GetValue("user", getenv("QUERY_STRING") );
		}
		else
		{
			DEBUG_Msg("QUERY_STRING is empty, can't check.");
		}
	}

/* Handle ~ notation */
	if (userStr)
	{
		if (userStr[0] == '~')
		{
			userStr++;
		}
	}

	if ( !userStr ) /* nothing at all found */
	{
		MSG_Error_General("Couldn't find user and script name, check your URL.");
	}

	return userStr;
}

char *FetchScriptString( char *basedir )
{
	char *tempStr, *tempStr2;
	char *pathInfoString;
	char *queryString;
	char *scrStr;
	struct stat fstat;
	int i, max, max_user, max_multiuser;

	scrStr = 0;
	pathInfoString = getenv("PATH_INFO");
	if ( pathInfoString )  /* use PATH_INFO */
	{
		if ( pathInfoString[0] != 0 )
		{
			DEBUG_Msg("Trying to extract script from PATH_INFO");

			scrStr = StripPathComponents(1,pathInfoString);
			if ( ! strlen(scrStr) ) { scrStr = 0; }

			DEBUG_Str("Extracted PATH_INFO", scrStr);
		}
		else
		{
			DEBUG_Msg("PATH_INFO is empty, can't check.");
		}
	}

	queryString = getenv("QUERY_STRING");
	if ( !scrStr && queryString )  /* or use QUERY_STRING */
	{
		if ( queryString[0] != 0 )
		{
			DEBUG_Msg("Trying to extract script from QUERY_STRING");

			DEBUG_Msg("Read in script keyword value");
			scrStr = GetValue("script", getenv("QUERY_STRING") );
			DEBUG_Str("Value", scrStr);
		}
		else
		{
			DEBUG_Msg("QUERY_STRING is empty, can't check.");
		}
	}


	if ( !scrStr ) /* nothing at all found */
	{
		MSG_Error_General("Couldn't find script name, check your URL.");
	}

	/* Initially neither a user nor a multiuser script was found */
	max_user = 0;
	max_multiuser = 0;

	/* Split out PATH_INFO from the script name */
	/* This pass searches the per-user script dir */
	for (i=1; i<=(CountSubDirs(scrStr)+1) && i>0; i++)
	{	
		tempStr = GetPathComponents(i, scrStr);
		tempStr2 = BuildScriptPath(basedir,tempStr);

		if ( !stat( tempStr2, &fstat ) )
		{
			max_user = i;
		} 		
		else
		{
			i = -1;
		}
		free(tempStr);
		free(tempStr2);
	}

#if defined(CONF_MULTIUSER_CGI_DIR)
	/* Split out PATH_INFO from the script name */
	/* This pass searches the per-user script dir */
	for (i=1; i<=(CountSubDirs(scrStr)+1) && i>0; i++)
	{	
		tempStr = GetPathComponents(i, scrStr);
		tempStr2 = BuildScriptPath(CONF_MULTIUSER_CGI_DIR,tempStr);

		if ( !stat( tempStr2, &fstat ) )
		{
			max_multiuser = i;
		} 		
		else
		{
			i = -1;
		}
		free(tempStr);
		free(tempStr2);
	}
#endif


#if !defined(CONF_MULTIUSER_CGI_DIR)
	if ( max_user < 1 )
#else
	if ( max_user < 1 && max_multiuser < 1 )
#endif	
	{
		MSG_Error_General("Script File Not Found!");
	}

	/* Indicate to rest of cgiwrap which script was chosen */
	/* Only use the multiuser one if it is more specific (more dirs in path) */
#if defined(CONF_MULTIUSER_CGI_DIR)
	if ( max_multiuser > 0 && max_multiuser > max_user )
	{
		DEBUG_Msg("Using multiuser cgi script.\n");
		Context.multiuser_cgi_script = 1;
		max = max_multiuser;
	}
	else
#endif
	{
		max = max_user;
	}

	/* Figure out the PATH_INFO and the script name */

#if 0
/* WHAT IS THIS DOING? WHY THE SPECIAL HANDLING??? */
/* leaving out till explanation */
#ifdef PATH_PROG_PHP
	if (Context.interpreted_script) {
		char *c;
		char *reqStr = getenv("REQUEST_URI");
		max = 0;

		if ((c = index(reqStr,'?')) != NULL)
		{
			tempStr = (char *) SafeMalloc (c - reqStr + 1, "reqStr");
			tempStr = strncat(tempStr, reqStr, c - reqStr);
			tempStr[(c-reqStr)] = '\0';
		}
		else
		{
			tempStr = strdup(reqStr);
		}
	}
	else
#endif
#endif
	{
		tempStr = StripPathComponents(max, scrStr);
	}
	tempStr2 = (char *) SafeMalloc( strlen(tempStr) + 12, 
		"PATH_INFO environment variable" );
	if ( !strcmp("", tempStr) )
	{
		strcpy(tempStr2, "PATH_INFO=");
	}
	else
	{
		sprintf(tempStr2, "PATH_INFO=/%s", tempStr);
	}
	SafePutenv(tempStr2, "set PATH_INFO environment variable");
	free(tempStr);

	return GetPathComponents(max, scrStr);
}



