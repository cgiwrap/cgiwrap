case $CONFIG in
'')
	if test -f config.sh; then TOP=.;
	elif test -f ../config.sh; then TOP=..;
	elif test -f ../../config.sh; then TOP=../..;
	elif test -f ../../../config.sh; then TOP=../../..;
	elif test -f ../../../../config.sh; then TOP=../../../..;
	else
		echo "Can't find config.sh."; exit 1
	fi
	. $TOP/config.sh
	;;
esac
case "$0" in
*/*) cd `expr X$0 : 'X\(.*\)/'` ;;
esac
echo "Extracting config.h (with variable substitutions)"
sed <<!GROK!THIS! >config.h -e 's!^#undef!/\*#define!' -e 's!^#un-def!#undef!'
/*
 * This file was produced by running the config_h.SH script, which
 * gets its values from config.sh, which is generally produced by
 * running Configure.
 *
 * Feel free to modify any of this as the need arises.  Note, however,
 * that running config_h.SH again will wipe out any changes you've made.
 * For a more permanent change edit config.sh and rerun config_h.SH.
 *
 * \$Id$
 */

/* Configuration time: $cf_time
 * Configured by: $cf_by
 * Target system: $myuname
 */

#ifndef _config_h_
#define _config_h_

/* HAS_PERROR:
 *	This symbol, if defined, indicates that the perror() routine is
 *	available to print system error messages. If not defined, you should
 *	use your own diagnostic routine...
 */
/* perror:
 *	This symbol is maped to null if the  perror() routine is not
 *	available to print system error messages.
 */
#$d_perror HAS_PERROR	/**/
#ifndef HAS_PERROR
#define perror(s)	;		/* mapped to a null statement */
#endif

/* HAS_SETEGID:
 *	This symbol, if defined, indicates that the setegid routine is available
 *	to change the effective gid of the current program.
 */
#$d_setegid HAS_SETEGID		/**/

/* HAS_SETEUID:
 *	This symbol, if defined, indicates that the seteuid routine is available
 *	to change the effective uid of the current program.
 */
#$d_seteuid HAS_SETEUID		/**/

/* HAS_SETREGID:
 *	This symbol, if defined, indicates that the setregid routine is
 *	available to change the real and effective gid of the current
 *	process.
 */
/* HAS_SETRESGID:
 *	This symbol, if defined, indicates that the setresgid routine is
 *	available to change the real, effective and saved gid of the current
 *	process.
 */
#$d_setregid HAS_SETREGID		/**/
#$d_setresgid HAS_SETRESGID		/**/

/* HAS_SETREUID:
 *	This symbol, if defined, indicates that the setreuid routine is
 *	available to change the real and effective uid of the current
 *	process.
 */
/* HAS_SETRESUID:
 *	This symbol, if defined, indicates that the setresuid routine is
 *	available to change the real, effective and saved uid of the current
 *	process.
 */
#$d_setreuid HAS_SETREUID		/**/
#$d_setresuid HAS_SETRESUID		/**/

/* HAS_SETRGID:
 *	This symbol, if defined, indicates that the setrgid routine is available
 *	to change the real gid of the current program.
 */
#$d_setrgid HAS_SETRGID		/**/

/* HAS_SETRUID:
 *	This symbol, if defined, indicates that the setruid routine is available
 *	to change the real uid of the current program.
 */
#$d_setruid HAS_SETRUID		/**/

/* HAS_STRDUP:
 *	This symbol, if defined, indicates that the strdup routine is
 *	available to duplicate strings in memory. Otherwise, roll up
 *	your own...
 */
#$d_strdup HAS_STRDUP		/**/

/* HAS_STRERROR:
 *	This symbol, if defined, indicates that the strerror routine is
 *	available to translate error numbers to strings. See the writeup
 *	of Strerror() in this file before you try to define your own.
 */
#$d_strerror HAS_STRERROR		/**/

/* HAS_SYSLOG:
 *	This symbol, if defined, indicates that the program can rely on the
 *	system providing syslog().  Otherwise, the syslog code provided by
 *	the package should be used.
 */
#$d_syslog HAS_SYSLOG	/**/

/* HAS_SYSTEM:
 *	This symbol, if defined, indicates that the system routine is
 *	available to issue a shell command.
 */
#$d_system HAS_SYSTEM	/**/

/* I_LIMITS:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <limits.h> to get definition of symbols like WORD_BIT or
 *	LONG_MAX, i.e. machine dependant limitations.
 */
#$i_limits I_LIMITS		/**/

/* I_PWD:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <pwd.h>.
 */
#$i_pwd I_PWD		/**/

/* I_STDLIB:
 *	This symbol, if defined, indicates that <stdlib.h> exists and should
 *	be included.
 */
#$i_stdlib I_STDLIB		/**/

/* I_STRING:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <string.h> (USG systems) instead of <strings.h> (BSD systems).
 */
#$i_string I_STRING		/**/

/* I_SYS_RESOURCE:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/resource.h>.
 */
#$i_sysresrc I_SYS_RESOURCE		/**/

/* I_SYS_TYPES:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/types.h>.
 */
#$i_systypes	I_SYS_TYPES		/**/

/* I_SYS_TIME:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <sys/time.h>.
 */
#$i_systime I_SYS_TIME		/**/

/* I_UNISTD:
 *	This symbol, if defined, indicates to the C program that it should
 *	include <unistd.h>.
 */
#$i_unistd I_UNISTD		/**/

/* VOIDFLAGS:
 *	This symbol indicates how much support of the void type is given by this
 *	compiler.  What various bits mean:
 *
 *	    1 = supports declaration of void
 *	    2 = supports arrays of pointers to functions returning void
 *	    4 = supports comparisons between pointers to void functions and
 *		    addresses of void functions
 *	    8 = suports declaration of generic void pointers
 *
 *	The package designer should define VOIDUSED to indicate the requirements
 *	of the package.  This can be done either by #defining VOIDUSED before
 *	including config.h, or by defining defvoidused in Myinit.U.  If the
 *	latter approach is taken, only those flags will be tested.  If the
 *	level of void support necessary is not present, defines void to int.
 */
#ifndef VOIDUSED
#define VOIDUSED $defvoidused
#endif
#define VOIDFLAGS $voidflags
#if (VOIDFLAGS & VOIDUSED) != VOIDUSED
#define void int		/* is void to be avoided? */
#define M_VOID			/* Xenix strikes again */
#endif

/* CONF_ALLOWFILE:
 *	Defined to be the filename that cgiwrap checks to see what users are
 *	allowed to use cgiwrap scripts.
 */
/* CONF_DENYFILE:
 *	Defined to be the filename that cgiwrap checks to see what users are
 *	not allowed to use cgiwrap scripts.
 */
/* CONF_ACCESS:
 *	Defined if you have enabled access control checking of the allow/deny
 *	files.
 */
/* CONF_CHECKHOST:
 *	Defined if you have enabled host address checking in the allow/deny
 *	files.
 */
#$d_conf_access CONF_ALLOWFILE "$allowfile"		/**/
#$d_conf_access CONF_DENYFILE "$denyfile"		/**/
#$d_conf_access CONF_ACCESS	/**/
#$d_conf_checkhost CONF_CHECKHOST	/**/

/* CONF_AFS_SETPAG:
 *	Defined if the cgiwrap should issue the AFS setpag() call..
 */
#$d_conf_afs_setpag CONF_AFS_SETPAG		/**/

/* CONF_SETGROUPS:
 *	Defined if you want cgiwrap to clear the auxilliary groups for the 
 *	script process.
 */
/* CONF_INITGROUPS:
 *	Defined if you want cgiwrap to set the auxilliary groups for the 
 *	script process.
 */
#$d_conf_setgroups CONF_SETGROUPS		/**/
#$d_conf_initgroups CONF_INITGROUPS		/**/

/* CONF_CGIDIR:
 *	Relative path from the users home dir to their cgi directory, 
 *	for example, "public_html/cgi-bin" or "public_html/auth-cgi-bin".
 */
#define CONF_CGIDIR "$conf_cgidir"		/**/

/* CONF_CHECK_SCRUID:
 *	If defined, cgiwrap will verify that owner of the script matches
 *	the UID it switched to.
 */
/* CONF_CHECK_SCRGID:
 *	If defined, cgiwrap will verify that group of the script matches
 *	the GID it switched to.
 */
/* CONF_CHECK_SCRSUID:
 *	If defined, cgiwrap will verify that the script is not set-uid
 */
/* CONF_CHECK_SCRSGID:
 *	If defined, cgiwrap will verify that the script is not set-gid
 */
/* CONF_CHECK_SCRGWRITE:
 *	If defined, cgiwrap will verify that the script is not group writable
 */
/* CONF_CHECK_SYMLINK:
 *	If defined, cgiwrap will verify that the script is not a symlink
 */
#$d_conf_check_scruid CONF_CHECK_SCRUID		/**/
#$d_conf_check_scrgid CONF_CHECK_SCRGID		/**/
#$d_conf_check_scrsuid CONF_CHECK_SCRSUID		/**/
#$d_conf_check_scrsgid CONF_CHECK_SCRSGID		/**/
#$d_conf_check_scrgwrite CONF_CHECK_SCRGWRITE	/**/
#$d_conf_check_symlink CONF_CHECK_SYMLINK		/**/

/* CONF_FIXED_PATHTRANS:
 *	Defined if you want cgiwrap to set the correct PATH_TRANSLATED
 *	environment variable, as opposed to the incorrect value that has
 *	been getting set for several versions.
 */
#$d_conf_fixed_pathtrans CONF_FIXED_PATHTRANS		/**/

/* CONF_HTTPD_USER:
 *	This symbol contains the userid that the web server runs as.
 */
/* CONF_CHECK_HTTPD_USER:
 *	This symbol contains the userid that the web server runs as.
 */
#$d_check_httpd_user CONF_CHECK_HTTPD_USER		/**/
#define CONF_HTTPD_USER "$httpd_user"		/**/

/* CONF_LOG_LOGFILE:
 *	Defined to be the filename that cgiwrap logs requests to.
 */
/* CONF_LOG_LABEL:
 *	Defined to be the label that identifies this copy of cgiwrap.
 */
/* CONF_LOG_USEFILE:
 *	Defined if cgiwrap will log to a file.
 */
/* CONF_LOG_USESYSLOG:
 *	Defined if cgiwrap will log to syslog..
 */
#$d_conf_log_usefile CONF_LOG_USEFILE		/**/
#$d_conf_log_usesyslog CONF_LOG_USESYSLOG		/**/
#define CONF_LOG_LOGFILE "$conf_log_logfile"		/**/
#define CONF_LOG_LABEL "$conf_log_label"		/**/
/* CONF_SUBDIRS:
 *	Defined if you want to allow users to store cgi scripts
 *	in subdirectories under their cgi-bin directory.
 */
#$d_conf_subdirs CONF_SUBDIRS		/**/

/* CONF_REDIR_STDERR:
 *	Sgould cgiwrap redirect stderr from scripts to stdout
 */
#$d_conf_redir_stderr CONF_REDIR_STDERR		/**/

/* CONF_USE_RLIMIT_CPU:
 *	Defined if you want cgiwrap to issue a set_rlimit call to limit
 *	the cgi script's usage of cpu resources.
 */
/* CONF_USE_RLIMIT_VMEM:
 *	Defined if you want cgiwrap to issue a set_rlimit call to limit
 *	the cgi script's usage of virtual memory resources.
 */
#$d_conf_use_rlimit_cpu CONF_USE_RLIMIT_CPU	/**/
#$d_conf_use_rlimit_vmem CONF_USE_RLIMIT_VMEM	/**/

/* CONF_USE_SYSTEM:
 *	Defined this if you want to execute script via system() instead
 *	of via exec().
 */
#$d_conf_use_system CONF_USE_SYSTEM		/**/

/* CONF_USERDIRFILE:
 *	Defined to be the filename that cgiwrap processes to rewrite user 
 *	directory paths.
 */
#$d_conf_userdirfile CONF_USERDIRFILE "$userdirfile"		/**/

/* HAS_INITGROUPS:
 *	If defined, this indicates that the system has the initgroups call.
 */
#$d_initgroups HAS_INITGROUPS		/**/

/* HAS_SETGID:
 *	This symbol, if defined, indicates that the setgid routine is available
 *	to change the real gid of the current program.
 */
#$d_setgid HAS_SETGID		/**/

/* HAS_SETGROUPS:
 *	If defined, this indicates that the system has the setgroups call.
 */
#$d_setgroups HAS_SETGROUPS		/**/

/* HAS_SETUID:
 *	This symbol, if defined, indicates that the setuid routine is available
 *	to change the real uid of the current program.
 */
#$d_setuid HAS_SETUID		/**/

/* HAS_SIGSET:
 *	This symbol, if defined, indicates that the sigset routine is available.
 */
#$d_sigset HAS_SIGSET		/**/

#endif
!GROK!THIS!
