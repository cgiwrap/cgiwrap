/* rcsid: $Id$ */

/**
 **  File: util.h
 **  Purpose: Prototypes and defines for routines in util.c
 **/ 

char **CreateARGV( char *scrStr, int argc, char *argv[]);
char *StripPrefix(char *, char *);
void ChangeID(struct passwd *user);
void ChangeAuxGroups(struct passwd *user);
void SetResourceLimits(void);
void SetSignals(void);
char *GetValue(char *keyword, char *string);
int CountSubDirs(char *path);
int CheckPath(char *path);
void ChangeToCgiDir(struct passwd *user);
void OutputEnvironment(void);
void CheckUser(struct passwd *user);
void CheckScriptFile(struct passwd *user, char *scriptPath);
void VerifyExecutingUser(void);
char *BuildScriptPath(struct passwd *user, char *scrStr);
char *GetPathComponents(int count, char *path);
char *StripPathComponents(int count, char *path);
void ChangeID ( struct passwd *user);
void ChangeAuxGroups(struct passwd *user);
int UserInFile(char *filename, char *user);
void SendHeader(char *type);
void DoPError (char *msg);
void DoError (char *msg);
void Log (char *user, char *script, char *msg);
void SetScriptName(char *userStr, char *scrStr );
void SetPathTranslated(char *scriptPath);
void Create_AFS_PAG(void);
