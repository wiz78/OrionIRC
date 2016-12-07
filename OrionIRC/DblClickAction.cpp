/* $Id: DblClickAction.cpp,v 1.1 2005/06/08 20:45:14 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DblClickAction.h"
#include "User.h"
#include "ChanWnd.h"
#include "QueryWnd.h"
#include "ServerWnd.h"
#include "Commands.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
DblClickAction::DblClickAction()
{
	Action = DEFAULT;
}
//---------------------------------------------------------------------------
DblClickAction::DblClickAction( const DblClickAction *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
DblClickAction& DblClickAction::operator =( const DblClickAction& copy )
{
	Action  = copy.Action;
    Command = copy.Command;

    return( *this );
}
//---------------------------------------------------------------------------
void DblClickAction::Execute( TChanWin *wnd ) const
{
	switch( Action ) {

    	case CHANNEL_PROPS:
        	wnd->EditProperties( NULL );
        	break;

        case CALL_CMD:
        	HandleCommand( wnd->GetServerWin(), wnd, FormatCmd( wnd->GetChannel() ));
        	break;
    }
}
//---------------------------------------------------------------------------
void DblClickAction::Execute( TServerWin *wnd ) const
{
	switch( Action ) {

    	case SERVER_STATS:
        	HandleCommand( wnd, NULL, "RAW LUSERS" );
        	break;

        case CALL_CMD:
        	HandleCommand( wnd, NULL, FormatCmd( wnd->Server.Host ));
        	break;
    }
}
//---------------------------------------------------------------------------
void DblClickAction::Execute( TQueryWin *wnd ) const
{
	switch( Action ) {

    	case WHOIS:
        	HandleCommand( wnd->GetServerWin(),
            			   reinterpret_cast<TChanWin *>( wnd ),
                           "WHOIS " + wnd->GetTargetNick() );
        	break;

        case CALL_CMD:
        	HandleCommand( wnd->GetServerWin(),
            			   reinterpret_cast<TChanWin *>( wnd ),
                           FormatCmd( wnd->GetTargetNick() ));
        	break;
    }
}
//---------------------------------------------------------------------------
void DblClickAction::Execute( TChanWin *wnd, TUser *user ) const
{
	switch( Action ) {

    	case WHOIS:
        	HandleCommand( wnd->GetServerWin(), wnd, "WHOIS " + user->GetNick() );
        	break;

        case OPEN_QUERY:
        	wnd->GetServerWin()->FindQueryWin( user->GetNick() )->BringToFront();
        	break;

        case CALL_CMD:
        	HandleCommand( wnd->GetServerWin(),
            			   reinterpret_cast<TChanWin *>( wnd ),
                           FormatCmd( user->GetNick() ));
        	break;
    }
}
//---------------------------------------------------------------------------
void DblClickAction::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	try { Action = static_cast<TDblClickActionType>( reg->ReadInteger( "Action" )); } catch(...) {}

        Command = reg->ReadString( "Command" );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void DblClickAction::Save( TRegistry *reg, AnsiString key ) const
{
	if( reg->OpenKey( key, true )) {

    	try {
        	reg->WriteInteger( "Action", Action );
            reg->WriteString( "Command", Command );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
AnsiString DblClickAction::FormatCmd( AnsiString arg ) const
{
	AnsiString cmd, fmt = Command;

    while( int pos = fmt.Pos( "%" )) {

    	cmd += fmt.SubString( 1, pos - 1 );

        if( pos + 1 <= fmt.Length() ) {

            switch( fmt[ ++pos ] ) {

                case '%':
                    cmd += "%";
                    break;

                case 's':
                    cmd += arg;
                    break;
            }
        }

    	fmt.Delete( 1, pos );
    }

    return( cmd + fmt );
}
//---------------------------------------------------------------------------
