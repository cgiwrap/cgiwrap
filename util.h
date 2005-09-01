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
 *  Copyright 2003-2005, Nathan Neulinger <nneul@umr.edu>
 *
 */

/* rcsid: $Id$ */

/**
 **  File: util.h
 **  Purpose: Prototypes and defines for routines in util.c
 **/ 

char *HTMLEncode( char *what );
char *SafeMalloc( size_t size, char *what );
void SafePutenv( char * string, char *what );
char **CreateARGV( char *scrStr, int argc, char *argv[]);
char **CreateInterpreterARGV( char *interp, char *scrStr, int argc, char *argv[]);
void ChangeID(struct passwd *user);
void ChangeAuxGroups(struct passwd *user);
void SetResourceLimits(void);
void SetEnvironmentVariables(void);
void SetSignals(void);
char *GetValue(char *keyword, char *string);
int CountSubDirs(char *path);
int CheckPath(char *path);
void ChangeToCgiDir(struct passwd *user);
void OutputEnvironment(void);
void SaveEnvironment(void);
void CheckUser(struct passwd *user);
void CheckAccess_Helper(struct passwd *user, char *allowfile, char *denyfile);
void CheckUserAccess(struct passwd *user);
void CheckVHostUserAccess(struct passwd *user);
char *CondenseSlashes(char *path);
void CheckScriptFile(void); 
void VerifyExecutingUser(void);
char *BuildScriptPath(char *basedir, char *scrStr);
char *GetPathComponents(int count, char *path);
char *StripPathComponents(int count, char *path);
void ChangeID ( struct passwd *user);
void ChangeAuxGroups(struct passwd *user);
int UserInFile(char *filename, char *user);
void DoPError (char *msg);
void ChangeToCGIDir(char *scriptPath);
void DoError (char *msg);
void Log (char *user, char *script, char *msg);
void SetScriptName(char *userStr, char *scrStr );
void SetScriptFilename(char *scriptPath );
void SetPathTranslated(char *cgiBaseDir, char *scriptPath);
void Create_AFS_PAG(void);
char *GetUserDir(char *user);
char *GetBaseDirectory(struct passwd *user);
int StringEndsWith(char *string, char *ending);

int FileExists(char *path);
int DirExists(char *path);
void LogInit (void);
void LogEnd (void);
