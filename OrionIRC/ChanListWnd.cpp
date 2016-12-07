/* $Id: ChanListWnd.cpp,v 1.1 2005/08/19 14:53:03 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChanListWnd.h"
#include "ServerWnd.h"
#include "IRCUtils.h"
//---------------------------------------------------------------------------

class TChanInfo
{
public:
					TChanInfo( AnsiString channel, int users, AnsiString topic )
                    {
                    	Channel = channel;
                        Users   = users;
                        Topic   = topic;
                    }

	AnsiString		Channel;
    int				Users;
    AnsiString		Topic;
};

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EnhListView"
#pragma link "ExtListView"
#pragma link "Placemnt"
#pragma link "RXSpin"
#pragma resource "*.dfm"
TChanListWin *ChanListWin;
//---------------------------------------------------------------------------
__fastcall TChanListWin::TChanListWin( TServerWin *Owner )
	: TForm(Owner)
{
	Server          = Owner;
    Channels        = new TList();
    VisibleChannels = new TList();
    SortAsc         = List->CurrentSortAscending;
    SortCol         = List->LastColumnClicked;
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::FormDestroy(TObject *Sender)
{
	Clear();

    delete Channels;
    delete VisibleChannels;
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::Clear( void )
{
	List->Items->BeginUpdate();
    List->Items->Clear();
    List->Items->EndUpdate();

    for( int i = 0; i < Channels->Count; i++ )
    	delete reinterpret_cast<TChanInfo *>( Channels->Items[ i ] );

    Channels->Clear();
    VisibleChannels->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::ListDblClick(TObject *Sender)
{
	int idx = List->ELV_GetNextItem( -1, sdAll, TItemStates() << isFocused );

    if(( idx >= 0 ) && ( idx < VisibleChannels->Count ))
    	Server->SendCmd( "JOIN " + reinterpret_cast<TChanInfo *>( VisibleChannels->Items[ idx ] )->Channel );
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::RefreshClick(TObject *Sender)
{
	Server->SendCmd( "LIST" );
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::BeginList( void )
{
	Clear();
    Show();
    BringToFront();

	Enabled    = false;
    SaveCursor = Cursor;
    Cursor     = crHourGlass;
}
//---------------------------------------------------------------------------
void __fastcall	TChanListWin::AddItem( AnsiString channel, int users, AnsiString topic )
{
	Channels->Add( new TChanInfo( channel, users, IRCUtils::StripCodes( topic )));
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::EndList( void )
{
	FilterClick( Filter );

    Cursor  = SaveCursor;
	Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::FilterClick(TObject *Sender)
{
	AnsiString			name = ChanName->Text.LowerCase();
    AnsiString			topic = Topic->Text.LowerCase();

    List->Items->BeginUpdate();
    List->Items->Clear();
    VisibleChannels->Clear();

    for( int i = 0; i < Channels->Count; i++ ) {
     	TChanInfo  *ch = reinterpret_cast<TChanInfo *>( Channels->Items[ i ] );
        bool		add;

       	add = ch->Users >= MinUsers->Value;

        if( add )
        	add = ch->Users <= MaxUsers->Value;

        if( add && !name.IsEmpty() )
        	add = ch->Channel.LowerCase().Pos( name ) > 0;

        if( add && !topic.IsEmpty() )
        	add = ch->Topic.LowerCase().Pos( topic ) > 0;

        if( add )
        	VisibleChannels->Add( ch );
    }

    SortChannels();

    List->SetItemCountEx( VisibleChannels->Count, TLVItemCountFlags() << lvsicfNoScroll );
    List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::ListVMGetItemInfo( TObject *Sender, int Item,
                                                 int SubItem, TLVVMMaskItems &Mask, int &Image, int &Param,
                                                 int &State, int &StateMask, int &Indent, AnsiString &Text )
{
   	TChanInfo  *ch = reinterpret_cast<TChanInfo *>( VisibleChannels->Items[ Item ] );

	if( Mask.Contains( lvifText )) {

    	switch( SubItem ) {

        	case 0:
            	Text = ch->Channel;
            	break;

            case 1:
            	Text = IntToStr( ch->Users );
            	break;

            case 2:
            	Text = ch->Topic;
            	break;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::ListColumnClick( TObject *Sender, TListColumn *Column )
{
	if( SortCol == Column->Index )
    	SortAsc = !SortAsc;

    else {

    	SortAsc = true;
        SortCol = Column->Index;
    }

    List->CurrentSortAscending = SortAsc;

    SortChannels();
}
//---------------------------------------------------------------------------
static int __fastcall CmpChan( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item1 )->Channel.AnsiCompareIC( reinterpret_cast<TChanInfo *>( Item2 )->Channel ));
}

static int __fastcall CmpUsers( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item1 )->Users - reinterpret_cast<TChanInfo *>( Item2 )->Users );
}

static int __fastcall CmpTopic( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item1 )->Topic.AnsiCompareIC( reinterpret_cast<TChanInfo *>( Item2 )->Topic ));
}

static int __fastcall CmpChanDesc( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item2 )->Channel.AnsiCompareIC( reinterpret_cast<TChanInfo *>( Item1 )->Channel ));
}

static int __fastcall CmpUsersDesc( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item2 )->Users - reinterpret_cast<TChanInfo *>( Item1 )->Users );
}

static int __fastcall CmpTopicDesc( void *Item1, void *Item2 )
{
	return( reinterpret_cast<TChanInfo *>( Item2 )->Topic.AnsiCompareIC( reinterpret_cast<TChanInfo *>( Item1 )->Topic ));
}
//---------------------------------------------------------------------------
void __fastcall TChanListWin::SortChannels( void )
{
	static const TListSortCompare	cmps[ 2 ][ 3 ] = {
    												{ CmpChan, CmpUsers, CmpTopic },
                                                    { CmpChanDesc, CmpUsersDesc, CmpTopicDesc }
    										   };

    List->Items->BeginUpdate();
    VisibleChannels->Sort( cmps[ SortAsc ? 0 : 1 ][ SortCol ] );
    List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
