static char *rcsid="$Id$";

/**
 **  File: messages.c
 **  Purpose: Routines for printing out error and other messages
 **/ 

#include "cgiwrap.h"	/* Headers for all CGIwrap source files */

/* 
 * Mode to output the message - plaintext or HTML
 */
int MSG_HTMLMessages = 1;   
int MSG_Need_NPH_Header = 0;

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
	MSG_Header("CGIWrap Error", message);
	printf(message);
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
		      prefix, suffix, getenv("SERVER_ADMIN"));
	}
	if ( getenv("SERVER_NAME") )
	{
	      printf("%sServer Name%s: %s\n", 
		      prefix, suffix, getenv("SERVER_NAME"));
	}
	if ( getenv("SERVER_HOST") )
	{
	      printf("%sServer Host%s: %s\n", 
		      prefix, suffix, getenv("SERVER_HOST"));
	}
	if ( getenv("SERVER_PORT") )
	{
	      printf("%sServer Port%s: %s\n", 
		      prefix, suffix, getenv("SERVER_PORT"));
	}
	if ( getenv("SERVER_PROTOCOL") )
	{
	      printf("%sServer Protocol%s: %s\n", 
		      prefix, suffix, getenv("SERVER_PROTOCOL"));
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
			prefix, suffix, getenv("HTTP_USER_AGENT"));
	}
	if ( getenv("REQUEST_METHOD") )
	{
		printf("%sRequest Method%s: %s\n", 
			prefix, suffix, getenv("REQUEST_METHOD"));
	}
	if ( getenv("REMOTE_HOST") )
	{
		printf("%sRemote Host%s: %s\n", 
			prefix, suffix, getenv("REMOTE_HOST"));
	}
	if ( getenv("REMOTE_ADDR") )
	{
		printf("%sRemote Address%s: %s\n", 
			prefix, suffix, getenv("REMOTE_ADDR"));
	}
	if ( getenv("REMOTE_PORT") )
	{
		printf("%sRemote Port%s: %s\n", 
			prefix, suffix, getenv("REMOTE_PORT"));
	}
	if ( getenv("REMOTE_USER") )
	{
		printf("%sRemote User%s: %s\n", 
			prefix, suffix, getenv("REMOTE_USER"));
	}
	if ( getenv("QUERY_STRING") )
	{
		if ( strlen(getenv("QUERY_STRING")) > 0)
		{	
			printf("%sQuery String%s: %s\n", 
				prefix, suffix, getenv("QUERY_STRING"));
		}
	}
	if ( getenv("PATH_INFO") )
	{
		if ( strlen(getenv("PATH_INFO")) > 0)
		{	
			printf("%sExtra Path Info%s: %s\n", 
				prefix, suffix, getenv("PATH_INFO"));
		}
	}
	if ( getenv("HTTP_REFERRER") )
	{
		printf("%sReferring Page%s: %s\n", 
			prefix, suffix, getenv("HTTP_REFERRER"));
	}
	if ( getenv("HTTP_REFERER") )
	{
		printf("%sReferring Page%s: %s\n", 
			prefix, suffix, getenv("HTTP_REFERER"));
	}


	if ( MSG_HTMLMessages )
	{
		printf("</DL>\n");
	}
}

/*
 * Error Messages 
 */

void MSG_Error_ServerUserMismatch(void)
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


void MSG_Error_ServerUserNotFound(void)
{
	MSG_Header("CGIWrap Error", "Server UserID Not Found");

	printf(
"CGIWrap was configured with a server userid that does not exist\n"
"on this server.\n");

	if ( MSG_HTMLMessages )
	{
		printf("<P>");
	}

	printf(
"This is a configuration/setup problem with CGIWrap on this server.\n"
"Please contact the server administrator.\n"
	);

	MSG_Footer();
	exit(1);
}


void MSG_Error_ExecutionNotPermitted(char *path, char *reason)
{
	MSG_Header("CGIWrap Error", "Execution of this script not permitted");

	if ( path )
	{
		printf("Execution of (%s) is not permitted\n",path);
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

	MSG_Footer();
	exit(1);
}

void MSG_Error_AccessControl(char *why)
{
	MSG_Header("CGIWrap Error", "Access Control");
	
	printf("CGIWrap access control mechanism denied execution of this\n");
	printf("script for the following reason:\n\n");

	if ( MSG_HTMLMessages )
	{
		printf("<P>\n");
	}
	printf("\t%s\n", why);	

	MSG_Footer();
	exit(1);
}

void MSG_Error_SystemError(char *when)
{
	MSG_Header("CGIWrap Error", "System Error");
	printf("CGIWrap encountered a system error:\n");
	
	if ( MSG_HTMLMessages )
	{
		printf("<DL>\n");
		printf("<DT>When: %s\n", when);
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

	MSG_Footer();
	exit(1);
}


void MSG_Error_ExecFailed(void)
{
	MSG_Header("CGIWrap Error", "Script Execution Failed");

	printf("CGIWrap encountered an error while attempting to execute\n");
	printf("this script:\n\n");

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

	printf("This message usually indicates there is a problem\n");
	printf("with the script itself. The owner of the script\n");
	printf("needs to check the #! line for correctness.\n");

	MSG_Footer();
	exit(1);
}

void MSG_Error_NoSuchUser(char *user)
{
	MSG_Header("CGIWrap Error", "User not found");

	printf("CGIWrap was unable to find the user '%s' in the\n", user);
	printf("password file on this server.\n\n");
	if ( MSG_HTMLMessages )
	{
		printf("<P>\n");
	}
	printf("Check the URL and try again.\n");

	MSG_Footer();
	exit(1);
}

void MSG_Error_NoScriptDir(void)
{
	MSG_Header("CGIWrap Error", "Script dir not found");

	printf("The specified user does not have a script directory set up\n");
	printf("for execution of cgi scripts, or the directory permissions\n");
	printf("prevent cgiwrap from using that directory.");

	MSG_Footer();
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
