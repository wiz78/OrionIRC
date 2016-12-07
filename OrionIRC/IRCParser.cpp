/* $Id: IRCParser.cpp,v 1.3 2003/03/16 15:48:11 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IRCParser.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
IRCParser::IRCParser( TServerWin *server )
{
	ServerWnd = server;
    Args      = new TStringList();
}
//---------------------------------------------------------------------------
IRCParser::~IRCParser()
{
	delete Args;
}
//---------------------------------------------------------------------------
bool IRCParser::Parse( char *msg )
{
	char  *ptr;

    Command = "";
    
    Args->Clear();

	if( msg[0] == ':' ) {

        ptr = strchr( ++msg, ' ' );

        if( ptr ) {

        	*ptr++ = '\0';

        	Origin.Set( msg );

            msg = ptr;
        }

    } else
    	Origin.Clear();

    if( ptr = strchr( msg, ' ' ))
    	*ptr++ = '\0';

    Command = msg;
    msg     = ptr;

	while( msg && *msg ) {

        if( *msg == ':' ) {

        	msg++;
            ptr = NULL;

        } else if( ptr = strchr( msg, ' ' ))
   			*ptr++ = '\0';

        Args->Add( msg );

        msg = ptr;
    }

    return( !Command.IsEmpty() );
}
//---------------------------------------------------------------------------
bool IRCParser::IsNumeric( void )
{
	return( Command.ToIntDef( -1 ) > 0 );
}
//---------------------------------------------------------------------------
int IRCParser::GetNumeric( void )
{
	return( Command.ToIntDef( 0 ));
}
//---------------------------------------------------------------------------
AnsiString IRCParser::GetArg( int index )
{
	return(( index < Args->Count ) ? Args->Strings[ index ] : AnsiString( "" ));
}
//---------------------------------------------------------------------------
