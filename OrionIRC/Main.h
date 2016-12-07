/* $Id: Main.h,v 1.41 2006/11/25 23:52:40 wiz Exp $ */
//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
//----------------------------------------------------------------------------
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Messages.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <StdCtrls.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <SysUtils.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <Placemnt.hpp>
#include <ToolWin.hpp>
//----------------------------------------------------------------------------
#include <WizUpdate.h>

#include "ServerWnd.h"
#include "iodispatcher.h"
#include "Identd.h"
#include "URLCatcher.h"
#include "Eyelin.h"
#include "irc.h"
#include "bitfield.h"

#ifndef WM_RESOLVED
#define WM_RESOLVED		(WM_USER + 1)
#endif

#define WM_TRAY_ICON 	(WM_USER + 2)

class TMainWin : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *HelpAboutItem;
	TMenuItem *WindowMinimizeItem;
	TStatusBar *StatusBar;
	TMenuItem *Tools1;
	TMenuItem *Rawmessages1;
	TFormStorage *FormStorage1;
	TMenuItem *N1;
	TMenuItem *Options1;
	TMenuItem *N2;
	TMenuItem *Connecttoaserver1;
	TMenuItem *Disconnect;
	TTimer *Timer;
	TMenuItem *DCCtransfers1;
	TToolBar *TaskBar;
	TMenuItem *SendGift;
	TPopupMenu *TrayMenu;
	TMenuItem *TrayMenuShow;
	TMenuItem *N4;
	TMenuItem *Exit1;
	TMenuItem *N5;
	TMenuItem *FavouritesMenu;
	TMenuItem *N6;
	TMenuItem *Orionontheweb1;
	TMenuItem *Homepage1;
	TMenuItem *Mailinglistarchive1;
	TMenuItem *URLcatcher1;
	TMenuItem *Notifies1;
	TMenuItem *Ignorelist1;
	TMenuItem *N3;
	TMenuItem *AutoUpdateMenu;
	TMenuItem *Checkforupdates1;
    TMenuItem *ScriptingConsoleMenu;
    TMenuItem *ScriptingSeparatorMenu;
	TImageList *Icons;
	TMenuItem *N8;
	TMenuItem *Reportabugsubmitasuggestion1;
	TMenuItem *N9;
	TMenuItem *ChannelListMenu;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall WindowCascadeItemClick(TObject *Sender);
	void __fastcall ActiveFormChange(TObject *Sender);
	void __fastcall WindowTileItemClick(TObject *Sender);
	void __fastcall WindowArrangeItemClick(TObject *Sender);
	void __fastcall WindowMinimizeItemClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall HelpAboutItemClick(TObject *Sender);
	void __fastcall FileExitItemClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Rawmessages1Click(TObject *Sender);
	void __fastcall Options1Click(TObject *Sender);
	void __fastcall Connecttoaserver1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TimerTimer(TObject *Sender);
	void __fastcall StatusBarDrawPanel(TStatusBar *StatusBar,
          TStatusPanel *Panel, const TRect &Rect);
	void __fastcall DCCtransfers1Click(TObject *Sender);
	void __fastcall SendGiftClick(TObject *Sender);
	void __fastcall DisconnectClick(TObject *Sender);
	void __fastcall TaskBarResize(TObject *Sender);
	void __fastcall TrayMenuShowClick(TObject *Sender);
	void __fastcall FavouritesMenuClick(TObject *Sender);
	void __fastcall Homepage1Click(TObject *Sender);
	void __fastcall URLcatcher1Click(TObject *Sender);
	void __fastcall Notifies1Click(TObject *Sender);
	void __fastcall Ignorelist1Click(TObject *Sender);
	void __fastcall AutoUpdateMenuClick(TObject *Sender);
	void __fastcall Checkforupdates1Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ScriptingConsoleMenuClick(TObject *Sender);
	void __fastcall Mailinglistarchive1Click(TObject *Sender);
	void __fastcall Reportabugsubmitasuggestion1Click(TObject *Sender);
	void __fastcall ChannelListMenuClick(TObject *Sender);
private:
	enum {
    	F_TRAY_ICON 		= (1 << 0),
        F_FIRST_WINDOW  	= (1 << 1),
        F_GOT_PRIV_MSGS		= (1 << 2),
    	F_TRAY_ICON_HIDDEN 	= (1 << 3),
        F_ALT_DOWN			= (1 << 4),
        F_IGNORE_SYS_CHAR	= (1 << 5),
    };

	WUPD_Data           			UpdateData;
	TList					   	   *DNSReqs;
    HANDLE							AppMutex;
	NOTIFYICONDATA      			TrayData;
    Graphics::TIcon			   	   *TrayIcon;
    BitField						Flags;
    UINT							TaskbarCreatedMsg;
    time_t							LastNotify;
    Eyelin							EyelinSupport;
    TList					   	   *TaskbarButtons;
    void						   *FPrevClientProc;
    void						   *FClientInstance;

    void __fastcall					AppMessage( tagMSG &Message, bool &Handled );
	void __fastcall 				AppActivate( TObject *Sender );
	void __fastcall 				AppDeactivate( TObject *Sender );
	void __fastcall 				MinimizeWnd( TObject *Sender );
	void __fastcall 				RestoreWnd( TObject *Sender );
    void __fastcall 				TrayIconEvent( TMessage &Message );
    void __fastcall 				TaskbarCreated( void );
	void __fastcall 				ShowHint( TObject *Sender );
	TIRCSession	* __fastcall 		GetActiveSession( void );
	static int __fastcall 			cmpTabBarServer( void *Item1, void *Item2 );
	static int __fastcall 			cmpTabBarType( void *Item1, void *Item2 );
	void __fastcall 				SortTaskbar( void );
	static int __fastcall 	  		GetSortPriority( TForm *form );
	void __fastcall 				TaskBarClick( TObject *Sender );
    void __fastcall					WMMDINext( TMessage &Message );
    void __fastcall					WMResolved( TMessage &Message );
    void __fastcall 				CheckNotify( time_t now );
	static TIRCSession * __fastcall GetSession( TForm *form );
    void __fastcall					ClientWndProc( TMessage &Message );
    void __fastcall					HandleAltDown( WORD &Key );
    void __fastcall					HandleAltUp( WORD &Key );

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_RESOLVED, TMessage, WMResolved )
    MESSAGE_HANDLER( WM_TRAY_ICON, TMessage, TrayIconEvent )
END_MESSAGE_MAP( TForm )

public:
	IODispatcher					IO;
    TIdentd				   	   	   *Ident;
    TURLCatcher						URLCatcher;

	virtual __fastcall 				TMainWin( TComponent *Owner );
	bool __fastcall 				FormExists( TForm *form );
	TServerWin * __fastcall			FindServerWin( AnsiString server );
	void __fastcall 				UpdateStatus( void );
	TSpeedButton * __fastcall 		AddTaskbarButton( AnsiString caption, TForm *form, int type, TPopupMenu *popup );
	void __fastcall 				RemoveTaskbarButton( TSpeedButton *button );
	void __fastcall 				EnableTaskbarButton( TSpeedButton *button );
	void __fastcall 				NeedDCCMgr( void );
    void __fastcall					DoDNS( AnsiString host, TServerWin *server );
    void __fastcall					PrefsChanged( void );
	void __fastcall					ForwardToActiveWin( const char *nick, AnsiString msg, TIRCEvent type, TForm *wnd );
	void __fastcall					DisplayOnActiveWin( const char *nick, AnsiString msg, TIRCEvent type );
	TServerWin * __fastcall			GetCurrentServerWin( void );
	TChanWin * __fastcall			GetCurrentChannelWin( void );
    void __fastcall					RecvdPrivMsg( void );
    void __fastcall					FlashStatusBar( void ) const;
    void __fastcall					AltDown( bool isDown, WORD &Key );
};

enum { TB_SERVER, TB_CHANNEL, TB_QUERY, TB_DCC, TB_SCRIPT };

//----------------------------------------------------------------------------
extern TMainWin *MainWin;
//----------------------------------------------------------------------------
#endif
