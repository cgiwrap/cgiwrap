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
void ChangeToCgiDir(struct passwd *user)
{
	if ( chdir(user->pw_dir) )
	{
		DoError("Couldn't change to home directory.");
	}

	if ( chdir(CONF_CGIDIR) )
	{
		DoError("Couldn't change to user's cgi-bin directory.");
	}
}

/*
 * Perform checks on the userid 
 */
void CheckUser(struct passwd *user)
{
#if defined(CONF_ALLOWFILE) || defined(CONF_DENYFILE)
#if defined(CONF_CHECKHOST)
      if ( ( !getenv("REMOTE_ADDR") ) | (getenv("REMOTE_ADDR")[0] == '\0') )
      {
              Log(user->pw_name, "-", "no remote host");
              DoError("Your host (null) is not allowed to run this");
      }
#endif
#endif
#if defined(CONF_ALLOWFILE)
	if ( ! UserInFile(CONF_ALLOWFILE,user->pw_name) )
	{
		Log(user->pw_name, "-", "not in allow config");
		DoError("User not in allow config.");
	}
#endif

#if defined(CONF_DENYFILE)
	if ( UserInFile(CONF_DENYFILE,user->pw_name) )
	{
		Log(user->pw_name, "-", "user in deny config");
		DoError("User in deny config.");
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
		DoPError("Could not stat script file!");
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
                DoError("CONF_HTTPD_USER not found.");
        }
        else
        {
                if ( getuid() != user->pw_uid )
                {
                        DoError("Real UID does not match CONF_HTTPD_USER.");
                }
        }
#endif
}


/*
 * Construct string containing full path to script
 */
char *BuildScriptPath(struct passwd *user, char *scrStr)
{
	char *tmp;

	tmp = (char *) malloc( strlen(scrStr) + strlen(user->pw_dir) + 
		strlen(CONF_CGIDIR) + 4 );
	
	if ( !tmp )
	{
		DoPError("Couldn't malloc memory for scriptPath");
	}

	sprintf(tmp, "%s/%s/%s", user->pw_dir, CONF_CGIDIR, scrStr);

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
#if defined(HAS_SETGROUPS) & defined(CONF_SETGROUPS)
	if ( setgroups(0, NULL) == -1 )
		DoPError("setgroups() failed!");
#endif

#if defined(HAS_INITGROUPS) & defined(CONF_INITGROUPS)
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

#if defined(CONF_LOG_USEFILE)
	DEBUG_Msg("");
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
	DEBUG_Msg("");
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

	DEBUG_Msg("Fixing SCRIPT_NAME environment variable.");

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

	DEBUG_Msg("Fixing PATH_TRANSLATED environment variable.");

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
