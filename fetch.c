/**
 **  File: fetch.c
 **  Purpose: Routines to extract the user and script name from request
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */



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
		DoError("Couldn't find user and script name, check your URL.");
	}

	return userStr;
}




char *FetchScriptString( struct passwd *user)
{
	char *tempStr, *tempStr2;
	char *pathInfoString;
	char *queryString;
	char *scrStr;
	struct stat fstat;
	int i, max;

	DEBUG_Msg("\n");

	scrStr = 0;
	pathInfoString = getenv("PATH_INFO");
	if ( pathInfoString )  /* use PATH_INFO */
	{
		if ( pathInfoString[0] != 0 )
		{
			DEBUG_Msg("Trying to extract script from PATH_INFO");

			scrStr = StripPathComponents(1,pathInfoString);
			if ( ! strlen(scrStr) ) { scrStr = 0; }
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
		DoError("Couldn't find script name, check your URL.");
	}

	/** Now, need to split out PATH_INFO from the script name */
	for (i=1; i<=(CountSubDirs(scrStr)+1) && i>0; i++)
	{	
		tempStr = GetPathComponents(i, scrStr);
		tempStr2 = BuildScriptPath(user,tempStr);

		if ( !stat( tempStr2, &fstat ) )
		{
			max = i;
		} 		
		else
		{
			i = -1;
		}
		free(tempStr);
		free(tempStr2);
	}	

	/* Figure out the PATH_INFO and the script name */
	tempStr = StripPathComponents(max, scrStr);
	tempStr2 = (char *) malloc( strlen(tempStr) + 12 );
	if ( !strcmp("", tempStr) )
	{
		strcpy(tempStr2, "PATH_INFO=");
	}
	else
	{
		sprintf(tempStr2, "PATH_INFO=/%s", tempStr);
	}
	putenv(tempStr2);
	free(tempStr);

	return GetPathComponents(max, scrStr);
}



