/* rcsid: $Id$ */

/**
 **  File: util.h
 **  Purpose: Prototypes and defines for routines in util.c
 **/ 

char *SafeMalloc( size_t size, char *what );
void SafePutenv( char * string, char *what );
char **CreateARGV( char *scrStr, int argc, char *argv[]);
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
void CheckUser(struct passwd *user);
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
void SetPathTranslated(char *scriptPath);
void Create_AFS_PAG(void);
char *GetUserDir(char *user);
char *GetBaseDirectory(struct passwd *user);

int FileExists(char *path);
int DirExists(char *path);
void LogInit (void);
void LogEnd (void);
