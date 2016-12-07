/* $Id: BufferSearchWnd.cpp,v 1.1 2005/02/20 18:41:02 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <pcre.h>

#include "BufferSearchWnd.h"
#include "SearchResultsWnd.h"
#include "Main.h"
#include "IRCList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TBufferSearchWin *BufferSearchWin;
//---------------------------------------------------------------------------
__fastcall TBufferSearchWin::TBufferSearchWin( TComponent *Owner, TIRCList *list )
	: TForm(Owner)
{
	List = list;
}
//---------------------------------------------------------------------------
void __fastcall TBufferSearchWin::OkClick(TObject *Sender)
{
	try {
        pcre	   *re = NULL;
        pcre_extra *rex = NULL;
        TList	   *results = new TList();

        if( RegExp->Checked )
        	PrepareRegExp( &re, &rex );

        DoSearch( re, rex, results );

	    pcre_free( re );
    	pcre_free( rex );

        if( results->Count == 0 )
        	ShowMessage( "No results" );
        else {
        	TSearchResultsWin *wnd = new TSearchResultsWin( MainWin, results );

            wnd->Caption = "Search results for \"" + TextToFind->Text + "\"";

            wnd->Show();
        }
    }
    catch( const char *str ) {
    	ShowMessage( str );
        ModalResult = mrNone;
    }
}
//---------------------------------------------------------------------------
void __fastcall TBufferSearchWin::PrepareRegExp( pcre **re, pcre_extra **rex )
{
    const char *error;
    int			erroff;

    *re = pcre_compile( TextToFind->Text.c_str(),
    					PCRE_CASELESS | PCRE_UNGREEDY,
    					&error, &erroff, NULL );

    if( !*re )
        throw "Unparsable regular expression!";

    *rex = pcre_study( *re, 0, &error );
}
//---------------------------------------------------------------------------
void __fastcall TBufferSearchWin::DoSearch( pcre *re, pcre_extra *rex, TList *results )
{
	int i = 0;

    if( RegExp->Checked ) {

        while( IRCListRow *row = List->GetRow( i++ )) {
            AnsiString str = row->GetText();

            if( pcre_exec( re, rex, str.c_str(), str.Length(), 0, 0, NULL, 0 ) >= 0 )
                results->Add( row );
        }

    } else {
        AnsiString needle = TextToFind->Text.LowerCase();

        while( IRCListRow *row = List->GetRow( i++ )) {

            if( row->GetText().LowerCase().Pos( needle ) != 0 )
                results->Add( row );
        }
    }
}
//---------------------------------------------------------------------------
