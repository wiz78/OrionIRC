/* $Id: OrionScriptInterface.cpp,v 1.9 2005/02/08 18:59:19 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdarg>

#include "OrionScriptInterface.h"
#include "Main.h"
#include "ScriptsMgr.h"
#include "ScriptingWnd.h"
#include "ScriptBasic.h"
#include "ScriptClasses.h"
#include "ScriptConstants.h"
#include "ScriptEvent.h"
#include "irc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

static OrionScript		 *Instance;

#if SCRIPTING
static zend_module_entry *Modules[] =
{
	&BasicModule,
    NULL
};
#endif

//---------------------------------------------------------------------------
OrionScript::OrionScript()
{
#if SCRIPTING
    OrionCallbacks	cbs;

    for( int i = 0; i < SE_COUNT; i++ )
    	EventCallbacks[ i ] = new TList();

    memset( &cbs, 0, sizeof( cbs ));

    cbs.Write           = Write;
    cbs.GetCfgDirective = GetCfgDirective;
    cbs.Ticks           = Ticks;

    OS_Setup( &cbs, Modules );

	ScriptConstants::Register();
    ScriptClasses::Register();
#endif
}
//---------------------------------------------------------------------------
OrionScript::~OrionScript()
{
#if SCRIPTING
	ClearEvents();

    for( int i = 0; i < SE_COUNT; i++ )
    	delete EventCallbacks[ i ];

	OS_Cleanup();
#endif
}
//---------------------------------------------------------------------------
void OrionScript::ClearEvents( void )
{
    for( int i = 0; i < SE_COUNT; i++ ) {
    	TList *list = EventCallbacks[ i ];

        for( int j = 0; j < list->Count; j++ )
        	delete reinterpret_cast<TScriptEvent *>( list->Items[ j ] );

        list->Clear();
    }
}
//---------------------------------------------------------------------------
void OrionScript::Setup( void )
{
 	Instance = new OrionScript();
}
//---------------------------------------------------------------------------
void OrionScript::Cleanup( void )
{
	delete Instance;

    Instance = NULL;
}
//---------------------------------------------------------------------------
int ORIONSCRIPTCC OrionScript::Write( const char *str, uint str_length )
{
	char *buf = new char[ str_length + 1 ], *ptr, *start;

	if( !ScriptingWin )
		ScriptingWin = new TScriptingWin( MainWin );

    ScriptingWin->Show();

    memcpy( buf, str, str_length );

    buf[ str_length ] = '\0';
    start             = buf;

    do{
	    ptr = strchr( start, '\n' );

    	if( ptr ) {

    		*ptr++ = '\0';

            Instance->OutputBuffer += start;

		    ScriptingWin->AddRow( Instance->OutputBuffer );

            Instance->OutputBuffer = "";

        } else
        	Instance->OutputBuffer += start;

    } while( start = ptr );

    delete[] buf;

	return( SUCCESS );
}
//---------------------------------------------------------------------------
int ORIONSCRIPTCC OrionScript::GetCfgDirective( char *name, uint name_length, zval *contents )
{
	return( FAILURE );
}
//---------------------------------------------------------------------------
void ORIONSCRIPTCC OrionScript::Ticks( int ticks )
{
}
//---------------------------------------------------------------------------
void OrionScript::Compile( AnsiString script )
{
#if SCRIPTING
   	OS_Compile( script.c_str() );
#endif
}
//---------------------------------------------------------------------------
void OrionScript::CallFunction( AnsiString func )
{
#if SCRIPTING
   	OS_CallFunction( func.c_str() );
#endif
}
//---------------------------------------------------------------------------
void OrionScript::Eval( AnsiString code )
{
#if SCRIPTING
	OS_Eval( code.c_str() );
#endif
}
//---------------------------------------------------------------------------
void OrionScript::CodeEval( AnsiString code )
{
	Eval( "eval( '" + EscapeTicks( code ) + ";' );"  );
}
//---------------------------------------------------------------------------
void OrionScript::Reload( ScriptsMgr *mgr )
{
#if SCRIPTING
	OS_SaveState();

	Instance->ClearEvents();

	OS_ResetCode();

    mgr->Compile();

	OS_RestoreState();
#endif
}
//---------------------------------------------------------------------------
AnsiString OrionScript::EscapeTicks( AnsiString str )
{
    AnsiString ret, tmp = str;

    while( int pos = tmp.Pos( "\\" )) {

        ret += tmp.SubString( 1, pos ) + "\\";

        tmp.Delete( 1, pos );
    }

    tmp = ret + tmp;
    ret = "";

    while( int pos = tmp.Pos( "'" )) {

        ret += tmp.SubString( 1, pos - 1 ) + "\\'";

        tmp.Delete( 1, pos );
    }

    return( ret + tmp );
}
//---------------------------------------------------------------------------
void OrionScript::FlushOutput( void )
{
	if( Instance ) {

        if( !Instance->OutputBuffer.IsEmpty() ) {

            if( !ScriptingWin )
                ScriptingWin = new TScriptingWin( MainWin );

            ScriptingWin->Show();
            ScriptingWin->AddRow( Instance->OutputBuffer );

            Instance->OutputBuffer = "";
        }
    }
}
//---------------------------------------------------------------------------
int OrionScript::AddCallback( zval *callback, ScriptEvent event )
{
#if SCRIPTING
	TScriptEvent *se = new TScriptEvent( callback, event );

    Instance->EventCallbacks[ event ]->Add( se );

    return( reinterpret_cast<int>( se ));
#else
    return( 0 );
#endif
}
//---------------------------------------------------------------------------
void OrionScript::RemoveCallback( int callback, ScriptEvent event )
{
#if SCRIPTING
	TList 		   *list = Instance->EventCallbacks[ event ];
    TScriptEvent   *se = reinterpret_cast<TScriptEvent *>( callback );
    int				idx = list->IndexOf( se );

    if( idx != -1 ) {

		list->Delete( idx );

        delete se;
    }
#endif
}
//---------------------------------------------------------------------------
int OrionScript::FireEvent( ScriptEvent event, ... )
{
	int 		ok = 1;
#if SCRIPTING
	TList 	   *list = Instance->EventCallbacks[ event ];
    va_list 	va;

    va_start( va, event );

    for( int i = 0; ok && ( i < list->Count ); i++ ) {
    	TScriptEvent *ev = reinterpret_cast<TScriptEvent *>( list->Items[ i ] );

        ok = ev->Fire( va );
    }

    va_end( va );
#endif

    return( ok );
}
//---------------------------------------------------------------------------

