/* 
 * httpd user check
 */
#undef CONF_CHECK_HTTPD_USER
#undef CONF_HTTPD_USER 

/*
 * Chroot prefix, requires significant infrastructure to use
 */
#undef CONF_CHROOT_PREFIX

/*
 * Informational definitions
 */
#undef CONF_LOCAL_SITE_URL
#undef CONF_LOCAL_DOC_URL
#undef CONF_LOCAL_INFO_ENABLED
#undef CONF_LOCAL_CONTACT_NAME
#undef CONF_LOCAL_CONTACT_EMAIL
#undef CONF_LOCAL_CONTACT_PHONE
#undef CONF_LOCAL_CONTACT_URL

/* 
 * Environment variable setting
 */
#undef CONF_SETENV_ANY
#undef CONF_SETENV_PATH
#undef CONF_SETENV_TZ

/*
 * Script checks
 */
#undef CONF_STRICT_NAMES
#undef CONF_CHECK_SCRUID
#undef CONF_CHECK_SCRGID
#undef CONF_CHECK_SCRSUID
#undef CONF_CHECK_SCRSGID
#undef CONF_CHECK_SCRGWRITE
#undef CONF_CHECK_SCROWRITE
#undef CONF_CHECK_SYMLINK

/*
 * Logging options
 */
#undef CONF_LOG_USEFILE
#undef CONF_LOG_USESYSLOG
#undef CONF_LOG_LOGFILE
#undef CONF_LOG_LABEL

/*
 * Function availability
 */
#undef HAS_SIGSET
#undef HAS_INITGROUPS
#undef HAS_SETGROUPS
#undef HAS_GETGROUPS
#undef HAS_SETGID
#undef HAS_SETUID
#undef HAS_SETEGID
#undef HAS_SETEUID
#undef HAS_SETRGID
#undef HAS_SETRUID
#undef HAS_SETREGID
#undef HAS_SETREUID
#undef HAS_SETRESGID
#undef HAS_SETRESUID
#undef HAS_PERROR
#undef HAS_STRDUP
#undef HAS_SYSLOG
#undef HAS_SETRLIMIT
#undef HAS_STRERROR
#undef HAS_PUTENV
#undef HAS_WAIT3

/*
 * Various options
 */
#undef CONF_MINIMUM_UID
#undef CONF_MINIMUM_GID
#undef CONF_REDIR_STDERR
#undef CONF_SUBDIRS
#undef CONF_USERDIRFILE
#undef CONF_INITGROUPS
#undef CONF_SETGROUPS
#undef CONF_ALLOWFILE
#undef CONF_DENYFILE
#undef CONF_CHECKHOST
#undef CONF_CHECKSHELL
#undef CONF_AFS_SETPAG
#undef CONF_CGIDIR
#undef CONF_MULTIUSER_CGI_DIR

/*
 * RLimit Options
 */
#undef CONF_USE_RLIMIT_ANY
#undef CONF_USE_RLIMIT_AS
#undef CONF_USE_RLIMIT_CPU
#undef CONF_USE_RLIMIT_VMEM
#undef CONF_USE_RLIMIT_FSIZE
#undef CONF_USE_RLIMIT_DATA
#undef CONF_USE_RLIMIT_STACK
#undef CONF_USE_RLIMIT_CORE
#undef CONF_USE_RLIMIT_RSS
#undef CONF_USE_RLIMIT_NPROC
#undef CONF_USE_RLIMIT_NOFILE
#undef CONF_USE_RLIMIT_MEMLOCK

/*
 * Support for reporting rusage/return status
 */
#undef CONF_REPORT_RUSAGE

/*
 * OS Specific Definitions
 */
#undef _HPUX_SOURCE

/*
 * PHP support
 */
#undef PATH_PROG_PHP

/*
 * Quiet warning about rcsid
 */
#define RCSID(msg) \
static /**/const char *const rcsid[] = { (char *)rcsid, "\100(#)" msg }
