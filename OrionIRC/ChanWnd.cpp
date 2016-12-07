/* $Id: ChanWnd.cpp,v 1.69 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Clipbrd.hpp>
#include <stdlib.h>
#include <time.h>

#include "ChanWnd.h"
#include "QueryWnd.h"
#include "ServerWnd.h"
#include "ChanPropWnd.h"
#include "FavouritesWnd.h"
#include "BufferSearchWnd.h"
#include "FavWnd.h"
#include "KickBanWnd.h"
#include "IRCList.h"
#include "IRCEdit.h"
#include "IRCTopic.h"
#include "IRCUtils.h"
#include "User.h"
#include "Commands.h"
#include "Favourite.h"
#include "Prefs.h"
#include "Main.h"
#include "OrionScriptInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "DropPIDLTarget"
#pragma link "DropSource"
#pragma link "DropTarget"
#pragma link "DropURLTarget"
#pragma resource "*.dfm"
TChanWin *ChanWin;
//---------------------------------------------------------------------------
__fastcall TChanWin::TChanWin( TComponent* Owner, TServerWin *server, AnsiString channel )
	: TForm(Owner)
{
	Display    = new TIRCList( this, server, Prefs.GetColorSet() );
    Input	   = new TIRCEdit( this );
    Topic      = new TIRCTopic( this );
    Server     = server;
    Channel    = channel;
    Caption    = Channel + " on " + Server->Session.Server;
    StillIn    = true;
	NamesDone  = true;
    Properties = NULL;

    StatusLedBitmap = new Graphics::TBitmap();

    StatusLedBitmap->Width  = UserPics->Width;
    StatusLedBitmap->Height = UserPics->Height;

    MenuEyelin->Visible = Eyelin::GetInstance()->IsInstalled();

    Input->Parent    = PanelBottom;
    Input->Align     = alClient;
    Input->OnMessage = OnMessage;
    Input->OnCommand = OnCommand;

    Input->SetNickList( Users );
    Input->SetWheelTarget( Display );
    Input->SetChannelName( Channel );

    Display->Parent     = PanelDisplay;
    Display->Align      = alClient;
    Display->OnDblClick = DisplayDblClick;
    Display->PopupMenu  = ChanPopup;

    Display->SetInputCtrl( Input );
    Display->ShowTimestamp( Prefs.ShowTimestamp() );
    Display->ShowNick( true );
    Display->SetAdaptToNick( Prefs.AdaptiveNick() );

    if( Prefs.LogChannels() )
    	Log.Open( Server->Session.Server,
        		  Channel.SubString( 2, Channel.Length() ));

    AddRow( NULL, "You are now talking on " + Channel, IRC_INFO, true );

    Topic->Parent    = PanelTopic;
    Topic->Align     = alClient;
    Topic->PopupMenu = TopicPopup;
    Topic->ShowHint  = true;

    Topic->SetInputCtrl( Input );

    SetFonts();

	ActiveControl = Input;

	srand( time( NULL ));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormDestroy(TObject *Sender)
{
    ClearUsers();

    delete StatusLedBitmap;

    DropPIDLTarget->Unregister();
    DropURLTarget->Unregister();
    DropURLTargetInput->Unregister();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormShow(TObject *Sender)
{
	LayoutModes();
   	UpdateNickLabel();

	TaskbarButton    = MainWin->AddTaskbarButton( Channel, this, TB_CHANNEL, ChanPopup );
    LogMenu->Checked = Log.IsOpen();

	DropPIDLTarget->Register( Users );
	DropURLTarget->Register( Display );
	DropURLTargetInput->Register( Input );

    OrionScript::FireEvent( SE_OPEN, this );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormClose(TObject *Sender, TCloseAction &Action)
{
	if( StillIn ) {
		Server->SendCmd( "PART " + Channel + " :" + Server->Server.GetPartReason() );
        Server->ClosingChannelWin( this );
    }

	Action = caFree;

    Server->SetActiveWin( NULL );
    MainWin->RemoveTaskbarButton( TaskbarButton );

    OrionScript::FireEvent( SE_CLOSE, this );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::SetFonts( void )
{
    Topic->Font->Assign( Prefs.GetTopicFont() );
    Display->Font->Assign( Prefs.GetChannelFont() );
    Input->Font->Assign( Prefs.GetInputFont() );
    NickLabel->Font->Assign( Prefs.GetInputFont() );
    Users->Font->Assign( Prefs.GetNicklistFont() );

    PanelBottom->Height = 6 - Input->Font->Height;
    Input->Height       = PanelBottom->Height - 2;
    PanelTopic->Height  = 7 - Topic->Font->Height;
    Users->ItemHeight   = 3 - Users->Font->Height;

   	UpdateNickLabel();
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::OnCommand( System::TObject *Sender, AnsiString msg )
{
	AnsiString cmd = "/" + msg;

	if( OrionScript::FireEvent( SE_INPUT, this, cmd.c_str() ))
		HandleCommand( Server, this, msg );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::OnMessage( System::TObject *Sender, AnsiString msg )
{
	if( OrionScript::FireEvent( SE_INPUT, this, msg.c_str() ))
		Say( msg );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::Say( AnsiString msg )
{
	if( Server->Session.Flags.IsSet( IRC_SESS_CONNECTED )) {
		int max = IRC_MAX_MSG_LENGTH - 2 - sizeof( "PRIVMSG  " ), start = 1;

		Server->SendMsg( Channel, msg );

    	do {
	    	AnsiString s = msg.SubString( start, max );

			AddRow( Server->Session.Nick.c_str(), s, IRC_OWN_MSG, true );

        	start += s.Length();

	    } while( start <= msg.Length() );

    } else
    	AddRow( NULL, "Not connected to the server!", IRC_ERROR, true );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::CloseNow( void )
{
	StillIn = false;

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormActivate(TObject *Sender)
{
	Server->SetActiveWin( this );
    MainWin->EnableTaskbarButton( TaskbarButton );
    Input->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::Joined( void )
{
	if( ChanData.Flags.IsSet( IRC_CHAN_FLAG_LEFT ))
	    AddRow( NULL, "You are now talking on " + Channel, IRC_INFO, true );

	ChanData.Reset();
    SetModeButtons();

    ShowDisabled( false );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::ShowDisabled( bool disabled )
{
	Input->Color = disabled ? clGray : clWindow;
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::UpdateNames( AnsiString list )
{
	if( NamesDone ) {

		Users->Items->BeginUpdate();
		ClearUsers();

        NamesDone = false;
    }

	while( int pos = list.Pos( " " )) {

    	AddUser( list.SubString( 1, pos - 1 ));

        list.Delete( 1, pos );
    }

    if( !list.IsEmpty() )
    	AddUser( list );

	UpdateNickLabel();
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::ClearUsers( void )
{
	for( int i = 0; i < Users->Items->Count; i++ )
    	delete reinterpret_cast<TUser *>( Users->Items->Objects[ i ] );

    Users->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::AddUser( AnsiString user )
{
	TUser  *usr = new TUser();

    for( int i = 0; i < 5; i++ )
        switch( user[ 1 ] ) {

            case '@':
                usr->SetOp( true );
                user.Delete( 1, 1 );
                break;

            case '%':
            	usr->SetHalfOp( true );
                user.Delete( 1, 1 );
                break;

            case '+':
                usr->SetVoice( true );
                user.Delete( 1, 1 );
                break;

            case '&':
                usr->SetChanAdmin( true );
                user.Delete( 1, 1 );
                break;

            case '~':
                usr->SetOwner( true );
                user.Delete( 1, 1 );
                break;
        }

    usr->SetNick( user );

   	Users->Items->AddObject( user, (System::TObject *)usr );
    UpdateUserStats();
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::RemoveUser( AnsiString user )
{
	int	idx = Users->Items->IndexOf( user );

    if( idx >= 0 )
    	RemoveUser( idx );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::RemoveUser( int idx )
{
	delete reinterpret_cast<TUser *>( Users->Items->Objects[ idx ]);

	Users->Items->Delete( idx );

    UpdateUserStats();
    SortUsers();
}
//---------------------------------------------------------------------------
TUser * __fastcall TChanWin::GetUser( AnsiString nick )
{
	TUser  *ret = NULL;
	int 	idx = Users->Items->IndexOf( nick );

    if( idx >= 0 )
    	ret = reinterpret_cast<TUser *>( Users->Items->Objects[ idx ]);

    return( ret );
}
//---------------------------------------------------------------------------
TUser * __fastcall TChanWin::GetUser( int idx )
{
	TUser  *ret = NULL;

    if( idx < Users->Items->Count )
    	ret = reinterpret_cast<TUser *>( Users->Items->Objects[ idx ]);

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::EndOfNames( void )
{
	NamesDone = true;

	Users->Items->EndUpdate();

    UpdateUserStats();
    SortUsers();

    if( ChanData.Flags.IsSet( IRC_CHAN_FLAG_JOINED )) {

    	if( Prefs.DoWhoOnJoin() && ( Users->Items->Count < Prefs.GetWhoOnJoinMaxUsers() )) {

            Server->SendCmd( "WHO " + Channel );
        	Server->AddWhoAction( SERV_WHO_NOTHING );
        }

    	ChanData.Flags.Clear( IRC_CHAN_FLAG_JOINED );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::ModeChange( IRCParser& parser, bool change )
{
	bool		add, updusers = false;
    AnsiString  modes;
    int			user;
    AnsiString	ops, deops, voices, unvoices, modestr, owners, notowners, halfops, dehalfops;
    AnsiString	bans, excepts, invites, unbans, unexcepts, uninvites;
    bool		rawModes = false;

    if( change ) {
    	modes = parser.GetArg( 1 );
        user  = 2;
    } else {
    	modes = parser.GetArg( 2 );
        user  = 3;
    }

    for( int i = 1; i <= modes.Length(); i++ )
    	switch( modes[ i ]) {

			case '+':
            	add = true;
                break;

            case '-':
            	add = false;
                break;

            case 'o':
            	if( add )
            		ops += ", " + parser.GetArg( user );
                else
            		deops += ", " + parser.GetArg( user );

                OrionScript::FireEvent( add ? SE_OP : SE_DEOP, this, &parser.GetOrigin(), parser.GetArg( user ).c_str() );

            	if( TUser *usr = GetUser( parser.GetArg( user++ )))
                	usr->SetOp( add );

                updusers = true;
            	break;

            case 'h':
            	if( add )
            		halfops += ", " + parser.GetArg( user );
                else
            		dehalfops += ", " + parser.GetArg( user );

                OrionScript::FireEvent( add ? SE_HALFOP : SE_DEHALFOP, this, &parser.GetOrigin(), parser.GetArg( user ).c_str() );

            	if( TUser *usr = GetUser( parser.GetArg( user++ )))
                	usr->SetHalfOp( add );

                updusers = true;
            	break;

            case 'v':
            	if( add )
            		voices += ", " + parser.GetArg( user );
                else
            		unvoices += ", " + parser.GetArg( user );

                OrionScript::FireEvent( add ? SE_VOICE : SE_DEVOICE, this, &parser.GetOrigin(), parser.GetArg( user ).c_str() );

            	if( TUser *usr = GetUser( parser.GetArg( user++ )))
                	usr->SetVoice( add );

                updusers = true;
            	break;

            case 'O':
            	if( add )
            		owners += ", " + parser.GetArg( user );
                else
            		notowners += ", " + parser.GetArg( user );

            	if( TUser *usr = GetUser( parser.GetArg( user++ )))
                	usr->SetOwner( add );

                updusers = true;
            	break;

            case 'a':
            	if( add )
            		modestr += ", anonymous ops";
                else
            		modestr += ", not anonymous ops";

            	ChanData.Modes.Set( IRC_CHAN_MODE_ANONYMOUS, add );
                break;

            case 'm':
            	if( add )
            		modestr += ", moderated";
                else
            		modestr += ", not moderated";

            	ChanData.Modes.Set( IRC_CHAN_MODE_MODERATED, add );
                break;

            case 'i':
            	if( add )
            		modestr += ", invite only";
                else
            		modestr += ", not invite only";

            	ChanData.Modes.Set( IRC_CHAN_MODE_INVITE, add );
                break;

            case 'n':
            	if( add )
            		modestr += ", no external messages";
                else
            		modestr += ", allow external messages";

            	ChanData.Modes.Set( IRC_CHAN_MODE_NO_MSGS, add );
                break;

            case 'q':
            	if( add )
            		modestr += ", quiet";
                else
            		modestr += ", not quiet";

            	ChanData.Modes.Set( IRC_CHAN_MODE_QUIET, add );
                break;

            case 'p':
            	if( add )
            		modestr += ", private";
                else
            		modestr += ", not private";

            	ChanData.Modes.Set( IRC_CHAN_MODE_PRIVATE, add );
                break;

            case 's':
            	if( add )
            		modestr += ", secret";
                else
            		modestr += ", not secret";

            	ChanData.Modes.Set( IRC_CHAN_MODE_SECRET, add );
                break;

            case 'r':
            	if( add )
            		modestr += ", reop";
                else
            		modestr += ", don't reop";

            	ChanData.Modes.Set( IRC_CHAN_MODE_REOP, add );
                break;

            case 't':
            	if( add )
            		modestr += ", topic protection";
                else
            		modestr += ", no topic protection";

            	ChanData.Modes.Set( IRC_CHAN_MODE_TOPIC_PROT, add );
                break;

            case 'l':
            	ChanData.Limit = parser.GetArg( user++ ).ToIntDef( 0 );

            	if( add )
            		modestr += ", limited to " + IntToStr( ChanData.Limit ) + " users";
                else
            		modestr += ", no users limit";

            	ChanData.Modes.Set( IRC_CHAN_MODE_LIMIT, add );
                break;

            case 'k':
            	ChanData.Keyword = parser.GetArg( user++ );

            	if( add )
            		modestr += ", keyword to " + ChanData.Keyword;
                else
            		modestr += ", no keyword";

            	ChanData.Modes.Set( IRC_CHAN_MODE_KEY, add );
                break;

            case 'b':
            	if( add )
            		bans += ", " + parser.GetArg( user );
                else
            		unbans += ", " + parser.GetArg( user );

                OrionScript::FireEvent( add ? SE_BAN : SE_UNBAN, this, &parser.GetOrigin(), parser.GetArg( user ).c_str() );

                user++;
            	break;

            case 'e':
            	if( add )
            		excepts += ", " + parser.GetArg( user );
                else
            		unexcepts += ", " + parser.GetArg( user );

                user++;
            	break;

            case 'I':
            	if( add )
            		invites += ", " + parser.GetArg( user );
                else
            		uninvites += ", " + parser.GetArg( user );

                user++;
            	break;

            default:
            	rawModes = true;
                break;
        }

    if( change ) {

    	if( !rawModes ) {

            if( !ops.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " opped " + ops.SubString( 3, ops.Length() ), IRC_MODE, true );

            if( !deops.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " deopped " + deops.SubString( 3, deops.Length() ), IRC_MODE, true );

            if( !halfops.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " half-opped " + halfops.SubString( 3, halfops.Length() ), IRC_MODE, true );

            if( !dehalfops.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " de-half-opped " + dehalfops.SubString( 3, dehalfops.Length() ), IRC_MODE, true );

            if( !voices.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " gave voice to " + voices.SubString( 3, voices.Length() ), IRC_MODE, true );

            if( !unvoices.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " removed voice from " + unvoices.SubString( 3, unvoices.Length() ), IRC_MODE, true );

            if( !owners.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " gave channel owner status to " + owners.SubString( 3, owners.Length() ), IRC_MODE, true );

            if( !notowners.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " removed channel owner status from " + notowners.SubString( 3, notowners.Length() ), IRC_MODE, true );

            if( !bans.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " banned " + bans.SubString( 3, bans.Length() ), IRC_MODE, true );

            if( !unbans.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " unbanned " + unbans.SubString( 3, unbans.Length() ), IRC_MODE, true );

            if( !excepts.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " added a ban exception for " + excepts.SubString( 3, excepts.Length() ), IRC_MODE, true );

            if( !unexcepts.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " removed the ban exception for " + unexcepts.SubString( 3, unexcepts.Length() ), IRC_MODE, true );

            if( !invites.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " added an auto-invite for " + invites.SubString( 3, invites.Length() ), IRC_MODE, true );

            if( !uninvites.IsEmpty() )
                AddRow( NULL, parser.GetOrigin().GetNick() + " removed the auto-invite for " + uninvites.SubString( 3, uninvites.Length() ), IRC_MODE, true );
        }

	    OrionScript::FireEvent( SE_MODE, this, &parser.GetOrigin(), modes.c_str() );
    }

    if( !modestr.IsEmpty() && !rawModes ) {

    	if( change )
    		AddRow( NULL, parser.GetOrigin().GetNick() + " set channel modes to " + modestr.SubString( 3, modestr.Length() ), IRC_MODE, true );
        else
    		AddRow( NULL, "Channel modes of " + Channel + " are " + modestr.SubString( 3, modestr.Length() ), IRC_MODE, true );
    }

    if( rawModes ) {

        for( int i = change ? 2 : 3; i < parser.GetArgsCount(); i++ )
	        modes += " " + parser.GetArg( i );

    	if( change )
    		AddRow( NULL, parser.GetOrigin().GetNick() + " sets mode " + modes, IRC_MODE, true );
        else
    		AddRow( NULL, "Channel modes of " + Channel + " are " + modes, IRC_MODE, true );
    }

    SetModeButtons();

    if( updusers ) {
        Users->Repaint();
	    UpdateUserStats();
		UpdateNickLabel();
        SortUsers();
    }
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::SetModeButtons( void )
{
    ModeA->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_ANONYMOUS  );
    ModeM->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_MODERATED  );
    ModeI->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_INVITE     );
    ModeN->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_NO_MSGS    );
    ModeQ->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_QUIET      );
    ModeP->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_PRIVATE    );
    ModeS->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_SECRET     );
    ModeR->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_REOP       );
    ModeT->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_TOPIC_PROT );
    ModeL->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_LIMIT      );
    ModeK->Down = ChanData.Modes.IsSet( IRC_CHAN_MODE_KEY        );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::DisplayEvent( int ev, AnsiString msg, TIRCEvent type )
{
    switch( Server->Server.GetEventDisplay( ev )) {

        case TServer::SHOW_CHANNEL:
            AddRow( NULL, msg, type, true );
            break;

        case TServer::SHOW_STATUS:
            Server->AddRow( NULL, msg, type, true );
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::NickChange( AnsiString oldnick, AnsiString newnick )
{
	int idx = Users->Items->IndexOf( oldnick );

    if( idx >= 0 ) {

    	reinterpret_cast<TUser *>( Users->Items->Objects[ idx ] )->SetNick( newnick );

    	Users->Items->Strings[ idx ] = newnick;

        DisplayEvent( TServer::EV_NICK,
        			  oldnick + " is now known as " + newnick,
        			  IRC_NICK );

    	UpdateNickLabel();
        SortUsers();

        OrionScript::FireEvent( SE_NICK, this, oldnick.c_str(), newnick.c_str() );
    }
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::UpdateUserStats( void )
{
	int ops = 0;

	for( int i = 0; i < Users->Items->Count; i++ ) {
    	TUser *usr = reinterpret_cast<TUser *>( Users->Items->Objects[ i ] );

        if( usr->IsOp() )
        	ops++;
    }

    if( ops )
	    UsersStats->Caption = Format( "%d users, %d ops", ARRAYOFCONST(( Users->Items->Count, ops )));
    else
	    UsersStats->Caption = Format( "%d users", ARRAYOFCONST(( Users->Items->Count )));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::UsersDrawItem(TWinControl *Control, int Index,
      TRect &Rect, TOwnerDrawState State)
{
    TListBox   *list = reinterpret_cast<TListBox *>( Control );
    TCanvas    *canvas = list->Canvas;

    // clear the rectangle
    if( State.Contains( odSelected ))
        canvas->Brush->Color = static_cast<TColor>( GetSysColor( COLOR_HIGHLIGHT ));
    else
        canvas->Brush->Color = list->Color;

    canvas->FillRect( Rect );

    if( Index < list->Items->Count ) {
   		TUser *user = reinterpret_cast<TUser *>( Users->Items->Objects[ Index ] );
	   	int    flags = -1, Offset = Rect.Left + UserPics->Width + 2;

        if( user->IsOwner() )
            flags = 3;
        else if( user->IsChanAdmin() )
            flags = 2;
        else if( user->IsOp() )
            flags = 0;
        else if( user->IsHalfOp() )
        	flags = 5;
        else if( user->HasVoice() )
            flags = 1;

        // symbol
        if( flags >= 0 )
            UserPics->Draw( canvas,
                            Rect.Left + 1,
                            Rect.Top + (( Rect.Bottom - Rect.Top - UserPics->Height ) / 2 ),
                            flags );

        // display the user nick
        canvas->Pen->Color = State.Contains( odSelected ) ? static_cast<TColor>( GetSysColor( COLOR_HIGHLIGHTTEXT )) : clBlack;

        Rect.Left += Offset;

        canvas->TextRect( Rect, Offset, Rect.Top, Users->Items->Strings[ Index ]);
    }
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::SetUserHost( AnsiString nick, AnsiString host )
{
	TUser *usr = GetUser( nick );

    if( usr )
    	usr->SetHost( host );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::UserJoin( HostMask& mask )
{
	AddUser( mask.GetNick() );
    GetUser( mask.GetNick() )->SetHost( mask.GetUserHost() );
    SortUsers();

    DisplayEvent( TServer::EV_JOIN,
                  mask.GetNick() + " (" + mask.GetUserHost() + ") has joined " + Channel,
                  IRC_JOIN );

    OrionScript::FireEvent( SE_JOIN, this, &mask );
}
//---------------------------------------------------------------------------
void __fastcall	TChanWin::UserPart( HostMask& mask, AnsiString reason, bool quit )
{
	int idx = Users->Items->IndexOf( mask.GetNick() );

    if( idx >= 0 ) {
    	AnsiString	verb = quit ? "quit" : "left";

        RemoveUser( idx );

        DisplayEvent( quit ? TServer::EV_QUIT : TServer::EV_PART,
                      mask.GetNick() + " (" + mask.GetUserHost() + ") has " +
       				  verb + " " + Channel + " (" + reason + ")",
                      quit ? IRC_QUIT : IRC_PART );
    }

    OrionScript::FireEvent( quit ? SE_QUIT : SE_PART, this, &mask );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Kick( HostMask& mask, AnsiString user, AnsiString reason )
{
	if( !user.AnsiCompareIC( Server->Session.Nick )) {

		AddRow( NULL,
        		"You've been kicked out " + Channel + " by " + mask.GetNick() + " (" + reason + ")",
                IRC_KICK,
                true );

        ShowDisabled( true );

        if( Prefs.RejoinOnKick() ) {
        	AnsiString cmd = "JOIN " + Channel;

        	AddRow( NULL, "Attempting a rejoin...", IRC_LOCAL, true );

            if( ChanData.Modes.IsSet( IRC_CHAN_MODE_KEY ))
            	cmd += " " + ChanData.Keyword;

        	Server->SendCmd( cmd );
        }

    } else {

    	RemoveUser( user );

        DisplayEvent( TServer::EV_KICK,
                      user + " has been kicked out " + Channel + " by " +
                      mask.GetNick() + " (" + reason + ")",
                      IRC_KICK );
    }

    OrionScript::FireEvent( SE_KICK, this, &mask, user.c_str(), reason.c_str() );
	Prefs.GetSounds()->Play( SoundsMgr::KICK );
}
//---------------------------------------------------------------------------
AnsiString __fastcall TChanWin::GetNick( int index )
{
	return( reinterpret_cast<TUser *>( Users->Items->Objects[ index ] )->GetNick() );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Whois1Click(TObject *Sender)
{
	if( Users->SelCount == 1 ) {

    	for( int i = 0; i < Users->Items->Count; i++ )
        	if( Users->Selected[ i ] ) {
				HandleCommand( Server, this, "WHOIS " + GetNick( i ));
                break;
            }

    } else {

        for( int i = 0; i < Users->Items->Count; ) {
            AnsiString	nicks;

            for( int j = 0; ( j < 3 ) && ( i < Users->Items->Count ); i++ ) {

                if( !Users->Selected[ i ] )
                    continue;

                if( j++ )
                    nicks += ",";

                nicks += GetNick( i );
            }

            if( !nicks.IsEmpty() )
                Server->SendCmd( "WHOIS " + nicks );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::VersionClick(TObject *Sender)
{
	AnsiString	ctcp;

    if( Sender == Version )
    	ctcp = "VERSION";
    else if( Sender == ClientInfo )
    	ctcp = "CLIENTINFO";
    else if( Sender == Finger )
    	ctcp = "FINGER";
    else if( Sender == Time )
    	ctcp = "TIME";
    else if( Sender == Ping )
    	ctcp = "PING " + IntToStr( time( NULL ));

   	for( int i = 0; i < Users->Items->Count; ) {
    	AnsiString	nicks;

		for( int j = 0; ( j < 3 ) && ( i < Users->Items->Count ); i++ ) {

        	if( !Users->Selected[ i ] )
            	continue;

			if( j++ )
            	nicks += ",";

            nicks += GetNick( i );
        }

        if( !nicks.IsEmpty() ) {
			AnsiString	cmd = "PRIVMSG " + nicks + " :\001" + ctcp + "\001";

		    if( Sender == Ping )
    			Server->SendUrgCmd( cmd );
		    else
        		Server->SendCmd( cmd );
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Openaquery1Click(TObject *Sender)
{
   	for( int i = 0; i < Users->Items->Count; i++ )
       	if( Users->Selected[ i ] )
        	Server->FindQueryWin( GetNick( i ))->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::OpClick(TObject *Sender)
{
	AnsiString	mode, action;

    if( Sender == Op ) {
    	mode   = "o";
        action = "+";
    } else if( Sender == Deop ) {
    	mode   = "o";
        action = "-";
    } else if( Sender == Voice ) {
    	mode   = "v";
        action = "+";
    } else if( Sender == Unvoice ) {
    	mode   = "v";
        action = "-";
    }

   	for( int i = 0; i < Users->Items->Count; ) {
    	AnsiString	modes, nicks;

		for( int j = 0; ( j < Server->Session.ModesCount ) && ( i < Users->Items->Count ); i++ ) {

        	if( !Users->Selected[ i ] )
            	continue;

			if( j++ )
            	nicks += " ";

            nicks += GetNick( i );
            modes += mode;
        }

        if( !nicks.IsEmpty() )
        	Server->SendCmd( "MODE " + Channel + " " + action + modes + " " + nicks );
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Slap1Click(TObject *Sender)
{
   	for( int i = 0; i < Users->Items->Count; i++ )
       	if( Users->Selected[ i ] )
			HandleCommand( Server, this, "SLAP " + GetNick( i ));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::UsersMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Input->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::UsersMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 	int idx = Users->ItemAtPos( Point( X, Y ), true );

	if(( idx >= 0 ) && ( Button == mbRight )) {

    	if( !Shift.Contains( ssCtrl ) && !Users->Selected[ idx ])
        	for( int i = 0; i < Users->Items->Count; i++ )
				Users->Selected[ i ] = false;

		Users->Selected[ idx ] = true;
    }

	for( int i = 0; i < UserListMenu->Items->Count; i++ )
    	UserListMenu->Items->Items[ i ]->Enabled = Users->SelCount > 0;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TChanWin::GetTopic( void ) const
{
	return( Topic->GetTopic() );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::SetTopic( AnsiString topic )
{
    if( Prefs.StripColors() )
        topic = IRCUtils::StripColors( topic );

    ChanData.Topics->Insert( 0, topic );
	Topic->SetTopic( topic );

    Topic->Hint = IRCUtils::StripCodes( topic );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::CopyTopicToClipboardClick(TObject *Sender)
{
    Clipboard()->SetTextBuf( Topic->GetTopic().c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DCCChat1Click(TObject *Sender)
{
	if( Users->ItemIndex >= -1 ) {
		TQueryWin *query = Server->FindQueryWin( GetNick( Users->ItemIndex ));

    	query->BringToFront();

	    if( !query->DCCInProgress() )
    		query->DCCChatClick( this );
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DCCSendfile1Click(TObject *Sender)
{
	if( Server->OpenDlg->Execute() ) {

	   	for( int i = 0; i < Users->Items->Count; i++ )
    	   	if( Users->Selected[ i ] ) {

	    		for( int j = 0; j < Server->OpenDlg->Files->Count; j++ )
    	    		Server->DCCSend( GetNick( i ), Server->OpenDlg->Files->Strings[ j ]);
            }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::AddRow( const char *nick, AnsiString msg, TIRCEvent type, bool skipfilters )
{
	MainWin->URLCatcher.Filter( nick ? nick : "-", msg );

	if( skipfilters || Prefs.GetFilters()->Process( nick, msg, type, this )) {

        if( Prefs.StripColors() )
            msg = IRCUtils::StripColors( msg );

		Display->AddRow( nick, msg, type );
        Log.Log( nick, msg, type );

    	if( !Active && TaskbarButton ) {

        	if( type == IRC_HIGHLIGHT )
	    		TaskbarButton->Font->Color = clGreen;
            else if( TaskbarButton->Font->Color != clGreen )
             	TaskbarButton->Font->Color = clRed;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::EditProperties( System::TObject *Sender )
{
	Properties = new TChanPropWin( NULL, this );

    Properties->ShowModal();

    delete Properties;

    Properties = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DNSMenuClick(TObject *Sender)
{
   	for( int i = 0; i < Users->Items->Count; i++ )
       	if( Users->Selected[ i ] )
			HandleCommand( Server, this, "DNS " + GetNick( i ));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::PrefsChanged( void )
{
	SetFonts();
    LayoutModes();
    SortUsers();

    Display->ShowTimestamp( Prefs.ShowTimestamp() );
    Display->SetAdaptToNick( Prefs.AdaptiveNick() );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Closewindow1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Properties1Click(TObject *Sender)
{
	EditProperties( Sender );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Clearbuffer1Click(TObject *Sender)
{
	Display->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Leaverejoin1Click(TObject *Sender)
{
	HandleCommand( Server, this, "CYCLE" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeNClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeN->Down ? "+" : "-" ) + "n" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeSClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeS->Down ? "+" : "-" ) + "s" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeTClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeT->Down ? "+" : "-" ) + "t" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeMClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeM->Down ? "+" : "-" ) + "m" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeIClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeI->Down ? "+" : "-" ) + "i" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModePClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeP->Down ? "+" : "-" ) + "p" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeRClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeR->Down ? "+" : "-" ) + "r" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeAClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeA->Down ? "+" : "-" ) + "a" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeQClick(TObject *Sender)
{
    Server->SendCmd( "MODE " + Channel + " " + AnsiString( ModeQ->Down ? "+" : "-" ) + "q" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeKClick(TObject *Sender)
{
	if( ModeK->Down )
		EditProperties( Sender );
    else
	    Server->SendCmd( "MODE " + Channel + " -k " + ChanData.Keyword );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::ModeLClick(TObject *Sender)
{
	if( ModeL->Down )
		EditProperties( Sender );
    else
	    Server->SendCmd( "MODE " + Channel + " -l" );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::PanelModesResize(TObject *Sender)
{
    ModeA->Height = PanelModes->Height;
    ModeM->Height = PanelModes->Height;
    ModeI->Height = PanelModes->Height;
    ModeN->Height = PanelModes->Height;
    ModeQ->Height = PanelModes->Height;
    ModeP->Height = PanelModes->Height;
    ModeS->Height = PanelModes->Height;
    ModeR->Height = PanelModes->Height;
    ModeT->Height = PanelModes->Height;
    ModeL->Height = PanelModes->Height;
    ModeK->Height = PanelModes->Height;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::LayoutModes( void )
{
	int				left = 0;
    struct {
    	TSpeedButton   *but;
        const char	   *mode;
    }				buttons[] = {
					    { ModeN, "n" },
					    { ModeS, "s" },
					    { ModeT, "t" },
					    { ModeM, "m" },
					    { ModeI, "i" },
					    { ModeP, "p" },
					    { ModeR, "r" },
					    { ModeA, "a" },
					    { ModeQ, "q" },
					    { ModeL, "k" },
					    { ModeK, "l" },
    				};
#define NUM_MODES	( sizeof( buttons ) / sizeof( buttons[ 0 ] ))

	for( int i = 0; i < NUM_MODES; i++ ) {

    	buttons[ i ].but->Visible = Server->Session.ChanModes.Pos( buttons[ i ].mode ) > 0;

	    if( buttons[ i ].but->Visible ) {
    		buttons[ i ].but->Left  = left;
        	left                   += buttons[ i ].but->Width;
	    }
    }

	PanelModes->Width   = left;
	PanelModes->Visible = Prefs.ShowChanModes();
#undef NUM_MODES
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Addtofavorites1Click(TObject *Sender)
{
	if( FavouritesWin )
    	FavouritesWin->AddClick( FavouritesWin->Add );
    else {
        TFavourite *fav = new TFavourite();
        TFavWin    *wnd = new TFavWin( NULL, fav );

        fav->SetChannel( Channel );
        fav->SetNetwork( Server->Session.Network );

        if( wnd->ShowModal() == mrOk ) {

        	Prefs.GetFavourites()->Add( fav );
            Prefs.Save();

        } else
            delete fav;

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::LogMenuClick(TObject *Sender)
{
	if( Log.IsOpen() )
    	Log.Close();
    else
    	Log.Open( Server->Session.Server, Channel.SubString( 2, Channel.Length() ));

    LogMenu->Checked = Log.IsOpen();

    if( Log.IsOpen() )
    	Display->AddRow( NULL, "Opened log file \"" + Log.GetLogFile() + "\"", IRC_LOCAL );
    else
    	Display->AddRow( NULL, "Log closed", IRC_LOCAL );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropPIDLTargetDrop(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	int idx = Users->ItemAtPos( Point, true );

    if( idx >= 0 ) {
    	AnsiString	nick = GetNick( idx );

        for( int i = 0; i < DropPIDLTarget->Filenames->Count; i++ )
            Server->DCCSend( nick, DropPIDLTarget->Filenames->Strings[ i ]);
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropPIDLTargetGetDropEffect(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	int idx = Users->ItemAtPos( Point, true );

    Effect = ( idx >= 0 ) ? DROPEFFECT_COPY : DROPEFFECT_NONE;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropURLTargetDrop(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	OnMessage( Input, DropURLTarget->URL );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropURLTargetInputDrop(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
	Input->SelText = DropURLTargetInput->URL;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropURLTargetInputGetDropEffect(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
    Effect = DropURLTargetInput->URL.IsEmpty() ? DROPEFFECT_NONE : DROPEFFECT_LINK;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DropURLTargetGetDropEffect(TObject *Sender,
      TShiftState ShiftState, tagPOINT &Point, int &Effect)
{
    Effect = DropURLTarget->URL.IsEmpty() ? DROPEFFECT_NONE : DROPEFFECT_LINK;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::KickMenuClick(TObject *Sender)
{
	TKickBanWin	*wnd = new TKickBanWin( NULL );
    int			 action = wnd->Execute();

    if( action != KB_CANCEL ) {

    	if( action != KB_KICK )
            for( int i = 0; i < Users->Items->Count; ) {
                AnsiString	modes, nicks;

                for( int j = 0; ( j < Server->Session.ModesCount ) && ( i < Users->Items->Count ); i++ ) {
                	TUser  	   *user;
                    AnsiString 	host;

                    if( !Users->Selected[ i ] )
                        continue;

                    if( j++ )
                        nicks += " ";

                    user = reinterpret_cast<TUser *>( Users->Items->Objects[ i ]);
                    host = user->GetHost();

                    if( host.IsEmpty() )
                    	nicks += user->GetNick();
                    else {
                    	HostMask	mask( user->GetNick() + "!" + host );

	                    nicks += mask.GetMask( wnd->GetMaskType() );
                    }

                    modes += "b";
                }

                if( !nicks.IsEmpty() )
                    Server->SendCmd( "MODE " + Channel + " +" + modes + " " + nicks );
            }

        if( action != KB_BAN )
        	KickUsers( wnd->Reason->Text );
    }

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::KickUsers( AnsiString reason )
{
    for( int i = 0; i < Users->Items->Count; ) {
        AnsiString	nicks;

        for( int j = 0; ( j < Server->Session.ModesCount ) && ( i < Users->Items->Count ); i++ ) {

            if( !Users->Selected[ i ] )
                continue;

            if( j++ )
                nicks += ",";

            nicks += GetNick( i );
        }

        if( !nicks.IsEmpty() )
            Server->SendCmd( "KICK " + Channel + " " + nicks + " :" + reason );
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Addtonotifylist1Click(TObject *Sender)
{
    for( int i = 0; i < Users->Items->Count; i++ ) {

        if( Users->Selected[ i ] )
        	Prefs.GetNotifies()->Add( GetNick( i ));
    }

    Prefs.Save();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Ignore1Click(TObject *Sender)
{
    for( int i = 0; i < Users->Items->Count; i++ ) {
        if( Users->Selected[ i ] ) {
	    	TUser  *user = reinterpret_cast<TUser *>( Users->Items->Objects[ i ] );
        	String 	item, host;

            item = user->GetNick();
            host = user->GetHost();

            if( !host.IsEmpty() )
            	item += "!" + host;

			HandleCommand( Server, this, "IGNORE " + item );
		}
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Savebuffer1Click(TObject *Sender)
{
	Display->SaveBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::MenuEyelinClick(TObject *Sender)
{
    for( int i = 0; i < Users->Items->Count; i++ )
        if( Users->Selected[ i ] )
            HandleCommand( Server, this, "DCC EYELIN " + GetNick( i ));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::PanelBottomResize(TObject *Sender)
{
	StatusLed->Top = ( PanelNick->Height - StatusLed->Height ) / 2;
	NickLabel->Top = ( PanelNick->Height - NickLabel->Height ) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::UpdateNickLabel( void )
{
	TUser  *user = GetUser( Server->Session.Nick );
    int		led = 4;

	NickLabel->Caption = Server->Session.Nick;
    PanelNick->Width   = NickLabel->Left + NickLabel->Width + 3;

    if( user ) {

        if( user->IsOwner() )
            led = 3;
        else if( user->IsChanAdmin() )
            led = 2;
        else if( user->IsOp() )
            led = 0;
        else if( user->IsHalfOp() )
        	led = 5;
        else if( user->HasVoice() )
            led = 1;
    }


    StatusLedBitmap->Canvas->Brush->Color = PanelNick->Color;

    StatusLedBitmap->Canvas->FillRect( Rect( 0, 0, StatusLedBitmap->Width, StatusLedBitmap->Height ));

    UserPics->Draw( StatusLedBitmap->Canvas, 0, 0, led );

    StatusLed->Picture->Bitmap = StatusLedBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::Search1Click(TObject *Sender)
{
	TBufferSearchWin *wnd = new TBufferSearchWin( NULL, Display );

    wnd->ShowModal();

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::SortUsers( void )
{
	static const TListSortCompare   sortFuncs[] = { NickSortAlpha, NickSortModes };
	TList  		   				   *list = new TList();
    int								i = 0;

	while( TUser *user = GetUser( i++ ))
    	list->Add( user );

    list->Sort( sortFuncs[ Prefs.GetNickSort() ] );

    Users->Items->BeginUpdate();
    Users->Items->Clear();

    for( int i = 0; i < list->Count; i++ ) {
    	TUser *user = reinterpret_cast<TUser *>( list->Items[ i ] );

        Users->Items->AddObject( user->GetNick(), reinterpret_cast<TObject *>( user ));
    }

    Users->Items->EndUpdate();

    delete list;
}
//---------------------------------------------------------------------------
int __fastcall TChanWin::NickSortAlpha( void *Item1, void *Item2 )
{
	TUser *a = reinterpret_cast<TUser *>( Item1 );
	TUser *b = reinterpret_cast<TUser *>( Item2 );

    return( a->GetNick().AnsiCompareIC( b->GetNick() ));
}
//---------------------------------------------------------------------------
int __fastcall TChanWin::NickSortModes( void *Item1, void *Item2 )
{
	TUser  *a = reinterpret_cast<TUser *>( Item1 );
	TUser  *b = reinterpret_cast<TUser *>( Item2 );
    int		ret = GetModePri( b ) - GetModePri( a );

    if( ret == 0 )
    	ret = a->GetNick().AnsiCompareIC( b->GetNick() );

    return( ret );
}
//---------------------------------------------------------------------------
int __fastcall TChanWin::GetModePri( TUser *user )
{
	int ret = 0;

    if( user->IsOwner() )
        ret = 5;
    else if( user->IsChanAdmin() )
        ret = 4;
    else if( user->IsOp() )
        ret = 3;
    else if( user->IsHalfOp() )
    	ret = 2;
    else if( user->HasVoice() )
        ret = 1;

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::UsersDblClick(TObject *Sender)
{
    for( int i = 0; i < Users->Items->Count; i++ )
        if( Users->Selected[ i ] )
			Prefs.GetDblClickAction( DBL_CLICK_NICK ).Execute( this, GetUser( i ));
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::DisplayDblClick( TObject *Sender )
{
	Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).Execute( this );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Shift.Contains( ssAlt ))
    	MainWin->AltDown( true, Key );
}
//---------------------------------------------------------------------------
void __fastcall TChanWin::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( !Shift.Contains( ssAlt ))
    	MainWin->AltDown( false, Key );
}
//---------------------------------------------------------------------------

