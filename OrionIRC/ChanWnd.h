/* $Id: ChanWnd.h,v 1.41 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ChanWndH
#define ChanWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Placemnt.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <DropPIDLTarget.hpp>
#include <DropSource.h>
#include <DropTarget.h>
#include <DropURLTarget.hpp>
//---------------------------------------------------------------------------
#include "IRCList.h"
#include "IRCParser.h"
#include "irc.h"
#include "Hostmask.h"
#include "Logger.h"

class TIRCList;
class TIRCEdit;
class TIRCTopic;
class TServerWin;
class TChanPropWin;
class TUser;

class TChanWin : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TPanel *PanelDisplay;
	TPanel *PanelRight;
	TSplitter *Splitter1;
	TListBox *Users;
	TPanel *PanelRightTop;
	TLabel *UsersStats;
	TFormStorage *FormStorage1;
	TImageList *UserPics;
	TPopupMenu *UserListMenu;
	TMenuItem *Operatorcommands1;
	TMenuItem *N1;
	TMenuItem *Openaquery1;
	TMenuItem *DCCSendfile1;
	TMenuItem *DCCChat1;
	TMenuItem *N2;
	TMenuItem *Whois1;
	TMenuItem *DNSMenu;
	TMenuItem *N3;
	TMenuItem *Ignore1;
	TMenuItem *Addtonotifylist1;
	TMenuItem *N4;
	TMenuItem *Slap1;
	TMenuItem *CTCP1;
	TMenuItem *Version;
	TMenuItem *ClientInfo;
	TMenuItem *Finger;
	TMenuItem *N5;
	TMenuItem *Time;
	TMenuItem *Ping;
	TMenuItem *N6;
	TMenuItem *Op;
	TMenuItem *Deop;
	TMenuItem *Voice;
	TMenuItem *Unvoice;
	TMenuItem *N7;
	TMenuItem *KickMenu;
	TPanel *PanelTopic;
	TPopupMenu *TopicPopup;
	TMenuItem *CopyTopicToClipboard;
	TPopupMenu *ChanPopup;
	TMenuItem *Properties1;
	TMenuItem *N8;
	TMenuItem *Leaverejoin1;
	TMenuItem *Clearbuffer1;
	TMenuItem *N9;
	TMenuItem *Addtofavorites1;
	TMenuItem *N10;
	TMenuItem *Closewindow1;
	TPanel *PanelModes;
	TSpeedButton *ModeN;
	TSpeedButton *ModeS;
	TSpeedButton *ModeM;
	TSpeedButton *ModeT;
	TSpeedButton *ModeA;
	TSpeedButton *ModeL;
	TSpeedButton *ModeK;
	TSpeedButton *ModeI;
	TSpeedButton *ModeR;
	TSpeedButton *ModeQ;
	TSpeedButton *ModeP;
	TMenuItem *N11;
	TMenuItem *LogMenu;
	TDropPIDLTarget *DropPIDLTarget;
	TDropURLTarget *DropURLTargetInput;
	TDropURLTarget *DropURLTarget;
	TMenuItem *Savebuffer1;
	TMenuItem *MenuEyelin;
	TPanel *PanelNick;
	TImage *StatusLed;
	TLabel *NickLabel;
	TMenuItem *N12;
	TMenuItem *Search1;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall UsersDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall Whois1Click(TObject *Sender);
	void __fastcall VersionClick(TObject *Sender);
	void __fastcall Openaquery1Click(TObject *Sender);
	void __fastcall OpClick(TObject *Sender);
	void __fastcall Slap1Click(TObject *Sender);
	void __fastcall UsersMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall UsersMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall CopyTopicToClipboardClick(TObject *Sender);
	void __fastcall DCCChat1Click(TObject *Sender);
	void __fastcall DCCSendfile1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DNSMenuClick(TObject *Sender);
	void __fastcall Closewindow1Click(TObject *Sender);
	void __fastcall Properties1Click(TObject *Sender);
	void __fastcall Clearbuffer1Click(TObject *Sender);
	void __fastcall Leaverejoin1Click(TObject *Sender);
	void __fastcall ModeNClick(TObject *Sender);
	void __fastcall ModeSClick(TObject *Sender);
	void __fastcall ModeTClick(TObject *Sender);
	void __fastcall ModeMClick(TObject *Sender);
	void __fastcall ModeIClick(TObject *Sender);
	void __fastcall ModePClick(TObject *Sender);
	void __fastcall ModeRClick(TObject *Sender);
	void __fastcall ModeAClick(TObject *Sender);
	void __fastcall ModeQClick(TObject *Sender);
	void __fastcall ModeKClick(TObject *Sender);
	void __fastcall ModeLClick(TObject *Sender);
	void __fastcall PanelModesResize(TObject *Sender);
	void __fastcall Addtofavorites1Click(TObject *Sender);
	void __fastcall LogMenuClick(TObject *Sender);
	void __fastcall DropPIDLTargetDrop(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropPIDLTargetGetDropEffect(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetDrop(TObject *Sender, TShiftState ShiftState,
          tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetInputDrop(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetInputGetDropEffect(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall DropURLTargetGetDropEffect(TObject *Sender,
          TShiftState ShiftState, tagPOINT &Point, int &Effect);
	void __fastcall KickMenuClick(TObject *Sender);
	void __fastcall Addtonotifylist1Click(TObject *Sender);
	void __fastcall Ignore1Click(TObject *Sender);
	void __fastcall Savebuffer1Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall MenuEyelinClick(TObject *Sender);
	void __fastcall PanelBottomResize(TObject *Sender);
	void __fastcall Search1Click(TObject *Sender);
	void __fastcall UsersDblClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	TIRCList   	   	   	   *Display;
	TIRCEdit   	   	   	   *Input;
    TIRCTopic			   *Topic;
    AnsiString				Channel;
    TServerWin 	   	   	   *Server;
    TSpeedButton		   *TaskbarButton;
    bool					StillIn;
    bool					NamesDone;
    TChanPropWin   		   *Properties;
    Logger					Log;
    Graphics::TBitmap	   *StatusLedBitmap;

	void __fastcall			OnCommand( System::TObject *Sender, AnsiString msg );
	void __fastcall			OnMessage( System::TObject *Sender, AnsiString msg );
	void __fastcall 		DisplayDblClick( TObject *Sender );

	void __fastcall			ClearUsers( void );
	void __fastcall			AddUser( AnsiString user );
	void __fastcall			RemoveUser( AnsiString user );
	void __fastcall			RemoveUser( int idx );
    AnsiString __fastcall	GetNick( int index );
	void __fastcall 		SortUsers( void );
	void __fastcall			UpdateUserStats( void );
	void __fastcall 		KickUsers( AnsiString reason );

	void __fastcall			SetModeButtons( void );
	void __fastcall			DisplayEvent( int ev, AnsiString msg, TIRCEvent type );
	void __fastcall			SetFonts( void );
	void __fastcall 		LayoutModes( void );
	void __fastcall 		UpdateNickLabel( void );

	static int __fastcall 	NickSortAlpha( void *Item1, void *Item2 );
	static int __fastcall 	NickSortModes( void *Item1, void *Item2 );
	static int __fastcall 	GetModePri( TUser *user );

public:		// User declarations
    TIRCChanData			ChanData;

	__fastcall 				TChanWin( TComponent* Owner, TServerWin *server, AnsiString channel );
    void __fastcall			AddRow( const char *nick, AnsiString msg, TIRCEvent type, bool skipfilters = false );

	void __fastcall			EditProperties( System::TObject *Sender );

    TIRCList * __fastcall	GetDisplay( void )       { return( Display ); }
    AnsiString __fastcall	GetChannel( void )       { return( Channel ); }
    TServerWin * __fastcall	GetServerWin( void )     { return( Server ); }
    TChanPropWin * __fastcall GetChanPropWin( void ) { return( Properties ); }

    void __fastcall			CloseNow( void );

	void __fastcall			ShowDisabled( bool disabled );

    void __fastcall			Joined( void );

    void __fastcall			UpdateNames( AnsiString list );
	void __fastcall			EndOfNames( void );

    void __fastcall			ModeChange( IRCParser& parser, bool change );
	void __fastcall			NickChange( AnsiString oldnick, AnsiString newnick );

    void __fastcall			SetUserHost( AnsiString nick, AnsiString host );
    TUser * __fastcall		GetUser( AnsiString nick );
    TUser * __fastcall		GetUser( int idx );
    void __fastcall			UserJoin( HostMask& mask );
	void __fastcall			UserPart( HostMask& mask, AnsiString reason, bool quit );

	void __fastcall			Say( AnsiString msg );
	void __fastcall			Kick( HostMask& mask, AnsiString user, AnsiString reason );

    AnsiString __fastcall	GetTopic( void ) const;
    void __fastcall			SetTopic( AnsiString topic );
    void __fastcall			SetTopicSetBy( AnsiString str )    { ChanData.TopicSetBy = str; }
    void __fastcall			SetTopicSetTime( AnsiString str )  { ChanData.TopicSetTime = str; }

	void __fastcall			PrefsChanged( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TChanWin *ChanWin;
//---------------------------------------------------------------------------
#endif
