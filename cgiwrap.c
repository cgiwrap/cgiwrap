/******************************************************************/
/* CGI Wrapper Program to allow access to scripts on HTTPD server */
/* Written by:  Nathan Neulinger  <nneul@umr.edu>                 */
/*                                                                */
/* Thanks to  legalize@xmission.com for sample source and         */
/* suggestion for PATH_INFO handling                              */
/*                                                                */
/*----------------------------------------------------------------*/
/* Provides various levels of security depending on which options */
/* are chosen in config.h                                         */
/******************************************************************/

/******************************************************************/
/* URL:  http://server/cgi-bin/cgiwrap?user=XXXX&script=YYYY      */
/* URL:  http://server/cgi-bin/cgiwrap/user/script                */
/* URL:  http://server/cgi-bin/cgiwrap/~user/script               */
/*                                                                */
/*    XXXX is the user who owns the script                        */
/*    YYYY is the name of the script itself                       */ 
/******************************************************************/

/******************************************************************/
/* Changes since 1.0:                                             */
/*   Added support for PATH_INFO specification of user/script     */
/*   Added stderr redirection to stdout                           */
/*   Added option for doing debugging output by cmd name          */
/*   Added option to use exec or system calls                     */
/******************************************************************/


#include <stdio.h>     /* For passing data into called script */
#include <stdlib.h>    /* Standard library functions */
#include <pwd.h>       /* For getting uids from username */
#include <unistd.h>    /* Stuff for setting uid      */
#include <sys/types.h> /* Types for uid and stuff */
#include <limits.h>   /* Limit on directory length and stuff */
#include <string.h>     /* String stuff */
#include <strings.h>    /* String stuff */
#include <sys/stat.h>  /* For file stating */
#include <sys/time.h>  /* Time for logging stuff */

#include "config.h"    /* Configuration Options */


/* Global Variables */
int DEBUG = 0;


/***/
/**   Macro for safely using null strings */
/***/
#define NullCheck(s) ( (s) ? (s) : ("<NULL>") )


/***/
/**   Display an error message and exit the program */
/***/
void DoError (char *msg)
{
	extern int errno;
	extern char *sys_errlist[];

	printf ("Content-Type: text/plain\n\n");

	printf (" CGI Wrapper Error: %s \n", msg);
	printf (" Last Error: %s [%d]\n", sys_errlist[errno], errno);

	exit(1);
}


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

void DEBUG_Int (char *msg, int var)
{
	if (DEBUG)
	{
		printf("%s '%d'\n", NullCheck(msg), var );
		fflush(stdout);
	}
}


/***/
/**   Log a request to the log file. */
/***/
void Log (char *user, char *script )
{

	time_t timevar;
	FILE *logFile;

	DEBUG_Msg("   Opening log file.");

	if ( (logFile = fopen(LOGFILE, "a")) == NULL )
	{
		DoError("Could not open log file for appending!");
		exit(1);
	}

	DEBUG_Msg("   Writing log entry.");

	fprintf(logFile, "%s\t", NullCheck( user ) );
	fprintf(logFile, "%s\t", NullCheck( script ) );
	fprintf(logFile, "%s\t", NullCheck( getenv("REMOTE_HOST") ) );
	fprintf(logFile, "%s\t", NullCheck( getenv("REMOTE_ADDR") ) );

	time(&timevar);
	fprintf(logFile, "%s", ctime(&timevar) );

	DEBUG_Msg("   Closing log file.");

	fclose(logFile);

	DEBUG_Msg("   Done logging request.");

}




/***/
/**   Copy a string into a newly allocated block of memory */
/***/
char *mystrcpy(char *str)
{
	char *temp;

	temp = (char *) malloc ( strlen(str) + 1 );
	if (!temp)
	{
		DoError("Couldn't malloc memory for string!");
	}
	strcpy(temp,str);

	return temp;
}



/***/
/**   Extract and return the value portion of a key=value pair found in a string */
/***/
char *GetValue( char *keyword, char *string )
{
	char *returnStr;
	char *dataStr;
	char *tempStr;
	char *theString;
	int i;

	theString = mystrcpy(string);

	returnStr = (char *) 0;
	tempStr = (char *) strtok(theString, "&");
	while (tempStr)
	{
		dataStr = (char *) strstr(tempStr, keyword);
		if ( dataStr == tempStr )
		{
			returnStr = (char *) strchr (dataStr, '=') + 1;
			tempStr = (char *) 0;
		}
		else
		{
			tempStr = (char *) strtok( (char *) 0, "&");
		}
	}


	DEBUG_Str("   Keyword: ", keyword);
	DEBUG_Str("   Value: ", returnStr);

	returnStr = mystrcpy(returnStr);
	free(theString);
	return returnStr;
}



/***/
/**   Extract name and script from PathInfo */
/***/
void ExtractPathInfo(char **userStr, char **scrStr )
{
	char *dataStr;
	char *tempStr;
	char *pathStr;
	static char putEnvString[500];

	int i;

	pathStr = mystrcpy( (char *) getenv("PATH_INFO"));
	tempStr = mystrcpy(pathStr);

	*userStr = (char *) strtok(tempStr, "/"); /* Get usestring */
	*scrStr = (char *) strtok( (char *) 0, "/"); /* Get script string */

	tempStr = (char *) strtok( (char *) 0, "/");

	if (tempStr)
	{
		strcpy(pathStr, "/");
		strcat(pathStr, tempStr);	
	}
	else
	{
		strcpy(pathStr, "");
	}

	if (*userStr)
	{
		if (*userStr[0] == '~')
		{
			userStr++;
		}
	}
	
		
	DEBUG_Str("   User: ", *userStr);
	DEBUG_Str("   Script: ", *scrStr);
	DEBUG_Str("   New PathInfo: ", pathStr);

	DEBUG_Msg("\nStripping user and script data from PATH_INFO envar.\n");

	sprintf(putEnvString, "%s=%s", "PATH_INFO", pathStr);
	putenv(putEnvString);
}





/***/
/**  Clean up a username and script name - removing non printables and whitespace  */
/***/
void Sanitize(char *string)
{
	int len, in;

	len = strlen(string);
	if (string) 
	{
		for (in=0; in<len; in++)
		{

			if ( !isprint(string[in]) || isspace(string[in]) )
			{
				string[in] = '_'; 
			}

#ifdef NO_SUB_DIRS
			if ( string[in] == '/' ) 
			{
				string[in] = '_';
			}
#endif
		}

	}
}



/***/
/**   Main program */
/***/
void main (int argc, char *argv[])
{
	char *userStr; /* User name */
	char *scrStr; /* Name of script */
	struct passwd *user; /* For getting uid from name */
	char *dir; /* For checking current directory */
	struct stat fileStat; /* For checking file status */
	int statErr; /* For checking reason why stat failed */
	int scriptErr; /* Return code from executing script */
	char *execStr; /* String that is actually executed */
	char *homeDir;
	char *curDir;

	int validRequest;
	int debugExtLen;
	int cmdLen;
	int tempLength;

/* Determine if debugging output should be done */
#ifdef DEBUG_BY_NAME
	cmdLen = strlen(argv[0]);
	debugExtLen = strlen(DEBUG_EXTENSION);
	
	if ( debugExtLen <= cmdLen )	
	{
		tempLength = cmdLen - debugExtLen;
		DEBUG = !strcmp(argv[0]+tempLength, DEBUG_EXTENSION);
	}	
#endif

#ifdef FORCEDEBUG
	DEBUG = 1;
#endif	


	DEBUG_Msg("HTTP/1.0 200 Ok");
	DEBUG_Msg("Content-Type: text/plain\n\n");

/* Redirect stderr to stdout */
#ifdef REDIR_STDERR
	DEBUG_Msg("\nRedirecting STDERR to STDOUT");
	dup2(1,2);
#endif


	/***/
	/**   Print out the URL used to query this script */
	/***/
	DEBUG_Str("Query_String:", (char *) getenv("QUERY_STRING") );
	DEBUG_Str("Path_Info:", (char *) getenv("PATH_INFO") );
	DEBUG_Str("Remote Host:", (char *) getenv("REMOTE_HOST") );
	DEBUG_Str("Remote Addr:", (char *) getenv("REMOTE_ADDR") );

/* heres where I need to process to get the result from either the
   query string or the extra path info */


	validRequest = 0;
	if ( getenv("PATH_INFO") )  /* use PATH_INFO */
	{
		DEBUG_Msg("\nTrying to extract user/script from PATH_INFO\n");
		ExtractPathInfo(&userStr, &scrStr);		
		
		validRequest = userStr && scrStr;
	}
	
	if ( !validRequest && getenv("QUERY_STRING") )  /* or use QUERY_STRING */
	{
		DEBUG_Msg("\nTrying to extract user/script from QUERY_STRING\n");		

		DEBUG_Msg("\nRead in user keyword value");
		userStr = GetValue("user", getenv("QUERY_STRING") );
	
		DEBUG_Msg("\nRead in script keyword value");
		scrStr = GetValue("script", getenv("QUERY_STRING") );

		validRequest = userStr && scrStr;
	}
	
	
	if ( !validRequest ) /* nothing at all found */
	{
		DoError("No path_info or query string was specified, check your URL.");
	}



	/***/
	/**   Sanitize user name and script name */
	/***/
#ifdef SANITIZE
	DEBUG_Str("\nSanitize user name:", userStr);
	Sanitize(userStr);
	DEBUG_Str(  "      Sanitized to:", userStr);
	DEBUG_Str("Sanitize script name:", scrStr);
	Sanitize(scrStr);
	DEBUG_Str(  "      Sanitized to:", scrStr);
#endif




	/***/
	/**   Log the query request to the log file */
	/***/
#ifdef LOG_REQUESTS
	DEBUG_Msg("\nLog Request");
	Log(userStr, scrStr);
#endif 


	/***/
	/**   Lookup username to get UID */
	/***/
	if ( !(user = getpwnam(userStr)) )
	{
		DoError("User not found.");
	}

	homeDir = mystrcpy(user->pw_dir);

	DEBUG_Msg("\nUser Data Retrieved:");
	DEBUG_Str("   UserName:", user->pw_name);
	DEBUG_Int("   UID:", user->pw_uid);
	DEBUG_Int("   GID:", user->pw_gid);
	DEBUG_Str("   Directory:", homeDir);


	/***/
	/**   Change real&effective UIDs to match username and group of that user */
	/***/
#ifdef SETUID
	setgid( user->pw_gid );
	setuid( user->pw_uid );
#endif

#ifdef SETREUID
	setregid( user->pw_gid, user->pw_gid );
	setreuid( user->pw_uid, user->pw_uid );
#endif

#ifdef SETRESUID
	setresgid( user->pw_gid, user->pw_gid, user->pw_gid );
	setresuid( user->pw_uid, user->pw_uid, user->pw_uid );
#endif


	DEBUG_Msg("\nUIDs/GIDs Changed To:");
	DEBUG_Int("   RUID:", getuid());
	DEBUG_Int("   EUID:", geteuid());
	DEBUG_Int("   RGID:", getgid());
	DEBUG_Int("   EGID:", getegid()); 


	/***/
	/**   Check to make sure the RUID actually changed with the setuid call */
	/***/
#ifdef CHECK_RUID
	if ( getuid() != user->pw_uid ) 
	{
		DoError("UID could not be changed!");
	}
#endif

	/***/
	/**   Check to make sure the RGID actually changed with the setgid call */
	/***/
#ifdef CHECK_RGID
	if ( getgid() != user->pw_gid )
	{
		DoError("GID could not be changed!");
	}
#endif


	/***/
	/**   Change to home directory of that user */
	/***/
	if ( chdir( homeDir ) )
	{
		DoError("Could not chdir to home directory!");
	}


	/***/
	/**   Change to script directory */
	/***/
	if ( chdir( CGIDIR ) )
	{
		DoError("Could not chdir to script directory!");
	}

	curDir = (char *) getcwd(0,200);
	DEBUG_Str("\nCurrent Directory: ", curDir );


	/***/
	/**   Do a stat or something on the script to execute to make sure it is */
	/**   Owned by person who is specified as the username, will prevent */
	/**   sym links to other people scripts */
	/***/

	if ( statErr = stat(scrStr, &fileStat) ) 
	{
		DoError("Could not stat script file!");
	}

	DEBUG_Msg("\nResults of stat:");
	DEBUG_Int("   File Owner:", (int) fileStat.st_uid);
	DEBUG_Int("   File Group:", (int) fileStat.st_gid); 


#ifdef CHECK_UID
	if (fileStat.st_uid != user->pw_uid) 
	{
		DoError("Script does not have same UID");
	}
#endif


#ifdef CHECK_GID
	if (fileStat.st_gid != user->pw_gid)
	{
		DoError("Script does not have same GID");
	}
#endif


#ifdef CHECK_SUID
	if (fileStat.st_mode & S_ISUID)
	{
		DoError("Script is SUID - Will not Execute!");
	}
#endif


#ifdef CHECK_SGID 
	if (fileStat.st_mode & S_ISGID)
	{
		DoError("Script is SGID - Will not Execute!");
	}
#endif



	/***/
	/**   Prepend "./" onto the script string so it can be executed without */
	/**   the current path affecting anything */
	/***/
	execStr = (char *) malloc( (size_t) (strlen(scrStr) + 2) );
	strcpy(execStr, "./");
	strcat(execStr, scrStr);



	/***/
	/**   Exec the script - script will need to be run with /bin/sh so it */
	/**   can start up perl and other things... */
	/***/

	DEBUG_Msg("\n\nOutput of script follows:");
	DEBUG_Msg("=====================================================");

#ifdef USE_SYSTEM
	scriptErr = system(execStr);
#else
	scriptErr = execl(execStr, scrStr);  
#endif


}
