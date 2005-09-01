/**
 **  File: messages.c
 **  Purpose: Routines for printing out error and other messages
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */
RCSID("$Id$");

/* 
 * Mode to output the message - plaintext or HTML
 */
int MSG_HTMLMessages = 1;   
int MSG_Need_NPH_Header = 0;

#ifdef CONF_QUIET_ERRORS
int MSG_QuietErrors = 1;
#else
int MSG_QuietErrors = 0;
#endif


/*
 * Print out a content-type message, but only if one hasn't been
 * printed out already.
 */
void MSG_ContentType(char *typestring)
{
	static int printed = 0;
	if ( !printed )
	{
		printed = 1;
		if ( MSG_Need_NPH_Header )
		{
			printf("HTTP/1.0 200 Ok\n");
		}

		printf("Content-type: %s\n\n", typestring);
	}
}

/*
 * Utility Routines
 */
void MSG_Header(char *title, char *msg)
{
	if ( MSG_HTMLMessages )
	{
		MSG_ContentType("text/html");
		MSG_HTML_Header(title,msg);
	}
	else
	{
		MSG_ContentType("text/plain");
		MSG_Plain_Header(title,msg);
	}
}

void MSG_HTML_Header(char *title, char *msg)
{
	printf("<HTML>\n<HEAD>\n");
	printf("<TITLE>%s: %s</TITLE>\n",title,msg);
	printf("<CENTER><H2>%s: %s</H2></CENTER>\n",title,msg);
	printf("<HR><p></HEAD><BODY>\n");
}

void MSG_Footer(void)
{
	MSG_Info();
	if ( MSG_HTMLMessages )
	{
		MSG_HTML_Footer();
	}
	else
	{
		MSG_Plain_Footer();
	}
}

void MSG_HTML_Footer(void)
{
	printf("\n</BODY></HTML>\n");
}
	
void MSG_Plain_Header(char *title, char *msg)
{
	printf("\n");
	MSG_BoxedText(title);
	printf("\n");
}

void MSG_Plain_Footer(void)
{
}

void MSG_Error_General(char *message)
{
	if ( MSG_QuietErrors )
	{
		MSG_Header("CGIWrap Error", "Error executing script");
		printf("There was an error executing the script.");
	}
	else
	{
		MSG_Header("CGIWrap Error", message);
		printf("%s", message);
	}
	MSG_Footer();
	exit(1);
}

void MSG_Info(void)
{
	char *prefix_html = "<DD><B>";
	char *prefix_plain = "\t";
	char *suffix_html = "</B>";
	char *suffix_plain = "";
	char *prefix, *suffix;
	
	/* Handle the prefix and suffix for list items */
	if ( MSG_HTMLMessages )
	{
		prefix = prefix_html;
		suffix = suffix_html;
	}
	else
	{
		prefix = prefix_plain;
		suffix = suffix_plain;
	}


#if defined(CONF_LOCAL_INFO_ENABLED)
	if ( MSG_HTMLMessages )
	{
		printf("<P><HR><P>\n");
		printf("<DL>\n");
		printf("<DT><B>Local Information and Documentation:</B>\n");
		printf("<P>\n");
		printf("</DL>\n");
	}
	else
	{
		printf("\n\n");
		MSG_BoxedText("Local Information and Documentation:");
		printf("\n");
	}
#endif

#if defined(CONF_LOCAL_SITE_URL)
	if ( MSG_HTMLMessages )
	{
		printf("%sWeb Site%s: <A HREF=\"%s\">%s</A>\n", 
			prefix, suffix, CONF_LOCAL_SITE_URL, CONF_LOCAL_SITE_URL);
	}
	else
	{
		printf("%sWeb Site%s: %s\n", 
			prefix, suffix, CONF_LOCAL_SITE_URL);
	}
#endif
#if defined(CONF_LOCAL_DOC_URL)
	if ( MSG_HTMLMessages )
	{
		printf("%sCGIWrap Docs%s: <A HREF=\"%s\">%s</A>\n", 
			prefix, suffix, CONF_LOCAL_DOC_URL, CONF_LOCAL_DOC_URL);
	}
	else
	{
		printf("%sCGIWrap Docs%s: %s\n", 
			prefix, suffix, CONF_LOCAL_DOC_URL);
	}
#endif

#if defined(CONF_LOCAL_CONTACT_NAME)
	printf("%sContact Name%s: %s\n", 
		prefix, suffix, CONF_LOCAL_CONTACT_NAME);
#endif
#if defined(CONF_LOCAL_CONTACT_EMAIL)
	if ( MSG_HTMLMessages )
	{
		printf("%sContact EMail%s: <A HREF=\"mailto:%s\">%s</A>\n", 
			prefix, suffix, CONF_LOCAL_CONTACT_EMAIL, 
			CONF_LOCAL_CONTACT_EMAIL);
	}
	else
	{
		printf("%sContact EMail%s: %s\n", 
			prefix, suffix, CONF_LOCAL_CONTACT_EMAIL);
	}
#endif
#if defined(CONF_LOCAL_CONTACT_PHONE)
	printf("%sContact Phone%s: %s\n", 
		prefix, suffix, CONF_LOCAL_CONTACT_PHONE);
#endif
#if defined(CONF_LOCAL_CONTACT_URL)
	if ( MSG_HTMLMessages )
	{
		printf("%sContact Web Site%s: <A HREF=\"%s\">%s</A>\n", 
			prefix, suffix, CONF_LOCAL_CONTACT_URL, 
			CONF_LOCAL_CONTACT_URL);
	}
	else
	{
		printf("%sContact Web Site%s: %s\n", 
			prefix, suffix, CONF_LOCAL_CONTACT_URL);
	}
#endif


	if ( MSG_HTMLMessages )
	{
		printf("<P>\n");
		printf("<DL>\n");
		printf("<DT><B>Server Data:</B>\n");
		printf("<P>\n");
	}
	else
	{
		printf("\n");
		MSG_BoxedText("Server Data:");
		printf("\n");
	}
	
			
	if ( getenv("SERVER_ADMIN") )
	{
	      printf("%sServer Administrator/Contact%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("SERVER_ADMIN")));
	}
	if ( ! MSG_QuietErrors && getenv("SERVER_NAME") )
	{
	      printf("%sServer Name%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("SERVER_NAME")));
	}
	if ( ! MSG_QuietErrors && getenv("SERVER_HOST") )
	{
	      printf("%sServer Host%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("SERVER_HOST")));
	}
	if ( ! MSG_QuietErrors && getenv("SERVER_PORT") )
	{
	      printf("%sServer Port%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("SERVER_PORT")));
	}
	if ( ! MSG_QuietErrors && getenv("SERVER_PROTOCOL") )
	{
	      printf("%sServer Protocol%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("SERVER_PROTOCOL")));
	}
	if ( ! MSG_QuietErrors && getenv("HTTP_HOST") )
	{
	      printf("%sVirtual Host%s: %s\n", 
		      prefix, suffix, HTMLEncode(getenv("HTTP_HOST")));
	}
	
	if ( MSG_HTMLMessages )
	{
		printf("</DL>\n");
		printf("<P>\n");
		printf("<DL>\n");
		printf("<DT><B>Request Data:</B>\n");
		printf("<P>\n");
	}
	else
	{
		printf("\n");
		MSG_BoxedText("Request Data:");
		printf("\n");
	}
	
	if ( getenv("HTTP_USER_AGENT") )
	{
		printf("%sUser Agent/Browser%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("HTTP_USER_AGENT")));
	}
	if ( getenv("REQUEST_METHOD") )
	{
		printf("%sRequest Method%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("REQUEST_METHOD")));
	}
	if ( getenv("REMOTE_HOST") )
	{
		printf("%sRemote Host%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("REMOTE_HOST")));
	}
	if ( getenv("REMOTE_ADDR") )
	{
		printf("%sRemote Address%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("REMOTE_ADDR")));
	}
	if ( getenv("REMOTE_PORT") )
	{
		printf("%sRemote Port%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("REMOTE_PORT")));
	}
	if ( getenv("REMOTE_USER") )
	{
		printf("%sRemote User%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("REMOTE_USER")));
	}
	if ( getenv("QUERY_STRING") )
	{
		if ( strlen(getenv("QUERY_STRING")) > 0)
		{	
			printf("%sQuery String%s: %s\n", 
				prefix, suffix, HTMLEncode(getenv("QUERY_STRING")));
		}
	}
	if ( ! MSG_QuietErrors && getenv("PATH_INFO") )
	{
		if ( strlen(getenv("PATH_INFO")) > 0)
		{	
			printf("%sExtra Path Info%s: %s\n", 
				prefix, suffix, HTMLEncode(getenv("PATH_INFO")));
		}
	}
	if ( getenv("HTTP_REFERRER") )
	{
		printf("%sReferring Page%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("HTTP_REFERRER")));
	}
	if ( getenv("HTTP_REFERER") )
	{
		printf("%sReferring Page%s: %s\n", 
			prefix, suffix, HTMLEncode(getenv("HTTP_REFERER")));
	}


	if ( MSG_HTMLMessages )
	{
		printf("</DL>\n");
	}
}

/*
 * Error Messages 
 */

void MSG_Error_CGIWrapNotSetUID(void)
{
	if ( MSG_QuietErrors )
	{
		MSG_Error_ServerConfigError();
	}
	else
	{
	    MSG_Header("CGIWrap Error", "CGIWrap is not setuid");

	    printf(
"The cgiwrap executable(s) were not made setuid-root. This is required\n"
"for it to function properly. (SetUID root is needed in order to change\n"
"the uid to that of the script owner. This is an installation error\n"
"please make the executable setuid root, or use the 'make install'\n"
"method of installing the executables.\n"
	    );

	    MSG_Footer();
	    exit(1);
	}
}


void MSG_Error_ServerUserMismatch(void)
{
	if ( MSG_QuietErrors )
	{
		MSG_Error_ServerConfigError();
	}
	else
	{
		MSG_Header("CGIWrap Error", "Server UserID Mismatch");

		printf(
"The userid that the web server ran cgiwrap as does not match the\n"
"userid that was configured into the cgiwrap executable.\n\n"
"This is a configuration/setup problem with cgiwrap on this server.\n"
"Please contact the server administrator.\n"
		);

		MSG_Footer();
		exit(1);
	}
}


void MSG_Error_ServerUserNotFound(void)
{
	if ( MSG_QuietErrors )
	{
		MSG_Error_ServerConfigError();
	}
	else
	{
	struct passwd *pw;
	char *user = "";

	pw = getpwuid(getuid());
	if ( pw )
	{
		user = pw->pw_name;
	}

	MSG_Header("CGIWrap Error", "Server UserID Not Found");

	printf(
"CGIWrap was configured with a server userid that does not exist\n"
"on this server. The '--with-httpd-user' option should be specified\n"
"when configuring cgiwrap. The user that should be specified is the one\n"
"in the 'User' line of your httpd.conf file.\n"
	);

	if ( MSG_HTMLMessages )
	{
		printf("<P>");
	}

	printf(
"This is a configuration/setup problem with CGIWrap on this server.\n"
"Please contact the server administrator.\n"
	);

	if ( MSG_HTMLMessages )
	{
		printf("<P>");
	}

	printf(
"As near as can be determined from the current execution environment,\n"
"the userid this server is running as is \"%s\". Try reconfiguring\n"
"cgiwrap using '--with-httpd-user=%s'.\n", user, user
	);

	MSG_Footer();
	exit(1);
	}
}


void MSG_Error_ExecutionNotPermitted(char *path, char *reason)
{
	MSG_Header("CGIWrap Error", "Execution of this script not permitted");

	if ( MSG_QuietErrors )
	{
		MSG_Error_RequestError();
	}
	else
	{
		if ( path )
		{
			printf("Execution of (%s) is not permitted\n",HTMLEncode(path));
		}
		else
		{
			printf("Execution of that script is not permitted\n");
		}
		printf("for the following reason:\n\n");

		if ( MSG_HTMLMessages )
		{
			printf("<P><DL><DT>%s</DL>\n", reason);
		}
		else
		{
			printf("\t%s\n", reason);
		}
	}

	MSG_Footer();
	exit(1);
}

void MSG_Error_AccessControl(char *why, char *allowfile, char *denyfile)
{

	if ( MSG_QuietErrors )
	{
		MSG_Error_RequestError();
	}
	else
	{
		MSG_Header("CGIWrap Error", "Access Control");
		printf("CGIWrap access control mechanism denied execution of this\n");
		printf("script for the following reason:\n\n");

		if ( MSG_HTMLMessages )
		{
			printf("<P>\n");
		}
		printf("\t%s\n", why);	

		if ( allowfile || denyfile )
		{
			if ( MSG_HTMLMessages )
			{
				printf("<P>\n");
			}
			if ( allowfile )
			{
				printf("\tAccess Control Allow File: %s\n", HTMLEncode(allowfile));
			}
			if ( denyfile )
			{
				printf("\tAccess Control Deny File: %s\n", HTMLEncode(denyfile));
			}
		}
		MSG_Footer();
	}
	exit(1);
}

void MSG_Error_SystemError(char *when)
{
	MSG_Header("CGIWrap Error", "System Error");
	printf("CGIWrap encountered a system error.\n");

	if ( ! MSG_QuietErrors )
	{
	if ( MSG_HTMLMessages )
	{
		printf("<DL>\n");
		printf("<DT>When: %s\n", HTMLEncode(when));
#if defined(HAS_STRERROR)
		printf("<DT>Error Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("<DT>Error Message");
#endif
		printf("<DT>Error Number: %d\n", errno);
		printf("</DL>\n");
	}
	else
	{
		printf("\tWhen: %s\n", when);
#if defined(HAS_STRERROR)
		printf("\tError Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("\tError Message");
#endif
		printf("\tError Number: %d\n\n", errno);
	}
	}
	
	MSG_Footer();
	exit(1);
}


void MSG_Error_ExecFailed(void)
{
	MSG_Header("CGIWrap Error", "Script Execution Failed");

	printf("CGIWrap encountered an error while attempting to execute\n");
	printf("this script:\n\n");

	if ( ! MSG_QuietErrors )
	{
	if ( MSG_HTMLMessages )
	{
		printf("<DL>\n");
#if defined(HAS_STRERROR)
		printf("<DT>Error Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("<DT>Error Message");
#endif
		printf("<DT>Error Number: %d\n", errno);
		printf("</DL>\n");
	}
	else
	{
#if defined(HAS_STRERROR)
		printf("\tError Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("\tError Message");
#endif
		printf("\tError Number: %d\n\n", errno);
	}

	printf(
	"This message usually indicates there is a problem with the script\n"
	"itself. Often this indicates either that the #! line of the script\n"
	"is incorrect, or the script was uploaded in binary mode instead of\n"
	"ascii mode. Check to make sure that the script does not have\n"
	"control-M's at the end of every line. That will prevent it from\n"
	"executing. An easy fix that takes care of this most of the time\n"
	"is to put '#!/.../perl --' instead of '#!/.../perl' on the first\n"
	"line of the script.\n"
	"This is typically a problem if the script was edited or uploaded\n"
	"from a DOS/Windows/Macintosh station to a unix based server.\n"
	);

	if ( MSG_HTMLMessages )
	{
		printf("<P>\n");
	}
	
	printf(
	"If you are not the owner of this script, please forward this error\n"
	"and the URL that caused it to the script owner. That is often the\n"
	"component in the URL right after /cgiwrap/.\n"
	);
	}
	
	MSG_Footer();
	exit(1);
}

void MSG_Error_NoSuchUser(char *user)
{
	if ( MSG_QuietErrors )
	{
		MSG_Error_RequestError();
	}
	else
	{
	MSG_Header("CGIWrap Error", "User not found");

	printf("CGIWrap was unable to find the user '%s' in the\n", 
		HTMLEncode(user));
	printf("password file on this server.\n\n");
	if ( MSG_HTMLMessages )
	{
		printf("<P>\n");
	}
	printf("Check the URL and try again.\n");

	MSG_Footer();
	exit(1);
	}
}

void MSG_Error_NoScriptDir(void)
{
	if ( MSG_QuietErrors )
	{
		MSG_Error_RequestError();
	}
	else
	{
		MSG_Header("CGIWrap Error", "Script dir not found");

		printf("The specified user does not have a script directory set up\n");
		printf("for execution of cgi scripts, or the directory permissions\n");
		printf("prevent cgiwrap from using that directory.");
	}
	MSG_Footer();
	exit(1);
}

void MSG_Error_ServerConfigError(void)
{
#ifdef CONF_QUIET_ERRORS
	MSG_Header("CGIWrap Error", "Server Configuration Error");

	printf(
"There is a problem with the server configuration for execution\n"
"of CGI scripts. Please contact the server administrator.\n"
	);

	MSG_Footer();
#endif
	exit(1);
}

void MSG_Error_RequestError(void)
{
#ifdef CONF_QUIET_ERRORS
	MSG_Header("CGIWrap Error", "Request Error");

	printf(
"There is a problem with the request. Please contact the owner of the site you are\n"
"trying to access.\n"
	);

	MSG_Footer();
#endif
	exit(1);
}


void MSG_BoxedText(char *string)
{
	int i, l; 
	
	l = strlen(string);	
	for (i=0; i<=l+3; i++)
	{
		printf("*");
	}
	printf("\n* %s *\n", string);
	for (i=0; i<=l+3; i++)
	{
		printf("*");
	}
	printf("\n");
}
