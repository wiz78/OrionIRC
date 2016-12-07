/* $Id: QueryWnd.h,v 1.24 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef QueryWndH
#define QueryWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
#include <DropPIDLTarget.hpp>
#include <DropSource.h>
#include <DropTarget.h>
#include <DropURLTarget.hpp>
//---------------------------------------------------------------------------
#include "IRCList.h"
#include "IRCEdit.h"
#include "TcpSocket.h"
#include "Logger.h"
#include "User.h"

class TServerWin;
class TChanWin;

class TQueryWin : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TToolBar *ToolBar;
	TToolButton *Whois;
	TToolButton *Ignore;
	TToolButton *ToolButton3;
	TToolButton *DCCChat;
	TToolButton *ToolButton5;
	TToolButton *DCCSend;
	TToolButton *ToolButton1;
	TPopupMenu *Popup;
	TMenuItem *Whois1;
	TMenuItem *N1;
	TMenuItem *DCCChat1;
	TMenuItem *DCCSendfile1;
	TMenuItem *N2;
	TMenuItem *CTCP1;
	TMenuItem *N3;
	TMenuItem *DNS1;
	TMenuItem *Ignore1;
	TMenuItem *Addtonotify1;
	TMenuItem *N4;
	TMenuItem *Closewindow1;
	TMenuItem *Ping;
	TMenuItem *Version;
	TMenuItem *Time;
	TMenuItem *Userinfo;
	TMenuItem *Clientinfo;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *Finger;
	TToolButton *ToolButton2;
	TToolButton *LogButton;
	TDropPIDLTarget *DropPIDLTarget;
	TDropURLTarget *DropURLTargetInput;
	TMenuItem *N7;
	TMenuItem *Savebuffer1;
	TMenuItem *Clearbuffer1;
	TToolButton *SepEyelin;
	TToolButton *Eyelin;
	TMenuItem *Search1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall WhoisClick(TObject *Sender);
	void __fastcall DCCChatClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall DCCSendClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DNS1Click(TObject *Sender);
	void __fastcall Closewindow1Click(TObject *Sender);
	void __fastcall IgnoreClick(TObject *Sender);
	void __fastcall PingClick(TObject *Sender);
	void __fastcall LogButtonClick(TObject *Sender);
	void __fastcall DropPIDLTargetDrop(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetInputDrop(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetInputGetDropEffect(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropPIDLTargetGetDropEffect(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall Addtonotify1Click(TObject *Sender);
	void __fastcall Savebuffer1Click(TObject *Sender);
	void __fastcall Clearbuffer1Click(TObject *Sender);
	void __fastcall EyelinClick(TObject *Sender);
	void __fastcall Search1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	TIRCList	   		   *Display;
	TServerWin	   		   *Server;
	TUser					Target;
	TIRCEdit   	   		   *Input;
    TSpeedButton  		   *TaskbarButton;
    TcpSocket			   *DCC;
    bool					DCCConnected;
    char					DCCBuffer[ 1024 ];
    int						DCCBufLen;
    Logger					Log;

	void __fastcall			OnCommand( System::TObject *Sender, AnsiString msg );
	void __fastcall			OnMessage( System::TObject *Sender, AnsiString msg );
	void __fastcall 		DisplayDblClick( TObject *Sender );

	bool __fastcall 		DCCBind( void );
	void __fastcall 		CloseDCC( AnsiString msg );
	static void 			SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata );
	void __fastcall			SocketEvent( Prom_SC_Reason reason, int data );
	void __fastcall 		AcceptDCC( TcpSocket *sock );
	void __fastcall 		StartDCC( void );
	void __fastcall			HandleIncomingData( int len );
	void __fastcall 		SetFonts( void );

public:
	__fastcall 				TQueryWin( TComponent* Owner, TServerWin *server, AnsiString target );

    void __fastcall			Say( AnsiString msg );
    void __fastcall			AddRow( const char *nick, AnsiString msg, TIRCEvent type, bool skipfilters, bool nosounds );

    TIRCList * __fastcall	GetDisplay( void )    { return( Display ); }
	TServerWin *__fastcall	GetServerWin( void )  { return( Server ); }
    AnsiString __fastcall	GetTargetNick( void ) { return( Target.GetNick() ); }

	void __fastcall			NickChange( AnsiString oldnick, AnsiString newnick );
	void __fastcall			SetUserHost( AnsiString nick, AnsiString host );

    void __fastcall			InitDCC( void );
    void __fastcall			ConnectDCC( Prom_Addr addr, int port );
    bool __fastcall			DCCInProgress( void ) { return( DCC != NULL ); }

	void __fastcall 		PrefsChanged( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TQueryWin *QueryWin;
//---------------------------------------------------------------------------
#endif
