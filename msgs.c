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
	printf("\n*\n");
	printf("* %s\n",title);
	printf("*\n\n");
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
	if ( MSG_HTMLMessages )
	{
		printf("<P><HR><P>\n");
		printf("<DL>\n");
		printf("<DT><B>Server and Request Information:</B>\n");
		printf("<P>\n");
		if ( getenv("SERVER_NAME") )
		{
			printf("<DD><B>SERVER_NAME</B>: %s\n", getenv("SERVER_NAME"));
		}
		printf("</DL>\n");

		printf("<P><HR><P>\n");
		printf("<DL>\n");
		printf("<DT><B>Contact Information and Documentation:</B>\n");
		printf("<P>\n");


		printf("</DL>\n");
	}
	else
	{
		printf("\n\n");
		printf("Server and Request Information:\n");
		printf("\n");
		if ( getenv("SERVER_NAME") )
		{
			printf("\tSERVER_NAME: %s\n", getenv("SERVER_NAME"));
		}

		printf("\n\n");
		printf("Contact Information and Documentation:\n");
		printf("\n");
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
"on this server.\n"

"This is a configuration/setup problem with cgiwrap on this server.\n"
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
	printf("for the following reason:\n\n%s\n", reason);

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
	printf("%s\n", why);	

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
		printf("When: %s\n", when);
#if defined(HAS_STRERROR)
		printf("Error Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("Error Message");
#endif
		printf("Error Number: %d\n\n", errno);
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
		printf("Error Message: %s\n", strerror(errno));
#elif defined(HAS_PERROR)
		perror("Error Message");
#endif
		printf("Error Number: %d\n\n", errno);
	}

	if ( MSG_HTMLMessages )
	{
		printf("<PRE>\n");
	}

	printf("This message usually indicates there is a problem\n");
	printf("with the script itself. The owner of the script\n");
	printf("needs to check the #! line for correctness.\n");

	if ( MSG_HTMLMessages )
	{
		printf("</PRE>\n");
	}


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

