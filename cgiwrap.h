/* rcsid: $Id$ */

#include "config.h"		/* Generated by Configure */

#include <stdio.h>		/* For passing data into called script */

#if defined(HAVE_CTYPE_H)
#include <ctype.h> 
#endif

#if defined(HAVE_SYS_STAT_H)
#include <sys/stat.h>		/* For file stating */
#endif

#if defined(HAVE_SIGNAL_H)
#include <signal.h>		/* For signals stuff */
#endif

#if defined(HAVE_SYS_SIGNAL_H)
#include <sys/signal.h>		/* For signals stuff */
#endif

#if defined(HAVE_ERRNO_H)
#include <errno.h>      /* Error numbers for functions */
#endif

#if defined(HAVE_SYS_ERRNO_H)
#include <sys/errno.h>      /* Error numbers for functions */
#endif

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>	/* Standard library functions */
#endif

#if defined(HAVE_PWD_H)
#include <pwd.h>	/* For getting uids from username */
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>	/* Stuff for setting uid      */
#endif

#if defined(HAVE_SYS_TYPES_H)
#include <sys/types.h>	/* Types for uid and stuff */
#endif

#if defined(HAVE_LIMITS_H)
#include <limits.h>	/* Limit on directory length and stuff */
#endif

#if defined(HAVE_STRING_H)		/* string routines */
#include <string.h>
#elif defined(HAVE_STRINGS_H) 
#include <strings.h>
#endif

#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>	/* Time for logging stuff */
#endif

#if defined(HAVE_TIME_H)
#include <time.h>	/* Time for logging stuff */
#endif

#if defined(HAVE_SYS_RESOURCE_H)	/* For rlimit use */
#include <sys/resource.h>
#endif

#if defined(HAVE_SYSLOG_H)
#include <syslog.h>
#endif

#if defined(HAVE_FCNTL_H)
#include <fcntl.h>
#endif

#if defined(HAVE_GRP_H)
#include <grp.h>
#endif

#if defined(HAVE_SYS_WAIT_H) && defined(CONF_REPORT_RUSAGE)
#include <sys/wait.h>
#endif

#if defined(HUGE_STRING_LEN)
#undef HUGE_STRING_LEN
#endif
#define HUGE_STRING_LEN 2500

/*
 * Global context structure
 */
struct cgiwrap_context
{
	/* User data retrieved from password file */
	struct passwd user;

	/* Path of script relative to file system root */
	char *scriptFullPath;
	
	/* Path of script relative to user cgi directory */
	char *scriptRelativePath;

	/* Previously opened log file */
	FILE *logFile;

	/* Is this script executing out of the multiuser cgi dir */
	int multiuser_cgi_script;
};
extern struct cgiwrap_context Context;

/*
 * Local headers for prototypes and defines
 */ 
#include "util.h"
#include "debug.h"
#include "fetch.h"
#include "stdutil.h"
#include "msgs.h"
