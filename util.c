/**
 **  File: util.c
 **  Purpose: Various utility routines used by cgiwrap
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

/*
 * Encode string to protect against cross-site scripting, but only
 * encode if we are outputting in HTML. Otherwise send as is so that
 * error message shows up as transmitted.
 *
 * Strings is returned in malloced space, but should be treated as if
 * it was allocated staticly.
 */
char *HTMLEncode(char *what)
{
	static char *res = NULL;
	static int len = 0;
	int i,j;

	/* passed a null string, so return same */
	if ( !what )
	{
		return NULL;	
	}

	/* if we are in plaintext mode, just return string as is */
	if ( ! MSG_HTMLMessages )
	{
		return what;
	}

	/* deallocate previous mem if not sufficient for current string */
	/* largest encoding is &amp; so *5 should be sufficient. */
	if ( res && ((strlen(what)*5+1) > len) )
	{
		free(res);
		res = NULL;
		len = 0;
	}

	/* allocate mem */
	if ( ! res )
	{
		/* default to HUGE_STRING_LEN to eliminate excess mallocing */
		len = (strlen(what)*5+1);
		if ( len < HUGE_STRING_LEN )
		{
			len = HUGE_STRING_LEN;
		}
		res = SafeMalloc(len, "SafeEncode");
	}

	/* copy string to result, encoding as we go */
	j = 0;
	for ( i = 0; i<=strlen(what); i++ )
	{
		if ( what[i] == '<' )
		{
			res[j++] = '&';
			res[j++] = 'l';
			res[j++] = 't';
			res[j++] = ';';
		}
		else if ( what[i] == '>' )
		{
			res[j++] = '&';
			res[j++] = 'g';
			res[j++] = 't';
			res[j++] = ';';
		}
		else if ( what[i] == '&' )
		{
			res[j++] = '&';
			res[j++] = 'a';
			res[j++] = 'm';
			res[j++] = 'p';
			res[j++] = ';';
		}
		else
		{
			res[j++] = what[i];
		}
	}
	res[j] = '\0';

	return res;
}

/*
 * Safe malloc
 */
char *SafeMalloc(size_t size, char *what)
{
	char *tmp;
	
	tmp = (char *) malloc( size );
	if ( !tmp )
	{
		char msg[500];
		sprintf(msg, "Couldn't malloc() (%d) bytes for (%s).\n",
			size, what);
		MSG_Error_SystemError(msg);
	}
	return tmp;
}

/*
 * Safe putenv
 */
void SafePutenv(char * string, char *what)
{
	if (putenv(string) != 0) 
	{
		char msg[500];
		sprintf(msg, "Malloc failure caused putenv() failure for (%s).\n", what);
		MSG_Error_SystemError(msg);
	}
}

/*
 * Build the ARGV array for passing to the called script
 */
char **CreateARGV( char *scrStr, int argc, char *argv[])
{
	char **temp;
	int i;

	temp = (char **) SafeMalloc( (argc+1) * sizeof(char *), "ARGV array");
	
	temp[0] = StripPathComponents( CountSubDirs(scrStr), scrStr );
	temp[argc] = 0;

	for (i=1; i<argc; i++)
	{
		temp[i] = argv[i];
	}

	return temp;
}


/*
 *   Extract and return the value portion of a key=value pair found in a string
 */
char *GetValue( char *keyword, char *string )
{
	char *returnStr;
	char *dataStr;
	char *tempStr;
	char *theString;

	theString = strdup(string);

	returnStr = (char *) 0;
	tempStr = (char *) strtok(theString, "&");
	while (tempStr)
	{
		dataStr = (char *) strstr(tempStr, keyword);
		if ( dataStr == tempStr )
		{
			returnStr = (char *) strchr (dataStr, '=');
			if ( returnStr) returnStr++;
			tempStr = (char *) 0;
		}
		else
		{
			tempStr = (char *) strtok( (char *) 0, "&");
		}
	}

	if ( returnStr )
	{
		returnStr = strdup(returnStr);
	}
	free(theString);
	return returnStr;
}


/*
 * Check if a path is safe to use
 *   Return true if 'path' contains any whitespace or non-printables
 *   Return true if 'path' contains '../'
 */
int CheckPath(char *path)
{
	int i,c;

#if defined(CONF_STRICT_NAMES)
	for (i=0; i< strlen(path); i++)
	{
		c = path[i];
		if ( !isprint(c) || isspace(c) )
		{
			return 1;
		}
	}
#endif

	return ( strstr(path, "../") != NULL );
}

/*
 * Condense slashes, removing duplicates and trailers
 */
char *CondenseSlashes(char *path)
{
	char *tmp;
	int i,j;

	tmp = (char *) SafeMalloc( strlen(path)+1, "CondenseSlashes");

	for (i=0, j=0; i<strlen(path); i++)
	{
		if ( !((path[i] == '/') &&
			((path[i+1] == '/') || (path[i+1] == '\0')) ))
		{
			tmp[j++] = path[i];
		}
	}
	tmp[j] = 0;

	return tmp;
}

/*
 * Count the number of /'s in a string
 */
int CountSubDirs(char *path)
{
	int i, count;
	count = 0;

	for (i=0; i<strlen(path); i++)
	{
		if ( path[i] == '/' )
		{
			count++;
		}
	}
	return count;
}





/*
 * Output the contents of several important environment variables
 */
void OutputEnvironment(void)
{
	DEBUG_Msg("");
 	DEBUG_Msg("Environment Variables:");  
	DEBUG_Str("     QUERY_STRING:", (char *) getenv("QUERY_STRING") );
        DEBUG_Str("      SCRIPT_NAME:", (char *) getenv("SCRIPT_NAME") );
        DEBUG_Str("  SCRIPT_FILENAME:", (char *) getenv("SCRIPT_FILENAME") );
        DEBUG_Str("        PATH_INFO:", (char *) getenv("PATH_INFO") );
        DEBUG_Str("  PATH_TRANSLATED:", (char *) getenv("PATH_TRANSLATED") );
        DEBUG_Str("      REMOTE_USER:", (char *) getenv("REMOTE_USER") );
        DEBUG_Str("      REMOTE_HOST:", (char *) getenv("REMOTE_HOST") );
        DEBUG_Str("      REMOTE_ADDR:", (char *) getenv("REMOTE_ADDR") );
}

/*
 * Change to the users cgi-bin directory, this serves to provide
 * a consistent strting point for paths.
 */
void ChangeToCGIDir(char *scriptPath)
{
	char *tempdir;
	char *tempstring;
	int i;

	i = CountSubDirs(scriptPath) - 1;
	tempdir = GetPathComponents(i, scriptPath);

	tempstring = (char *) SafeMalloc (strlen(tempdir) + 5,
		"cgi directory path");
	tempstring[0] = '/';
	tempstring[1] = 0;
	strcat(tempstring, tempdir);

	DEBUG_Str("\nChanging current directory to", tempstring);
	if (chdir(tempstring) == -1) 
	{
		MSG_Error_SystemError("Cannot change to user's CGI directory via chdir()!");
	}

	free(tempdir);
	free(tempstring);
}

/*
 * Perform checks on the userid 
 */
void CheckUser(struct passwd *user)
{
#if defined(CONF_DENYFILE) || defined(CONF_ALLOWFILE)
	int deny_exists=0, allow_exists=0;
	int in_deny, in_allow;	

#if defined(CONF_CHECKHOST)
	if ( ( !getenv("REMOTE_ADDR") ) || (getenv("REMOTE_ADDR")[0] == '\0') )
	{
		Log(user->pw_name, "-", "no remote host");
		MSG_Error_General("Your host (null) is not allowed to run this");
	}
#endif

#if defined(CONF_DENYFILE)
	deny_exists = FileExists(CONF_DENYFILE);
#endif
#if defined(CONF_ALLOWFILE)
	allow_exists = FileExists(CONF_ALLOWFILE);
#endif
	in_deny = 0;
	in_allow = 0;

#if defined(CONF_DENYFILE)
	if ( deny_exists )
	{
	        DEBUG_Str("Checking deny file for",user->pw_name);
		in_deny = UserInFile(CONF_DENYFILE, user->pw_name);
	}
#endif
#if defined(CONF_ALLOWFILE)
	if ( allow_exists )
	{
	        DEBUG_Str("Checking allow file for",user->pw_name);
		in_allow = UserInFile(CONF_ALLOWFILE, user->pw_name);
	}
#endif

	if ( !deny_exists && !allow_exists )
	{
		Log(user->pw_name, "-", "access control files not found");
#if defined(CONF_ALLOWFILE) && defined(CONF_DENYFILE)
		MSG_Error_AccessControl("Access control files not found!",
			CONF_ALLOWFILE " and " CONF_DENYFILE);
#elif defined(CONF_ALLOWFILE)
		MSG_Error_AccessControl("Access control file not found!",
			CONF_ALLOWFILE);
#elif defined(CONF_DENYFILE)
		MSG_Error_AccessControl("Access control file not found!",
			CONF_DENYFILE);
#endif
	}

	if ( (in_allow && in_deny) ||
		( allow_exists && !in_allow ) ||
		( deny_exists && in_deny ) )
	{
		Log(user->pw_name, "-", "user/host not permitted");
		MSG_Error_AccessControl("Script userid and/or remote host not permitted!",NULL);
	}
#endif

#if defined(CONF_MINIMUM_UID)
	if ( user->pw_uid < CONF_MINIMUM_UID )
	{
		Log(user->pw_name, "-", "uid less than minimum");
		MSG_Error_AccessControl("UID of script userid less than configured minimum.",NULL);
	}
#endif

#if defined(CONF_MINIMUM_GID)
	if ( user->pw_gid < CONF_MINIMUM_GID )
	{
		Log(user->pw_name, "-", "gid less than minimum");
		MSG_Error_AccessControl("GID of script userid less than configured minimum.",NULL);
	}
#endif

#if defined(CONF_CHECKSHELL)
	{
	char *sh, *getusershell();
	int found = 0;

	while ( sh = getusershell() )
	{
		if (0 == strcmp( sh, user->pw_shell ))
		{
		 	found = 1;
			break;
		}
	}
	if (found == 0)
	{
	  Log(user->pw_name, "-", "restricted login shell");
	  MSG_Error_AccessControl("Restricted login shell, permission denied.",NULL);
	}
	}
#endif

}



/*
 * Perform file checks on the script file
 */
void CheckScriptFile(void)
{
	struct stat fileStat; /* For checking file status */
	struct stat fileLStat; /* For checking symlink status */

	if ( CheckPath(Context.scriptFullPath) )
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptFullPath,
			"Script path contains illegal components");
	}
	
#if !defined(CONF_SUBDIRS)
	/* Check if script is in a subdirectory */
	if ( CountSubDirs(Context.scriptRelativePath) > 0 )
	{
		Log(Context.user.pw_name, Context.scriptFullPath, 
			"script in subdir not allowed");
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Scripts in subdirectories are not allowed");
	}
#endif

	if ( stat(Context.scriptFullPath, &fileStat) )
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script file not found.");
	}

	if ( lstat(Context.scriptFullPath, &fileLStat) )
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script file not found.");
	}

#if defined(CONF_CHECK_SYMLINK)
	if ( S_ISLNK(fileLStat.st_mode) )
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is a symbolic link");
	}
#endif		

	if ( !S_ISREG(fileStat.st_mode) )
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is not a regular file");
	}

	if (!(fileStat.st_mode & S_IXUSR))
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is not executable. Issue 'chmod 755 filename'");
	}


#if defined(CONF_CHECK_SCRUID)
	if (!S_ISLNK(fileLStat.st_mode))
	{
		if (!Context.multiuser_cgi_script && fileStat.st_uid != Context.user.pw_uid)
		{
			MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
				"Script does not have same UID");
		}
	}
	else
	{
		if (!Context.multiuser_cgi_script && fileLStat.st_uid != Context.user.pw_uid)
		{
			MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
				"Symlink to script does not have same UID");
		}
	}
#endif


#if defined(CONF_CHECK_SCRGID)
	if (!Context.multiuser_cgi_script && fileStat.st_gid != Context.user.pw_gid)
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script does not have same GID");
	}
#endif


#if defined(CONF_CHECK_SCRSUID)
	if (fileStat.st_mode & S_ISUID)
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is setuid");
	}
#endif


#if defined(CONF_CHECK_SCRSGID)
	if (fileStat.st_mode & S_ISGID)
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is setgid");
	}
#endif

#if defined(CONF_CHECK_SCRGWRITE)
	if (fileStat.st_mode & S_IWGRP)
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is group writable.");
	}
#endif

#if defined(CONF_CHECK_SCROWRITE)
	if (fileStat.st_mode & S_IWOTH)
	{
		MSG_Error_ExecutionNotPermitted(Context.scriptRelativePath,
			"Script is world writable.");
	}
#endif
}


/*
 *  Verify the CGIwrap is being executed by the server userid
 *  and that it is running as effective uid root.
 */
void VerifyExecutingUser(void)
{
#if defined(CONF_CHECK_HTTPD_USER)
	struct passwd *user;

	if ( !(user = getpwnam(CONF_HTTPD_USER)) )
	{
		MSG_Error_ServerUserNotFound();
	}

	if ( getuid() != user->pw_uid )
	{
		MSG_Error_ServerUserMismatch();
	}
#endif

	if ( geteuid() != 0 )
	{
		MSG_Error_CGIWrapNotSetUID();
	}
}

/*
 * Construct string containing full path to script
 */
char *BuildScriptPath(char *basedir, char *scrStr)
{
	char *tmp;
	tmp = (char *) SafeMalloc ( strlen(basedir) + strlen(scrStr) + 5,
		"scriptPath");

	sprintf(tmp, "%s/%s", basedir, scrStr);
	return tmp;
}


/*
 * Extract the first 'count' components of 'path'
 */
char *GetPathComponents(int count, char *path)
{
	char *tmp;
	int i, j, found;
	int done;
	int len;

	tmp = strdup(path);	
	len = strlen(tmp);

	/* First skip over any leading /'s */
	i = 0;
	done = 0;
	while ( i<len && !done )
	{
		if ( path[i] == '/' )
		{
			i++;
		}
		else
		{
			done = 1;
		}
	}

	
	/* Now, only copy a certain number of components */
	j = 0;
	found = 0;
	while ( i<len && found < count)
	{
		if ( path[i] == '/' )
		{
			found++;
		}	
		if ( found < count )
		{
			tmp[j] = path[i];
			j++;
		}
		i++;
	}
	tmp[j] = 0;		

	return tmp;
}



/*
 * Extract all but the first 'count' components of 'path'
 */
char *StripPathComponents(int count, char *path)
{
	char *tmp;
	int i, found;
	int done;
	int len;

	len = strlen(path);

	/* First skip over any leading /'s */
	i = 0;
	done = 0;
	while ( i<len && !done )
	{
		if ( path[i] == '/' )
		{
			i++;
		}
		else
		{
			done = 1;
		}
	}

	
	/* Now, skip over a certain number of components */
	found = 0;
	while ( i<len && found < count)
	{
		if ( path[i] == '/' )
		{
			found++;
		}	
		i++;
	}
	tmp = strdup(path + i);

	return tmp;
}


/*
 * Set Environment Variables
 */
void SetEnvironmentVariables(void)
{
#if defined(CONF_SETENV_ANY)
	int i;
	char msg[500];

	struct cgiwrap_setenv_table
	{
		char *variable;
		char *value;
		char *setstring;
	} cgiwrap_setenvs[] = {
#if defined(CONF_SETENV_PATH)
		{"PATH", CONF_SETENV_PATH, "PATH=" CONF_SETENV_PATH},
#endif
#if defined(CONF_SETENV_TZ)
		{"TZ", CONF_SETENV_TZ, "TZ=" CONF_SETENV_TZ},
#endif
		{0,0,0}
	};

	for (i=0; cgiwrap_setenvs[i].variable; i++)
	{
		sprintf(msg, "Setting Environment Variable(%s) to (%s)\n", 
			cgiwrap_setenvs[i].variable,
			cgiwrap_setenvs[i].value);
		DEBUG_Msg(msg);

#if defined(HAS_PUTENV)
		SafePutenv(cgiwrap_setenvs[i].setstring, 
			"set environment variable");
#endif
	}
#endif
}


/*
 * Set Resource Limits
 */
void SetResourceLimits(void)
{
#if defined(CONF_USE_RLIMIT_ANY)
	struct rlimit limstruct;
	int i;
	char msg[200];

	struct cgiwrap_rlimit_table
	{
		int which;
		char *label;
		long value;
	} cgiwrap_rlimits[] = {
#if defined(CONF_USE_RLIMIT_CPU) && defined(RLIMIT_CPU)
		{RLIMIT_CPU, "cpu time", CONF_USE_RLIMIT_CPU},
#endif
#if defined(CONF_USE_RLIMIT_VMEM) && defined(RLIMIT_VMEM)
		{RLIMIT_VMEM, "virtual memory", CONF_USE_RLIMIT_VMEM},
#endif
#if defined(CONF_USE_RLIMIT_FSIZE) && defined(RLIMIT_FSIZE)
		{RLIMIT_FSIZE, "writable file size", CONF_USE_RLIMIT_FSIZE},
#endif
#if defined(CONF_USE_RLIMIT_DATA) && defined(RLIMIT_DATA)
		{RLIMIT_DATA, "data size", CONF_USE_RLIMIT_DATA},
#endif
#if defined(CONF_USE_RLIMIT_STACK) && defined(RLIMIT_STACK)
		{RLIMIT_STACK, "stack size", CONF_USE_RLIMIT_STACK},
#endif
#if defined(CONF_USE_RLIMIT_AS) && defined(RLIMIT_AS)
		{RLIMIT_AS, "total available memory", CONF_USE_RLIMIT_AS},
#endif
#if defined(CONF_USE_RLIMIT_CORE) && defined(RLIMIT_CORE)
		{RLIMIT_CORE, "core file size", CONF_USE_RLIMIT_CORE},
#endif
#if defined(CONF_USE_RLIMIT_RSS) && defined(RLIMIT_RSS)
		{RLIMIT_RSS, "resident set size", CONF_USE_RLIMIT_RSS},
#endif
#if defined(CONF_USE_RLIMIT_NPROC) && defined(RLIMIT_NPROC)
		{RLIMIT_NPROC, "number of processes", CONF_USE_RLIMIT_NPROC},
#endif
#if defined(CONF_USE_RLIMIT_NOFILE) && defined(RLIMIT_NOFILE)
		{RLIMIT_NOFILE, "number of open files", CONF_USE_RLIMIT_NOFILE},
#endif
#if defined(CONF_USE_RLIMIT_MEMLOCK) && defined(RLIMIT_MEMLOCK)
		{RLIMIT_MEMLOCK, "lockable memory", CONF_USE_RLIMIT_MEMLOCK},
#endif
		{0,0,0}
	};

	for (i=0; cgiwrap_rlimits[i].label; i++)
	{
		limstruct.rlim_cur = cgiwrap_rlimits[i].value;
		limstruct.rlim_max = cgiwrap_rlimits[i].value;

		sprintf(msg, "Limiting (%s) to (%ld)\n", 
			cgiwrap_rlimits[i].label,
			cgiwrap_rlimits[i].value);
		DEBUG_Msg(msg);

		setrlimit(cgiwrap_rlimits[i].which, &limstruct);
	}
#endif
}


/*
 * Set default signal behavior
 */
void SetSignals(void)
{
#if defined(SIGXCPU) && defined(HAS_SIGSET)
	DEBUG_Msg("Setting SIGXCPU to default behaviour\n");
	sigset(SIGXCPU, SIG_DFL);
#endif
}


/*
 * Change real and effective UID and GID to those of 'user'
 */
void ChangeID ( struct passwd *user)
{


#if defined(HAS_SETGID) && defined(HAS_SETUID)
	setgid( user->pw_gid );
	setuid( user->pw_uid );
#elif defined(HAS_SETRESGID) && defined(HAS_SETRESUID)
	setresgid( user->pw_gid, user->pw_gid, user->pw_gid );
	setresuid( user->pw_uid, user->pw_uid, user->pw_uid );
#elif defined(HAS_SETREGID) && defined(HAS_SETREUID)
	setregid( user->pw_gid, user->pw_gid );
	setreuid( user->pw_uid, user->pw_uid );
#elif defined(HAS_SETEGID) && defined(HAS_SETEUID) && defined(HAS_SETRGID) && defined(HAS_SETRUID)
	setegid( user->pw_gid );
	setrgid( user->pw_gid );
	seteuid( user->pw_uid );
	setruid( user->pw_uid );
#else
#error "Configuration Error, No Way to Change IDs"
#endif


	DEBUG_Msg("\nUIDs/GIDs Changed To:");
	DEBUG_Int("   RUID:", getuid());
	DEBUG_Int("   EUID:", geteuid());
	DEBUG_Int("   RGID:", getgid());
	DEBUG_Int("   EGID:", getegid());


	/***/
	/**   Check if ID's were actually changed */
	/***/
	if ( getuid() != user->pw_uid )
	{
		MSG_Error_General("Real UID could not be changed!");
	}
	if ( geteuid() != user->pw_uid )
	{
		MSG_Error_General("Effective UID could not be changed!");
	}
	if ( getgid() != user->pw_gid )
	{
		MSG_Error_General("Real GID could not be changed!");
	}
	if ( getegid() != user->pw_gid )
	{
		MSG_Error_General("Effective GID could not be changed!");
	}
}

/*
 * Set the process's auxilliary groups
 */
void ChangeAuxGroups(struct passwd *user)
{
#if defined(CONF_MINIMUM_GID) && defined(HAS_GETGROUPS)
	int i,j;
	gid_t *groups;
#endif

#if defined(HAS_SETGROUPS) && defined(CONF_SETGROUPS)
	gid_t grouplist[1];
	grouplist[0] = user->pw_gid;

	if ( setgroups(1, grouplist) == -1 )
		MSG_Error_SystemError("setgroups() failed!");
#endif

#if defined(HAS_INITGROUPS) && defined(CONF_INITGROUPS)
	if ( initgroups( user->pw_name, user->pw_gid ) == -1 )
		MSG_Error_SystemError("initgroups() failed!");
#endif

	/* verify here that we didn't enable a group less than minimum */
#if defined(CONF_MINIMUM_GID) && defined(HAS_GETGROUPS)
	i = getgroups(0, groups);
	if ( i > 0 )
	{
		groups = (gid_t *)SafeMalloc(i * sizeof(gid_t), "Auxilliary Group List");
		getgroups(i, groups);

		for ( j=0; j<i; j++ )
		{
printf("checking %d against %d\n", groups[j], CONF_MINIMUM_GID);
			if ( groups[j] < CONF_MINIMUM_GID )
			{
                		Log(user->pw_name, "-", "supplementary gid less than minimum");
                		MSG_Error_AccessControl("Supplementary GID of script userid less than configured minimum.",NULL);
			}
		}
	}
#endif
}


/*
 * Return true if 'user' is listed in file 'filename'
 */
int UserInFile(char *filename, char *user)
{
	FILE *file;
	char temp[HUGE_STRING_LEN], *tmpuser;
#if defined(CONF_CHECKHOST)
	int count, remote_addr[4],spec_mask[4],spec_addr[4];
	char *i;
#endif
	int j, intail;

#if defined(CONF_CHECKHOST)
	if (sscanf(getenv("REMOTE_ADDR"),"%d.%d.%d.%d",
		&remote_addr[0],&remote_addr[1],
		&remote_addr[2],&remote_addr[3]) != 4 )
	{
		Log(user, "-", "no remote host");
		MSG_Error_General("Your host is not allowed to run this");
	}
#endif

	if ( (file=fopen(filename,"r")) == NULL )
	{
		MSG_Error_SystemError("Couldn't open access control file");
	}

	while ( !feof(file) )
	{
		temp[0] = 0;
		if ( !fgets(temp, HUGE_STRING_LEN, file) )
		{
			fclose(file);
			return 0;
		}
		intail=1;
		for (j=strlen(temp)-1; j>=0; j--)
		{
			if ( !isprint(temp[j]) )
			{
				temp[j] = 0;
			}
			else 
                        { 
                             if (intail) 
			     {
			           if (isspace(temp[j]))
				   {
				         temp[j] = 0;
				   }
				   else 
				   {
				         intail = 0;
				   }
			     }
			}
		}

		if ( !strcmp(temp,user) )
		{
			fclose(file);
			DEBUG_Str("Found",user);
			return 1;
		}
#if defined(CONF_CHECKHOST)
		i = strchr(temp, '@');
		tmpuser = strtok(temp, "@");
		if ( (i != NULL) && (tmpuser != NULL) &&
			( (!strcmp(tmpuser,user)) || (!strcmp(tmpuser, "*")) ) )
		{
			while (i != NULL)
			{
				spec_mask[0]=255; 
				spec_mask[1]=255; 
				spec_mask[2]=255; 
				spec_mask[3]=255;
				i++;
				count = sscanf(i,"%d.%d.%d.%d/%d.%d.%d.%d",
					&spec_addr[0], &spec_addr[1],
					&spec_addr[2], &spec_addr[3],
					&spec_mask[0], &spec_mask[1],
					&spec_mask[2], &spec_mask[3]);

				if (count != 4 && count != 8) {
					Log(user, "-", "incorrectly formatted allow/deny line");
					MSG_Error_General("Invalid line in access control file");
				}

				if (((spec_mask[0] & spec_addr[0]) == (spec_mask[0] & remote_addr[0])) &&
					((spec_mask[1] & spec_addr[1]) == (spec_mask[1] & remote_addr[1])) &&
					((spec_mask[2] & spec_addr[2]) == (spec_mask[2] & remote_addr[2])) &&
					((spec_mask[3] & spec_addr[3]) == (spec_mask[3] & remote_addr[3])))
				{
					fclose(file);
					return 1;
            	}
    			i = strchr(i,',');
    		}
		}
#endif
	}
	fclose(file);

	return 0;
}


/*
 * Initialize the logging
 */
void LogInit (void)
{
#if defined(CONF_LOG_USEFILE)
	FILE *logFile;
	int logfd;
#endif

	DEBUG_Msg("Initializing Logging");

#if defined(CONF_LOG_USEFILE)
	logfd = open(CONF_LOG_LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 
		S_IRUSR | S_IWUSR);
	if ( logfd == -1 )
	{
		MSG_Error_SystemError("Could not open log file for appending!");
	}
	/* set the close-on-exec flag for logfd */
	if (fcntl(logfd, F_SETFD, 1) == -1) 
	{
		MSG_Error_SystemError("Could not set close-on-exec flag for log file!");
	}

	/* open a file pointer from that file descriptor */
	logFile = fdopen(logfd, "a");
	if ( !logFile )
	{
		MSG_Error_SystemError("Could not open file stream from file descriptor!");
	}

	Context.logFile = logFile;
#endif
#if defined(CONF_LOG_USESYSLOG) && defined(HAS_SYSLOG)
	openlog("cgiwrap", LOG_PID | LOG_NOWAIT, LOG_DAEMON);
#endif
}

/*
 * Add an entry to the log file
 */
void Log (char *user, char *script, char *msg)
{
	time_t timevar;
	char *timeString;

	time(&timevar);
	timeString = ctime(&timevar);

	DEBUG_Msg("");

#if defined(CONF_LOG_USEFILE)
	DEBUG_Msg("Logging Request (File)");

	fprintf(Context.logFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		NullCheck( user ),
		NullCheck( script ),
		NullCheck( getenv("REMOTE_HOST") ),
		NullCheck( getenv("REMOTE_ADDR") ),
		NullCheck( getenv("REMOTE_USER") ),
		NullCheck( msg ),
		NullCheck( timeString ) );
	fflush(Context.logFile);
#endif
#if defined(CONF_LOG_USESYSLOG) && defined(HAS_SYSLOG)
	DEBUG_Msg("Logging Request (syslog)");

	syslog(LOG_INFO, "[%s] %s, %s, %s, %s, %s, %s",
		NullCheck( CONF_LOG_LABEL ),
		NullCheck( user ), 
		NullCheck( script ),
		NullCheck( getenv("REMOTE_HOST") ), 
		NullCheck( getenv("REMOTE_ADDR") ),
		NullCheck( getenv("REMOTE_USER") ),
		NullCheck( msg ) );

/* don't want to close log at this point */
/*	closelog(); */
#endif
}

/*
 * Terminate logging
 */
void LogEnd(void)
{
#if defined(CONF_LOG_USEFILE)
	if ( Context.logFile )
	{
		fclose(Context.logFile);
	}
#endif
}

/*
 * Set the correct SCRIPT_NAME environment variable
 */
void SetScriptName(char *userStr, char *scrStr )
{
	char *buf;
	char *name;

	name = getenv("SCRIPT_NAME");
	if ( name ) {
		/* only set SCRIPT_NAME if it was already set */

		buf = (char*) SafeMalloc (strlen("SCRIPT_NAME") +
		    strlen(name) + strlen(userStr)
		    + strlen(scrStr) + 5, "new SCRIPT_NAME environment variable");

		sprintf(buf, "%s=%s/%s/%s", "SCRIPT_NAME", 
		    name, userStr, scrStr);
		SafePutenv(buf, "set SCRIPT_NAME environment variable");
	}
}


/*
 * Set the correct SCRIPT_FILENAME environment variable (PHP uses this)
 */
void SetScriptFilename (char *scriptPath) 
{
	char *buf;
  char *name;

	name = getenv ("SCRIPT_FILENAME");
	if ( name ) {
		/* only set SCRIPT_FILENAME if it was already set */

		buf = (char*) SafeMalloc (strlen("SCRIPT_FILENAME") +
		    + strlen(scriptPath) + 5, "new SCRIPT_FILENAME environment variable");

		sprintf(buf, "%s=%s", "SCRIPT_FILENAME", scriptPath);
		SafePutenv(buf, "set SCRIPT_NAME environment variable");
	}
}



/*
 * Set the correct PATH_TRANSLATED environment variable
 */
void SetPathTranslated( char *cgiBaseDir, char *scriptPath )
{
	char *buf, *pathinfo;
	
	pathinfo = getenv("PATH_INFO");

	if ( !pathinfo )
	{
		SafePutenv("PATH_TRANSLATED=", 
			"set PATH_TRANSLATED environment variable");
		return;
	}

	buf = (char *) SafeMalloc( strlen("PATH_TRANSLATED") +
		strlen(cgiBaseDir) + strlen(pathinfo) + 5, 
		"new PATH_TRANSLATED environment variable");

	sprintf(buf, "%s=%s%s", "PATH_TRANSLATED", cgiBaseDir, pathinfo); 
	putenv(buf);
}



/*
 * If using AFS, create a process authentication group for this process
 * This protects the server from any authentication changes that the script
 * might make. It also prevents the script from inheriting the servers
 * authentication if it is running authenticated
 */
#if defined(CONF_AFS_SETPAG)
	void setpag(void);
#endif
void Create_AFS_PAG(void)
{
#if defined(CONF_AFS_SETPAG)
	DEBUG_Msg("");
	DEBUG_Msg("Setting AFS Process Authentication Group (PAG)");
	setpag();
#endif
}

/*
 * Rewrite user dir from configuration file if option is enabled
 */
char *GetUserDir(char *user)
{
#if defined(CONF_USERDIRFILE)
	FILE *file;
	static char temp[500];
	int i, j;

	DEBUG_Msg("\nProcessing user directory configuration file.");

	if ( (file=fopen(CONF_USERDIRFILE,"r")) == NULL )
	{
		MSG_Error_SystemError("Couldn't open user directory config file");
	}

	temp[0]=0;
	while ( !feof(file) )
	{
		fgets(temp, 450, file);
		i = strlen(user);

		if ( !strncmp(user, temp, i) && temp[i] == ':' )
		{
			for ( j=0; j<strlen(temp); j++)
			{
				if ( !isprint(temp[j]) )
				{
					temp[j] = 0;
				} 
			}	
		
			fclose(file);
			return &temp[i+1];
		}
	}
	fclose(file);
#endif

	return NULL;
}


/*
 * Determine the base directory for user's scripts
 */
char *GetBaseDirectory(struct passwd *user)
{
	char *userdir, *basedir;

	userdir = GetUserDir(user->pw_name);

	if ( userdir )
	{
		DEBUG_Msg("Using configured base directory.\n");
		basedir = (char *) SafeMalloc ( strlen(userdir) + 4,
			"script base directory");
		strcpy(basedir, userdir);
	}
	else
	{
		basedir = (char *) SafeMalloc (strlen(user->pw_dir) +
			strlen(CONF_CGIDIR) + 4, "script base directory");
		sprintf(basedir, "%s/%s", user->pw_dir, CONF_CGIDIR);
	}

	return basedir;
}



/*
 * Check that a file exists
 */
int FileExists(char *path)
{
	struct stat fileStat; /* For checking file status */

	if ( stat(path, &fileStat) )
	{
		return 0;
	}

	if ( !S_ISREG(fileStat.st_mode) )
	{
		return 0;
	}

	return 1;
}



/*
 * Check that a directory exists
 */
int DirExists(char *path)
{
	struct stat fileStat; /* For checking file status */

	if ( stat(path, &fileStat) )
	{
		return 0;
	}

	if ( !S_ISDIR(fileStat.st_mode) )
	{
		return 0;
	}

	return 1;
}
