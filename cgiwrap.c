/**
 **  File: cgiwrap.c
 **  Purpose: Main program for cgiwrap
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/*
 * Main program
 */
void main (int argc, char *argv[])
{
	char *userStr; /* User name */
	char *scrStr; /* Name of script */
	char *scriptPath; /* Path to script file */
	struct passwd *user; /* For getting uid from name */

	/* Determine if debugging output should be done */
	if ( strlen(argv[0]) >= 1 )
	{
		CONF_DEBUG = !strcmp( argv[0] + strlen(argv[0]) - 1, "d");
	}

	/* Output a status header if running in NoParseHeaders mode */
	if ( !strncmp(argv[0], "nph-", 4) )
	{
		DEBUG_Msg("HTTP/1.0 200 Ok");
	}
	
	/* Output a Content-type header if in debugging mode */
	if ( CONF_DEBUG )
	{
		SendHeader("text/plain");
		printf("\n");
	}

	/* Redirect stderr to stdout */
#if defined(CONF_REDIR_STDERR)
	DEBUG_Msg("\nRedirecting STDERR to STDOUT");
	dup2(1,2);
#endif

	/* Check who is running this script */
	VerifyExecutingUser();

	/* Set any default signal behavior */
	SetSignals();

	/* Set CPU and other limits */
	SetResourceLimits();

	/* Output the contents of important environment variables */
	OutputEnvironment();
	
	/* Get the user name from the given data */
	userStr = FetchUserString();
	DEBUG_Str("Retrieved User Name", userStr);

	/* Now, get whatever information that is available about that */
	/* user - fetch this information from the passwd file or NIS */
	if ( !(user = getpwnam(userStr)) )
	{
		DoError("User not found in passwd file.");
	}

	DEBUG_Msg("User Data Retrieved:");
	DEBUG_Str("   UserID:", user->pw_name);
	DEBUG_Int("   UID:", user->pw_uid);
	DEBUG_Int("   GID:", user->pw_gid);
	DEBUG_Str("   Home Dir:", user->pw_dir);

	/* Perform checks to make sure this user is allow to use CGI scripts */
	CheckUser(user);

	/* Get the script name from the given data */
	scrStr = FetchScriptString(user);
	scriptPath = BuildScriptPath(user,scrStr);

	DEBUG_Str("Retrieved Script Name", scrStr);
	DEBUG_Str("Retrieved Script Path", scriptPath);

	/* Set the Correct Values of SCRIPT_NAME envir. var. */
	SetScriptName(userStr, scrStr);
	
	/* Output the modified environment variables */
	OutputEnvironment();

	/* Log the query request to the log file */
	Log(userStr, scrStr, "ok");

	/* Change auxilliary groups to match this user */
	ChangeAuxGroups(user);
	
	/* Change real and effective user and group id's to match this user */
	ChangeID(user);

	/* Change to the user's cgi-bin directory */
	ChangeToCgiDir(user);

	/* Check to see if ok to execute script file */
	CheckScriptFile(user, scriptPath);

	/* Perform any AFS related tasks before executing script */
	Create_AFS_PAG();
	
	/* Execute the script */
	DEBUG_Msg("\n\n");
	DEBUG_Msg("Output of script follows:");
	DEBUG_Msg("=====================================================");

#if defined(CONF_USE_SYSTEM)
#if defined(HAS_SYSTEM)
	system(scriptPath);
#else
	DoError("Configuration Error: system() call not available");
#endif
#else
	execv(scriptPath, CreateARGV(scrStr, argc,argv));
#endif

}
