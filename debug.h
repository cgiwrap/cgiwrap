/**
 **  File: debug.h
 **  Purpose: Prototypes and defines for routines in debug.c
 **/ 

void DEBUG_Msg (char *msg);
void DEBUG_Str (char *msg, char *var);
void DEBUG_StrStr (char *msg, char *var, char *var2);
void DEBUG_Int (char *msg, int var);
extern int CONF_DEBUG;

#if 0
#define DEBUG_printf(X)  if (CONF_DEBUG) { printf(X); }
#endif

#define NullCheck(s) ( (s) ? (s) : ("<NULL>") )
