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
 * that running config.h.SH again will wipe out any changes you've made.
 * For a more permanent change edit config.sh and rerun config.h.SH.
 *
 * \$Id$
 */

/* Configuration time: $cf_time
 * Configured by: $cf_by
 * Target system: $myuname
 */

#ifndef _config_h_
#define _config_h_

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

/* HAS_STRERROR:
 *	This symbol, if defined, indicates that the strerror routine is
 *	available to translate error numbers to strings.
 */
/* HAS_SYS_ERRLIST:
 *	This symbol, if defined, indicates that the sys_errlist array is
 *	available to translate error numbers to strings. The extern int
 *	sys_nerr gives the size of that table.
 */
/* strerror:
 *	This preprocessor symbol is defined as a macro if strerror() is
 *	not available to translate error numbers to strings but sys_errlist[]
 *	array is there.
 */
#$d_strerror HAS_STRERROR		/**/
#$d_syserrlst HAS_SYS_ERRLIST	/**/
#$d_strerrm strerror(e) ((e)<0||(e)>=sys_nerr?"unknown":sys_errlist[e]) /**/

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

/* CAN_PROTOTYPE:
 *	If defined, this macro indicates that the C compiler can handle
 *	function prototypes.
 */
/* DOTS:
 *	This macro is used to specify the ... in function prototypes which
 *	have arbitrary additional arguments.
 */
/* NXT_ARG:
 *	This macro is used to separate arguments in the declared argument list.
 */
/* P_FUNC:
 *	This macro is used to declare "private" (static) functions.
 *	It takes three arguments: the function type and name, a parenthesized
 *	traditional (comma separated) argument list, and the declared argument
 *	list (in which arguments are separated with NXT_ARG, and additional
 *	arbitrary arguments are specified with DOTS).  For example:
 *
 *		P_FUNC(int foo, (bar, baz), int bar NXT_ARG char *baz[])
 */
/* P_FUNC_VOID:
 *	This macro is used to declare "private" (static) functions that have
 *	no arguments.  The macro takes one argument: the function type and name.
 *	For example:
 *
 *		P_FUNC_VOID(int subr)
 */
/* V_FUNC:
 *	This macro is used to declare "public" (non-static) functions.
 *	It takes three arguments: the function type and name, a parenthesized
 *	traditional (comma separated) argument list, and the declared argument
 *	list (in which arguments are separated with NXT_ARG, and additional
 *	arbitrary arguments are specified with DOTS).  For example:
 *
 *		V_FUNC(int main, (argc, argv), int argc NXT_ARG char *argv[])
 */
/* V_FUNC_VOID:
 *	This macro is used to declare "public" (non-static) functions that have
 *	no arguments.  The macro takes one argument: the function type and name.
 *	For example:
 *
 *		V_FUNC_VOID(int fork)
 */
/* _:
 *	This macro is used to declare function parameters for folks who want
 *	to make declarations with prototypes using a different style than
 *	the above macros.  Use double parentheses.  For example:
 *
 *		int main _((int argc, char *argv[]));
 */
#$prototype	CAN_PROTOTYPE	/**/
#ifdef CAN_PROTOTYPE
#define	NXT_ARG ,
#define	DOTS , ...
#define	V_FUNC(name, arglist, args)name(args)
#define	P_FUNC(name, arglist, args)static name(args)
#define	V_FUNC_VOID(name)name(void)
#define	P_FUNC_VOID(name)static name(void)
#define	_(args) args
#else
#define	NXT_ARG ;
#define	DOTS
#define	V_FUNC(name, arglist, args)name arglist args;
#define	P_FUNC(name, arglist, args)static name arglist args;
#define	V_FUNC_VOID(name)name()
#define	P_FUNC_VOID(name)static name()
#define	_(args) ()
#endif

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

/* CONF_AFS_SETPAG:
 *	Defined if the cgiwrap should issue the AFS setpag() call..
 */
#$d_conf_afs_setpag CONF_AFS_SETPAG		/**/

/* CONF_CGIDIR:
 *	Relative path from the users home dir to their cgi directory, 
 *	for example, "public_html/cgi-bin" or "public_html/auth-cgi-bin".
 */
#define CONF_CGIDIR "$conf_cgidir"		/**/

/* CONF_CHECK_GID:
 *	If defined, cgiwrap will verify that the real and effective
 *	GID actually changed.
 */
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
/* CONF_CHECK_SCRMODE:
 *	If defined, cgiwrap will verify that the script is executable.
 */
#$d_conf_check_gid CONF_CHECK_GID		/**/
#$d_conf_check_scruid CONF_CHECK_SCRUID		/**/
#$d_conf_check_scrgid CONF_CHECK_SCRGID		/**/
#$d_conf_check_scrsuid CONF_CHECK_SCRSUID		/**/
#$d_conf_check_scrsgid CONF_CHECK_SCRSGID		/**/
#$d_conf_check_scrmode CONF_CHECK_SCRMODE		/**/

/* CONF_HTTPD_USER:
 *	This symbol contains the userid that the web server runs as.
 */
/* CONF_CHECK_HTTPD_USER:
 *	This symbol contains the userid that the web server runs as.
 */
#$d_check_httpd_user CONF_CHECK_HTTPD_USER		/**/
#define CONF_HTTPD_USER "$httpd_user"		/**/

/* CONF_LOGFILE:
 *	Defined to be the filename that cgiwrap logs requests to.
 */
/* CONF_LOG_REQUESTS:
 *	Defined this if cgiwrap should log all requests to a file.
 */
#$d_log_requests CONF_LOG_REQUESTS		/**/
#define CONF_LOGFILE "$log_file"		/**/

/* CONF_NOSUBDIRS:
 *	Defined if you don't want to allow users to store cgi scripts
 *	in a subdirectory under their cgi-bin directory.
 */
#$d_conf_nosubdirs CONF_NOSUBDIRS		/**/

/* CONF_REDIR_STDERR:
 *	Sgould cgiwrap redirect stderr from scripts to stdout
 */
#$d_conf_redir_stderr CONF_REDIR_STDERR		/**/

/* CONF_SETGROUPS:
 *	Defined if you want cgiwrap to set the auxilliary groups for the 
 *	script process.
 */
#$d_conf_setgroups CONF_SETGROUPS		/**/

/* CONF_USE_RLIMIT:
 *	Defined if you want cgiwrap to issue a set_rlimit call to limit
 *	the cgi script's usage of resources.
 */
#$d_conf_use_rlimit CONF_USE_RLIMIT		/**/

/* CONF_USE_SYSTEM:
 *	Defined this if you want to execute script via system() instead
 *	of via exec().
 */
#$d_conf_use_system CONF_USE_SYSTEM		/**/

/* HAS_INITGROUPS:
 *	If defined, this indicates that the system has the initgroups call.
 */
#$d_initgroups HAS_INITGROUPS		/**/

/* HAS_SETGROUPS:
 *	If defined, this indicates that the system has the setgroups call.
 */
#$d_setgroups HAS_SETGROUPS		/**/

#endif
!GROK!THIS!
