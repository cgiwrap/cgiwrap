/***************************************/
/* Main Configuration File for CGIWrap */
/***************************************/

/**/
/**  CGIWrap File and Directory Locations  */
/**  LOGFILE - filename to store log in    */
/**  CGIDIR  - directory for CGI's relative to users home dir  */
/**/

/* #define LOGFILE "cgiwrap.log" /**/
#define LOGFILE "/usr/adm/cgiwrap.log" /**/
#define CGIDIR  "public_html/cgi-bin"

/**/
/**  CGIWrap Security and Compile Options      */
/**  #define to turn on, #undef to turn off    */
/**/


/* Choose which method to run CGI wrap */
#undef SETUID    	/* Server run as root */
#define SETREUID  	/* Server run as other, cgiwrap suid root, A/UX */
#undef SETRESUID	/* Server run as other, cgiwrap suid root, HPUX */


/* Debugging Output */
#undef FORCE_DEBUG     /* Force debugging output on */
#define DEBUG_BY_NAME  /* Debugging output on if correct name */


/* Set what the name of the executable must end with to trigger */
/* debugging output if DEBUG_BY_NAME is defined */
#define DEBUG_EXTENSION "d"


/* Error if user UID <> script owner ID */
#define CHECK_UID


/* Error if user GID <> script group ID */
#define CHECK_GID


/* Error if SUID bit of script is set */
#define CHECK_SUID


/* Error if SGID bit of script is set */
#define CHECK_SGID


/* Do not allow subdirectories of cgi-bin dir */
#define NO_SUB_DIRS


/* Log all requests coming into this script */
#define LOG_REQUESTS


/* Sanitize Directory and User Names  */
#define SANITIZE


/* Double check to make sure UID changed */
#define CHECK_RUID


/* Double check to make sure GID changed */
#define CHECK_RGID


/* Use system() call instead of exec to run script */
#undef USE_SYSTEM


/* Redirect STDERR to STDOUT before executing script - this way, 
   if running in debugging mode, you'll see stderr instead of having to look
   at the error_log. */
#define REDIR_STDERR


/* Set rlimit to limit CPU usage */
#undef USE_RLIMIT
#define RLIMIT_SETTING { 9, 10 }

/* Set auxilliary groups */
#define SETGROUPS


