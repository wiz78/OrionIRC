/* $Id: NotifyHandler.cpp,v 1.6 2005/05/26 15:06:08 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NotifyHandler.h"
#include "ServerWnd.h"
#include "Prefs.h"
#include "OrionScriptInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

class NotifyItem
{
public:
	AnsiString	Nick;
    bool		IsOn;
};

//---------------------------------------------------------------------------
TNotifyHandler::TNotifyHandler()
{
	List = new TList();
}
//---------------------------------------------------------------------------
TNotifyHandler::~TNotifyHandler()
{
	Clear();

    delete List;
}
//---------------------------------------------------------------------------
void TNotifyHandler::Clear( void )
{
	for( int i = 0; i < List->Count; i++ )
    	delete Get( i );

    List->Clear();
}
//---------------------------------------------------------------------------
NotifyItem *TNotifyHandler::Get( int i )
{
	NotifyItem *ret = NULL;

    if( i < List->Count )
		ret = reinterpret_cast<NotifyItem *>( List->Items[ i ]);

	return( ret );
}
//---------------------------------------------------------------------------
void TNotifyHandler::BeginUpdate( void )
{
	for( int i = 0; i < List->Count; i++ ) {
    	NotifyItem *rec = Get( i );

        rec->IsOn = false;
    }
}
//---------------------------------------------------------------------------
void TNotifyHandler::EndUpdate( TServerWin *server )
{
	for( int i = List->Count - 1; i >= 0; i-- ) {
    	NotifyItem *item = Get( i );

        if( !item->IsOn ) {

        	DisplayEvent( server, item->Nick + " has left IRC" );

            OrionScript::FireEvent( SE_NOTIFY, server, item->Nick.c_str(), "off" );

	        Prefs.GetNotifies()->IsOn( item->Nick, false );

        	delete item;

            List->Delete( i );
        }
    }
}
//---------------------------------------------------------------------------
void TNotifyHandler::IsOn( TServerWin *server, AnsiString nick )
{
	NotifyItem *rec = NULL;

	for( int i = 0; !rec && ( i < List->Count ); i++ ) {
		NotifyItem *tmp = Get( i );

        if( !tmp->Nick.AnsiCompareIC( nick ))
        	rec = tmp;
    }

    if( !rec ) {

    	rec       = new NotifyItem();
        rec->Nick = nick;

        List->Add( rec );

    	DisplayEvent( server, nick + " has joined IRC" );
        Prefs.GetSounds()->Play( SoundsMgr::NOTIFY );

		OrionScript::FireEvent( SE_NOTIFY, server, nick.c_str(), "on" );

        Prefs.GetNotifies()->IsOn( nick, true );
    }

    rec->IsOn = true;
}
//---------------------------------------------------------------------------
void TNotifyHandler::DisplayEvent( TServerWin *server, AnsiString msg )
{
	switch( server->Server.GetEventDisplay( TServer::EV_NOTIFY )) {

    	case TServer::SHOW_CHANNEL:
	    	server->DisplayOnActiveWin( NULL, msg, IRC_NOTIFY );
        	break;

    	case TServer::SHOW_STATUS:
	    	server->AddRow( NULL, msg, IRC_NOTIFY );
        	break;
    }
}
//---------------------------------------------------------------------------

