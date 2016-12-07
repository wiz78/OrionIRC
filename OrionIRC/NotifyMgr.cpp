/* $Id: NotifyMgr.cpp,v 1.4 2004/12/23 22:56:58 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "NotifyMgr.h"
#include "Notify.h"
#include "NotifyWnd.h"
#include "ServerWnd.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
NotifyMgr::NotifyMgr()
{
    Notify = new TList();
}
//---------------------------------------------------------------------------
NotifyMgr::~NotifyMgr()
{
	Clear();

    delete Notify;
}
//---------------------------------------------------------------------------
int NotifyMgr::GetCount( void )
{
	return( Notify->Count );
}
//---------------------------------------------------------------------------
TNotify *NotifyMgr::Get( int i )
{
	TNotify *ret = NULL;

    if( i < Notify->Count )
		ret = reinterpret_cast<TNotify *>( Notify->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void NotifyMgr::Clear( void )
{
    for( int i = GetCount() - 1; i >= 0; i-- )
    	delete Get( i );

   	Notify->Clear();
}
//---------------------------------------------------------------------------
void NotifyMgr::Add( AnsiString nick )
{
	for( int i = 0; i < GetCount(); i++ ) {
    	TNotify	*rec = Get( i );

        if( !rec->GetNick().AnsiCompareIC( nick ))
        	return;
	}

	Add( new TNotify( nick ));
}
//---------------------------------------------------------------------------
void NotifyMgr::Add( TNotify *fav )
{
	Notify->Add( fav );
}
//---------------------------------------------------------------------------
void NotifyMgr::Remove( AnsiString nick )
{
	for( int i = 0; i < GetCount(); i++ ) {
    	TNotify	*rec = Get( i );

        if( !rec->GetNick().AnsiCompareIC( nick )) {

        	Remove( rec );

            delete rec;
            
            break;
        }
	}
}
//---------------------------------------------------------------------------
void NotifyMgr::Remove( TNotify *fav )
{
	Notify->Remove( fav );
}
//---------------------------------------------------------------------------
void NotifyMgr::IsOn( AnsiString nick, bool on )
{
	for( int i = 0; i < GetCount(); i++ ) {
    	TNotify	*rec = Get( i );

        if( !rec->GetNick().AnsiCompareIC( nick )) {

        	rec->SetIsOn( on );

            if( NotifyWin )
				NotifyWin->UpdateItem( rec );

        	break;
        }
	}
}
//---------------------------------------------------------------------------
void NotifyMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        if( num > Notify->Capacity )
        	Notify->Capacity = num;

        for( int i = 0; i < num; i++ ) {
			TNotify	*rec = new TNotify();

            rec->Load( reg, key + "\\Notify" + IntToStr( i ));

            Add( rec );
        }
    }
}
//---------------------------------------------------------------------------
void NotifyMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Notify" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------
void NotifyMgr::SendIsON( TServerWin *server )
{
	AnsiString	cmd;

	for( int i = 0; i < GetCount(); i++ ) {
        AnsiString	nick = Get( i )->GetNick();

        if( cmd.Length() + nick.Length() + 8 > IRC_MAX_MSG_LENGTH ) {
			server->SendCmd( "ISON" + cmd );
            cmd = "";
        }

        cmd += " " + nick;
    }

    if( !cmd.IsEmpty() )
	    server->SendCmd( "ISON" + cmd );
}
//---------------------------------------------------------------------------
