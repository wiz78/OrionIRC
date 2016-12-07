/* $Id: ServerWnd.cpp,v 1.98 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <mmsystem.h>

#include "ServerWnd.h"
#include "IRCList.h"
#include "IRCEdit.h"
#include "IRCUtils.h"
#include "Main.h"
#include "RawWnd.h"
#include "ChanWnd.h"
#include "QueryWnd.h"
#include "ChanPropWnd.h"
#include "FavouritesWnd.h"
#include "DCCChatOfferWnd.h"
#include "DCCSendOfferWnd.h"
#include "DCCMgrWnd.h"
#include "BufferSearchWnd.h"
#include "ChanListWnd.h"
#include "DCCTransfer.h"
#include "Commands.h"
#include "Eyelin.h"
#include "OrionScriptInterface.h"
#include "Ignore.h"
#include "Prefs.h"
#include "Version.h"
//---------------------------------------------------------------------
#pragma link "Placemnt"
#pragma resource "*.dfm"

class TWHOAction
{
public:
	int			Action;
    void		*Data;
    AnsiString  Str;

    			TWHOAction( int act ) { Action = act; }
            	TWHOAction( int act, void *data ) { Action = act; Data = data; }
            	TWHOAction( int act, AnsiString str ) { Action = act; Str = str; }
};

//---------------------------------------------------------------------
static void SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata )
{
	((TServerWin *)userdata )->SocketEvent( reason, data );
}
//---------------------------------------------------------------------
__fastcall TServerWin::TServerWin( TComponent *Owner, TServer *server )
	: TForm( Owner ), Parser( this )
{
	Server             = *server;
	Session.Server     = server->Host;
    Session.Port       = server->Port.ToIntDef( 0 );
    Session.ChanTypes  = "#&+!";
    Session.ModesCount = 3;
    Session.Lag        = 0;
    Session.ServerWin  = this;
    ServerMsgLen       = 0;
    NickToTry          = 0;
	Display            = new TIRCList( this, this, Prefs.GetColorSet() );
    Input		       = new TIRCEdit( this );
    Channels           = new TList();
    Queries            = new TList();
    CmdList			   = new TStringList();
    Sock               = NULL;
    WhoActions		   = new TList();
    ChannelList		   = new TChanListWin( this );

    Server.ClearChildren();

    Input->Parent    = PanelBottom;
    Input->OnMessage = OnMessage;
    Input->OnCommand = OnCommand;

    Display->Parent     = this;
    Display->Top        = 0;
    Display->Left       = 0;
    Display->Align      = alClient;
    Display->PopupMenu  = PopupMenu;
    Display->OnDblClick = DisplayDblClick;

    Display->SetInputCtrl( Input );
    Display->ShowTimestamp( Prefs.ShowTimestamp() );
    Display->ShowNick( false );

    Input->SetWheelTarget( Display );

    SetFonts();

	Caption = "Status: " + Session.Server;

    if( Prefs.LogServers() )
    	Log.Open( Session.Server, Session.Server );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::SetFonts( void )
{
    Display->Font->Assign( Prefs.GetStatusFont() );
    Input->Font->Assign( Prefs.GetInputFont() );

    PanelBottom->Height = 6 - Input->Font->Height;
    Input->Height       = PanelBottom->Height - 2;
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::FormDestroy(TObject *Sender)
{
	Disconnect();

	if( AsyncReq )
   		WSACancelAsyncRequest( AsyncReq );

    for( int i = 0; i < WhoActions->Count; i++ )
    	delete reinterpret_cast<TWHOAction *>( WhoActions->Items[ i ]); 

    delete Channels;
    delete Queries;
    delete CmdList;
	delete Sock;
    delete WhoActions;
}
//---------------------------------------------------------------------
void __fastcall TServerWin::FormClose(TObject *Sender, TCloseAction &Action)
{
	Disconnect();

	Action = caFree;

    MainWin->RemoveTaskbarButton( TaskbarButton );

    if( Sock )
    	MainWin->Ident->RemInfo( Sock->GetLocalPort() );

    OrionScript::FireEvent( SE_CLOSE, this );
}
//---------------------------------------------------------------------
void __fastcall TServerWin::FormShow(TObject *Sender)
{
	Session.ActiveWin = this;

    TaskbarButton    = MainWin->AddTaskbarButton( Session.Server, this, TB_SERVER, PopupMenu );
    LogMenu->Checked = Log.IsOpen();

    Input->SetFocus();

    OrionScript::FireEvent( SE_OPEN, this );

    Connect();
}
//---------------------------------------------------------------------
void __fastcall TServerWin::FormActivate(TObject *Sender)
{
	SetActiveWin( this );
    MainWin->EnableTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ConnectTo( TServer *server )
{
	Server = *server;

	ConnectTo( server->Host, server->Port.ToIntDef( 6667 ));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ConnectTo( AnsiString host, int port )
{
	if( Sock ) {

    	MainWin->Ident->RemInfo( Sock->GetLocalPort() );

    	Sock->Printf( "QUIT :%s\r\n", Server.GetQuitReason().c_str() );
    }

	Session.Flags.Clear( IRC_SESS_CONNECTED | IRC_SESS_CONNECTING | IRC_SESS_DONT_RECONNECT );
    NotifyHandler.Clear();
    Flags.Clear();
    DisableChannels();
    ChannelList->Clear();

    Server.Host    = host;
    Server.Port    = IntToStr( port );
    Session.Server = host;
    Session.Port   = port;
	Caption        = "Status: " + Session.Server;

    if( TaskbarButton )
	    TaskbarButton->Caption = Session.Server;

    Connect();
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::Connect( void )
{
    if( Sock )
    	MainWin->Ident->RemInfo( Sock->GetLocalPort() );

   	delete Sock;

    Sock = new TcpSocket();

    if( Sock->IsValid() ) {

    	Sock->UseDispatcher( &MainWin->IO );
	    Sock->SetAsyncCallback( SocketCB, this );

        ReconnectTimerTimer( NULL );

    } else {
    	AnsiString error = "Cannot create a socket!";

    	AddRow( NULL, error, IRC_ERROR, true );
        OrionScript::FireEvent( SE_CONNECTFAIL, this, error.c_str() );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::WMResolved( TMessage &Message )
{
	AsyncReq = NULL;

	if( WSAGETASYNCERROR( Message.LParam ))
    	AddRow( NULL, "Cannot resolve " + Session.Server + " (" + IntToStr( WSAGetLastError() ) + ")", IRC_ERROR, true );
    else {
        struct hostent *ent = (struct hostent *)HostEnt;

		memcpy( &ServerAddr.s_addr, ent->h_addr, ent->h_length );

    	Sock->AsyncConnect( &ServerAddr, Session.Port );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::SocketEvent( Prom_SC_Reason reason, int data )
{
	switch( reason ) {

		case PROM_SOCK_READ:
        	if( data ) {

        		ServerMsgLen += data;

	        	CheckServerMsg();

            } else
            	Disconnected( false );
        	break;

    	case PROM_SOCK_CONNECTED:
        	if( data )
            	Disconnected( true );
            else
        		Login();
        	break;

        case PROM_SOCK_TIMEOUT:
		case PROM_SOCK_ERROR:
        	AddRow( NULL, "Socket error " + IntToStr( WSAGetLastError() ), IRC_ERROR, true );
            Disconnected( false );
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::Login( void )
{
	int flags = 0;

    MainWin->Ident->AddInfo( Sock->GetLocalPort(), Server.Username );

   	Session.Nick = Server.GetNick( NickToTry++ );

    if( OrionScript::FireEvent( SE_LOGON, this )) {

        if( !Server.Password.IsEmpty() )
            Sock->AsyncPrintf( "PASS %s\r\n", Server.Password.c_str() );

        if( Server.Flags.IsSet( SERVF_INVISIBLE ))
            flags |= IRC_USER_INVISIBLE;

        if( Server.Flags.IsSet( SERVF_WALLOPS ))
            flags |= IRC_USER_WALLOPS;

        Sock->AsyncPrintf( "NICK %s\r\n"
                           "USER %s %d * :%s\r\n",
                           Session.Nick.c_str(),
                           Server.Username.c_str(), flags, Server.Realname.c_str() );
    }

    Sock->AsyncRecv( ServerMsg, sizeof( ServerMsg ));
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::Connected( void )
{
	Session.Flags.Set( IRC_SESS_CONNECTED );
	Session.Flags.Clear( IRC_SESS_CONNECTING );
	Flags.Clear( SERVF_LAG_PROBE_SENT );

    NotifyHandler.Clear();

	Session.Nick = Parser.GetArg( 0 );

    SendCmd( "WHO " + Session.Nick );

    AddWhoAction( SERV_WHO_MY_HOSTMASK );

    MainWin->UpdateStatus();

    OrionScript::FireEvent( SE_CONNECT, this );

    for( int i = 0; i < Server.GetCommandsOnConnectCount(); i++ )
    	HandleCommand( this, NULL, Server.GetCommandOnConnect( i ));

    for( int i = 0; i < Server.GetChannelsCount(); i++ )
    	SendCmd( "JOIN " + Server.GetChannel( i ));

    if( Prefs.PopupFavOnConnect() ) {

	    if( FavouritesWin )
    	   	FavouritesWin->SetServer( Session.Server );
	    else
	    	FavouritesWin = new TFavouritesWin( this, Session.Server );

	    FavouritesWin->Show();
    	FavouritesWin->BringToFront();
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::Disconnect( void )
{
	if( Sock ) {

    	MainWin->Ident->RemInfo( Sock->GetLocalPort() );

    	Sock->Printf( "QUIT :%s\r\n", Server.GetQuitReason().c_str() );

        Session.Flags.Set( IRC_SESS_DONT_RECONNECT );
    }

    for( int i = 0; i < Channels->Count; i++ )
    	((TChanWin *)Channels->Items[ i ] )->CloseNow();

    Channels->Clear();
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::Disconnected( bool error )
{
	AnsiString msg;

	Session.Flags.Clear( IRC_SESS_CONNECTED | IRC_SESS_CONNECTING );
    NotifyHandler.Clear();
    DisableChannels();

    NickToTry = 0;

    if( error )
    	msg = "Cannot connect to " + Session.Server + " (" + IntToStr( WSAGetLastError() ) + ")";
    else {

    	msg = "Disconnected by the server";

        Prefs.GetSounds()->Play( SoundsMgr::DISCONNECT );
    }

	AddRow( NULL, msg, IRC_ERROR, true );
	OrionScript::FireEvent( SE_DISCONNECT, this, msg.c_str() );

    if( Sock->IsValid() )
    	MainWin->Ident->RemInfo( Sock->GetLocalPort() );

    delete Sock;

    Sock = new TcpSocket();

    Sock->UseDispatcher( &MainWin->IO );
    Sock->SetAsyncCallback( SocketCB, this );

    if( !Session.Flags.IsSet( IRC_SESS_DONT_RECONNECT ))
        switch( Server.FailureAction ) {

            case TServer::RECONNECT:
                if( Server.ReconnectDelay > 0 ) {

                    AddRow( NULL, "Waiting " + IntToStr( Server.ReconnectDelay ) + " seconds before reconnecting...", IRC_LOCAL, true );

                    ReconnectTimer->Interval = Server.ReconnectDelay * 1000;
                    ReconnectTimer->Enabled  = true;

                } else
                    ReconnectTimerTimer( NULL );
                break;

            case TServer::CONNECT_NEXT:
                Server.FindNext();
                ReconnectTimerTimer( NULL );
                break;
        }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::CheckServerMsg( void )
{
    bool	ok;

    do {
		char			msg[ IRC_MAX_MSG_LENGTH ], *src, *dest;
	    unsigned int	len = ( ServerMsgLen < sizeof( msg )) ? ServerMsgLen : sizeof( msg );

    	dest = msg;
	    src  = ServerMsg;

    	for( unsigned int i = 0; i < len; i++ ) {

    		if( *src == '\n' ) {

	        	*dest++ = '\0';
				src++;
            	break;
    	    }

	    	*dest++ = *src++;
	    }

        // is this line complete?
        ok = !dest[ -1 ];

	    if( ok ) {

    		ServerMsgLen -= src - ServerMsg;

            if( ServerMsgLen > 0 )
	    		memcpy( ServerMsg, src, ServerMsgLen );

            if( strlen( msg ) > 0 ) {

	            src = strchr( msg, '\0' ) - 1;

                if( *src == '\r' )
                	*src = '\0';
			}

    	    ParseServerMsg( msg );
        }

    } while( ok && ( ServerMsgLen > 0 ));

    // it should never happen, just check it to protect against malicious servers
    if( ServerMsgLen >= sizeof( ServerMsg ))
     	ServerMsgLen = 0;

    Sock->AsyncRecv( &ServerMsg[ ServerMsgLen ], sizeof( ServerMsg ) - ServerMsgLen );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::ParseServerMsg( char *msg )
{
	RawWin->AddLine( msg );

    if( OrionScript::FireEvent( SE_RAW, this, msg ) && Parser.Parse( msg )) {

    	if( Parser.IsNumeric() ) {

        	HandleNumerics();

        } else {
        	AnsiString cmd = Parser.GetCommand();

            if( !cmd.AnsiCompareIC( "PRIVMSG" ))
            	HandlePrivMsg();
            else if( !cmd.AnsiCompareIC( "NOTICE" ))
            	HandleNotice();
            else if( !cmd.AnsiCompareIC( "MODE" ))
            	HandleMode();
            else if( !cmd.AnsiCompareIC( "JOIN" ))
            	HandleJoin();
            else if( !cmd.AnsiCompareIC( "PART" ))
            	HandlePart();
            else if( !cmd.AnsiCompareIC( "QUIT" ))
            	HandleQuit();
            else if( !cmd.AnsiCompareIC( "TOPIC" ))
            	HandleTopicChange();
            else if( !cmd.AnsiCompareIC( "ERROR" ))
            	HandleError();
            else if( !cmd.AnsiCompareIC( "NICK" ))
            	HandleNick();
            else if( !cmd.AnsiCompareIC( "KICK" ))
            	HandleKick();
            else if( !cmd.AnsiCompareIC( "INVITE" ))
            	HandleInvite();
            else if( !cmd.AnsiCompareIC( "PING" ) && Parser.GetArgsCount() ) {
            	OrionScript::FireEvent( SE_PING, this, Parser.GetArg( 0 ).c_str() );
            	Sock->AsyncPrintf( "PONG %s\r\n", Parser.GetArg( 0 ).c_str() );
            } else if( !cmd.AnsiCompareIC( "PONG" ))
            	HandlePong();
            else if( !cmd.AnsiCompareIC( "WALLOPS" ))
            	HandleWallops();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNumerics( void )
{
	switch( Parser.GetNumeric() ) {

    	case 1:	// welcome
        	Connected();
            HandleServerInfo();
            break;

        case 4: // :irc1.tin.it 004 Wiz irc1.tin.it 2.10.3p3+hemp+p65v2 aoOirw abeiIklmnoOpqrstv
        	Session.ChanModes = Parser.GetArg( 4 );
        	HandleServerInfo();
            break;

        case 2:
        case 3:
        case 242:
        case 251:
        case 252:
        case 253:
        case 254:
        case 255:
        case 257:
        case 258:
        case 259:
        case 265:
        case 266:
        case 371:
        case 394:
        case 395:
        case 422:
        case 424:
        case 445:
        case 446:
        case 451:
        case 462:
        case 463:
        case 464:
        case 465:
        case 481: // no privileges - not an oper
        case 483: // can't kill a server
        case 484: // restricted
        case 485: // not the original chan op
        case 491: // no O lines
        case 501: // unknown mode flag
        case 502: // can't change mode for other users
        	HandleServerInfo();
        	break;

        case 5: // I support
			HandleISupport();
            break;

    	case 375:
        case 372:
        	HandleMOTD();
        	break;

        case 376:	// end of motd
        	HandleMOTD();
            MainWin->Ident->RemInfo( Sock->GetLocalPort() );
            break;

        case 353: // names
        	HandleNames( true );
            break;

        case 366: // end of names
        	HandleNames( false );
            break;

        case 324: // channel mode is
        	HandleChannelModeIs();
            break;

        case 433: // nick in use
        	HandleNickInUse();
            break;

        case 332: // topic
        	HandleTopic();
        	break;

		case 333: // topic set by
        	HandleTopicSetBy();
			break;

		case 303: // ison
        	HandleIsOn();
            break;

		case 305: // unaway
        	Session.Flags.Clear( IRC_SESS_AWAY );
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ), IRC_INFO );
            break;

		case 306: // away
        	Session.Flags.Set( IRC_SESS_AWAY );
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ), IRC_INFO );
            break;

    	case 311:
           	BroadcastUserHost( Parser.GetArg( 1 ), Parser.GetArg( 2 ) + "@" + Parser.GetArg( 3 ));
        	DisplayOnActiveWin( NULL,
            					Parser.GetArg( 1 ) + " is " + Parser.GetArg( 2 ) + "@" +
                                Parser.GetArg( 3 ) + " " + Parser.GetArg( 5 ),
                                IRC_WHOIS );
        	break;

        case 312:
        	DisplayOnActiveWin( NULL,
            					Parser.GetArg( 1 ) + " is using " + Parser.GetArg( 2 ) +
            					" " + Parser.GetArg( 3 ),
                                IRC_WHOIS );
        	break;

    	case 313:
        case 310: // user modes
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + " " + Parser.GetArg( 2 ), IRC_WHOIS );
        	break;

    	case 301: // user is away
        	DisplayOnActiveWin( "첔WAY�", Parser.GetArg( 1 ) + " is away: " + Parser.GetArg( 2 ), IRC_WHOIS );
        	break;

        case 302: // user host
//			HandleUserHost();
        	break;

    	case 314:
        	DisplayOnActiveWin( "첳HOWAS�",
            					Parser.GetArg( 1 ) + " was " + Parser.GetArg( 2 ) + "@" +
                                Parser.GetArg( 3 ) + " " + Parser.GetArg( 4 ),
                                IRC_WHOIS );
        	break;

    	case 317:
        	HandleWhoisIdle();
        	break;

        case 319:
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + " is on " + Parser.GetArg( 2 ), IRC_WHOIS );
        	break;

        case 340: // user ip
             DisplayOnActiveWin( "첱SERIP�", Parser.GetArg( 1 ), IRC_INFO );
             break;

        case 341: // inviting
        	if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), Parser.GetArg( 1 ), IGN_INVITE ))
	        	DisplayOnActiveWin( NULL, Parser.GetArg( 2 ) + " has been invited to " + Parser.GetArg( 1 ), IRC_INVITE );
        	break;

        case 381:
        	DisplayOnActiveWin( NULL, "You are now an IRC operator", IRC_INFO );
        	break;

        case 401: // no such nick
        case 402: // no such server
        case 403: // no such channel
        case 406: // there was no such nick name
        case 408: // no such service
        case 467: // key sey
        case 471: // channel full
        case 472: // unknown mode
        case 473: // invite only
        case 474: // banned
        case 475: // bad key
        case 476: // bad channel mask
        case 477: // channel doesn't support modes
        case 263: // try again
        case 256: // admin info
        case 331: // no topic
        case 382: // rehashing
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + ": " + Parser.GetArg( 2 ), IRC_INFO );
        	break;

        case 404: // cannot send to channel
        case 405: // too many channels
        case 407: // too many targets
        case 432: // erroneous nickname
        case 436: // nick collision
        case 442: // not in that channel
        case 444: // not logged in
        case 460: // half-ops cannot set mode
        case 461: // not enough parameters
        case 482: // not channel operator
        case 499: // not channel owner
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + ": " + Parser.GetArg( 2 ), IRC_ERROR );
        	break;

		case 421: // unknown command
        	HandleUnknownCommand();
        	break;

        case 480: // cannot knock
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ), IRC_ERROR );
        	break;

        case 620: // dccallow help
        case 619: // end of dccallow help
        	DisplayOnActiveWin( "첞ELP�", Parser.GetArg( 1 ), IRC_INFO );
        	break;

        case 704: // begin of HELP
        case 705: // HELP text
        case 706: // end of HELP
        	DisplayOnActiveWin( "첞ELP�", Parser.GetArg( 2 ), IRC_INFO );
        	break;

        case 437: // nick/channel is temporarily unavailable
        	HandleNickTempUnavail();
        	break;

        case 441: // they aren't on that channel
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + " is/are not on " + Parser.GetArg( 2 ), IRC_ERROR );
        	break;

        case 443: // already on channel
        	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + " is/are already on " + Parser.GetArg( 2 ), IRC_ERROR );
        	break;

        case 478: // channel list is full
        	DisplayOnActiveWin( NULL,
            					Parser.GetArg( 1 ) + " " + Parser.GetArg( 3 ) + " (" +
                                Parser.GetArg( 2 ) + ")",
                                IRC_ERROR );
        	break;

        case 367: // ban list
        	HandleChanList( IRC_CHAN_LIST_BAN, false );
            break;

        case 368: // end of ban list
        	HandleChanList( IRC_CHAN_LIST_BAN, true );
            break;

        case 346: // invite list
        	HandleChanList( IRC_CHAN_LIST_INVITE, false );
            break;

        case 347: // end of invite list
        	HandleChanList( IRC_CHAN_LIST_INVITE, true );
            break;

        case 348: // except list
        	HandleChanList( IRC_CHAN_LIST_EXCEPT, false );
            break;

        case 349: // end of except list
        	HandleChanList( IRC_CHAN_LIST_EXCEPT, true );
            break;

        case 352: // who
        	HandleWho();
        	break;

        case 315: // end of who
        	HandleEndOfWho();
            break;

        case 321: // LIST start
        	ChannelList->BeginList();
        	break;

        case 322: // list channel
        	ChannelList->AddItem( Parser.GetArg( 1 ), Parser.GetArg( 2 ).ToIntDef( 0 ), Parser.GetArg( 3 ));
        	break;

        case 323: // LIST end
        	ChannelList->EndList();
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleUnknownCommand( void )
{
	AnsiString	cmd = Parser.GetArg( 1 ).UpperCase();

    if( cmd == "WHO" )
    	Session.Flags.Set( IRC_SESS_NO_WHO );

   	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + ": " + Parser.GetArg( 2 ), IRC_ERROR );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleISupport( void )
{
// :irc.edisontel.it 005 WizTest MAP PREFIX=(ov)@+ MODES=3 CHANTYPES=#&!+ MAXCHANNELS=20 NICKLEN=9 TOPICLEN=160 KICKLEN=160 NETWORK=IRCNet CHANMODES=beI,k,l,imnpsaqr :are supported by this server
	AnsiString	str;

	for( int i = 1; i < Parser.GetArgsCount(); i++ ) {
    	AnsiString	arg = Parser.GetArg( i ), value;

    	if( i > 1 )
        	str += " ";

    	str  += arg;
        value = "";

        if( int pos = arg.Pos( "=" )) {

        	value = arg.SubString( pos + 1, arg.Length() );

            arg.Delete( pos, arg.Length() );
        }

        if( arg == "MODES" )
        	Session.ModesCount = value.ToIntDef( Session.ModesCount );
        else if( arg == "CHANTYPES" )
        	Session.ChanTypes = value;
//        else if( arg == "CHANMODES" ) // not reliable, at least on IRCNet
//        	Session.ChanModes = value;
        else if( arg == "NETWORK" )
        	Session.Network = value;
    }

    MainWin->UpdateStatus();

    DisplayOnActiveWin( NULL, str, IRC_NORMAL );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNickInUse( void )
{
	// :irc.edisontel.it 433 * Wiz :Nickname is already in use.
	if( Session.Flags.IsSet( IRC_SESS_CONNECTING ))
    	TryNextNick();

    else {
    	AnsiString msg = Parser.GetArg( 1 ) + ": " + Parser.GetArg( 2 );

        DisplayOnActiveWin( NULL, msg, IRC_ERROR );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNickTempUnavail( void )
{
	DisplayOnActiveWin( NULL, Parser.GetArg( 1 ) + ": " + Parser.GetArg( 2 ), IRC_ERROR );

	if( Session.Flags.IsSet( IRC_SESS_CONNECTING ))
    	TryNextNick();
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::TryNextNick( void )
{
   	if( NickToTry < Server.GetNicksCount() ) {

    	Session.Nick = Server.GetNick( NickToTry++ );

		Sock->AsyncPrintf( "NICK %s\r\n", Session.Nick.c_str() );

    } else {

        Display->AddRow( NULL, "I've tried every configured nick, but they're all taken. You must choose another one...", IRC_LOCAL );

		Session.Flags.Set( IRC_SESS_CONNECTED ); // needed to handle /NICK

    	Input->Text = "/NICK ";
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleTopic( void )
{
	//:server 332 mynick channel :topic
	AnsiString	msg, target;

    target = Parser.GetArg( 1 );
    msg    = "Topic for " + target + " is \"" + Parser.GetArg( 2 ) + "\"";

    if( TChanWin *win = FindChannelWin( target )) {

	    OrionScript::FireEvent( SE_TOPIC, win, &Parser.GetOrigin(), Parser.GetArg( 2 ).c_str() );

    	win->SetTopic( Parser.GetArg( 2 ));
       	win->AddRow( NULL, msg, IRC_TOPIC );

    } else
    	DisplayOnActiveWin( NULL, msg, IRC_TOPIC );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleTopicSetBy( void )
{
	//:server 333 mynick channel nick!user@host timestamp
	AnsiString	msg, target, setby;
    time_t		datetime;
    char		date[256];
    HostMask	mask( Parser.GetArg( 2 ));

    datetime = (time_t)Parser.GetArg( 3 ).ToIntDef( 0 );

    strftime( date, sizeof( date ), "%#c", localtime( &datetime ));

    target = Parser.GetArg( 1 );
    setby  = mask.GetNick();

    if( !mask.GetUserHost().IsEmpty() )
		setby += " (" + mask.GetUserHost() + ")";

    msg = "Topic for " + target + " has been set by " + setby + " on " +
          AnsiString( date );

    if( TChanWin *win = FindChannelWin( target )) {

    	win->SetTopicSetBy( setby );
        win->SetTopicSetTime( AnsiString( date ));
       	win->AddRow( NULL, msg, IRC_TOPIC );

    } else
    	DisplayOnActiveWin( NULL, msg, IRC_TOPIC );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleTopicChange( void )
{
	// :origin TOPIC channel :topic
    if( TChanWin *win = FindChannelWin( Parser.GetArg( 0 ))) {
		AnsiString  setby;
    	char 		date[256];
        time_t  	dt;

        time( &dt );
	    strftime( date, sizeof( date ), "%#c", localtime( &dt ));

        setby = Parser.GetOrigin().GetNick();

        if( !Parser.GetOrigin().GetUserHost().IsEmpty() )
        	setby += " (" + Parser.GetOrigin().GetUserHost() + ")";

    	win->SetTopic( Parser.GetArg( 1 ));
    	win->SetTopicSetBy( setby );
        win->SetTopicSetTime( AnsiString( date ));

       	win->AddRow( NULL,
 					 Parser.GetOrigin().GetNick() + " has set the topic to \"" + Parser.GetArg( 1 ) + "\"",
                     IRC_TOPIC );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleMOTD( void )
{
	if( Server.Flags.IsSet( SERVF_SHOW_MOTD ))
		AddRow( NULL, Parser.GetArg( 1 ), IRC_MOTD );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNames( bool names )
{
	if( names ) {
		TChanWin *wnd = FindChannelWin( Parser.GetArg( 2 ));

    	AddRow( NULL, Parser.GetArg( 2 ) + " " + Parser.GetArg( 3 ), IRC_INFO, true );

		if( wnd )
    		wnd->UpdateNames( Parser.GetArg( 3 ));

    } else {
		TChanWin *wnd = FindChannelWin( Parser.GetArg( 1 ));

    	AddRow( NULL, Parser.GetArg( 2 ), IRC_INFO, true );

    	if( wnd )
        	wnd->EndOfNames();
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleChannelModeIs( void )
{
	TChanWin *wnd = FindChannelWin( Parser.GetArg( 1 ));

   	if( wnd )
       	wnd->ModeChange( Parser, false );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleServerInfo( void )
{
	AnsiString	str;

	for( int i = 1; i < Parser.GetArgsCount(); i++ ) {

    	if( i > 1 )
        	str += " ";

    	str += Parser.GetArg( i );
    }

    DisplayOnActiveWin( NULL, str, IRC_NORMAL );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandlePrivMsg( void )
{
    if( Parser.GetArgsCount() >= 2 ) {
    	AnsiString target = Parser.GetArg( 0 ), txt = Parser.GetArg( 1 );

        if(( txt[1] == 1 ) && ( txt[ txt.Length() ] == 1 ))
        	HandleCTCP( txt.SubString( 2, txt.Length() - 2 ));

    	else if( !target.AnsiCompareIC( Session.Nick ))
        	HandlePrivMsgQuery( "" );

        else if( TChanWin *win = FindChannelWin( target )) {
        	AnsiString	str = Parser.GetArg( 1 );

        	if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), str, IGN_CHANNEL ) &&
			    OrionScript::FireEvent( SE_TEXT, win, &Parser.GetOrigin(), str.c_str() ))
	        	win->AddRow( Parser.GetOrigin().GetNick().c_str(), str, IRC_NORMAL );

        } else // msgs to ops, voiced users or whatever
        	HandlePrivMsgQuery( Format( "%s[%s]%s ", ARRAYOFCONST(( TEXT_BOLD, target, TEXT_BOLD ))));
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandlePrivMsgQuery( AnsiString prefix )
{
    AnsiString	str = Parser.GetArg( 1 );

    if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), str, IGN_PRIVATE )) {
        TQueryWin *wnd = FindQueryWin( Parser.GetOrigin().GetNick() );

        if( OrionScript::FireEvent( SE_TEXT, wnd, &Parser.GetOrigin(), str.c_str() ))
            wnd->AddRow( Parser.GetOrigin().GetNick().c_str(), prefix + str, IRC_MSG, false, false );

    	MainWin->RecvdPrivMsg();
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNotice( void )
{
	// psybnc can send a notice asking for password before anything else
	Session.Flags.Set( IRC_SESS_CONNECTED );

    if( Parser.GetArgsCount() >= 2 ) {
    	AnsiString 	target = Parser.GetArg( 0 ), txt = Parser.GetArg( 1 );
        AnsiString	str = Parser.GetArg( 1 );

        if(( txt.Length() > 2 ) && ( txt[1] == 1 ) && ( txt[ txt.Length() ] == 1 ))
        	HandleCTCPResponse( txt.SubString( 2, txt.Length() - 2 ));

        else if( !Parser.GetOrigin().GetNick().AnsiCompareIC( Session.Server )) {

        	if( OrionScript::FireEvent( SE_SNOTICE, this, str.c_str() )) {
	        	AddRow( Parser.GetOrigin().GetNick().c_str(), str, IRC_NOTICE );
		        Prefs.GetSounds()->Play( SoundsMgr::NOTICE );
            }

    	} else if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), str, IGN_NOTICE ) &&
        		   OrionScript::FireEvent( SE_NOTICE, this, &Parser.GetOrigin(), str.c_str() ) &&
                   ( Server.GetEventDisplay( TServer::EV_NOTICE ) != TServer::HIDE )) {
        	TChanWin   *win;

	        Prefs.GetSounds()->Play( SoundsMgr::NOTICE );

            if( Server.GetEventDisplay( TServer::EV_NOTICE ) == TServer::SHOW_STATUS )
            	AddRow( NULL, "[" + Parser.GetOrigin().GetNick() + " => " + target + "] " + str, IRC_NOTICE );
            else {

                if( !target.AnsiCompareIC( Session.Nick )) {
                    TQueryWin *wnd = FindQueryWin( Parser.GetOrigin().GetNick(), false );

                    if( wnd )
                        wnd->AddRow( Parser.GetOrigin().GetNick().c_str(), str, IRC_NOTICE, false, true );
                    else
                        DisplayOnActiveWin( Parser.GetOrigin().GetNick().c_str(), str, IRC_NOTICE );

                } else if( win = FindChannelWin( target ))
                    win->AddRow( Parser.GetOrigin().GetNick().c_str(), str, IRC_NOTICE );
                else if(( target[ 1 ] == '@' ) && ( win = FindChannelWin( target.SubString( 2, target.Length() ))))
                    win->AddRow( Parser.GetOrigin().GetNick().c_str(), str, IRC_NOTICE );
                else
                    DisplayOnActiveWin( Parser.GetOrigin().GetNick().c_str(),
                                        "[" + target + "] " + str,
                                        IRC_NOTICE );
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleMode( void )
{
	AnsiString target = Parser.GetArg( 0 );

	if( TChanWin *wnd = FindChannelWin( target ))
    	wnd->ModeChange( Parser, true );
	else {
    	AnsiString	modes, away, tmp = Parser.GetArg( 1 );
        bool		set = false;

        for( int i = 1; i <= tmp.Length(); i++ )
        	switch( tmp[ i ]) {

            	case '+':
                	set = true;
                    break;

                case '-':
                	set = false;
                    break;

                case 'i':
                	if( !set )
                    	modes += "not ";

                    modes += "invisible, ";
                    break;

                case 'r':
                	modes += "restricted, ";
                    break;

                case 'w':
                	if( !set )
                    	modes += "don't ";

                	modes += "receive wallops, ";
                    break;

                case 'o':
                	if( !set )
                    	modes += "not ";

                    modes += "operator, ";
                    break;

                case 'O':
                	if( !set )
                    	modes += "not ";

                    modes += "local operator, ";
                	break;

                case 'a':
                	away = set ? " is away" : " has returned";
                    break;

                case 's':
                	if( !set )
                    	modes += "don't ";

                    modes += "receive server notices, ";
                    break;
        	}

        if( !modes.IsEmpty() )
	    	AddRow( NULL, Session.Nick + " sets user mode(s) " + modes.SubString( 1, modes.Length() - 2 ), IRC_INFO );

        if( !away.IsEmpty() )
	    	AddRow( NULL, Session.Nick + away, IRC_INFO );

        OrionScript::FireEvent( SE_USERMODE, this, tmp.c_str() );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleJoin( void )
{
	AnsiString 	target = Parser.GetArg( 0 );
    TChanWin   *wnd = FindChannelWin( target );

   	if( !Session.Nick.AnsiCompareIC( Parser.GetOrigin().GetNick() )) {

    	SendCmd( "MODE " + target );

        if( !wnd ) {

        	wnd = new TChanWin( this, this, target );

            Channels->Add( wnd );

        } else
        	wnd->Joined();

    } else if( wnd )	// other user has joined
    	wnd->UserJoin( Parser.GetOrigin() );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandlePart( void )
{
	AnsiString  target = Parser.GetArg( 0 );
   	TChanWin   *wnd = FindChannelWin( target );

   	if( !Session.Nick.AnsiCompareIC( Parser.GetOrigin().GetNick() )) {

    	if( wnd ) {

        	if( !wnd->ChanData.Flags.IsSet( IRC_CHAN_FLAG_DONT_CLOSE )) {

        		wnd->CloseNow();
	            Channels->Remove( wnd );

    	    } else
            	wnd->ChanData.Flags.Clear( IRC_CHAN_FLAG_DONT_CLOSE );
    	}

    } else if( wnd ) // other user has parted
    	wnd->UserPart( Parser.GetOrigin(), Parser.GetArg( 1 ), false );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleQuit( void )
{
   	if( !Session.Nick.AnsiCompareIC( Parser.GetOrigin().GetNick() )) {

    	Session.Flags.Clear( IRC_SESS_CONNECTED );

        MainWin->Ident->RemInfo( Sock->GetLocalPort() );

    } else { // other user has quit

    	for( int i = 0; i < Channels->Count; i++ )
    		((TChanWin *)Channels->Items[ i ])->UserPart( Parser.GetOrigin(), Parser.GetArg( 0 ), true );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleError( void )
{
   	Session.Flags.Clear( IRC_SESS_CONNECTED );
    DisplayOnActiveWin( NULL, Parser.GetArg( 0 ), IRC_ERROR );
    OrionScript::FireEvent( SE_ERROR, this, Parser.GetArg( 0 ).c_str() );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleNick( void )
{
	if( Parser.GetOrigin().GetNick() == Session.Nick ) {

    	Session.Nick = Parser.GetArg( 0 );

	    MainWin->UpdateStatus();
    }

	if( Channels->Count ) {

		for( int i = 0; i < Channels->Count; i++ )
    		((TChanWin *)Channels->Items[ i ])->NickChange( Parser.GetOrigin().GetNick(), Parser.GetArg( 0 ));

		for( int i = 0; i < Queries->Count; i++ )
    		((TQueryWin *)Queries->Items[ i ])->NickChange( Parser.GetOrigin().GetNick(), Parser.GetArg( 0 ));

    } else
    	AddRow( NULL, Parser.GetOrigin().GetNick() + " is now known as " + Parser.GetArg( 0 ), IRC_NICK );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleKick( void )
{
 	if( TChanWin *win = FindChannelWin( Parser.GetArg( 0 )))
    	win->Kick( Parser.GetOrigin(), Parser.GetArg( 1 ), Parser.GetArg( 2 ));
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleCTCP( AnsiString origCtcp )
{
	AnsiString	cmd, ctcp, sender;
    int			pos = origCtcp.Pos( " " );

    if( pos ) {

    	cmd  = origCtcp.SubString( 1, pos - 1 );
        ctcp = origCtcp.SubString( pos + 1, origCtcp.Length() - pos );

    } else {

        ctcp = origCtcp;
    	cmd  = ctcp;
    }

    sender = Parser.GetOrigin().GetNick();

    if( !cmd.AnsiCompareIC( "VERSION" ) ||
    	OrionScript::FireEvent( SE_CTCP, this, &Parser.GetOrigin(), origCtcp.c_str() )) {

        if( cmd.AnsiCompareIC( "ACTION" ) &&
            cmd.AnsiCompareIC( "DCC" ) &&
            cmd.AnsiCompareIC( "EYELIN" ) &&
            !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), cmd, cmd.AnsiCompareIC( "PAGE" ) ? IGN_CTCP : IGN_PAGE )) {

            DisplayOnActiveWin( NULL, "Received CTCP " + cmd + " from " + sender, IRC_CTCP );

            if( Session.ActiveWin != this )
                AddRow( NULL, "Received CTCP " + cmd + " from " + sender, IRC_CTCP );
        }

        if( !cmd.AnsiCompareIC( "DCC" ))
            HandleDCCOffer( ctcp );

        else if( !cmd.AnsiCompareIC( "PAGE" ))
            HandleCTCPPage( ctcp );

        else if( !cmd.AnsiCompareIC( "VERSION" ))
            SendCmd( "NOTICE " + sender + " :\001VERSION " + Version.GetProductName() + " " + Version.GetVersionStr() + " " + Version.GetCopyright() + " "PROD_EMAIL"\001" );

        else if( !cmd.AnsiCompareIC( "PING" ))
            SendCmd( "NOTICE " + sender + " :\001PING " + ctcp + "\001" );

        else if( !cmd.AnsiCompareIC( "SOURCE" ))
            SendCmd( "NOTICE " + sender + " :\001SOURCE " + Version.GetProductName() + " can be found on "PROD_URL"\001" );

        else if( !cmd.AnsiCompareIC( "TIME" ))
            SendCmd( "NOTICE " + sender + " :\001TIME " + TDateTime::CurrentDateTime().FormatString( "dddd, dd mmmm yyyy, hh:nn:ss" ) + "\001" );

        else if( !cmd.AnsiCompareIC( "FINGER" ))
            SendCmd( "NOTICE " + sender + " :\001FINGER " + Server.GetFingerMsg() + "\001" );

        else if( !cmd.AnsiCompareIC( "CLIENTINFO" ))
            SendCmd( "NOTICE " + sender + " :\001CLIENTINFO VERSION PING CLIENTINFO SOURCE TIME FINGER DCC\001" );

        else if( !cmd.AnsiCompareIC( "ACTION" ))
            HandleCTCPAction( ctcp );

        else if( !cmd.AnsiCompareIC( "EYELIN" ))
            Eyelin::GetInstance()->HandleCTCP( this, Parser.GetOrigin(), ctcp );

        else
            SendCmd( "NOTICE " + sender + " :\001ERRMSG Unknown CTCP command (" + cmd + ")\001" );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleCTCPAction( AnsiString ctcp )
{
    if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), ctcp, IGN_ACTION )) {
    	AnsiString sender = Parser.GetOrigin().GetNick();

        if( TChanWin *win = FindChannelWin( Parser.GetArg( 0 ))) {

        	if( OrionScript::FireEvent( SE_ACTION, win, &Parser.GetOrigin(), ctcp.c_str() ))
	            win->AddRow( NULL, sender + " " + ctcp, IRC_ACTION );

        } else if( TQueryWin *win2 = FindQueryWin( sender )) {

        	if( OrionScript::FireEvent( SE_ACTION, win2, &Parser.GetOrigin(), ctcp.c_str() ))
	            win2->AddRow( NULL, sender + " " + ctcp, IRC_ACTION, false, false );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleCTCPPage( AnsiString page )
{
	TPageWhen when = Prefs.GetPageWhen();

	if(( when != PAGE_NEVER ) && !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), page, IGN_PAGE )) {

    	if( Session.Flags.IsSet( IRC_SESS_AWAY ) || ( when == PAGE_ALWAYS )) {

			PlaySound( Prefs.GetPagerSound().c_str(), NULL, SND_ASYNC | SND_FILENAME );

	        DisplayOnActiveWin( "첧AGER�", Parser.GetOrigin().GetNick() + " is paging you", IRC_CTCP );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleDCCOffer( AnsiString dcc )
{
	if( OrionScript::FireEvent( SE_DCC, this, &Parser.GetOrigin(), dcc.c_str() )) {
        int				pos;
        TStringList    *args = new TStringList();
        bool			malformed = true;

        while( pos = dcc.Pos( " " )) {

            args->Add( dcc.SubString( 1, pos - 1 ));

            dcc.Delete( 1, pos );
        }

        if( !dcc.IsEmpty() )
            args->Add( dcc );

        if( args->Count >= 4 ) {
            Prom_Addr	addr;
            int			port;

            GetDCCAddr( args->Strings[ 2 ], &addr );

            port = args->Strings[ 3 ].ToIntDef( 0 );

            if( !args->Strings[ 0 ].AnsiCompareIC( "CHAT" ) &&
                !args->Strings[ 1 ].AnsiCompareIC( "chat" )) {

                malformed = HandleDCCChat( dcc, &addr, port );

            } else if( !args->Strings[ 0 ].AnsiCompareIC( "CHAT" ) &&
                       !args->Strings[ 1 ].SubString( 1, 6 ).AnsiCompareIC( "EYELIN" )) {

                malformed = Eyelin::GetInstance()->HandleDCC( this, args->Strings[ 1 ],
                                                              Parser.GetOrigin().GetNick(),
                                                              &addr );

            } else if( !args->Strings[ 0 ].AnsiCompareIC( "SEND" )) {

                malformed = false;

                if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), dcc, IGN_DCC_FILE ))
                    HandleDCCSend( args, &addr, port );

            } else if( !args->Strings[ 0 ].AnsiCompareIC( "RESUME" )) {

                malformed = false;

                MainWin->NeedDCCMgr();

                if( DCCMgrWin->SetResumeFrom( args->Strings[ 2 ].ToIntDef( 0 ), args->Strings[ 3 ].ToIntDef( 0 )))
                    SendCmd( "PRIVMSG " + Parser.GetOrigin().GetNick() + " :\001DCC ACCEPT " +
                             args->Strings[ 1 ] + " " + args->Strings[ 2 ] + " " +
                             args->Strings[ 3 ] + "\001" );

            } else if( !args->Strings[ 0 ].AnsiCompareIC( "ACCEPT" )) {

                malformed = false;

                MainWin->NeedDCCMgr();
                DCCMgrWin->Resume( args->Strings[ 2 ].ToIntDef( 0 ));
            }
        }

        if( malformed ) {

            DisplayOnActiveWin( NULL,
                                "Malformed DCC request from " +
                                Parser.GetOrigin().GetNick() + ": " +
                                Parser.GetArg( 1 ).SubString( 2, Parser.GetArg( 1 ).Length() - 2 ),
                                IRC_ERROR );

            SendCmd( "NOTICE " + Parser.GetOrigin().GetNick() + " :\001ERRMSG Malformed DCC command\001" );
        }

        delete args;
    }
}
//---------------------------------------------------------------------------
bool __fastcall	TServerWin::HandleDCCChat( AnsiString dcc, Prom_Addr *addr, int port )
{
    if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), dcc, IGN_DCC_CHAT ))
        switch( Prefs.GetChatReqAction() ) {

            case DCC_ACCEPT:
                FindQueryWin( Parser.GetOrigin().GetNick() )->ConnectDCC( *addr, port );
                break;

            case DCC_ASK: {
                TDCCChatOfferWin   *wnd = new TDCCChatOfferWin( this, TDCCChatOfferWin::CHAT,
                                                                addr, port, Parser.GetOrigin() );

				Prefs.GetSounds()->Play( SoundsMgr::DCC_OFFER );
                wnd->Show();
            } 	break;
        }

    return( false );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleDCCSend( TStringList *args, Prom_Addr *addr, int port )
{
    if( Prefs.GetFileFilters()->IsAcceptable( args->Strings[ 1 ] )) {
    	bool ask = false;

        switch( Prefs.GetSendReqAction() ) {

            case DCC_ACCEPT: {
            	bool		exists, ignore = false;
                AnsiString  file = Prefs.GetDownloadsDir( args->Strings[ 1 ] ) + args->Strings[ 1 ];
                int			size = args->Strings[ 4 ].ToIntDef( 0 );

                exists = FileExists( file );

                if( exists )
                	switch( Prefs.GetRecvAction() ) {

                    	case DCC_RECV_ASK:
                        	ask    = true;
                            ignore = true;
                            break;

                        case DCC_RECV_OVERWRITE:
                        	exists = false;
                            break;

                        case DCC_RECV_RESUME:
                        	ignore = TDCCTransfer::GetFileSize( file ) == size;
                        	break;
                    }

                if( !ignore ) {

                	MainWin->NeedDCCMgr();

	                DCCMgrWin->Receive( this, Parser.GetOrigin().GetNick(), file,
    	                                size, addr, port,
        	                            exists );
                }
            }	break;

            case DCC_ASK:
            	ask = true;
            	break;
        }

        if( ask ) {
            TDCCSendOfferWin   *wnd = new TDCCSendOfferWin( this, addr, port,
            												Parser.GetOrigin(),
                                                            args->Strings[ 1 ],
                                                            args->Strings[ 4 ].ToIntDef( 0 ));

			Prefs.GetSounds()->Play( SoundsMgr::DCC_OFFER );
            wnd->Show();
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::GetDCCAddr( AnsiString str, Prom_Addr *addr )
{
	int	ip;

    sscanf( str.c_str(), "%u", &ip );

    addr->s_addr = (( ip & 0xff000000 ) >> 24 ) |
    			   (( ip & 0x00ff0000 ) >> 8 ) |
				   (( ip & 0x0000ff00 ) << 8 ) |
				   (( ip & 0x000000ff ) << 24 );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleCTCPResponse( AnsiString ctcp )
{
	if( OrionScript::FireEvent( SE_CTCPREPLY, this, &Parser.GetOrigin(), ctcp.c_str() )) {
        AnsiString	cmd, msg;
        int			pos = ctcp.Pos( " " );

        if( pos ) {

            cmd  = ctcp.SubString( 1, pos - 1 );
            ctcp = ctcp.SubString( pos + 1, ctcp.Length() - pos );

        } else
            cmd = ctcp;

        msg = "CTCP " + cmd + " reply: ";

        if( cmd == "PING" )
            msg += FormatTimeInterval( time( NULL ) - ctcp.ToIntDef( 0 ));
        else
            msg += ctcp;

        if( !Prefs.GetIgnore()->Ignore( Parser.GetOrigin(), msg, IGN_CTCP ))
            DisplayOnActiveWin( Parser.GetOrigin().GetNick().c_str(), msg, IRC_CTCP );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleWallops( void )
{
	if( OrionScript::FireEvent( SE_WALLOPS, this, &Parser.GetOrigin(), Parser.GetArg( 0 ).c_str() ))
		DisplayOnActiveWin( NULL, Parser.GetArg( 0 ), IRC_WALLOP );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleWhoisIdle( void )
{
	int			idle;
    time_t  	signon;
    AnsiString  msg;

    idle   = Parser.GetArg( 2 ).ToIntDef( 0 );
    signon = Parser.GetArg( 3 ).ToIntDef( 0 );

    msg = Parser.GetArg( 1 ) + " has been idle " + FormatTimeInterval( idle );

    if( signon ) {
    	char	buf[ 256 ];

        strftime( buf, sizeof( buf ), "%#c", localtime( &signon ));

        msg += ", signed on " + AnsiString( buf );
    }

	DisplayOnActiveWin( NULL, msg, IRC_WHOIS );
}
//---------------------------------------------------------------------------
AnsiString __fastcall TServerWin::FormatTimeInterval( int secs )
{
    AnsiString  msg;
   
	if( secs > 0 ) {
        int mins, hours, days;

        days  = secs / ( 3600 * 24 );
        secs -= days * 3600 * 24;
        hours = secs / 3600;
        secs -= hours * 3600;
        mins  = secs / 60;
        secs -= mins * 60;

        if( days ) {

            msg += IntToStr( days ) + " day";

            if( days > 1 )
                msg += "s";
        }

        if( hours ) {

            if( days )
                msg += ", ";

            msg += IntToStr( hours ) + " hour";

            if( hours > 1 )
                msg += "s";
        }

        if( mins ) {

            if( days || hours )
                msg += ", ";

            msg += IntToStr( mins ) + " minute";

            if( mins > 1 )
                msg += "s";
        }

        if( secs ) {

            if( days || hours || mins )
                msg += ", ";

            msg += IntToStr( secs ) + " sec";

            if( secs > 1 )
                msg += "s";
        }

    } else
    	msg = "0 secs";

    return( msg );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::OnCommand( System::TObject *Sender, AnsiString msg )
{
	AnsiString cmd = "/" + msg;

	if( OrionScript::FireEvent( SE_INPUT, this, cmd.c_str() ))
		HandleCommand( this, NULL, msg );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::OnMessage( System::TObject *Sender, AnsiString msg )
{
	if( OrionScript::FireEvent( SE_INPUT, this, msg.c_str() ))
		AddRow( NULL, "You cannot talk in this window!", IRC_ERROR, true );
}
//---------------------------------------------------------------------------
TChanWin * __fastcall TServerWin::FindChannelWin( AnsiString chan )
{
	TChanWin *ret = NULL;

    for( int i = 0; !ret && ( i < Channels->Count ); i++ ) {
    	TChanWin *win = (TChanWin *)Channels->Items[ i ];

        if( !chan.AnsiCompareIC( win->GetChannel() ))
        	ret = win;
	}

    return( ret );
}
//---------------------------------------------------------------------------
TQueryWin * __fastcall TServerWin::FindQueryWin( AnsiString target, bool open, bool skipDCC )
{
	TQueryWin *ret = NULL;

    for( int i = 0; !ret && ( i < Queries->Count ); i++ ) {
    	TQueryWin *win = (TQueryWin *)Queries->Items[ i ];

        if( !target.AnsiCompareIC( win->GetTargetNick() ) &&
        	( !skipDCC || !win->DCCInProgress() ))
        	ret = win;
	}

    if( !ret && open ) {

		ret = new TQueryWin( this, this, target );

        Queries->Add( ret );
    }

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ClosingChannelWin( TChanWin *wnd )
{
   	Channels->Remove( wnd );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ClosingQueryWin( TQueryWin *wnd )
{
   	Queries->Remove( wnd );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::SendMsg( AnsiString target, AnsiString msg )
{
	int max = IRC_MAX_MSG_LENGTH - 4 - sizeof( "PRIVMSG  " ) -
    		  target.Length() - Session.Hostmask.Length();
    int start = 1;

    do {
    	AnsiString s = msg.SubString( start, max );

		SendCmd( "PRIVMSG " + target + " :" + s );

        start += s.Length();

    } while( start <= msg.Length() );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::DisplayOnActiveWin( const char *nick, AnsiString msg, TIRCEvent type )
{
	if(( Session.ActiveWin == this ) || !Session.ActiveWin )
    	AddRow( NULL, msg, type );

    if( TChanWin *chan = dynamic_cast<TChanWin *>( Session.ActiveWin ))
    	chan->AddRow( nick, msg, type );

    else if( TQueryWin *query = dynamic_cast<TQueryWin *>( Session.ActiveWin ))
    	query->AddRow( nick, msg, type, false, true );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::TimerTimer(TObject *Sender)
{
	if( Prefs.UseLagMeter() && Session.Flags.IsSet( IRC_SESS_CONNECTED )) {

		if( !Flags.IsSet( SERVF_LAG_PROBE_SENT )) {

    		Sock->Printf( "PING LAG%u\r\n", time( &LagProbeTime ));

        	Flags.Set( SERVF_LAG_PROBE_SENT );

        } else
        	Session.Lag += Timer->Interval / 1000;
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandlePong( void )
{
	if( Flags.IsSet( SERVF_LAG_PROBE_SENT )) {
    	time_t	now;

        time( &now );

        Session.Lag = now - LagProbeTime;

    	Flags.Clear( SERVF_LAG_PROBE_SENT );

    } else if( OrionScript::FireEvent( SE_PONG, this, Parser.GetArg( 0 ).c_str() ))
    	DisplayOnActiveWin( NULL, "Received PING reply from the server", IRC_INFO );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleInvite( void )
{
	// :origin INVITE nick channel
    if( OrionScript::FireEvent( SE_INVITE, this, &Parser.GetOrigin(), Parser.GetArg( 1 ))) {
    	bool		hide = false;
        AnsiString	msg = Parser.GetOrigin().GetNick() + " (" + Parser.GetOrigin().GetUserHost() +
                          ") has invited you to " + Parser.GetArg( 1 );

        switch( Server.GetEventDisplay( TServer::EV_INVITE )) {

        	case TServer::SHOW_CHANNEL:
                DisplayOnActiveWin( NULL, msg, IRC_INVITE );
        		break;

        	case TServer::SHOW_STATUS:
                AddRow( NULL, msg, IRC_INVITE );
        		break;

            case TServer::HIDE:
            	hide = true;
            	break;
    	}

    	if( !hide )
	        Prefs.GetSounds()->Play( SoundsMgr::INVITE );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleChanList( TIRCChanList list, bool end )
{
	AnsiString		target, item;
    TChanWin   	   *wnd;
    TChanPropWin   *prop;

    target = Parser.GetArg( 1 );
    item   = Parser.GetArg( 2 );

    wnd = FindChannelWin( target );

    if( wnd && ( prop = wnd->GetChanPropWin() )) {

		if( !end ) {
            time_t		datetime;
            char		date[256];

            datetime = (time_t)Parser.GetArg( 4 ).ToIntDef( 0 );

            if( datetime )
                strftime( date, sizeof( date ), "%c", localtime( &datetime ));
            else
                date[ 0 ] = '\0';

            prop->AddListItem( list, item, Parser.GetArg( 3 ), AnsiString( date ));
        }

    } else
    	DisplayOnActiveWin( NULL, target + ": " + item, IRC_INFO );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::SendCmd( AnsiString cmd )
{
	CmdList->Add( cmd );

    if( !ThrottleTimer->Enabled )
    	ThrottleTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ThrottleTimerTimer(TObject *Sender)
{
	if( CmdList->Count ) {
		SendUrgCmd( CmdList->Strings[ 0 ]);
		CmdList->Delete( 0 );
    }

	if( !CmdList->Count )
    	ThrottleTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::SendUrgCmd( AnsiString cmd )
{
	cmd += "\r\n";

	Sock->AsyncSend( cmd.c_str(), cmd.Length() );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::ClearSendQueue( void )
{
	CmdList->Clear();

    ThrottleTimer->Enabled = false;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TServerWin::GetMyIPForDCC( void )
{
	char  			buf[ 256 ];
    Prom_Addr   	addr;
    unsigned int    ip;

    Sock->NameToAddr( Sock->GetLocalName(), &addr );

    ip = (( addr.s_addr & 0xff000000 ) >> 24 ) |
    	 (( addr.s_addr & 0x00ff0000 ) >> 8 ) |
         (( addr.s_addr & 0x0000ff00 ) << 8 ) |
         (( addr.s_addr & 0x000000ff ) << 24 );

    sprintf( buf, "%u", ip );

    return( AnsiString( buf ));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::DCCSend( AnsiString target, AnsiString file )
{
	if( file.IsEmpty() && OpenDlg->Execute() ) {

    	for( int i = 0; i < OpenDlg->Files->Count; i++ )
        	DCCSend( target, OpenDlg->Files->Strings[ i ]);

    } else if( !file.IsEmpty() ) {

    	if( FileExists( file )) {

        	MainWin->NeedDCCMgr();
    		DCCMgrWin->Send( this, target, file );

        } else
        	DisplayOnActiveWin( NULL, "DCC send error: \"" + file + "\" does not exists!", IRC_ERROR );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::GetHostForDNS( AnsiString user )
{
	SendCmd( "WHO " + user );
    WhoActions->Add( new TWHOAction( SERV_WHO_DNS ));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::GetHostForIgnore( AnsiString user, int flags )
{
	SendCmd( "WHO " + user );
    WhoActions->Add( new TWHOAction( SERV_WHO_IGNORE, reinterpret_cast<void *>( flags )));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::WhoForWhois( AnsiString user )
{
	SendCmd( "WHO " + user );
    WhoActions->Add( new TWHOAction( SERV_WHO_WHOIS, user ));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::WhoForQuery( AnsiString user )
{
	SendCmd( "WHO " + user );
    WhoActions->Add( new TWHOAction( SERV_WHO_QUERY, user ));
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleWho( void )
{
	AnsiString	chan, user, host, server, nick, flags, hops, name;

    chan   = Parser.GetArg( 1 );
    user   = Parser.GetArg( 2 );
    host   = Parser.GetArg( 3 );
    server = Parser.GetArg( 4 );
    nick   = Parser.GetArg( 5 );
    flags  = Parser.GetArg( 6 );
    name   = Parser.GetArg( Parser.GetArgsCount() - 1 );
    hops   = name.SubString( 1, name.Pos( " " ) - 1 );

    // not sure whether it's needed, can't bother trying
    // to interpret the RFC on this point
    for( int i = 7; i < Parser.GetArgsCount() - 1; i++ )
    	flags += " " + Parser.GetArg( i );

    name.Delete( 1, name.Pos( " " ));

    if( WhoActions->Count > 0 ) {
        TWHOAction *action = reinterpret_cast<TWHOAction *>( WhoActions->First() );

        switch( action->Action ) {

            case SERV_WHO_DNS:
                MainWin->DoDNS( host, this );
                break;

            case SERV_WHO_IGNORE:
                IgnoreUser( HostMask( nick, user, host ), (int)action->Data );
                break;

            case SERV_WHO_MY_HOSTMASK:
                Session.Hostmask = nick + "!" + user + "@" + host;
                break;

            case SERV_WHO_WHOIS:
            	SendCmd( "WHOIS " + server + " " + nick );
                action->Str = "";
            	break;

            case SERV_WHO_NOTHING:
            case SERV_WHO_QUERY: // everything is done by BroadcastUserHost below
                break;
        }

    } else
        DisplayOnActiveWin( "첳HO�",
                            nick + " (" + user + "@" + host + ") [" + name + "]: " +
                            server + " (" + hops + " hops) [" + flags + "]",
                            IRC_WHOIS );

    BroadcastUserHost( nick, user + "@" + host );
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::HandleEndOfWho( void )
{
	if( WhoActions->Count <= 0 )
    	DisplayOnActiveWin( "첳HO�", "End of WHO list", IRC_WHOIS );
    else {
        TWHOAction *action = reinterpret_cast<TWHOAction *>( WhoActions->First() );

        switch( action->Action ) {

			case SERV_WHO_WHOIS:
            	if( !action->Str.IsEmpty() )
                	SendCmd( "WHOIS " + action->Str );
            	break;
        }

    	delete action;
    	WhoActions->Delete( 0 );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::ReconnectTimerTimer(TObject *Sender)
{
	Session.Server = Server.Host;
    Session.Port   = Server.Port.ToIntDef( 0 );

    Session.Flags.Set( IRC_SESS_CONNECTING );

    AddRow( NULL,
    		"Connecting to " + Session.Server + ":" + IntToStr( Session.Port ) + "...",
            IRC_LOCAL,
            true );

	AsyncReq = WSAAsyncGetHostByName( Handle, WM_RESOLVED, Session.Server.c_str(), HostEnt, sizeof( HostEnt ));

    ReconnectTimer->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::PrefsChanged( void )
{
	SetFonts();

    Display->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::AddRow( const char *nick, AnsiString msg, TIRCEvent type, bool skipfilters )
{
	MainWin->URLCatcher.Filter( nick ? nick : "-", msg );

	if( skipfilters || Prefs.GetFilters()->Process( nick, msg, type, this )) {

        if( Prefs.StripColors() )
            msg = IRCUtils::StripColors( msg );

		Display->AddRow( nick, msg, type );
        Log.Log( nick, msg, type );

    	if( !Active )
    		TaskbarButton->Font->Color = ( type == IRC_HIGHLIGHT ) ? clGreen : clRed;
    }
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::Disconnect1Click(TObject *Sender)
{
	HandleCommand( this, NULL, "QUIT" );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::LogMenuClick(TObject *Sender)
{
	if( Log.IsOpen() )
    	Log.Close();
    else
    	Log.Open( Session.Server, "ServerStatus" );

    LogMenu->Checked = Log.IsOpen();

    if( Log.IsOpen() )
    	Display->AddRow( NULL, "Opened log file \"" + Log.GetLogFile() + "\"", IRC_LOCAL );
    else
    	Display->AddRow( NULL, "Log closed", IRC_LOCAL );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::BroadcastUserHost( AnsiString user, AnsiString host )
{
    for( int i = 0; i < Channels->Count; i++ ) {
    	TChanWin *win = (TChanWin *)Channels->Items[ i ];

        win->SetUserHost( user, host );
	}

    for( int i = 0; i < Queries->Count; i++ ) {
    	TQueryWin *win = (TQueryWin *)Queries->Items[ i ];

        win->SetUserHost( user, host );
	}
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::HandleIsOn( void )
{
	//:server 303 yournick :nick nick nick
	AnsiString nicks = Parser.GetArg( 1 );

    NotifyHandler.BeginUpdate();

    do {
        int 		pos = nicks.Pos( " " );
        AnsiString  nick;

        if( pos ) {

        	nick = nicks.SubString( 1, pos - 1 );

            nicks.Delete( 1, pos );

        } else {
        	nick  = nicks;
            nicks = "";
        }

        if( !nick.IsEmpty() )
	        NotifyHandler.IsOn( this, nick );

    } while( !nicks.IsEmpty() );

    NotifyHandler.EndUpdate( this );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::IgnoreUser( HostMask user, int flags )
{
	TIgnore    *ign = new TIgnore();
    AnsiString 	mask = user.GetMask( MASK_USER_TLD );
    int	 		events = 0;

    if( flags & IGNORE_PUB )
    	events |= IGN_CHANNEL;

    if( flags & IGNORE_MSG )
        events |= IGN_PRIVATE | IGN_NOTICE | IGN_DCC_CHAT;

    if( flags & IGNORE_CTCP )
    	events |= IGN_CTCP | IGN_INVITE | IGN_DCC_FILE | IGN_ACTION | IGN_PAGE;

    ign->SetMask( mask );
    ign->SetEvents( events );
    ign->SetExcluded( false );

	Prefs.GetIgnore()->Add( ign );
    Prefs.Save();

    DisplayOnActiveWin( "첟GNORE�", mask + " is now being ignored", IRC_LOCAL );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::Savebuffer1Click(TObject *Sender)
{
	Display->SaveBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::AddWhoAction( int action )
{
    WhoActions->Add( new TWHOAction( action ));
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::Clearbuffer1Click(TObject *Sender)
{
	Display->Clear();
}
//---------------------------------------------------------------------------
void __fastcall	TServerWin::DisableChannels( void )
{
    for( int i = 0; i < Channels->Count; i++ ) {
    	TChanWin *win = reinterpret_cast<TChanWin *>( Channels->Items[ i ] );

        win->ShowDisabled( true );
	}
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::Search1Click(TObject *Sender)
{
	TBufferSearchWin *wnd = new TBufferSearchWin( NULL, Display );

    wnd->ShowModal();

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::DisplayDblClick( TObject *Sender )
{
	Prefs.GetDblClickAction( DBL_CLICK_SERVER ).Execute( this );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Shift.Contains( ssAlt ))
    	MainWin->AltDown( true, Key );
}
//---------------------------------------------------------------------------
void __fastcall TServerWin::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( !Shift.Contains( ssAlt ))
    	MainWin->AltDown( false, Key );
}
//---------------------------------------------------------------------------
