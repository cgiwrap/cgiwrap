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
#define SETREUID  	/* Server run as other, cgiwrap suid root */
#undef SETRESUID	/* Server run as other, cgiwrap suid root, hpux */

/* Enable Debugging Output */
/* #undef DEBUG */


/* Error if user UID <> script owner ID */
#undef CHECK_UID


/* Error if user GID <> script group ID */
#undef CHECK_GID


/* Error if SUID bit of script is set */
#define CHECK_SUID


/* Error if SGID bit of script is set */
#define CHECK_SGID


/* Do not allow subdirectories of cgi-bin dir */
#define NO_SUB_DIRS


/* Check to make sure script is not a sym-link (NOT FUNCTIONAL YET) */
#undef NO_SCRIPT_SYMLINK


/* Log all requests coming into this script */
#define LOG_REQUESTS


/* Sanitize Directory and User Names  */
#define SANITIZE


/* Double check to make sure UID changed */
#define CHECK_RUID


/* Double check to make sure GID changed */
#define CHECK_RGID
