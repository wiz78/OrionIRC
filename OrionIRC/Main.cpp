/* $Id: Main.cpp,v 1.61 2006/11/25 23:52:40 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <winsock2.h>

#include "Main.h"
#include "About.h"
#include "RawWnd.h"
#include "ServerWnd.h"
#include "ChanWnd.h"
#include "QueryWnd.h"
#include "OptionsWnd.h"
#include "ServersWnd.h"
#include "DCCMgrWnd.h"
#include "FavouritesWnd.h"
#include "URLCatcherWnd.h"
#include "ScriptingWnd.h"
#include "NotifyWnd.h"
#include "IgnoreWnd.h"
#include "MySpeedButton.h"
#include "Server.h"
#include "DNSReq.h"
#include "Commands.h"
#include "Prefs.h"
#include "OrionScriptInterface.h"
#include "Version.h"
//---------------------------------------------------------------------------
#pragma link "Placemnt"
#pragma resource "*.dfm"
TMainWin *MainWin;

#ifndef FLASHW_STOP
typedef struct {
	UINT cbSize;
    HWND hwnd;
    DWORD dwFlags;
    UINT uCount;
    DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

#define FLASHW_STOP 		0
#define FLASHW_CAPTION 		1
#define FLASHW_TRAY 		2
#define FLASHW_ALL          (FLASHW_CAPTION | FLASHW_TRAY)
#define FLASHW_TIMER        4
#define FLASHW_TIMERNOFG    0xC

typedef BOOL ( __stdcall *FlashWndExFunc )( PFLASHWINFO );
#endif
//---------------------------------------------------------------------------
__fastcall TMainWin::TMainWin(TComponent *Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FormCreate(TObject *Sender)
{
    WORD	VersionRequested = MAKEWORD( 2, 0 );
    WSADATA WsaData;

#if !SCRIPTING
    ScriptingConsoleMenu->Visible   = false;
    ScriptingSeparatorMenu->Visible = false;
#endif

    AppMutex = CreateMutex( NULL, FALSE, "ST_Orion" );

    SetCurrentDir( ExtractFilePath( ParamStr( 0 )));

    CreateDir( "Downloads" );
    CreateDir( "Logs" );
    CreateDir( "Data" );

    if( WSAStartup( VersionRequested, &WsaData ) != 0 ) {
        ShowMessage( "Cannot initialise Winsock v2!" );
        Application->Terminate();
    }

	FClientInstance = MakeObjectInstance( ClientWndProc );
  	FPrevClientProc = reinterpret_cast<void *>( GetWindowLong( ClientHandle, GWL_WNDPROC ));

  	SetWindowLong( ClientHandle, GWL_WNDPROC, reinterpret_cast<LONG>( FClientInstance ));

    TaskbarCreatedMsg          = RegisterWindowMessage( TEXT( "TaskbarCreated" ));
	Application->OnHint        = ShowHint;
    Application->OnMessage     = AppMessage;
    Application->OnMinimize    = MinimizeWnd;
    Application->OnRestore     = RestoreWnd;
    Application->OnActivate    = AppActivate;
    Application->OnDeactivate  = AppDeactivate;
    Screen->OnActiveFormChange = ActiveFormChange;
    Ident                      = new TIdentd();
    DNSReqs					   = new TList();
    TaskbarButtons			   = new TList();
    TrayIcon				   = new Graphics::TIcon();

    Flags.Set( F_FIRST_WINDOW );

    OrionScript::Setup();

    memset( &UpdateData, 0, sizeof( UpdateData ));

    UpdateData.AppName     = "Orion";
    UpdateData.URL         = "http://updates.tellini.info/orion.txt";
    UpdateData.Version     = MAKELONG( Version.GetRevision(), Version.GetVersion() );
    UpdateData.Build	   = MAKELONG( Version.GetBuild(),    Version.GetRelease() );
    UpdateData.HomePage    = "http://tellini.info";
    UpdateData.RegistryKey = "Software\\WizSoftware\\Orion";

    WUPD_CheckUpdate( &UpdateData, 0 );

    AutoUpdateMenu->Checked = WUPD_IsEnabled( &UpdateData );

    time( &LastNotify );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FormDestroy(TObject *Sender)
{
	SetWindowLong( ClientHandle, GWL_WNDPROC, reinterpret_cast<LONG>( FPrevClientProc ));
  	FreeObjectInstance( FClientInstance );

	OrionScript::FireEvent( SE_EXIT );
	OrionScript::Cleanup();

	WUPD_CleanUp( &UpdateData );

	WSACleanup();

    if( Prefs.SaveURLs() )
    	URLCatcher.Save( Prefs.GetURLCatcherFile() );

    if( Flags.IsSet( F_TRAY_ICON ))
        Shell_NotifyIcon( NIM_DELETE, &TrayData );

	Screen->OnActiveFormChange = NULL;

    for( int i = 0; i < DNSReqs->Count; i++ )
    	delete reinterpret_cast<DNSReq *>( DNSReqs->Items[ i ]);

    delete TrayIcon;
    delete TaskbarButtons;
	delete Ident;

    CloseHandle( AppMutex );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FormShow(TObject *Sender)
{
	TServer	server;

	Prefs.Load();
    FormResize( Sender );

    if( Prefs.SaveURLs() )
    	URLCatcher.Load( Prefs.GetURLCatcherFile() );

    Prefs.GetScripts()->Compile();

	TRegistry	*reg = new TRegistry();

	server.Load( reg, "Software\\WizSoftware\\Orion\\Servers" );
    server.AutoConnect();

    delete reg;
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::AppMessage( tagMSG &Message, bool &Handled )
{
	if( Message.message == TaskbarCreatedMsg ) {

        TaskbarCreated();

        Handled = true;

    } else switch( Message.message ) {

    	case WM_SYSCHAR:
        	if( Flags.IsSet( F_IGNORE_SYS_CHAR )) {
            	Handled = true;
            	Flags.Clear( F_IGNORE_SYS_CHAR );
            }
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::AppActivate( TObject *Sender )
{
	OrionScript::FireEvent( SE_APPACTIVE );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::AppDeactivate( TObject *Sender )
{
	OrionScript::FireEvent( SE_APPDEACTIVE );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::MinimizeWnd( TObject *Sender )
{
	if( Prefs.MinimizeToTray() ) {

    	memset( &TrayData, 0, sizeof( TrayData ));

        TrayData.cbSize           = sizeof( NOTIFYICONDATA );
        TrayData.hWnd             = Handle;
        TrayData.uID              = 0;
        TrayData.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        TrayData.uCallbackMessage = WM_TRAY_ICON;
        TrayData.hIcon            = Application->Icon->Handle;

        strcpy( TrayData.szTip, "Orion" );

        ShowWindow( Application->Handle, SW_HIDE );

        Flags.Set( F_TRAY_ICON, Shell_NotifyIcon( NIM_ADD, &TrayData ));
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::RestoreWnd( TObject *Sender )
{
    if( Flags.IsSet( F_TRAY_ICON ))
        Shell_NotifyIcon( NIM_DELETE, &TrayData );

    Flags.Clear( F_TRAY_ICON | F_TRAY_ICON_HIDDEN | F_GOT_PRIV_MSGS );

    Application->BringToFront();
    Application->ProcessMessages();
    Activate();
    BringToFront();
    Update();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TaskbarCreated( void )
{
	if( Flags.IsSet( F_TRAY_ICON )) {

        Shell_NotifyIcon( NIM_DELETE, &TrayData );
        Flags.Set( F_TRAY_ICON, Shell_NotifyIcon( NIM_ADD, &TrayData ));
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TrayIconEvent( TMessage &Message )
{
    POINT MousePos;

    switch( Message.LParam ) {

        case WM_RBUTTONUP:
            if( GetCursorPos( &MousePos )) {

                TrayMenu->PopupComponent = this;

                SetForegroundWindow( Handle );

                TrayMenu->Popup( MousePos.x, MousePos.y );
            }
            break;

        case WM_LBUTTONDBLCLK:
            TrayMenuShowClick( this );
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall	TMainWin::ClientWndProc( TMessage &Message )
{
	switch( Message.Msg ) {

    	case WM_MDINEXT:
        	WMMDINext( Message );
        	break;

        default:
        	Message.Result = CallWindowProc( reinterpret_cast<int (__stdcall *)()>( FPrevClientProc ),
            								 ClientHandle, Message.Msg,
                                             Message.WParam, Message.LParam );
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WMMDINext( TMessage &Message )
{
	if( TaskBar->ButtonCount ) {
        int 			idx = -1;
        TSpeedButton   *button;

        for( int i = 0; ( idx < 0 ) && ( i < TaskBar->ButtonCount ); i++ )
            if( reinterpret_cast<TSpeedButton *>( TaskBar->Buttons[ i ])->Down )
            	idx = i;

        if( !Message.LParam )
        	idx = ( idx + 1 ) % TaskBar->ButtonCount;
        else if( --idx < 0 )
			idx = TaskBar->ButtonCount - 1;

        button = reinterpret_cast<TSpeedButton *>( TaskBar->Buttons[ idx ]);

        button->Down = true;

        button->Click();

        Message.Result = 0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::ShowHint( TObject *Sender )
{
	StatusBar->Panels->Items[ 0 ]->Text = Application->Hint;
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FileExitItemClick(TObject *Sender)
{
	PostMessage( Handle, WM_CLOSE, 0, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WindowCascadeItemClick(TObject *Sender)
{
	Cascade();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WindowTileItemClick(TObject *Sender)
{
	Tile();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WindowArrangeItemClick(TObject *Sender)
{
	ArrangeIcons();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WindowMinimizeItemClick(TObject *Sender)
{
	//---- Must be done backwards through the MDIChildren array ----
	for( int i = MDIChildCount - 1; i >= 0; i-- )
		MDIChildren[ i ]->WindowState = wsMinimized;
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::ActiveFormChange( TObject *Sender )
{
	WindowCascadeItem->Enabled  = MDIChildCount > 0;
	WindowTileItem->Enabled     = MDIChildCount > 0;
	WindowMinimizeItem->Enabled = MDIChildCount > 0;
    Disconnect->Enabled         = GetActiveSession() != NULL;
    ChannelListMenu->Enabled    = Disconnect->Enabled;

    UpdateStatus();

    if( dynamic_cast<TServerWin *>( ActiveMDIChild ) ||
	    dynamic_cast<TChanWin *>( ActiveMDIChild ) ||
	    dynamic_cast<TQueryWin *>( ActiveMDIChild ))
    	OrionScript::FireEvent( SE_ACTIVE, ActiveMDIChild );
}
//---------------------------------------------------------------------------
bool __fastcall TMainWin::FormExists( TForm *form )
{
	for( int i = 0; i < MDIChildCount; i++ )
		if( MDIChildren[ i ] == form )
        	return( true );

	return( false );
}
//---------------------------------------------------------------------------
TServerWin * __fastcall TMainWin::FindServerWin( AnsiString server )
{
	TServerWin *ret = NULL;

	for( int i = 0; !ret && ( i < MDIChildCount ); i++ ) {
    	TServerWin *win = dynamic_cast<TServerWin *>( MDIChildren[ i ]);

		if( win && !win->Session.Server.AnsiCompareIC( server ))
        	ret = win;
    }

	return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::UpdateStatus( void )
{
    TIRCSession	*sess = GetActiveSession();

    if( sess ) {

	    StatusBar->Panels->Items[ 1 ]->Text = sess->Network;
    	StatusBar->Panels->Items[ 2 ]->Text = sess->Nick;

    } else {

	    StatusBar->Panels->Items[ 1 ]->Text = "";
    	StatusBar->Panels->Items[ 2 ]->Text = "";
    }
}
//---------------------------------------------------------------------------
TIRCSession	* __fastcall TMainWin::GetActiveSession( void )
{
    return( GetSession( Screen->ActiveForm ));
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::HelpAboutItemClick(TObject *Sender)
{
    AboutWin->Show();
    AboutWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Rawmessages1Click(TObject *Sender)
{
	RawWin->Show();
    RawWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Options1Click(TObject *Sender)
{
	if( !OptionsWin )
    	OptionsWin = new TOptionsWin( this );

    OptionsWin->Show();
    OptionsWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Connecttoaserver1Click(TObject *Sender)
{
	TIRCSession	*sess = GetActiveSession();

	ServersWin->Show();
    ServersWin->SetCurrentServer( sess ? sess->Server : AnsiString( "" ));
	ServersWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FormResize(TObject *Sender)
{
	int	size = 32;

	for( int i = 1; i < StatusBar->Panels->Count; i++ )
    	size += StatusBar->Panels->Items[ i ]->Width;

	StatusBar->Panels->Items[ 0 ]->Width = StatusBar->Width - size;
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TimerTimer(TObject *Sender)
{
	time_t	now;

    time( &now );

    if( Flags.AllSet( F_TRAY_ICON | F_GOT_PRIV_MSGS )) {

    	if( Flags.IsSet( F_TRAY_ICON_HIDDEN )) {

	        TrayData.hIcon = Application->Icon->Handle;

            Flags.Clear( F_TRAY_ICON_HIDDEN );

        } else {

        	Icons->GetIcon( 0, TrayIcon );

        	TrayData.hIcon = TrayIcon->Handle;

			Flags.Set( F_TRAY_ICON_HIDDEN );
        }

        Shell_NotifyIcon( NIM_MODIFY, &TrayData );
    }

    CheckNotify( now );
    OrionScript::FlushOutput();

	StatusBar->Panels->Items[ StatusBar->Panels->Count - 1 ]->Text = TDateTime::CurrentTime().FormatString( "hh:nn:ss" );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::CheckNotify( time_t now )
{
    if( now >= LastNotify + Prefs.GetNotifyInterval() ) {
    
        for( int i = 0; i < MDIChildCount; i++ ) {
            TServerWin *win = dynamic_cast<TServerWin *>( MDIChildren[ i ]);

            if( win )
                Prefs.GetNotifies()->SendIsON( win );
        }

    	LastNotify = now;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::StatusBarDrawPanel(TStatusBar *StatusBar,
      TStatusPanel *Panel, const TRect &PRect)
{
    TCanvas    	*canvas = StatusBar->Canvas;

    canvas->Brush->Style = bsSolid;

    if( Prefs.UseLagMeter() ) {
        TIRCSession	*sess = GetActiveSession();
        int       	 lag = 0;

        if( sess )
            lag = sess->Lag;

        if( lag ) {
            int 		right = ( lag * ( PRect.Right - PRect.Left - 1 )) / 10; // 10 = max lag
            AnsiString  str = "Lag: " + IntToStr( lag ) + "s";

            if( lag > 8 )
                canvas->Brush->Color = clRed;
            else if( lag > 3 )
                canvas->Brush->Color = clYellow;
            else
                canvas->Brush->Color = clGreen;

            right += PRect.Left + 1;

            if( right > PRect.Right - 1 )
                right = PRect.Right - 1;

            canvas->FillRect( Rect( PRect.Left + 1, PRect.Top + 1, right, PRect.Bottom - 1 ));

            canvas->Brush->Style = bsClear;
            canvas->Font         = StatusBar->Font;

            canvas->TextRect( PRect,
                              PRect.Left + (( PRect.Right - PRect.Left - canvas->TextWidth( str )) / 2 ),
                              3,
                              str );

        } else {
            AnsiString str = "No lag";

            canvas->Brush->Color = clBtnFace;

            canvas->FillRect( PRect );

            canvas->TextRect( PRect,
                              PRect.Left + (( PRect.Right - PRect.Left - canvas->TextWidth( str )) / 2 ),
                              3,
                              str );
        }

    } else {

        canvas->Brush->Color = clBtnFace;

    	canvas->FillRect( PRect );
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::DCCtransfers1Click(TObject *Sender)
{
	if( !DCCMgrWin )
    	DCCMgrWin = new TDCCMgrWin( this );

    DCCMgrWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::NeedDCCMgr( void )
{
	if( !DCCMgrWin ) {
    	TForm  *cur = ActiveMDIChild;

    	DCCMgrWin = new TDCCMgrWin( this );

    	if( cur )
        	cur->BringToFront();
    }
}
//---------------------------------------------------------------------------
TSpeedButton * __fastcall TMainWin::AddTaskbarButton( AnsiString caption, TForm *form, int type, TPopupMenu *popup )
{
	MySpeedButton   *button = new MySpeedButton( NULL );

    button->Caption    = caption;
    button->Tag        = reinterpret_cast<int>( form );
    button->Flat       = true;
    button->GroupIndex = 1;
    button->Width      = 100;
    button->OnClick    = TaskBarClick;

	if( popup )
    	button->PopupMenu = popup;

    TaskbarButtons->Add( button );

    SortTaskbar();
    TaskBarResize( TaskBar );

    TaskBar->Visible   = true;
    TaskBar->AutoSize  = true;

    if( Flags.IsSet( F_FIRST_WINDOW )) {
    	form->WindowState = Prefs.MaximizeOnStart() ? wsMaximized : wsNormal;
    	Flags.Clear( F_FIRST_WINDOW );
    }

	return( button );
}
//---------------------------------------------------------------------------
int __fastcall TMainWin::cmpTabBarServer( void *Item1, void *Item2 )
{
    MySpeedButton  *a = reinterpret_cast<MySpeedButton *>( Item1 );
    MySpeedButton  *b = reinterpret_cast<MySpeedButton *>( Item2 );
    TForm		   *aForm = reinterpret_cast<TForm *>( a->Tag );
    TForm		   *bForm = reinterpret_cast<TForm *>( b->Tag );
	TIRCSession	   *aSess = GetSession( aForm );
	TIRCSession	   *bSess = GetSession( bForm );
	int 			ret = 0;

    if( aSess && bSess )
    	ret = aSess->Server.AnsiCompareIC( bSess->Server );

    if( ret == 0 )
		ret = cmpTabBarType( Item1, Item2 );

    return( ret );
}
//---------------------------------------------------------------------------
int __fastcall TMainWin::cmpTabBarType( void *Item1, void *Item2 )
{
    MySpeedButton  *a = reinterpret_cast<MySpeedButton *>( Item1 );
    MySpeedButton  *b = reinterpret_cast<MySpeedButton *>( Item2 );
    TForm		   *aForm = reinterpret_cast<TForm *>( a->Tag );
    TForm		   *bForm = reinterpret_cast<TForm *>( b->Tag );
	int 			ret = GetSortPriority( aForm ) - GetSortPriority( bForm );

    if( ret == 0 )
        ret = a->Caption.AnsiCompareIC( b->Caption );

    return( ret );
}
//---------------------------------------------------------------------------
int __fastcall TMainWin::GetSortPriority( TForm *form )
{
	int ret = 1000;

    if( dynamic_cast<TServerWin *>( form ))
        ret = 0;
    else if( dynamic_cast<TChanWin *>( form ))
        ret = 1;
    else if( dynamic_cast<TQueryWin *>( form ))
        ret = 2;

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::SortTaskbar( void )
{
	static const TListSortCompare	sortFuncs[] = { NULL, cmpTabBarServer, cmpTabBarType };
    TTabBarSort						sortBy = Prefs.GetTabBarSort();

    if( sortBy != TAB_SORT_NONE )
		TaskbarButtons->Sort( sortFuncs[ sortBy ] );

    for( int i = 0; i < TaskbarButtons->Count; i++ )
    	reinterpret_cast<MySpeedButton *>( TaskbarButtons->Items[ i ] )->Parent = NULL;

    for( int i = 0; i < TaskbarButtons->Count; i++ )
    	reinterpret_cast<MySpeedButton *>( TaskbarButtons->Items[ i ] )->Parent = TaskBar;
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TaskBarResize(TObject *Sender)
{
	if( TaskBar->ButtonCount ) {
        int	width;

        width = 100 * TaskBar->ButtonCount;

        if( width > TaskBar->Width ) {

        	width = TaskBar->Width / TaskBar->ButtonCount;

            if( width < 16 )
            	width = 16;

        } else
			width = 100;

        TaskBar->ButtonWidth = width;

        for( int i = 0; i < TaskBar->ButtonCount; i++ )
        	reinterpret_cast<TSpeedButton *>( TaskBar->Buttons[ i ])->Width = width;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::RemoveTaskbarButton( TSpeedButton *button )
{
    if( button ) {

    	TaskBar->AutoSize = false;
	    button->Parent    = NULL;
        TaskBar->Visible  = TaskBar->ButtonCount > 0;

        TaskbarButtons->Remove( button );

        delete button;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::EnableTaskbarButton( TSpeedButton *button )
{
    if( button ) {
    	button->Font->Color = clBlack;
	    button->Down        = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TaskBarClick( TObject *Sender )
{
	TSpeedButton *b = reinterpret_cast<TSpeedButton *>( Sender );
    TForm		 *form = reinterpret_cast<TForm *>( b->Tag );
/*
    if( form == Screen->ActiveForm ) {

      	switch( form->WindowState ) {

        	case wsNormal:
            case wsMaximized:
            	form->WindowState = wsMinimized;
            	break;

            case wsMinimized: {
				TWindowState state = wsMaximized;

				if( MDIChildCount )
					state = MDIChildren[ 0 ]->WindowState;

            	form->WindowState = state;
            }	break;
        }

    } else {
*/
    	form->Show();
	    form->BringToFront();
    	form->SetFocus();
//    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::SendGiftClick(TObject *Sender)
{
	ShellExecute( Handle, "open", "http://www.tellini.info/software/giftware.html", NULL, NULL, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::DisconnectClick(TObject *Sender)
{
	TIRCSession	*sess = GetActiveSession();

    if( sess )
    	HandleCommand( sess->ServerWin, NULL, "QUIT" );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::DoDNS( AnsiString host, TServerWin *server )
{
	server->DisplayOnActiveWin( "«DNS»", "Looking up " + host, IRC_LOCAL );

    DNSReqs->Add( new DNSReq( host, server ));
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::WMResolved( TMessage &Message )
{
	DNSReq *req = NULL;

    for( int i = 0; !req && ( i < DNSReqs->Count ); i++ ) {
    	DNSReq *tmp = reinterpret_cast<DNSReq *>( DNSReqs->Items[ i ]);

        if( tmp->GetHandle() == (HANDLE)Message.WParam )
        	req = tmp;
    }

    if( req ) {
	    TServerWin *server = FindServerWin( req->GetServer() );

        if( server ) {

            if( WSAGETASYNCERROR( Message.LParam ))
                server->DisplayOnActiveWin( "«DNS»", "Cannot resolve " + req->GetHost(), IRC_LOCAL );
            else
                server->DisplayOnActiveWin( "«DNS»", "Resolved " + req->GetHost() + " to " + req->GetResolvedName(), IRC_LOCAL );
        }

        req->Done();

        DNSReqs->Remove( req );

        delete req;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::PrefsChanged( void )
{
	for( int i = 0; i < MDIChildCount; i++ ) {

	    if( TServerWin *serv = dynamic_cast<TServerWin *>( MDIChildren[ i ] ))
    		serv->PrefsChanged();
	    else if( TChanWin *chan = dynamic_cast<TChanWin *>( MDIChildren[ i ] ))
    		chan->PrefsChanged();
	    else if( TQueryWin *query = dynamic_cast<TQueryWin *>( MDIChildren[ i ] ))
    		query->PrefsChanged();
    }

    SortTaskbar();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::ForwardToActiveWin( const char *nick, AnsiString msg, TIRCEvent type, TForm *wnd )
{
	if( wnd != ActiveMDIChild ) {
    	TServerWin  *aserv, *serv;
        TChanWin    *achan, *chan;
        TQueryWin   *aquery, *query;
	    TIRCSession	*sess;
        AnsiString	source;

        if( serv = dynamic_cast<TServerWin *>( wnd ))
            sess = &serv->Session;
        else if( chan = dynamic_cast<TChanWin *>( wnd )) {
            sess   = &chan->GetServerWin()->Session;
            source = chan->GetChannel();
        } else if( query = dynamic_cast<TQueryWin *>( wnd )) {
            sess   = &query->GetServerWin()->Session;
            source = "Query";
        }

        if( sess != GetSession( ActiveMDIChild )) {

            if( !source.IsEmpty() )
                source.Insert( ":", 0 );

            source.Insert( sess->Server, 0 );
        }

	    if( aserv = dynamic_cast<TServerWin *>( ActiveMDIChild ))
        	aserv->AddRow( NULL, "<" + AnsiString( nick ? nick : "*" ) + "> [" + source + "] " + msg, type );

	    else if( achan = dynamic_cast<TChanWin *>( ActiveMDIChild ))
        	achan->AddRow( nick, "[" + source + "] " + msg, type, true );

	    else if( aquery = dynamic_cast<TQueryWin *>( ActiveMDIChild ))
        	aquery->AddRow( nick, "[" + source + "] " + msg, type, true, false );
    }
}
//---------------------------------------------------------------------------
TIRCSession * __fastcall TMainWin::GetSession( TForm *form )
{
	TIRCSession *ret = NULL;

    if( TServerWin *serv = dynamic_cast<TServerWin *>( form ))
        ret = &serv->Session;
    else if( TChanWin *chan = dynamic_cast<TChanWin *>( form ))
        ret = &chan->GetServerWin()->Session;
    else if( TQueryWin *query = dynamic_cast<TQueryWin *>( form ))
        ret = &query->GetServerWin()->Session;

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::DisplayOnActiveWin( const char *nick, AnsiString msg, TIRCEvent type )
{
   	TServerWin  *aserv;
    TChanWin    *achan;
    TQueryWin   *aquery;

    if( aserv = dynamic_cast<TServerWin *>( ActiveMDIChild ))
        aserv->AddRow( NULL, msg, type );

    else if( achan = dynamic_cast<TChanWin *>( ActiveMDIChild ))
        achan->AddRow( nick, msg, type, true );

    else if( aquery = dynamic_cast<TQueryWin *>( ActiveMDIChild ))
        aquery->AddRow( nick, msg, type, true, true );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::TrayMenuShowClick(TObject *Sender)
{
	Application->Restore();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FavouritesMenuClick(TObject *Sender)
{
	TIRCSession	*sess = GetActiveSession();

    if( FavouritesWin )
       	FavouritesWin->SetServer( sess ? sess->Server : AnsiString( "" ));
    else if( sess )
    	FavouritesWin = new TFavouritesWin( sess->ServerWin, sess->Server );
    else
    	FavouritesWin = new TFavouritesWin( this, "" );

    FavouritesWin->Show();
    FavouritesWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Homepage1Click(TObject *Sender)
{
	ShellExecute( Handle, "open", "http://www.tellini.info/software/orion/", NULL, NULL, 0 );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::URLcatcher1Click(TObject *Sender)
{
	if( !URLCatcherWin )
		URLCatcherWin = new TURLCatcherWin( this );

    URLCatcherWin->Show();
    URLCatcherWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Notifies1Click(TObject *Sender)
{
	if( !NotifyWin )
    	NotifyWin = new TNotifyWin( this );

    NotifyWin->Show();
    NotifyWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Ignorelist1Click(TObject *Sender)
{
	if( !IgnoreWin )
    	IgnoreWin = new TIgnoreWin( this );

    IgnoreWin->Show();
    IgnoreWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::AutoUpdateMenuClick(TObject *Sender)
{
    AutoUpdateMenu->Checked = !AutoUpdateMenu->Checked;

    WUPD_EnableUpdate( &UpdateData, AutoUpdateMenu->Checked );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Checkforupdates1Click(TObject *Sender)
{
	WUPD_CheckUpdate( &UpdateData, WUPDF_MANUAL );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FormCloseQuery( TObject *Sender, bool &CanClose )
{
	CanClose = false;

    if( Prefs.ConfirmQuit() )
		CanClose = Application->MessageBox( "Do you really want to quit?",
        							    	"Quit confirmation",
                                     		MB_YESNO | MB_ICONQUESTION ) == IDYES;
    else
    	CanClose = true;

    if( CanClose ) {
    	bool max = false;

		for( int i = 0; !max && ( i < MDIChildCount ); i++ ) {
	    	TForm *win = MDIChildren[ i ];

            if( win->WindowState == wsMaximized )
            	max = true;
        }

        if( Prefs.MaximizeOnStart() != max ) {
        	Prefs.SetMaximizeOnStart( max );
            Prefs.Save();
        }
    }
}
//---------------------------------------------------------------------------
TServerWin * __fastcall TMainWin::GetCurrentServerWin( void )
{
   	TServerWin  *ret;

    ret = dynamic_cast<TServerWin *>( ActiveMDIChild );

    if( !ret ) {
    	TChanWin    *achan;
	    TQueryWin   *aquery;

    	if( achan = dynamic_cast<TChanWin *>( ActiveMDIChild ))
        	ret = achan->GetServerWin();
	    else if( aquery = dynamic_cast<TQueryWin *>( ActiveMDIChild ))
    	    ret = aquery->GetServerWin();
    }

    return( ret );
}
//---------------------------------------------------------------------------
TChanWin * __fastcall TMainWin::GetCurrentChannelWin( void )
{
    return( dynamic_cast<TChanWin *>( ActiveMDIChild ));
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::ScriptingConsoleMenuClick(TObject *Sender)
{
	if( !ScriptingWin )
		ScriptingWin = new TScriptingWin( this );

    ScriptingWin->Show();
    ScriptingWin->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Mailinglistarchive1Click(TObject *Sender)
{
	ShellExecute( Handle, "open", "http://www.freelists.org/archives/orion/", NULL, NULL, SW_SHOWNORMAL );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::RecvdPrivMsg( void )
{
	if( Flags.IsSet( F_TRAY_ICON )) {

		if( Prefs.RestoreOnPrivMsg() )
			Application->Restore();
	    else
			Flags.Set( F_GOT_PRIV_MSGS );

    } else {

    	if( Prefs.FlashStatusBar() )
        	FlashStatusBar();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::FlashStatusBar( void ) const
{
    HMODULE	dll = LoadLibrary( "user32.dll" );

    if( dll ) {
        FlashWndExFunc func = (FlashWndExFunc)GetProcAddress( dll, "FlashWindowEx" );

        if( func ) {
            FLASHWINFO	info;

            memset( &info, 0, sizeof( info ));

            info.cbSize  = sizeof( info );
            info.hwnd    = Application->Handle;
            info.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;

            func( &info );
        }

        FreeLibrary( dll );
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::Reportabugsubmitasuggestion1Click(TObject *Sender)
{
	ShellExecute( Handle, "open", "http://bugs.tellini.org/", NULL, NULL, SW_SHOWNORMAL );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::ChannelListMenuClick(TObject *Sender)
{
	TIRCSession *session = GetActiveSession();

    if( session )
    	session->ServerWin->SendCmd( "LIST" );
}
//---------------------------------------------------------------------------
void __fastcall	TMainWin::AltDown( bool isDown, WORD &Key )
{
	if( isDown )
        HandleAltDown( Key );
	else
    	HandleAltUp( Key );
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::HandleAltDown( WORD &Key )
{
    int key = -1;

    if(( Key < VK_NUMPAD0 ) || ( Key > VK_NUMPAD9 ))
	    key = AnsiString::StringOfChar( MapVirtualKey( Key, 2 ), 1 ).ToIntDef( -1 );

    if( !Flags.IsSet( F_ALT_DOWN )) {
        int num = min( TaskBar->ButtonCount, 10 );

        for( int i = 0; i < num; i++ ) {
            MySpeedButton *button = reinterpret_cast<MySpeedButton *>( TaskBar->Buttons[ i ]);

            button->SetAltNum(( i + 1 ) % 10 );
        }

        Flags.Set( F_ALT_DOWN );
    }

    if( key >= 0 )
        for( int i = 0; i < TaskBar->ButtonCount; i++ ) {
            MySpeedButton  *button = reinterpret_cast<MySpeedButton *>( TaskBar->Buttons[ i ]);
            int				num = button->GetAltNum();

            if( key == num ) {

                button->Click();

                Key = 0;

		    	Flags.Set( F_IGNORE_SYS_CHAR );
                PostMessage( Handle, WM_KEYDOWN, VK_MENU, 0 );
                PostMessage( Handle, WM_KEYUP, VK_MENU, 0 );
            }
        }
}
//---------------------------------------------------------------------------
void __fastcall TMainWin::HandleAltUp( WORD &Key )
{
    int key = AnsiString::StringOfChar( MapVirtualKey( Key, 2 ), 1 ).ToIntDef( -1 );

    if(( key >= 0 ) && ( key <= 9 ))
    	Key = 0;

    for( int i = 0; i < TaskBar->ButtonCount; i++ ) {
        MySpeedButton  *button = reinterpret_cast<MySpeedButton *>( TaskBar->Buttons[ i ]);
        int				num = button->GetAltNum();

        if( num >= 0 )
			button->SetAltNum( -1 );
    }

	Flags.Clear( F_ALT_DOWN );
}
//---------------------------------------------------------------------------
