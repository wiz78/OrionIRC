/* $Id: Server.cpp,v 1.18 2005/01/22 17:50:00 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdlib.h>

#include "Server.h"
#include "ServerWnd.h"
#include "Version.h"
#include "Prefs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TServer::TServer()
{
	Init();
}
//---------------------------------------------------------------------------
TServer::TServer( const TServer *copy )
{
	Init();

    *this = *copy;
}
//---------------------------------------------------------------------------
TServer::~TServer()
{
	ClearChildren();

	delete Children;
    delete Nicks;
    delete Channels;
    delete ConnectCmds;
}
//---------------------------------------------------------------------------
void TServer::Init( void )
{
	Children       = new TList();
    Parent         = NULL;
    Nicks          = new TStringList();
    Channels       = new TStringList();
    ConnectCmds	   = new TStringList();
    FailureAction  = NOTHING;
    ReconnectDelay = 0;

    for( int i = 0; i < EV_COUNT; i++ )
    	EventDisplay[ i ] = SHOW_CHANNEL;
}
//---------------------------------------------------------------------------
void TServer::ClearChildren( void )
{
	int i = 0;

    while( TServer *child = GetChild( i++ ))
    	delete child;

	Children->Clear();
}
//---------------------------------------------------------------------------
void TServer::ClearChildrenList( void )
{
	Children->Clear();
}
//---------------------------------------------------------------------------
TServer *TServer::GetChild( int i ) const
{
	TServer *ret = NULL;

    if( i < Children->Count )
    	ret = (TServer *)Children->Items[ i ];

    return( ret );
}
//---------------------------------------------------------------------------
void TServer::AddChild( TServer *serv )
{
	Children->Add( serv );
    serv->SetParent( this );
}
//---------------------------------------------------------------------------
void TServer::InsertIntoTree( TTreeView *tree, TTreeNode *parent )
{
	int i = 0;

	parent = tree->Items->AddChildObject( parent, Descr, this );

    while( TServer *serv = GetChild( i++ ))
    	serv->InsertIntoTree( tree, parent );

    if( Flags.IsSet( SERVF_EXPANDED ))
    	parent->Expand( false );
}
//---------------------------------------------------------------------------
TServer& TServer::operator =( const TServer& rhs )
{
	int i = 0;

	Descr    = rhs.Descr;
    Host     = rhs.Host;
    Port     = rhs.Port;
    Password = rhs.Password;
    Flags    = rhs.Flags;

    CopyUserStuff( rhs );
    CopyConnectionStuff( rhs );
    CopyOnConnectStuff( rhs );
    CopyMessagesStuff( rhs );
    CopyEventDisplayStuff( rhs );

    ClearChildren();

    while( TServer *child = rhs.GetChild( i++ ))
    	AddChild( new TServer( child ));

	return( *this );
}
//---------------------------------------------------------------------------
void TServer::CopyUserStuff( const TServer& rhs )
{
    Username = rhs.Username;
    Realname = rhs.Realname;

    Nicks->Assign( rhs.Nicks );
}
//---------------------------------------------------------------------------
void TServer::CopyConnectionStuff( const TServer& rhs )
{
    Flags.Clear( SERVF_SHOW_MOTD | SERVF_INVISIBLE | SERVF_WALLOPS );
    Flags.Set( rhs.Flags.Get() & ( SERVF_SHOW_MOTD | SERVF_INVISIBLE | SERVF_WALLOPS ));

    FailureAction  = rhs.FailureAction;
    ReconnectDelay = rhs.ReconnectDelay;
}
//---------------------------------------------------------------------------
void TServer::CopyOnConnectStuff( const TServer& rhs )
{
    Channels->Assign( rhs.Channels );
    ConnectCmds->Assign( rhs.ConnectCmds );
}
//---------------------------------------------------------------------------
void TServer::CopyMessagesStuff( const TServer& rhs )
{
    Flags.Clear( SERVF_MESSAGES_MASK );
    Flags.Set( rhs.Flags.Get() & SERVF_MESSAGES_MASK );

    PartReason = rhs.PartReason;
    QuitReason = rhs.QuitReason;
    FingerMsg  = rhs.FingerMsg;
    AwayMsg    = rhs.AwayMsg;
    PartFile   = rhs.PartFile;
    QuitFile   = rhs.QuitFile;
    FingerFile = rhs.FingerFile;
    AwayFile   = rhs.AwayFile;
    AwayNick   = rhs.AwayNick;
}
//---------------------------------------------------------------------------
void TServer::CopyEventDisplayStuff( const TServer& rhs )
{
    memcpy( EventDisplay, rhs.EventDisplay, sizeof( EventDisplay ));
}
//---------------------------------------------------------------------------
void TServer::SetNicks( AnsiString nicks )
{
	Nicks->Clear();

    nicks = nicks.Trim();

    while( int pos = nicks.Pos( "," )) {

    	AddNick( nicks.SubString( 1, pos - 1 ));

        nicks.Delete( 1, pos );
    }

    if( !nicks.IsEmpty() )
    	AddNick( nicks );
}
//---------------------------------------------------------------------------
int TServer::GetNicksCount( void )
{
	return( Nicks->Count );
}
//---------------------------------------------------------------------------
void TServer::AddNick( AnsiString nick )
{
	Nicks->Add( nick.Trim() );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetNick( int num )
{
    return( Nicks->Strings[ num % Nicks->Count ] );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetNicks( void )
{
	AnsiString nicks;

    for( int i = 0; i < Nicks->Count; i++ ) {

    	if( i )
        	nicks += ", ";

        nicks += Nicks->Strings[ i ];
    }

    return( nicks );
}
//---------------------------------------------------------------------------
void TServer::SetChannels( TStrings *chans )
{
    Channels->Assign( chans );
}
//---------------------------------------------------------------------------
void TServer::ClearChannels( void )
{
    Channels->Clear();
}
//---------------------------------------------------------------------------
int TServer::GetChannelsCount( void )
{
    return( Channels->Count );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetChannel( int num )
{
    return( Channels->Strings[ num ] );
}

//---------------------------------------------------------------------------
void TServer::SetCommandsOnConnect( TStrings *cmds )
{
	ConnectCmds->Assign( cmds );
}
//---------------------------------------------------------------------------
void TServer::ClearCommandsOnConnect( void )
{
	ConnectCmds->Clear();
}
//---------------------------------------------------------------------------
int TServer::GetCommandsOnConnectCount( void )
{
	return( ConnectCmds->Count );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetCommandOnConnect( int num )
{
	return( ConnectCmds->Strings[ num ]);
}
//---------------------------------------------------------------------------
void TServer::PropagateSettings( void )
{
	for( int i = 0; i < Children->Count; i++ ) {
    	TServer *serv = GetChild( i );

        if( serv->Flags.IsSet( SERVF_INHERIT_USER ))
        	serv->CopyUserStuff( *this );

        if( serv->Flags.IsSet( SERVF_INHERIT_CONNECTION ))
        	serv->CopyConnectionStuff( *this );

        if( serv->Flags.IsSet( SERVF_INHERIT_ON_CONNECT ))
        	serv->CopyOnConnectStuff( *this );

        if( serv->Flags.IsSet( SERVF_INHERIT_MESSAGES ))
        	serv->CopyMessagesStuff( *this );

        if( serv->Flags.IsSet( SERVF_INHERIT_EVENT_DISPLAY ))
        	serv->CopyEventDisplayStuff( *this );

        serv->PropagateSettings();
    }
}
//---------------------------------------------------------------------------
void TServer::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {
    	int num;

        Host     = reg->ReadString( "Host"     );
        Descr    = reg->ReadString( "Descr"    );
        Port     = reg->ReadString( "Port"     );
        Password = reg->ReadString( "Password" );
        Username = reg->ReadString( "Username" );
        Realname = reg->ReadString( "Realname" );
        AwayNick = reg->ReadString( "AwayNick" );

        SetNicks( reg->ReadString( "Nicks" ));

        Flags.Clear();
        try{ Flags.Set( reg->ReadInteger( "Flags" )); } catch(...) {}

        try{ ReconnectDelay = reg->ReadInteger( "ReconnectDelay" ); } catch(...) { ReconnectDelay = 0; }
        try{ FailureAction = reg->ReadInteger( "FailureAction" ); } catch(...) { FailureAction = NOTHING; }

        try{ num = reg->ReadInteger( "Children" ); } catch(...) { num = 0; }

    	reg->CloseKey();

        ClearChildren();

        for( int i = 0; i < num; i++ ) {
        	TServer *serv = new TServer();

            serv->Load( reg, key + "\\Child" + IntToStr( i ));

            AddChild( serv );
        }

        LoadChannels( reg, key + "\\Channels" );
        LoadMessages( reg, key + "\\Messages" );
        LoadCommands( reg, key + "\\Commands" );
        LoadEventDisplay( reg, key + "\\EventDisplay" );
    }

    if( PartReason.IsEmpty() )
		PartReason = "Guess what? I'm leaving this channel!";

    if( QuitReason.IsEmpty() )
		QuitReason = Version.GetProductName() + " " + Version.GetVersionStr() + " - Get it at http://tellini.info/index_en.html";

    if( FingerMsg.IsEmpty() )
		FingerMsg = "I'm a proud " + Version.GetProductName() + " user!";

    if( AwayMsg.IsEmpty() )
	    AwayMsg = "I guess I'm not here right now";
}
//---------------------------------------------------------------------------
void TServer::LoadChannels( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) { num = 0; }

        Channels->Clear();

        for( int i = 0; i < num; i++ )
            Channels->Add( reg->ReadString( "Channel" + IntToStr( i )));

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::LoadMessages( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {

        PartReason = reg->ReadString( "Part"       );
        QuitReason = reg->ReadString( "Quit"       );
        FingerMsg  = reg->ReadString( "Finger"     );
        AwayMsg    = reg->ReadString( "Away"       );
        PartFile   = reg->ReadString( "PartFile"   );
        QuitFile   = reg->ReadString( "QuitFile"   );
        FingerFile = reg->ReadString( "FingerFile" );
        AwayFile   = reg->ReadString( "AwayFile"   );

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::LoadCommands( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {

        ConnectCmds->Text = reg->ReadString( "OnConnect" );

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::LoadEventDisplay( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {

    	for( int i = 0; i < EV_COUNT; i++ )
        	try {
            	EventDisplay[ i ] = reg->ReadInteger( "Event" + IntToStr( i ));
            }
            catch(...) {
            	EventDisplay[ i ] = SHOW_CHANNEL;
            }

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

        	reg->WriteString( "Host",     Host       );
        	reg->WriteString( "Descr",    Descr      );
        	reg->WriteString( "Port",     Port       );
        	reg->WriteString( "Password", Password   );
        	reg->WriteString( "Username", Username   );
        	reg->WriteString( "Realname", Realname   );
        	reg->WriteString( "Nicks",    GetNicks() );
        	reg->WriteString( "AwayNick", AwayNick   );

            reg->WriteInteger( "Flags",    		 Flags.Get()     );
        	reg->WriteInteger( "ReconnectDelay", ReconnectDelay  );
        	reg->WriteInteger( "FailureAction",  FailureAction   );
            reg->WriteInteger( "Children",       Children->Count );

           	reg->CloseKey();

            for( int i = 0; i < Children->Count; i++ ) {
            	TServer *serv = GetChild( i );

                serv->Save( reg, key + "\\Child" + IntToStr( i ));
            }

			TPrefs::RecursiveDeleteKey( key + "\\Channels" );
			TPrefs::RecursiveDeleteKey( key + "\\Messages" );
			TPrefs::RecursiveDeleteKey( key + "\\Commands" );
			TPrefs::RecursiveDeleteKey( key + "\\EventDisplay" );

			SaveChannels( reg, key + "\\Channels" );
			SaveMessages( reg, key + "\\Messages" );
			SaveCommands( reg, key + "\\Commands" );
			SaveEventDisplay( reg, key + "\\EventDisplay" );
		}
        catch(...) {
	    	reg->CloseKey();
        }
   	}
}
//---------------------------------------------------------------------------
void TServer::SaveChannels( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        reg->WriteInteger( "Count", Channels->Count );

        for( int i = 0; i < Channels->Count; i++ )
            reg->WriteString( "Channel" + IntToStr( i ), GetChannel( i ));

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::SaveMessages( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        reg->WriteString( "Part",       PartReason );
        reg->WriteString( "Quit",       QuitReason );
        reg->WriteString( "Finger",     FingerMsg  );
        reg->WriteString( "Away",       AwayMsg    );
        reg->WriteString( "PartFile",   PartFile   );
        reg->WriteString( "QuitFile",   QuitFile   );
        reg->WriteString( "FingerFile", FingerFile );
        reg->WriteString( "AwayFile",   AwayFile   );

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::SaveCommands( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        reg->WriteString( "OnConnect", ConnectCmds->Text );

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TServer::SaveEventDisplay( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

    	for( int i = 0; i < EV_COUNT; i++ )
        	reg->WriteInteger( "Event" + IntToStr( i ), EventDisplay[ i ] );

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
AnsiString TServer::GetPartReason( void )
{
	AnsiString	ret;

    if( Flags.IsSet( SERVF_MSG_FROM_FILE_PART ))
    	ret = GetRandomLine( PartFile );

    if( ret.IsEmpty() )
	    ret = PartReason;

    return( ret );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetQuitReason( void )
{
	AnsiString	ret;

    if( Flags.IsSet( SERVF_MSG_FROM_FILE_QUIT ))
    	ret = GetRandomLine( QuitFile );

    if( ret.IsEmpty() )
	    ret = QuitReason;

    return( ret );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetFingerMsg( void )
{
	AnsiString	ret;

    if( Flags.IsSet( SERVF_MSG_FROM_FILE_FINGER ))
    	ret = GetRandomLine( FingerFile );

    if( ret.IsEmpty() )
	    ret = FingerMsg;

    return( ret );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetAwayMsg( void )
{
	AnsiString	ret;

    if( Flags.IsSet( SERVF_MSG_FROM_FILE_AWAY ))
    	ret = GetRandomLine( AwayFile );

    if( ret.IsEmpty() )
	    ret = AwayMsg;

    return( ret );
}
//---------------------------------------------------------------------------
AnsiString TServer::GetRandomLine( AnsiString file )
{
	TStringList	*list = new TStringList();
    AnsiString	ret;

    list->LoadFromFile( file );

    if( list->Count )
    	ret = list->Strings[ rand() % list->Count ];

    delete list;

    return( ret );
}
//---------------------------------------------------------------------------
void TServer::AutoConnect( void )
{
	if( Flags.IsSet( SERVF_AUTO_CONNECT ))
		new TServerWin( Application, this );

	for( int i = 0; i < Children->Count; i++ )
    	GetChild( i )->AutoConnect();
}
//---------------------------------------------------------------------------
void TServer::FindNext( void )
{
	TRegistry	*reg = new TRegistry();
    TServer	   	*ret, server;

	server.Load( reg, "Software\\WizSoftware\\Orion\\Servers" );

    ret = server.FindNext( *this );

    delete reg;

    if( ret ) {

    	ret->ClearChildren();

        *this = *ret;
    }
}
//---------------------------------------------------------------------------
TServer *TServer::FindNext( TServer& serv )
{
	TServer *ret = NULL;

	for( int i = 0; !ret && ( i < Children->Count ); i++ ) {
    	TServer *s = GetChild( i );

		if( !s->Host.AnsiCompareIC( serv.Host ) &&
    		!s->Port.AnsiCompareIC( serv.Port )) {
            
            do {

	            ret = GetChild( ++i % Children->Count );

            } while( !ret->CanConnect() && ( ret != s ));
        }
    }

	for( int i = 0; !ret && ( i < Children->Count ); i++ )
    	ret = GetChild( i )->FindNext( serv );

    return( ret );
}
//---------------------------------------------------------------------------
