/******************************************************************/
/* CGI Wrapper Program to allow access to scripts on HTTPD server */
/* Written by:  Nathan Neulinger  <nneul@umr.edu>                 */
/* Includes many fixes and improvements as suggested by users     */
/*----------------------------------------------------------------*/
/* Provides various levels of security depending on which options */
/* are chosen in config.h                                         */
/******************************************************************/

/* SCRIPT DEBUGGING ROUTINE PROTOTYPES */

void DEBUG_Msg (char *msg);
void DEBUG_Str (char *msg, char *var);
void DEBUG_StrStr (char *msg, char *var, char *var2);
void DEBUG_Int (char *msg, int var);
