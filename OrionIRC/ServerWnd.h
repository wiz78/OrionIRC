/* $Id: ServerWnd.h,v 1.52 2005/09/09 22:52:50 wiz Exp $ */
//----------------------------------------------------------------------------
#ifndef ServerWndH
#define ServerWndH
//----------------------------------------------------------------------------
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
#include <Placemnt.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//----------------------------------------------------------------------------
#include "TcpSocket.h"
#include "irc.h"
#include "IRCParser.h"
#include "Server.h"
#include "Hostmask.h"
#include "Logger.h"
#include "bitfield.h"
#include "NotifyHandler.h"

class TIRCList;
class TIRCEdit;
class TChanWin;
class TQueryWin;
class TChanListWin;

#define WM_RESOLVED	(WM_USER + 1)

class TServerWin : public TForm
{
__published:
	TPanel *PanelBottom;
	TFormStorage *FormStorage1;
	TTimer *Timer;
	TOpenDialog *OpenDlg;
	TTimer *ThrottleTimer;
	TTimer *ReconnectTimer;
	TPopupMenu *PopupMenu;
	TMenuItem *Disconnect1;
	TMenuItem *N1;
	TMenuItem *LogMenu;
	TMenuItem *Savebuffer1;
	TMenuItem *Clearbuffer1;
	TMenuItem *N2;
	TMenuItem *Search1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ThrottleTimerTimer(TObject *Sender);
	void __fastcall ReconnectTimerTimer(TObject *Sender);
	void __fastcall Disconnect1Click(TObject *Sender);
	void __fastcall LogMenuClick(TObject *Sender);
	void __fastcall Savebuffer1Click(TObject *Sender);
	void __fastcall Clearbuffer1Click(TObject *Sender);
	void __fastcall Search1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	
private:
	TIRCList	   		   *Display;
    TIRCEdit	   	   	   *Input;
    TSpeedButton   		   *TaskbarButton;
    TcpSocket	   	   	   *Sock;
    short					NickToTry;
    char					HostEnt[ MAXGETHOSTSTRUCT ];
    HANDLE					AsyncReq;
    Prom_Addr				ServerAddr;
    char					ServerMsg[ IRC_MAX_MSG_LENGTH * 5 ];
    unsigned int			ServerMsgLen;
    IRCParser				Parser;
    TList		   	   	   *Channels;
    TList		   		   *Queries;
    BitField				Flags;
    time_t					LagProbeTime;
    TStringList	   		   *CmdList;
    TList		   		   *WhoActions;
    Logger					Log;
    TNotifyHandler			NotifyHandler;
    TChanListWin		   *ChannelList;

	void __fastcall 		Connect( void );
    void __fastcall			WMResolved( TMessage &Message );

	void __fastcall			Login( void );
	void __fastcall			Connected( void );
	void __fastcall			Disconnect( void );
	void __fastcall			Disconnected( bool error = false );

	void __fastcall			CheckServerMsg( void );
	void __fastcall			ParseServerMsg( char *msg );

	void __fastcall			HandleNumerics( void );

	void __fastcall			HandleUnknownCommand( void );
	void __fastcall			HandleISupport( void );
	void __fastcall			HandleNickInUse( void );
    void __fastcall			HandleNickTempUnavail( void );
	void __fastcall			HandleTopic( void );
	void __fastcall			HandleTopicSetBy( void );
	void __fastcall			HandleTopicChange( void );
	void __fastcall			HandleMOTD( void );
	void __fastcall			HandleNames( bool names );
	void __fastcall			HandleChannelModeIs( void );
	void __fastcall			HandleServerInfo( void );
	void __fastcall			HandlePrivMsg( void );
	void __fastcall			HandlePrivMsgQuery( AnsiString prefix );
	void __fastcall			HandleNotice( void );
	void __fastcall			HandleMode( void );
	void __fastcall			HandleJoin( void );
	void __fastcall			HandlePart( void );
	void __fastcall			HandleQuit( void );
	void __fastcall			HandleError( void );
	void __fastcall			HandleNick( void );
	void __fastcall			HandleKick( void );
	void __fastcall			HandleCTCP( AnsiString ctcp );
	void __fastcall			HandleCTCPAction( AnsiString ctcp );
	void __fastcall			HandleCTCPPage( AnsiString page );
	void __fastcall			HandleDCCOffer( AnsiString dcc );
	bool __fastcall			HandleDCCChat( AnsiString dcc, Prom_Addr *addr, int port );
	void __fastcall			HandleDCCSend( TStringList *args, Prom_Addr *addr, int port );
	void __fastcall			HandleCTCPResponse( AnsiString ctcp );
	void __fastcall			HandleWallops( void );
	void __fastcall			HandleWhoisIdle( void );
	void __fastcall			HandlePong( void );
	void __fastcall			HandleInvite( void );
	void __fastcall			HandleChanList( TIRCChanList list, bool end );
	void __fastcall			HandleWho( void );
	void __fastcall			HandleEndOfWho( void );
	void __fastcall			HandleIsOn( void );

	void __fastcall			TryNextNick( void );
	AnsiString __fastcall 	FormatTimeInterval( int secs );

	void __fastcall			OnCommand( System::TObject *Sender, AnsiString msg );
	void __fastcall			OnMessage( System::TObject *Sender, AnsiString msg );
	void __fastcall 		DisplayDblClick( TObject *Sender );

	void __fastcall			GetDCCAddr( AnsiString str, Prom_Addr *addr );

	void __fastcall 		SetFonts( void );

	void __fastcall 		BroadcastUserHost( AnsiString user, AnsiString host );
    void __fastcall			DisableChannels( void );

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_RESOLVED, TMessage, WMResolved )
END_MESSAGE_MAP( TForm )

public:
    TIRCSession				Session;
    TServer					Server;

	virtual __fastcall 		TServerWin( TComponent *Owner, TServer *server );

    void __fastcall			SocketEvent( Prom_SC_Reason reason, int data );

    void __fastcall			SendMsg( AnsiString target, AnsiString msg );
    void __fastcall			SendCmd( AnsiString cmd );
    void __fastcall			SendUrgCmd( AnsiString cmd );
    void __fastcall			ClearSendQueue( void );

    void __fastcall			AddRow( const char *nick, AnsiString msg, TIRCEvent type, bool skipfilters = false );
    TIRCList * __fastcall	GetDisplay( void ) { return( Display ); }

    void __fastcall			SetActiveWin( TForm *form ) { Session.ActiveWin = form; }
    void __fastcall			DisplayOnActiveWin( const char *nick, AnsiString msg, TIRCEvent type );
	void __fastcall 		ClosingChannelWin( TChanWin *wnd );
	void __fastcall 		ClosingQueryWin( TQueryWin *wnd );

	TChanWin * __fastcall 	FindChannelWin( AnsiString chan );
	TQueryWin * __fastcall 	FindQueryWin( AnsiString target, bool open = true, bool skipDCC = false );

	AnsiString __fastcall 	GetMyIPForDCC( void );
    void __fastcall			DCCSend( AnsiString target, AnsiString file );

    void __fastcall			GetHostForDNS( AnsiString user );
	void __fastcall 		GetHostForIgnore( AnsiString user, int flags );
	void __fastcall 		WhoForWhois( AnsiString user );
	void __fastcall 		WhoForQuery( AnsiString user );

	void __fastcall			IgnoreUser( HostMask user, int flags );

	void __fastcall 		PrefsChanged( void );

    void __fastcall			ConnectTo( TServer *server );
    void __fastcall			ConnectTo( AnsiString host, int port );

    void __fastcall			AddWhoAction( int action );

    void __fastcall			EyelinAccept( AnsiString nick, Prom_Addr *addr );
};

#define SERVF_LAG_PROBE_SENT	(1 << 0)

// actions to perform after WHO
#define SERV_WHO_DNS			0
#define SERV_WHO_IGNORE			1
#define SERV_WHO_MY_HOSTMASK	2
#define SERV_WHO_NOTHING		3
#define SERV_WHO_WHOIS			4
#define SERV_WHO_QUERY			5
//----------------------------------------------------------------------------
#endif
