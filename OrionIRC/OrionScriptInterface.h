/* $Id: OrionScriptInterface.h,v 1.7 2005/04/26 20:16:46 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef OrionScriptH
#define OrionScriptH
//---------------------------------------------------------------------------

#include <OrionScript.h>

typedef enum {
	SE_ACTION, SE_ACTIVE, SE_APPACTIVE, SE_APPDEACTIVE, SE_BAN, SE_CHAT,
    SE_CLOSE, SE_CONNECT, SE_CONNECTFAIL, SE_CTCP, SE_CTCPREPLY, SE_DCC,
    SE_DEOP, SE_DEVOICE, SE_DISCONNECT,	SE_ERROR, SE_EXIT, SE_FILERCVD,
    SE_FILESENT, SE_GETFAIL, SE_INPUT, SE_INVITE, SE_JOIN, SE_KICK,
    SE_LOGON, SE_MODE, SE_NICK, SE_NOTICE, SE_NOTIFY, SE_OP, SE_OPEN,
    SE_PART, SE_PING, SE_PONG, SE_QUIT, SE_RAW, SE_SENDFAIL, SE_SIGNAL,
    SE_SNOTICE, SE_TEXT, SE_TOPIC, SE_UNBAN, SE_USERMODE, SE_VOICE,
    SE_WALLOPS, SE_HALFOP, SE_DEHALFOP,
	SE_COUNT
} ScriptEvent;

class ScriptsMgr;

class OrionScript
{
public:
	static void					Setup( void );
	static void					Cleanup( void );

    static void					Compile( AnsiString script );
    static void					CallFunction( AnsiString func );
    static void					Eval( AnsiString code );
    static void					CodeEval( AnsiString code );

    static void					Reload( ScriptsMgr *mgr );

    static AnsiString			EscapeTicks( AnsiString str );
    static void					FlushOutput( void );

    static int					AddCallback( zval *callback, ScriptEvent event );
	static void 				RemoveCallback( int callback, ScriptEvent event );
	static int	 				FireEvent( ScriptEvent event, ... );

private:
    AnsiString					OutputBuffer;
    TList					   *EventCallbacks[ SE_COUNT ];

								OrionScript();
							   ~OrionScript();

    void						ClearEvents( void );

	static int ORIONSCRIPTCC 	Write( const char *str, uint str_length );
	static int ORIONSCRIPTCC   	GetCfgDirective( char *name, uint name_length, zval *contents );
	static void	ORIONSCRIPTCC  	Ticks( int ticks );
};

#define METHOD_NOTSTATIC                                                                                    \
	if (!this_ptr) {                                                                                        \
		zend_error(E_ERROR, "%s() cannot be called statically", get_active_function_name(TSRMLS_C));        \
		return;                                                                                             \
	}                                                                                                       \

#define METHOD_NOTSTATIC_NUMPARAMS(c) METHOD_NOTSTATIC                                                      \
	if (ZEND_NUM_ARGS() != c) {                                                                              \
		ZEND_WRONG_PARAM_COUNT();                                                                           \
	}                                                                                                       \

#define DEFAULT_0_PARAMS \
	if (ZEND_NUM_ARGS() > 0) { \
		ZEND_WRONG_PARAM_COUNT(); \
	}

#define REQUIRED_PARAMS(c) \
	if (ZEND_NUM_ARGS() != c) { \
		ZEND_WRONG_PARAM_COUNT(); \
	}

#endif
