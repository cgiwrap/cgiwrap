/* rcsid: $Id$ */

/**
 **  File: messages.h
 **  Purpose: Prototypes and defines for routines in messages.c
 **/ 

extern int MSG_HTMLMessages;
extern int MSG_Need_NPH_Header;

void MSG_ContentType(char *typestring);
void MSG_Header(char *title, char *msg);
void MSG_Footer(void);
void MSG_HTML_Header(char *title, char *msg);
void MSG_HTML_Footer(void);
void MSG_Plain_Header(char *title, char *msg);
void MSG_Plain_Footer(void);
void MSG_Error_General(char *message);
void MSG_Error_ServerUser(char *msg);
void MSG_Error_ExecutionNotPermitted(char *path, char *reason);
void MSG_Error_SystemError(char *when);
void MSG_Error_AccessControl(char *why, char *controlfile);
void MSG_Error_ExecFailed(void);
void MSG_Error_NoSuchUser(char *user);
void MSG_Error_NoScriptDir(void);
void MSG_Info(void);
void MSG_Error_ServerUserMismatch(void);
void MSG_Error_ServerUserNotFound(void);
void MSG_BoxedText(char *string);
