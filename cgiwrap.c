static char *rcsid="$Id$";

/**
 **  File: cgiwrap.c
 **  Purpose: Main program for cgiwrap
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/*
 * Global context structure
 */
struct cgiwrap_context Context;

/*
 * Main program
 */
void main (int argc, char *argv[])
{
	char *userStr; /* User name */
	char *scrStr; /* Name of script */
	char *scriptPath; /* Path to script file */
	char *cgiBaseDir; /* Base directory for cgi scripts in user's dir */
	struct passwd *user; /* For getting uid from name */

	/* Determine if debugging output should be done */
	if ( strlen(argv[0]) >= 1 )
	{
		CONF_DEBUG = !strcmp( argv[0] + strlen(argv[0]) - 1, "d");
		if (CONF_DEBUG)
		{
			MSG_HTMLMessages = 0;
		}
	}

	/* Output a status header if running in NoParseHeaders mode */
	if ( !strncmp(argv[0], "nph-", 4) )
	{
		MSG_Need_NPH_Header = 1;
	}
	
	/* Output a Content-type header if in debugging mode */
	if ( CONF_DEBUG )
	{
		MSG_ContentType("text/plain");
	}

	/* Initialize the log */
	LogInit();

	/* Redirect stderr to stdout */
#if defined(CONF_REDIR_STDERR)
	DEBUG_Msg("Redirecting STDERR to STDOUT\n");
	dup2(1,2);
#endif

	/* Check who is running this script */
	VerifyExecutingUser();

	/* Set any default signal behavior */
	SetSignals();

	/* Set Environment Variables */
	SetEnvironmentVariables();

	/* Set CPU and other limits */
	SetResourceLimits();

	/* Output the contents of important environment variables */
	OutputEnvironment();
	
	/* Get the user name from the given data */
	userStr = FetchUserString();
	DEBUG_Str("Retrieved User Name: ", userStr);

	/* Now, get whatever information that is available about that */
	/* user - fetch this information from the passwd file or NIS */
	if ( !(user = getpwnam(userStr)) )
	{
		MSG_Error_NoSuchUser(userStr);
	}
	memcpy(&Context.user, user, sizeof(struct passwd));

	DEBUG_Msg("\nUser Data Retrieved:");
	DEBUG_Str("     UserID:", user->pw_name);
	DEBUG_Int("        UID:", user->pw_uid);
	DEBUG_Int("        GID:", user->pw_gid);
	DEBUG_Str("   Home Dir:", user->pw_dir);

	/* Perform checks to make sure this user is allow to use CGI scripts */
	CheckUser(user);

	/* Determine the base directory where this user's CGI scripts
		are to be stored */
	DEBUG_Msg("");
	cgiBaseDir = GetBaseDirectory(user);	
	DEBUG_Str("Script Base Directory: ", cgiBaseDir);
	if ( !DirExists(cgiBaseDir) )
	{
		MSG_Error_NoScriptDir();
	}

	/* Get the script name from the given data */
	scrStr = FetchScriptString(cgiBaseDir);
	scriptPath = BuildScriptPath(cgiBaseDir,scrStr);
	Context.scriptFullPath = CondenseSlashes(scriptPath);
	Context.scriptRelativePath = CondenseSlashes(scrStr);

	DEBUG_Str("\tScript Relative Path: ", scrStr);
	DEBUG_Str("\tScript Absolute Path: ", scriptPath);

	/* Set the Correct Values of environment variables */
	DEBUG_Msg("\nFixing Environment Variables.");
	SetScriptName(userStr, scrStr);
	SetPathTranslated( scriptPath );
	
	/* Output the modified environment variables */
	OutputEnvironment();

	/* Log the query request to the log file */
	Log(userStr, scrStr, "ok");

	/* Change auxilliary groups to match this user */
	ChangeAuxGroups(user);
	
	/* Change real and effective user and group id's to match this user */
	ChangeID(user);

	/* Change to the user's cgi-bin directory */
	ChangeToCGIDir(scriptPath);

	/* Check to see if ok to execute script file */
	CheckScriptFile();

	/* Perform any AFS related tasks before executing script */
	Create_AFS_PAG();
	
	/* Close the log files - some thought to doing this below 
		so could log failure */	
	LogEnd();

	/* Execute the script */
	DEBUG_Msg("\n\n");
	DEBUG_Msg("Output of script follows:");
	DEBUG_Msg("=====================================================");

	execv(scriptPath, CreateARGV(scrStr, argc,argv));
	MSG_Error_ExecFailed();
}
