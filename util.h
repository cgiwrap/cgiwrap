/***/
/**   Macro for safely using null strings */
/***/
#define NullCheck(s) ( (s) ? (s) : ("<NULL>") )

extern int CONF_DEBUG;
void MyPError(void);
