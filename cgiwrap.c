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
int main (int argc, char *argv[])
{
	char *userStr; /* User name */
	char *scrStr; /* Name of script */
	char *scriptPath; /* Path to script file */
	char *cgiBaseDir; /* Base directory for cgi scripts in user's dir */
	struct passwd *user; /* For getting uid from name */
#ifdef CONF_REPORT_RUSAGE
	pid_t childpid;
	struct rusage childrusage;
	int childstatus;
	char rusagemsg[1000];
	time_t stime;
	time_t etime;
	time_t elap_time;
#endif

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
	if (dup2(1,2) == -1) 
	{
		MSG_Error_SystemError("Call to dup2() to redirect stderr to stdout failed!");
	}
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
	SetScriptFilename( scriptPath );
	SetPathTranslated( scriptPath );

	/* Output the modified environment variables */
	OutputEnvironment();

#if !defined(CONF_REPORT_RUSAGE)
	/* Log the query request to the log file */
	Log(userStr, scrStr, "ok");
#endif

	/* Once log is initialized, we can chroot */
#if defined(CONF_CHROOT_PREFIX)
	DEBUG_Str("Attempting to chroot: ", CONF_CHROOT_PREFIX);
	if ( chdir(CONF_CHROOT_PREFIX) == -1 )
	{
		MSG_Error_SystemError("Failed to chdir for chrooting!");
	}
	if ( chroot(CONF_CHROOT_PREFIX) == -1 )
	{
		MSG_Error_SystemError("Failed to chroot!");
	}
#endif

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
	
#if 0
	/* disabled, now using F_SETFD to set close-on-exec */
	/* Close the log files - some thought to doing this below 
		so could log failure */	
	LogEnd();
#endif

	/* Execute the script */
	DEBUG_Msg("\n\n");
	DEBUG_Msg("Output of script follows:");
	DEBUG_Msg("=====================================================");

#if defined(CONF_REPORT_RUSAGE) && defined(HAS_WAIT3)
	stime = time(NULL);
	childpid = fork();
	if ( childpid < 0 ) /* fork failed */
	{
		Log(userStr, scrStr, "fork failed");
		exit(1);
	}
	else if ( childpid == 0 )
	{
		execv(scriptPath, CreateARGV(scrStr, argc,argv));
		MSG_Error_ExecFailed();
		Log(userStr, scrStr, "failed execv of script");
		exit(1);
	}
	else /* fork ok */
	{
		wait3(&childstatus, 0, &childrusage);
		etime = time(NULL);
		elap_time = etime-stime;
		sprintf(rusagemsg, 
			"status=%d wtime='%lds' utime='%ds %dus' stime='%ds %dus'",
			WEXITSTATUS(childstatus), 
			elap_time,
			childrusage.ru_utime.tv_sec, 
			childrusage.ru_utime.tv_usec,
			childrusage.ru_stime.tv_sec, 
			childrusage.ru_stime.tv_usec);
		Log(userStr, scrStr, rusagemsg);
		exit(WEXITSTATUS(childstatus));
	}
#else
	execv(scriptPath, CreateARGV(scrStr, argc,argv));
	MSG_Error_ExecFailed();
	exit(1);
#endif
}
