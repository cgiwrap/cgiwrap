/**
 **  File: cgiwrap.c
 **  Purpose: Main program for cgiwrap
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

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
	char *interPath = NULL; /* Path to interpreter - i.e. php */
#ifdef CONF_REPORT_RUSAGE
	pid_t childpid;
	struct rusage childrusage;
	int childstatus;
	char rusagemsg[1000];
	time_t stime;
	time_t etime;
	time_t elap_time;
#endif
	
	/* Initially not a multiuser cgi script */
	Context.multiuser_cgi_script = 0;
	Context.interpreted_script = 0;

#ifdef CONF_QUIET_ERRORS
	/* Default to quiet errors only */
	MSG_QuietErrors = 1;
#endif

	/* Determine if debugging output should be done */
	if ( strlen(argv[0]) >= 1 )
	{
		CONF_DEBUG = !strcmp( argv[0] + strlen(argv[0]) - 1, "d");
		if (CONF_DEBUG)
		{
			MSG_QuietErrors = 0;
			MSG_HTMLMessages = 0;
		}
	}

	/* Output a status header if running in NoParseHeaders mode */
	if ( !strncmp(argv[0], "nph-", 4) )
	{
		MSG_Need_NPH_Header = 1;
	}
#if defined(PATH_PROG_PHP)
	else if ( !strncmp(argv[0], "php-", 4) )
	{
		Context.interpreted_script = 1;
		interPath = PATH_PROG_PHP;
	}
#endif
	
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

	/* Save copies of the original values for use later */
	SaveEnvironment();
	
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
	CheckUser(user); /* minuid/mingid/shell */
	CheckUserAccess(user); /* global config files */
	CheckVHostUserAccess(user); /* vhost config files */

	/* Determine the base directory where this user's CGI scripts
		are to be stored */
	DEBUG_Msg("");
	cgiBaseDir = GetBaseDirectory(user);	
	DEBUG_Str("Script Base Directory: ", cgiBaseDir);
#if defined(CONF_MULTIUSER_CGI_DIR)
	DEBUG_Str("MultiUser Script Base Directory: ", CONF_MULTIUSER_CGI_DIR);
	if ( !DirExists(cgiBaseDir) &&
		!DirExists(CONF_MULTIUSER_CGI_DIR) )
#else
	if ( !DirExists(cgiBaseDir) )
#endif
	{
		MSG_Error_NoScriptDir();
	}

	/* Get the script name from the given data */
	DEBUG_Msg("\tFetching script string\n");
	scrStr = FetchScriptString(cgiBaseDir);
	DEBUG_Msg("\tBuilding script path\n");
#ifdef CONF_MULTIUSER_CGI_DIR
	if ( Context.multiuser_cgi_script )
	{
		scriptPath = BuildScriptPath(CONF_MULTIUSER_CGI_DIR,scrStr);
	}
	else
#endif
	{
		scriptPath = BuildScriptPath(cgiBaseDir,scrStr);
	}

	DEBUG_Msg("\tCondensing slashes.\n");
	Context.scriptFullPath = CondenseSlashes(scriptPath);
	Context.scriptRelativePath = CondenseSlashes(scrStr);

	DEBUG_Str("\tScript Relative Path: ", scrStr);
	DEBUG_Str("\tScript Absolute Path: ", scriptPath);

#if defined(CONF_PHP_INTERPRETER) && defined(PATH_PROG_PHP)
	DEBUG_Msg("\tChecking for special interpreted script (php).");
	/* don't double check if already php-cgiwrap */
	if ( ! interPath )
	{

		if ( 
			StringEndsWith(scriptPath, ".php") ||
			StringEndsWith(scriptPath, ".php3") ||
			StringEndsWith(scriptPath, ".php4") ||
			StringEndsWith(scriptPath, ".phtml") )
		{
			Context.interpreted_script = 1;
			interPath = PATH_PROG_PHP;
		}
	}
#endif

	if ( interPath )
	{
		DEBUG_Str("\tInterpreter Path: ", interPath);
	}

	/* Set the Correct Values of environment variables */
	DEBUG_Msg("\nFixing Environment Variables.");
	SetScriptName(userStr, scrStr);
	SetScriptFilename( scriptPath );
#if defined(CONF_MULTIUSER_CGI_DIR)
	if ( Context.multiuser_cgi_script )
	{
		SetPathTranslated( CONF_MULTIUSER_CGI_DIR, scriptPath );
	}
	else
#endif
	{
		SetPathTranslated( cgiBaseDir, scriptPath );
	}

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

	/* Change to the directory the cgi script is in */
	ChangeToCGIDir(scriptPath);

	/* Check to see if ok to execute script file */
	CheckScriptFile();

	/* Check to see if we should force handling with #! line */
#if defined(CONF_PHP_NONEXEC_ONLY)
	if ( Context.interpreted_script && Context.script_is_executable )
	{
		DEBUG_Msg("\tScript is executable, ignoring interpreter and using #! line.\n");
		Context.interpreted_script = 0;
	}
#endif

	/* Perform any AFS related tasks before executing script */
	Create_AFS_PAG();
	
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
		if ( interPath )
		{
			execv(interPath, CreateARGV(scrStr, argc, argv));
		}
		else
		{
			execv(scriptPath, CreateARGV(scrStr, argc, argv));
		}
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
			(int) childrusage.ru_utime.tv_sec, 
			(int) childrusage.ru_utime.tv_usec,
			(int) childrusage.ru_stime.tv_sec, 
			(int) childrusage.ru_stime.tv_usec);
		Log(userStr, scrStr, rusagemsg);
		exit(WEXITSTATUS(childstatus));
	}
#else
	if ( interPath && Context.interpreted_script )
	{
		execv(interPath, CreateARGV(scrStr, argc, argv));
	}
	else
	{
		execv(scriptPath, CreateARGV(scrStr, argc, argv));
	}
	MSG_Error_ExecFailed();
	exit(1);
#endif
}
