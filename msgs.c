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
void MSG_HTML_Header(char *title)
{
	printf("<HTML>\n<HEAD>\n");
	printf("<TITLE>%s</TITLE>\n",title);
	printf("<CENTER><H2>%s</H2></CENTER>\n",title);
	printf("<HR><P></HEAD><BODY><PRE>\n");
}

void MSG_HTML_Footer(void)
{
	printf("\n</PRE></BODY></HTML>\n");
}
	
void MSG_Plain_Header(char *title)
{
	printf("\n*\n");
	printf("* %s\n",title);
	printf("*\n\n");
}

void MSG_Plain_Footer(void)
{
}


/*
 * Error Messages 
 */

void MSG_Error_ServerUserWrong(void)
{
	if ( MSG_HTMLMessages )
	{
		MSG_ContentType("text/html");
		MSG_HTML_Header("CGIWrap Error: Server UserID Mismatch");
	}
	else
	{
		MSG_ContentType("text/plain");
		MSG_Plain_Header("CGIWrap Error: Server UserID Mismatch");
	}

	printf("The userid that the web server ran cgiwrap as\n");
	printf("does not match the userid that was configured\n");
	printf("into CGIWrap.\n");

	if ( MSG_HTMLMessages )
	{
		MSG_HTML_Footer();
	}
	else
	{
		MSG_Plain_Footer();
	}
}

void MSG_Error_ScrGrpWritable(void)
{

}


