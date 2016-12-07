/* $Id: QueryWnd.cpp,v 1.42 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "QueryWnd.h"
#include "ServerWnd.h"
#include "BufferSearchWnd.h"
#include "Commands.h"
#include "TcpSocket.h"
#include "IRCUtils.h"
#include "Eyelin.h"
#include "Main.h"
#include "Prefs.h"
#include "OrionScriptInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DropPIDLTarget"
#pragma link "DropSource"
#pragma link "DropTarget"
#pragma link "DropURLTarget"
#pragma resource "*.dfm"
TQueryWin *QueryWin;
//---------------------------------------------------------------------------
__fastcall TQueryWin::TQueryWin( TComponent* Owner, TServerWin *server, AnsiString target )
	: TForm(Owner), Target( target )
{
	Display   = new TIRCList( this, server, Prefs.GetColorSet() );
    Input	  = new TIRCEdit( this );
    Server    = server;
    Caption   = Target.GetNick() + " - " + Server->Session.Server;
    DCC		  = NULL;

    SepEyelin->Visible = Eyelin::GetInstance()->IsInstalled();
    Eyelin->Visible    = SepEyelin->Visible;

    Input->Parent    = PanelBottom;
    Input->OnMessage = OnMessage;
    Input->OnCommand = OnCommand;

    Display->Parent     = this;
    Display->Align      = alClient;
    Display->PopupMenu  = Popup;
    Display->OnDblClick = DisplayDblClick;

    Input->SetWheelTarget( Display );

    Display->SetInputCtrl( Input );
    Display->ShowTimestamp( Prefs.ShowTimestamp() );
    Display->ShowNick( true );
    Display->SetAdaptToNick( Prefs.AdaptiveNick() );

    SetFonts();

    ActiveControl = Input;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormDestroy(TObject *Sender)
{
	delete DCC;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::SetFonts( void )
{
    Display->Font->Assign( Prefs.GetQueryFont() );
    Input->Font->Assign( Prefs.GetInputFont() );

    PanelBottom->Height = 6 - Input->Font->Height;
    Input->Height       = PanelBottom->Height - 2;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormShow(TObject *Sender)
{
    TaskbarButton = MainWin->AddTaskbarButton( Target.GetNick(), this, TB_QUERY, Popup );

	DropURLTargetInput->Register( Input );
	DropPIDLTarget->Register( this );

    if( Prefs.LogQueries() ) {

    	LogButton->Down = true;

		LogButtonClick( LogButton );
	}

    OrionScript::FireEvent( SE_OPEN, this );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;

    OrionScript::FireEvent( SE_CLOSE, this );

    Server->ClosingQueryWin( this );
    MainWin->RemoveTaskbarButton( TaskbarButton );

    DropPIDLTarget->Unregister();
    DropURLTargetInput->Unregister();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormActivate(TObject *Sender)
{
	Server->SetActiveWin( this );
    MainWin->EnableTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall	TQueryWin::OnCommand( System::TObject *Sender, AnsiString msg )
{
	AnsiString cmd = "/" + msg;

	if( OrionScript::FireEvent( SE_INPUT, this, cmd.c_str() )) {

        if( DCCConnected && !msg.SubString( 1, 3 ).AnsiCompareIC( "ME " )) {

            msg.Delete( 1, 3 );

            msg = msg.Trim().SubString( 1, IRC_MAX_MSG_LENGTH - sizeof( "\001ACTION \001" ));

            AddRow( NULL, Server->Session.Nick + " " + msg, IRC_ACTION, true, true );

            msg = "\001ACTION " + msg + "\001\n";

            DCC->AsyncSend( msg.c_str(), msg.Length() );

        } else
            HandleCommand( Server, (TChanWin *)this, msg );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TQueryWin::OnMessage( System::TObject *Sender, AnsiString msg )
{
	if( OrionScript::FireEvent( SE_INPUT, this, msg.c_str() ))
    	Say( msg );
}
//---------------------------------------------------------------------------
void __fastcall	TQueryWin::Say( AnsiString msg )
{
    int max = IRC_MAX_MSG_LENGTH - 2 - sizeof( "PRIVMSG  " ), start = 1;

    if( msg.IsEmpty() )
    	msg = " ";

    if( !DCCConnected ) {

        if( !Server->Session.Flags.IsSet( IRC_SESS_CONNECTED )) {
            AddRow( NULL, "Not connected to the server!", IRC_ERROR, true, true );
            return;
        }

        Server->SendMsg( Target.GetNick(), msg );
    }

    do {
        AnsiString s = msg.SubString( start, max );

        AddRow( Server->Session.Nick.c_str(), s, DCCConnected ? IRC_DCC_CHAT : IRC_OWN_MSG, true, true );

        start += s.Length();

        if( DCCConnected ) {

            s += "\n";

            DCC->AsyncSend( s.c_str(), s.Length() );
        }

    } while( start <= msg.Length() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::NickChange( AnsiString oldnick, AnsiString newnick )
{
	if( !oldnick.AnsiCompareIC( Target.GetNick() )) {

    	AddRow( NULL, oldnick + " is now known as " + newnick, IRC_NICK, true, true );

        Target.SetNick( newnick );

	    Caption = Target.GetNick() + " - " + Server->Session.Server;

        if( TaskbarButton )
        	TaskbarButton->Caption = Target.GetNick();
    }
}
//---------------------------------------------------------------------------
void __fastcall	TQueryWin::SetUserHost( AnsiString nick, AnsiString host )
{
	if( !nick.AnsiCompareIC( Target.GetNick() )) {

    	Target.SetNick( nick );
    	Target.SetHost( host );

    	Caption = Target.GetNick() + " - " + Server->Session.Server;
    }
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::WhoisClick(TObject *Sender)
{
	HandleCommand( Server, reinterpret_cast<TChanWin *>( this ), "WHOIS " + Target.GetNick() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DCCChatClick(TObject *Sender)
{
	if( DCC ) {

    	delete DCC;

    	DCC          = NULL;
        DCCConnected = false;

    	AddRow( NULL, "DCC Chat link with " + Target.GetNick() + " closed.", IRC_DCC, true, true );

    } else
    	InitDCC();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::InitDCC( void )
{
	DCC          = new TcpSocket();
    DCCConnected = false;

    DCC->UseDispatcher( &MainWin->IO );
   	DCC->SetAsyncCallback( SocketCB, this );

    if( DCC->IsValid() && DCCBind() && DCC->Listen() ) {

		AddRow( NULL, "Sending a DCC Chat request to " + Target.GetNick(), IRC_DCC, true, true );

    	Server->SendCmd( "PRIVMSG " + Target.GetNick() + " :\001DCC CHAT Chat " +
    					 Server->GetMyIPForDCC() + " " +
    					 IntToStr( DCC->GetLocalPort() ) + "\001" );

    } else {

    	delete DCC;

        DCC           = NULL;
    	DCCChat->Down = false;

		AddRow( NULL, "Cannot create a DCC connection!", IRC_ERROR, true, true );
    }
}
//---------------------------------------------------------------------------
bool __fastcall TQueryWin::DCCBind( void )
{
	bool ret;

    if( Prefs.LimitDCCPorts() ) {

    	ret = false;

        for( int i = Prefs.GetDCCPortFrom(); !ret && ( i <= Prefs.GetDCCPortTo() ); i++ )
        	ret = DCC->Bind( i );

    } else
		ret = DCC->Bind( 0 );

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::CloseDCC( AnsiString msg )
{
    delete DCC;

	DCC           = NULL;
	DCCChat->Down = false;
    DCCConnected  = false;

	AddRow( NULL, msg, IRC_ERROR, true, true );
}
//---------------------------------------------------------------------
void TQueryWin::SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata )
{
	((TQueryWin *)userdata )->SocketEvent( reason, data );
}
//---------------------------------------------------------------------
void __fastcall TQueryWin::SocketEvent( Prom_SC_Reason reason, int data )
{
	switch( reason ) {

    	case PROM_SOCK_ACCEPT:
        	AcceptDCC((TcpSocket *)data );
        	break;

    	case PROM_SOCK_READ:
        	if( data )
            	HandleIncomingData( data );
            else
            	CloseDCC( "DCC Chat connection terminated by " + Target.GetNick() + "." );
        	break;

        case PROM_SOCK_CONNECTED:
        	StartDCC();
            break;

        case PROM_SOCK_ERROR:
        case PROM_SOCK_TIMEOUT:
			CloseDCC( "DCC Chat connection lost." );
        	break;
    }
}
//---------------------------------------------------------------------
void __fastcall TQueryWin::AcceptDCC( TcpSocket *sock )
{
	delete DCC;

    DCC = sock;

    if( DCC->IsValid() ) {

    	StartDCC();

    } else {

    	AddRow( NULL, "Cannot setup a DCC connection!", IRC_ERROR, true, true );

        delete DCC;

        DCC           = NULL;
        DCCChat->Down = false;
    }
}
//---------------------------------------------------------------------
void __fastcall TQueryWin::StartDCC( void )
{
	AddRow( NULL, "DCC Chat connection established.", IRC_DCC, true, true );

	DCCConnected  = true;
	DCCBufLen     = 0;
	DCCChat->Down = true;

	DCC->UseDispatcher( &MainWin->IO );
	DCC->SetAsyncCallback( SocketCB, this );
	DCC->AsyncRecv( DCCBuffer, sizeof( DCCBuffer ));
}
//---------------------------------------------------------------------
void __fastcall TQueryWin::HandleIncomingData( int len )
{
	bool	go, line = false;
    int		start = 0, i = 0;

    DCCBufLen += len;

	do {

        go = false;

		for( ; i < DCCBufLen; i++ )
        	if( DCCBuffer[ i ] == '\n' ) {

                if( char *ptr = strchr( &DCCBuffer[ start ], '\r' ))
                	*ptr = '\0';

                DCCBuffer[ i ] = '\0';

                if(( DCCBuffer[ start ] == '\001' ) && !strncmp( &DCCBuffer[ start + 1 ], "ACTION", 6 )) {
                	AnsiString act;

                	if( char *ptr = strchr( &DCCBuffer[ start + 1 ], '\001' ))
                    	*ptr = '\0';

                    act = Target.GetNick() + " " + AnsiString( &DCCBuffer[ start + 1 ]);

			        if( OrionScript::FireEvent( SE_CHAT, this, act.c_str() )) {
						AddRow( NULL, act, IRC_ACTION, true, false );
                        MainWin->RecvdPrivMsg();
                    }

                } else if( OrionScript::FireEvent( SE_CHAT, this, &DCCBuffer[ start ] )) {
	                AddRow( Target.GetNick().c_str(), &DCCBuffer[ start ], IRC_DCC_CHAT, true, false );
                    MainWin->RecvdPrivMsg();
                }

                line  = true;
                go    = true;
                start = i + 1;
            }

    } while( go );

    DCCBufLen -= start;

    if( !line && ( i >= sizeof( DCCBuffer ))) { // filled the buffer without finding \r\n
    	char	save = DCCBuffer[ sizeof( DCCBuffer ) - 1 ];

        DCCBuffer[ sizeof( DCCBuffer ) - 1 ] = '\0';

        if( OrionScript::FireEvent( SE_CHAT, this, DCCBuffer )) {
	    	AddRow( Target.GetNick().c_str(), DCCBuffer, IRC_DCC, true, false );
            MainWin->RecvdPrivMsg();
        }

        DCCBuffer[ 0 ] = save;
        DCCBufLen      = 1;

    } else if( DCCBufLen > 0 )
		memcpy( DCCBuffer, &DCCBuffer[ start ], DCCBufLen );

    DCC->AsyncRecv( &DCCBuffer[ DCCBufLen ], sizeof( DCCBuffer ) - DCCBufLen );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::ConnectDCC( Prom_Addr addr, int port )
{
    DCC = new TcpSocket();

    DCC->UseDispatcher( &MainWin->IO );
   	DCC->SetAsyncCallback( SocketCB, this );


    if( DCC->IsValid() && DCC->AsyncConnect( &addr, port )) {

        DCCChat->Down = true;

    	AddRow( NULL, "Establishing a DCC Chat connection...", IRC_DCC, true, true );

    } else {

    	AddRow( NULL, "Cannot create a DCC Chat connection!", IRC_ERROR, true, true );

        delete DCC;

        DCC = NULL;
    }
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DCCSendClick(TObject *Sender)
{
	Server->DCCSend( Target.GetNick(), "" );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::AddRow( const char *nick, AnsiString msg, TIRCEvent type,
								   bool skipfilters, bool nosounds )
{
	MainWin->URLCatcher.Filter( nick ? nick : "-", msg );

	if( skipfilters || Prefs.GetFilters()->Process( nick, msg, type, this )) {

        if( Prefs.StripColors() )
            msg = IRCUtils::StripColors( msg );
    
		Display->AddRow( nick, msg, type );
        Log.Log( nick, msg, type );

    	if( !Active ) {

        	if( type == IRC_HIGHLIGHT )
	    		TaskbarButton->Font->Color = clGreen;
            else if( TaskbarButton->Font->Color != clGreen )
             	TaskbarButton->Font->Color = clRed;
	    }

        if( !nosounds )
            switch( type ) {

                case IRC_MSG:
                case IRC_ACTION:
                case IRC_DCC_CHAT:
                    Prefs.GetSounds()->Play( SoundsMgr::QUERY );
                    break;
            }
    }
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::PrefsChanged( void )
{
	SetFonts();

    Display->ShowTimestamp( Prefs.ShowTimestamp() );
    Display->SetAdaptToNick( Prefs.AdaptiveNick() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DNS1Click(TObject *Sender)
{
	HandleCommand( Server, (TChanWin *)this, "DNS " + Target.GetNick() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::Closewindow1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::IgnoreClick(TObject *Sender)
{
	HandleCommand( Server, (TChanWin *)this, "IGNORE " + Target.GetNick() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::PingClick(TObject *Sender)
{
	AnsiString	ctcp;

    if( Sender == Version )
    	ctcp = "VERSION";
    else if( Sender == Clientinfo )
    	ctcp = "CLIENTINFO";
    else if( Sender == Userinfo )
    	ctcp = "USERINFO";
    else if( Sender == Finger )
    	ctcp = "FINGER";
    else if( Sender == Time )
    	ctcp = "TIME";
    else if( Sender == Ping )
    	ctcp = "PING " + IntToStr( time( NULL ));

    ctcp = "PRIVMSG " + Target.GetNick() + " :\001" + ctcp + "\001";

    if( Sender == Ping )
    	Server->SendUrgCmd( ctcp );
    else
	    Server->SendCmd( ctcp );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::LogButtonClick(TObject *Sender)
{
	if( LogButton->Down )
    	Log.Open( Server->Session.Server, Target.GetNick() );
    else
    	Log.Close();

    if( Log.IsOpen() )
    	Display->AddRow( NULL, "Opened log file \"" + Log.GetLogFile() + "\"", IRC_LOCAL );
    else
    	Display->AddRow( NULL, "Log closed", IRC_LOCAL );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DropPIDLTargetDrop(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	for( int i = 0; i < DropPIDLTarget->Filenames->Count; i++ )
    	Server->DCCSend( Target.GetNick(), DropPIDLTarget->Filenames->Strings[ i ]);
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DropURLTargetInputDrop(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	Input->SelText = DropURLTargetInput->URL;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DropURLTargetInputGetDropEffect(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
    Effect = DropURLTargetInput->URL.IsEmpty() ? DROPEFFECT_NONE : DROPEFFECT_LINK;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DropPIDLTargetGetDropEffect(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
    Effect = DROPEFFECT_COPY;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::Addtonotify1Click(TObject *Sender)
{
	Prefs.GetNotifies()->Add( Target.GetNick() );
    Prefs.Save();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::Savebuffer1Click(TObject *Sender)
{
	Display->SaveBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::Clearbuffer1Click(TObject *Sender)
{
	Display->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::EyelinClick(TObject *Sender)
{
	HandleCommand( Server, reinterpret_cast<TChanWin *>( this ),
    			   "DCC EYELIN " + Target.GetNick() );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::Search1Click(TObject *Sender)
{
	TBufferSearchWin *wnd = new TBufferSearchWin( NULL, Display );

    wnd->ShowModal();

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::DisplayDblClick( TObject *Sender )
{
	Prefs.GetDblClickAction( DBL_CLICK_QUERY ).Execute( this );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Shift.Contains( ssAlt ))
    	MainWin->AltDown( true, Key );
}
//---------------------------------------------------------------------------
void __fastcall TQueryWin::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( !Shift.Contains( ssAlt ))
    	MainWin->AltDown( false, Key );
}
//---------------------------------------------------------------------------

