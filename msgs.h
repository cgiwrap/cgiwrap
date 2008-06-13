/* 
 *  CGIWrap is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  CGIWrap is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with CGIWrap; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Copyright 2003-2005, Nathan Neulinger <nneul@neulinger.org>
 *
 */

/* rcsid: $Id$ */

/**
 **  File: messages.h
 **  Purpose: Prototypes and defines for routines in messages.c
 **/ 

extern int MSG_HTMLMessages;
extern int MSG_QuietErrors;
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
void MSG_Error_AccessControl(char *why, char *allowfile, char *denyfile);
void MSG_Error_ExecFailed(void);
void MSG_Error_NoSuchUser(char *user);
void MSG_Error_NoScriptDir(void);
void MSG_Error_ServerConfigError(void);
void MSG_Error_RequestError(void);

void MSG_Info(void);
void MSG_Error_ServerUserMismatch(void);
void MSG_Error_ServerUserNotFound(void);
void MSG_BoxedText(char *string);
void MSG_Error_CGIWrapNotSetUID(void);
