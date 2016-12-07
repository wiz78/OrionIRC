/* $Id: SearchResultsWnd.cpp,v 1.2 2005/04/08 17:31:58 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SearchResultsWnd.h"
#include "IRCList.h"
#include "Prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TSearchResultsWin *SearchResultsWin;
//---------------------------------------------------------------------------
__fastcall TSearchResultsWin::TSearchResultsWin( TComponent *Owner, TList *rows )
	: TForm(Owner)
{
	Display = new TIRCList( this, NULL, Prefs.GetColorSet() );

    Display->Parent    = this;
    Display->Top       = 0;
    Display->Left      = 0;
    Display->Align     = alClient;
    Display->PopupMenu = PopupMenu;

    Display->ShowTimestamp( true );
    Display->ShowNick( true );
    Display->Font->Assign( Prefs.GetChannelFont() );

    Fill( rows );
}
//---------------------------------------------------------------------------
void __fastcall TSearchResultsWin::FormClose( TObject *Sender, TCloseAction &Action )
{
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TSearchResultsWin::Savebuffer1Click(TObject *Sender)
{
	Display->SaveBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TSearchResultsWin::Fill( TList *rows )
{
	for( int i = 0; i < rows->Count; i++ ) {
    	IRCListRow *row = reinterpret_cast<IRCListRow *>( rows->Items[ i ] );

        Display->AddRow( row->GetNick().c_str(), row->GetText(), row->GetEvent() );
    }
}
//---------------------------------------------------------------------------

