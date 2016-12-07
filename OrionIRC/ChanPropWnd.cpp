/* $Id: ChanPropWnd.cpp,v 1.3 2003/05/10 13:32:45 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChanPropWnd.h"
#include "ChanWnd.h"
#include "ServerWnd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "RXSpin"
#pragma resource "*.dfm"
TChanPropWin *ChanPropWin;
//---------------------------------------------------------------------------
__fastcall TChanPropWin::TChanPropWin( TComponent* Owner, TChanWin *channel )
	: TForm(Owner)
{
	Channel = channel;
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::FormShow(TObject *Sender)
{
    TServerWin	   *server = Channel->GetServerWin();

	Pages->ActivePage     = Page1;
	NoExtMsgs->Checked    = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_NO_MSGS    );
	Moderated->Checked    = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_MODERATED  );
	InviteOnly->Checked   = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_INVITE     );
	Secret->Checked       = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_SECRET     );
	Private->Checked      = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_PRIVATE    );
   	TopicProt->Checked    = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_TOPIC_PROT );
	AnonOps->Checked      = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_ANONYMOUS  );
    AnonOps->Enabled      = server->Session.ChanModes.Pos( "a" ) > 0;
	Quiet->Checked        = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_QUIET      );
    Quiet->Enabled        = server->Session.ChanModes.Pos( "q" ) > 0;
	Reop->Checked         = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_REOP       );
    Reop->Enabled         = server->Session.ChanModes.Pos( "r" ) > 0;
	LimitUsers->Checked   = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_LIMIT      );
	UseKeyword->Checked   = Channel->ChanData.Modes.IsSet( IRC_CHAN_MODE_KEY        );
    UserLimit->Value      = Channel->ChanData.Limit;
    Keyword->Text         = Channel->ChanData.Keyword;
    TopicSetBy->Caption   = Channel->ChanData.TopicSetBy;
    TopicSetTime->Caption = Channel->ChanData.TopicSetTime;
	Keyword->Enabled      = UseKeyword->Checked;
	UserLimit->Enabled    = LimitUsers->Checked;

    Topic->Items->Assign( Channel->ChanData.Topics );

    if( Topic->Items->Count )
    	Topic->ItemIndex = 0;

    server->SendUrgCmd( "MODE " + Channel->GetChannel() + " +b" );

    if( server->Session.ChanModes.Pos( "e" ))
	    server->SendUrgCmd( "MODE " + Channel->GetChannel() + " +e" );

    if( server->Session.ChanModes.Pos( "I" ))
    	server->SendUrgCmd( "MODE " + Channel->GetChannel() + " +I" );
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::UseKeywordClick(TObject *Sender)
{
	Keyword->Enabled = UseKeyword->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::LimitUsersClick(TObject *Sender)
{
	UserLimit->Enabled = LimitUsers->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::OkClick(TObject *Sender)
{
	BitField		flags;
    int    			changed, numchanged;
    TServerWin	   *server = Channel->GetServerWin();
    AnsiString		plus, minus, set;
    static const struct {
    	unsigned int	Flag;
        const char	   *Mode;
    }				Modes[] = {
    	{ IRC_CHAN_MODE_NO_MSGS,    "n" },
    	{ IRC_CHAN_MODE_MODERATED,  "m" },
    	{ IRC_CHAN_MODE_INVITE,     "i" },
    	{ IRC_CHAN_MODE_SECRET,     "s" },
    	{ IRC_CHAN_MODE_PRIVATE,    "p" },
    	{ IRC_CHAN_MODE_TOPIC_PROT, "t" },
        { IRC_CHAN_MODE_ANONYMOUS,  "a" },
        { IRC_CHAN_MODE_QUIET,      "q" },
        { IRC_CHAN_MODE_REOP,       "r" },
    };
#define NUM_MODES ( sizeof( Modes ) / sizeof( Modes[0] ))

	flags.Set( IRC_CHAN_MODE_NO_MSGS,    NoExtMsgs->Checked  );
	flags.Set( IRC_CHAN_MODE_MODERATED,  Moderated->Checked  );
	flags.Set( IRC_CHAN_MODE_INVITE,     InviteOnly->Checked );
	flags.Set( IRC_CHAN_MODE_SECRET,     Secret->Checked     );
	flags.Set( IRC_CHAN_MODE_PRIVATE,    Private->Checked    );
   	flags.Set( IRC_CHAN_MODE_TOPIC_PROT, TopicProt->Checked  );
	flags.Set( IRC_CHAN_MODE_ANONYMOUS,  AnonOps->Checked    );
	flags.Set( IRC_CHAN_MODE_QUIET,      Quiet->Checked      );
	flags.Set( IRC_CHAN_MODE_REOP,       Reop->Checked       );
	flags.Set( IRC_CHAN_MODE_LIMIT,      LimitUsers->Checked );
	flags.Set( IRC_CHAN_MODE_KEY,        UseKeyword->Checked );

    changed = flags.Get() ^ Channel->ChanData.Modes.Get();

    for( int i = 0; i < NUM_MODES; i++ )
    	if( changed & Modes[ i ].Flag ) {

         	if( flags.IsSet( Modes[ i ].Flag ))
            	plus += Modes[ i ].Mode;
            else
            	minus += Modes[ i ].Mode;
        }

    numchanged = plus.Length() + minus.Length();

    for( int i = 1; i <= numchanged; ) {

    	set = "";

    	for( int j = 0; ( i <= numchanged ) && ( j < server->Session.ModesCount ); i++ ) {

        	if( i > plus.Length() )
            	set += "-" + AnsiString( minus[ i - plus.Length() ]);
            else
            	set += "+" + AnsiString( plus[ i ]);
        }

	    server->SendCmd( "MODE " + Channel->GetChannel() + " " + set );
    }

    if(( changed & IRC_CHAN_MODE_LIMIT ) ||
       ( UserLimit->Value != Channel->ChanData.Limit )) {

    	if( flags.IsSet( IRC_CHAN_MODE_LIMIT ))
		    server->SendCmd( "MODE " + Channel->GetChannel() + " +l " + IntToStr( UserLimit->Value ));
        else
		    server->SendCmd( "MODE " + Channel->GetChannel() + " -l" );
    }

    if(( changed & IRC_CHAN_MODE_KEY ) ||
       ( Keyword->Text != Channel->ChanData.Keyword )) {

    	if( flags.IsSet( IRC_CHAN_MODE_KEY )) {

        	if(!( changed & IRC_CHAN_MODE_KEY ))
			    server->SendCmd( "MODE " + Channel->GetChannel() + " -k " + Channel->ChanData.Keyword );

		    server->SendCmd( "MODE " + Channel->GetChannel() + " +k " + Keyword->Text );

        } else
		    server->SendCmd( "MODE " + Channel->GetChannel() + " -k " + Channel->ChanData.Keyword );
    }

    set = "";

    if( Channel->ChanData.Topics->Count )
    	set = Channel->ChanData.Topics->Strings[ 0 ];

    if( Topic->Text.AnsiCompare( set ))
    	server->SendCmd( "TOPIC " + Channel->GetChannel() + " :" + Topic->Text );

    ProcessList( Bans,    "b" );
    ProcessList( Excepts, "e" );
    ProcessList( Invites, "I" );
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::AddListItem( TIRCChanList list, AnsiString item,
										   AnsiString setby, AnsiString settime )
{
	TListView  *lw;
    TListItem  *li;

	switch( list ) {

    	case IRC_CHAN_LIST_BAN:
        	lw = Bans;
        	break;

    	case IRC_CHAN_LIST_EXCEPT:
        	lw = Excepts;
        	break;

    	case IRC_CHAN_LIST_INVITE:
        	lw = Invites;
        	break;
    }

    li          = lw->Items->Add();
    li->Caption = item;
    li->Checked = true;

    li->SubItems->Add( setby );
    li->SubItems->Add( settime );
}
//---------------------------------------------------------------------------
void __fastcall TChanPropWin::ProcessList( TListView *list, AnsiString flag )
{
    TServerWin *server = Channel->GetServerWin();

	for( int i = 0; i < list->Items->Count; i++ ) {
    	TListItem *item = list->Items->Item[ i ];

        if( !item->Checked )
        	server->SendCmd( "MODE " + Channel->GetChannel() + " -" + flag + " " + item->Caption );
    }
}
//---------------------------------------------------------------------------
