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
 **  File: debug.h
 **  Purpose: Prototypes and defines for routines in debug.c
 **/ 

void DEBUG_Msg (char *msg);
void DEBUG_Str (char *msg, char *var);
void DEBUG_StrStr (char *msg, char *var, char *var2);
void DEBUG_Int (char *msg, int var);
/*
 * Print an executable's path & argument array before making an execv call.
 */
void DEBUG_Exec(char *path, char **argv);

extern int CONF_DEBUG;

#if 0
#define DEBUG_printf(X)  if (CONF_DEBUG) { printf(X); }
#endif

#define NullCheck(s) ( (s) ? (s) : ("<NULL>") )
