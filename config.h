/***************************************/
/* Main Configuration File for CGIWrap */
/***************************************/

/**/
/**  CGIWrap File and Directory Locations  */
/**  LOGFILE - filename to store log in    */
/**  CGIDIR  - directory for CGI's relative to users home dir  */
/**/

/* #define LOGFILE "cgiwrap.log" /**/
#define LOGFILE "/home/local/testhttp.logs/cgiwrap.log" /**/
#define CGIDIR  "public_html/cgi-bin"

/**/
/**  CGIWrap Security and Compile Options      */
/**  #define to turn on, #undef to turn off    */
/**/


/* Choose which method to run CGI wrap */
/* This has nothing to do with the security of the server, this is only */
/* to choose whichever one of these calls your platform supports */
/* Only define one of these... */
#undef SETREUID  	/* Define if only have setre[ug]id() */
#define SETRESUID	/* Define if only have setres[ug]id() */
#undef SETUID    	/* Define if only have set[ug]id() */
#undef SETUID_SETEUID	/* Define if have set[ug]id() and sete[ug]id() */


/* Security Checks performed to make sure the SET operations worked */
#define CHECK_RUID	/* Double check to make sure real UID changed */
#define CHECK_RGID	/* Double check to make sure real GID changed */
#define CHECK_EUID	/* Double check to make sure effective UID changed */
#define CHECK_EGID	/* Double check to make sure effective GID changed */

/* Set auxilliary groups */
#define SETGROUPS


/* Debugging Output Options*/
/* Set what the name of the executable must end with to trigger */
/* debugging output if DEBUG_BY_NAME is defined */
#undef FORCE_DEBUG     /* Force debugging output on */
#define DEBUG_BY_NAME  /* Debugging output on if correct name */
#define DEBUG_EXTENSION "d"


/* Security Checks Performed on Script Itself */
#define CHECK_UID	/* Error if user UID <> script owner ID */
#undef CHECK_GID	/* Error if user GID <> script group ID */
#define CHECK_SUID	/* Error if SUID bit of script is set */
#define CHECK_SGID	/* Error if SGID bit of script is set */
#define CHECK_MODE	/* Check user exec bit on script */


/* Do not allow subdirectories of cgi-bin dir */
#define NO_SUB_DIRS


/* Log all requests coming into this script */
#define LOG_REQUESTS


/* Sanitize Directory and User Names  */
#define SANITIZE


/* Use system() call instead of exec to run script */
#undef USE_SYSTEM


/* Redirect STDERR to STDOUT before executing script - this way, 
   if running in debugging mode, you'll see stderr instead of having to look
   at the error_log. */
#define REDIR_STDERR


/* Set rlimit to limit CPU usage */
#undef USE_RLIMIT

#ifdef USE_RLIMIT
#define RLIMIT_SETTING { 9, 10 }
#endif
