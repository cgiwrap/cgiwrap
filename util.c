static char *rcsid="$Id$";

/**
 **  File: util.c
 **  Purpose: Various utility routines used by cgiwrap
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/*
 * Build the ARGV array for passing to the called script
 */
char **CreateARGV( char *scrStr, int argc, char *argv[])
{
	char **temp;
	int i;

	temp = (char **) malloc( (argc+1) * sizeof(char *) );

	if ( !temp )
	{
		DoPError("Couldn't malloc() memory for argv array\n");
	}
	
	temp[0] = StripPathComponents( CountSubDirs(scrStr), scrStr );
	temp[argc] = 0;

	for (i=1; i<argc; i++)
	{
		temp[i] = argv[i];
	}

	return temp;
}

/*
 * Strip one string from the beginning of another string
 */
char *StripPrefix( char *stringToStrip, char *stripFrom )
{
	if ( !stringToStrip ) { return stripFrom; }
	if ( !stripFrom ) { return ""; }

	if ( strlen(stripFrom) >= strlen(stringToStrip) )
	{
		if (!strncmp(stringToStrip, stripFrom, strlen(stringToStrip)))
		{
			return stripFrom + strlen(stringToStrip);
		}
	}

	return stripFrom;
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
			returnStr = (char *) strchr (dataStr, '=') + 1;
			tempStr = (char *) 0;
		}
		else
		{
			tempStr = (char *) strtok( (char *) 0, "&");
		}
	}

	returnStr = strdup(returnStr);
	free(theString);
	return returnStr;
}


/*
 * Check if a path is safe to use
 *   Return true if 'path' contains any whitespace or non-printables
 *   Return true if 'path' contains '../' or './'
 */
int CheckPath(char *path)
{
	int i;

	for (i=0; i< strlen(path); i++)
	{
		if ( !isprint(path[i]) || isspace(path[i]) )
		{
			return 1;
		}
	}

	return ( strstr(path, "../") || strstr(path, "./") );
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
		count += (path[i] == '/');
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

	tempstring = (char *) malloc(strlen(tempdir) + 5);
	tempstring[0] = '/';
	tempstring[1] = 0;
	strcat(tempstring, tempdir);

	DEBUG_Str("\nChanging Current Directory to", tempstring);
	chdir(tempstring);

	free(tempdir);
	free(tempstring);
}


/*
 * Perform checks on the userid 
 */
void CheckUser(struct passwd *user)
{
	int deny_exists, allow_exists;
	int in_deny, in_allow;	

#if defined(CONF_ACCESS)
#if defined(CONF_CHECKHOST)
	if ( ( !getenv("REMOTE_ADDR") ) | (getenv("REMOTE_ADDR")[0] == '\0') )
	{
		Log(user->pw_name, "-", "no remote host");
		DoError("Your host (null) is not allowed to run this");
	}
#endif

	deny_exists = FileExists(CONF_DENYFILE);
	allow_exists = FileExists(CONF_ALLOWFILE);
	in_deny = 0;
	in_allow = 0;

	if ( deny_exists )
	{
		in_deny = UserInFile(CONF_DENYFILE, user->pw_name);
	}

	if ( allow_exists )
	{
		in_allow = UserInFile(CONF_ALLOWFILE, user->pw_name);
	}

	if ( !deny_exists && !allow_exists )
	{
		Log(user->pw_name, "-", "access control files not found");
		DoError("Access control files not found!");
	}

	if ( in_allow && in_deny )
	{
		Log(user->pw_name, "-", "user in both allow and deny files");
		DoError("User is both allowed and denied!");
	}

	if ( allow_exists && !in_allow )
	{
		Log(user->pw_name, "-", "user not in allow file");
		DoError("User not in allow file!");
	}

	if ( deny_exists && in_deny )
	{
		Log(user->pw_name, "-", "user in deny file");
		DoError("User in deny file!");
	}
#endif
}



/*
 * Perform file checks on the script file
 */
void CheckScriptFile(struct passwd *user, char *scriptPath)
{
	struct stat fileStat; /* For checking file status */
	char tempErrString[255];

	if ( CheckPath(scriptPath) )
	{
		DoError("Script path contains illegal characters or '.' or '..'!");
	}
	
#if !defined(CONF_SUBDIRS)
	/* Check if script is in a subdirectory */
	if ( CountSubDirs(scriptPath) > 0 )
	{
		Log(user->pw_name, scriptPath, "script in subdir not allowed");
		DoError("Script is in sub-directory, that is not allowed.");
	}
#endif

	if ( stat(scriptPath, &fileStat) )
	{
		DoPError("Script file not found!");
	}

	if ( !S_ISREG(fileStat.st_mode) )
	{
		DoError("Script is not a regular file");
	}

	if (!(fileStat.st_mode & S_IXUSR))
	{
		sprintf(tempErrString, "Script is not executable. Issue chmod 755 %s", 
			scriptPath);
		DoError(tempErrString);
	}


#if defined(CONF_CHECK_SCRUID)
	if (fileStat.st_uid != user->pw_uid)
	{
		DoError("Script does not have same UID");
	}
#endif


#if defined(CONF_CHECK_SCRGID)
	if (fileStat.st_gid != user->pw_gid)
	{
		DoError("Script does not have same GID");
	}
#endif


#if defined(CONF_CHECK_SCRSUID)
	if (fileStat.st_mode & S_ISUID)
	{
		DoError("Script is SUID - Will not Execute!");
	}
#endif


#if defined(CONF_CHECK_SCRSGID)
	if (fileStat.st_mode & S_ISGID)
	{
		DoError("Script is SGID - Will not Execute!");
	}
#endif

}


/*
 *  Verify the CGIwrap is being executed by the server userid
 */
void VerifyExecutingUser(void)
{
	struct passwd *user;

#if defined(CONF_CHECK_HTTPD_USER)
        if ( !(user = getpwnam(CONF_HTTPD_USER)) )
        {
                DoError("Configured server userid not found.");
        }

	if ( getuid() != user->pw_uid )
	{
		DoError("Real UID does not match configured server userid!.");
	}
#endif
}


/*
 * Construct string containing full path to script
 */
char *BuildScriptPath(char *basedir, char *scrStr)
{
	char *tmp;
	tmp = (char *) malloc( strlen(basedir) + strlen(scrStr) + 5);

	if ( !tmp )
	{
		DoPError("Couldn't malloc memory for scriptPath");
	}

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
	int i, j, found;
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
	j = 0;
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
 * Set Resource Limits
 */
void SetResourceLimits(void)
{
#if defined(CONF_USE_RLIMIT_CPU) && defined(RLIMIT_CPU)
	/* CPU Time Limit */
	struct rlimit cpulim = { 9, 10 	}; 
#endif

#if defined(CONF_USE_RLIMIT_VMEM) && defined(RLIMIT_VMEM)
	/* Virtual Memory Limit */
	struct rlimit vmemlim = { 2000000, 2500000 };
#endif

#if defined(CONF_USE_RLIMIT_CPU) && defined(RLIMIT_CPU)
	DEBUG_Msg("\nSetting Limits (CPU Usage)\n");
	setrlimit( RLIMIT_CPU, &cpulim );
#endif

#if defined(CONF_USE_RLIMIT_VMEM) && defined(RLIMIT_VMEM)
	DEBUG_Msg("Setting Limits (Virtual Memory)\n");
	setrlimit( RLIMIT_VMEM, &vmemlim );
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

#if defined(CONF_USESET)
	/* dummy dependency for Configure */
	/* causes it to display stuff about setuid/etc. */
#endif


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
	DoError("Configuration Error, No Way to Change IDs");
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
		DoError("Real UID could not be changed!");
	}
	if ( geteuid() != user->pw_uid )
	{
		DoError("Effective UID could not be changed!");
	}
	if ( getgid() != user->pw_gid )
	{
		DoError("Real GID could not be changed!");
	}
	if ( getegid() != user->pw_gid )
	{
		DoError("Effective GID could not be changed!");
	}
}

/*
 * Set the process's auxilliary groups
 */
void ChangeAuxGroups(struct passwd *user)
{
#if defined(HAS_SETGROUPS) && defined(CONF_SETGROUPS)
	if ( setgroups(0, NULL) == -1 )
		DoPError("setgroups() failed!");
#endif

#if defined(HAS_INITGROUPS) && defined(CONF_INITGROUPS)
	if ( initgroups( user->pw_name, user->pw_gid ) == -1 )
		DoPError("initgroups() failed!");
#endif
}


/*
 * Return true if 'user' is listed in file 'filename'
 */
int UserInFile(char *filename, char *user)
{
	FILE *file;
	char temp[200];
#if defined(CONF_CHECKHOST)
      int unlen,addr[4],mask[4],snet[4];
      char *i;
#endif

#if defined(CONF_CHECKHOST)
      unlen = strlen(user);
      mask[0]=255; mask[1]=255; mask[2]=255; mask[3]=255;
      if (sscanf(getenv("REMOTE_ADDR"),"%d.%d.%d.%d",
              &addr[0],&addr[1],&addr[2],&addr[3]) != 4 )
      {
              Log(user, "-", "no remote host");
              DoError("Your host (undetermined) is not allowed to run this");
      }
#endif

	if ( (file=fopen(filename,"r")) == NULL )
	{
		DoPError("Couldn't Open User List File");
	}

	while ( !feof(file) )
	{
		fscanf(file,"%s",temp);
		if ( !strcmp(temp,user) )
		{
			fclose(file);
			return 1;
		}
#if defined(CONF_CHECKHOST)
              else if (( !strncmp(temp,user,unlen) ) && (temp[unlen] == '@') )
              {
                      i = &temp[unlen];
                      while (i != NULL)
                      {
                              i++;
                              sscanf(i,"%d.%d.%d.%d/%d.%d.%d.%d",
                              &snet[0],&snet[1],&snet[2],&snet[3],
                              &mask[0],&mask[1],&mask[2],&mask[3]);

                              if ((snet[0] == (mask[0] & addr[0])) &&
                                  (snet[1] == (mask[1] & addr[1])) &&
                                  (snet[2] == (mask[2] & addr[2])) &&
                                  (snet[3] == (mask[3] & addr[3])))
                              {
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
 * Send a Content-Type header if one hasn't already be sent
 */
void SendHeader(char *type)
{
	static int done = 0;

	if ( !done )
	{
		printf ("Content-Type: %s\n", type);
		done = 1;
	}
}


/*
 * Output an error message with system error message string
 */
void DoPError (char *msg)
{
	SendHeader("text/plain");
	printf("\n");

	printf("CGIwrap Error: %s\n", msg);
	printf("Error: %s (%d)\n", strerror(errno), errno);
	exit(1);
}


/*
 * Output a error message
 */
void DoError (char *msg)
{
	SendHeader("text/plain");
	printf("\n");

	printf("CGIwrap Error: %s \n", msg);
	exit(1);
}


/*
 * Add an entry to the log file
 */
void Log (char *user, char *script, char *msg)
{
	time_t timevar;
	FILE *logFile;
	char *timeString;

	time(&timevar);
	timeString = ctime(&timevar);

	DEBUG_Msg("");

#if defined(CONF_LOG_USEFILE)
	DEBUG_Msg("Logging Request (File)");

	if ( (logFile = fopen(CONF_LOG_LOGFILE, "a")) == NULL )
	{
		DoPError("Could not open log file for appending!");
	}
	
	fprintf(logFile, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
		NullCheck( user ),
		NullCheck( script ),
		NullCheck( getenv("REMOTE_HOST") ),
		NullCheck( getenv("REMOTE_ADDR") ),
		NullCheck( getenv("REMOTE_USER") ),
		NullCheck( msg ),
		NullCheck( timeString ) );

	fclose(logFile);
#endif
#if defined(CONF_LOG_USESYSLOG) && defined(HAS_SYSLOG)
	DEBUG_Msg("Logging Request (syslog)");

	openlog("cgiwrap", LOG_PID | LOG_NOWAIT, LOG_DAEMON);

	syslog(LOG_INFO, "[%s] %s, %s, %s, %s, %s, %s",
		NullCheck( CONF_LOG_LABEL ),
		NullCheck( user ), 
		NullCheck( script ),
		NullCheck( getenv("REMOTE_HOST") ), 
		NullCheck( getenv("REMOTE_ADDR") ),
		NullCheck( getenv("REMOTE_USER") ),
		NullCheck( msg ) );

	closelog();
#endif
}


/*
 * Set the correct SCRIPT_NAME environment variable
 */
void SetScriptName(char *userStr, char *scrStr )
{
	char *buf;

	buf = (char*) malloc (strlen("SCRIPT_NAME") +
	    strlen(getenv("SCRIPT_NAME")) + strlen(userStr)
	    + strlen(scrStr) + 5);

	if( !buf )
	{
		DoPError("Couldn't malloc memory for SCRIPT_NAME buf!");
	}

	sprintf(buf, "%s=%s/%s/%s", "SCRIPT_NAME", 
	    getenv("SCRIPT_NAME"), userStr, scrStr);
	putenv(buf);
}


/*
 * Set the correct PATH_TRANSLATED environment variable
 */
void SetPathTranslated( char *scriptPath )
{
	char *buf;

	buf = (char *) malloc( strlen("PATH_TRANSLATED") +
		strlen(scriptPath) + 5 );

	if( !buf )
	{
		DoPError("Couldn't malloc memory for PATH_TRANSLATED buf!");
	}

	sprintf(buf, "%s=%s", "PATH_TRANSLATED", scriptPath); 
	putenv(buf);
}



/*
 * If using AFS, create a process authentication group for this process
 * This protects the server from any authentication changes that the script
 * might make. It also prevents the script from inheriting the servers
 * authentication if it is running authenticated
 */
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
	FILE *file;
	static char temp[500];
	int i, j;

#if defined(CONF_USERDIRFILE)
	DEBUG_Msg("\nProcessing User Directory Configuration File.");

	if ( (file=fopen(CONF_USERDIRFILE,"r")) == NULL )
	{
		DoPError("Couldn't Open User Directory Config File");
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
	char *tmp;
	char *userdir;
	char *basedir;

	userdir = GetUserDir(user->pw_name);

	if ( userdir )
	{
		DEBUG_Msg("Using configured base directory.\n");
		basedir = (char *) malloc( strlen(userdir) + 4 );
		strcpy(basedir, userdir);

		if ( !basedir )
		{
			DoPError("Couldn't malloc memory for basedir");
		}
	}
	else
	{
		basedir = (char *) malloc( strlen(user->pw_dir) +
			strlen(CONF_CGIDIR) + 4 );
		
		if ( !basedir )
		{
			DoPError("Couldn't malloc memory for basedir");
		}

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


