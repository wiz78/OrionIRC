/* $Id: Filter.cpp,v 1.5 2005/06/06 18:34:31 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <mmsystem.h>
#include <pcre.h>

#include "Filter.h"
#include "QueryWnd.h"
#include "ChanWnd.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TFilter::TFilter()
{
    Apply      = FILTER_ALL;
    ColorToSet = clBlack;

	Actions.Set( FILTER_ENABLED );
}
//---------------------------------------------------------------------------
TFilter::TFilter( const TFilter *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TFilter& TFilter::operator=( const TFilter& rhs )
{
	Actions      = rhs.Actions;
    ColorToSet   = rhs.ColorToSet;
    Name         = rhs.Name;
    Filter       = rhs.Filter;
    Apply        = rhs.Apply;
    WinMatching  = rhs.WinMatching;
    WinExcluding = rhs.WinExcluding;
    Sound        = rhs.Sound;

    return( *this );
}
//---------------------------------------------------------------------------
void TFilter::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Actions.Clear();

    	try{ Actions.Set( reg->ReadInteger( "Actions" )); } catch(...) {}
    	try{ ColorToSet = (TColor)reg->ReadInteger( "ColorToSet" ); } catch(...) {}
    	try{ Apply      = (TFilterApply)reg->ReadInteger( "ApplyOn" ); } catch(...) {}

        Name         = reg->ReadString( "Name" );
        Filter       = reg->ReadString( "Filter" );
        WinMatching  = reg->ReadString( "WinMatching" );
        WinExcluding = reg->ReadString( "WinExcluding" );
        Sound 		 = reg->ReadString( "Sound" );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TFilter::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

            reg->WriteInteger( "Actions",    Actions.Get() );
            reg->WriteInteger( "ColorToSet", ColorToSet    );
            reg->WriteInteger( "ApplyOn",    Apply         );

            reg->WriteString( "Name",         Name         );
            reg->WriteString( "Filter",       Filter       );
            reg->WriteString( "WinMatching",  WinMatching  );
            reg->WriteString( "WinExcluding", WinExcluding );
            reg->WriteString( "Sound", 		  Sound        );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
bool TFilter::Matches( AnsiString line, TForm *wnd )
{
	bool	ret;

    switch( Apply ) {

		case FILTER_ALL:
        	ret = true;
        	break;

        case FILTER_QUERY:
        	ret = dynamic_cast<TQueryWin *>( wnd ) != NULL;
        	break;

        case FILTER_CHANNEL:
        	ret = dynamic_cast<TChanWin *>( wnd ) != NULL;
        	break;

        case FILTER_ALL_MATCHING:
        	ret = Match( WinMatching, wnd->Caption );
        	break;

        case FILTER_ALL_EXCLUDING:
        	ret = !Match( WinExcluding, wnd->Caption );
        	break;
    }

    if( ret )
    	ret = line.LowerCase().Pos( Filter.LowerCase() ) != 0;

    return( ret );
}
//---------------------------------------------------------------------------
bool TFilter::Match( AnsiString exp, AnsiString str )
{
	bool		ret = false;
	const char *error;
    int    		erroff;
    pcre   	   *regex;

    regex = pcre_compile( exp.c_str(),
    					  PCRE_CASELESS | PCRE_UNGREEDY,
                          &error, &erroff, NULL );

    if( regex ) {
    	int ovector[ 30 ];

    	ret = pcre_exec( regex, NULL, str.c_str(), str.Length(), 0, 0, ovector, sizeof( ovector )) > 0;

    	pcre_free( regex );
    }

    return( ret );
}
//---------------------------------------------------------------------------
void TFilter::PlaySnd( void )
{
	if( DoPlayBeep() )
    	Sysutils::Beep();
    else
    	PlaySound( Sound.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void TFilter::Describe( TRichEdit *edit )
{
	edit->Clear();

	edit->SelAttributes->Color = clBlack;
    edit->SelText              = "When incoming text contains: ";
	edit->SelAttributes->Color = clBlue;
    edit->SelText              = Filter + "\n";
	edit->SelAttributes->Color = clBlack;
    edit->SelText              = "in: ";
	edit->SelAttributes->Color = clBlue;

    switch( Apply ) {

		case FILTER_ALL:
		    edit->SelText = "All windows";
        	break;

        case FILTER_QUERY:
		    edit->SelText = "All query windows";
        	break;

        case FILTER_CHANNEL:
		    edit->SelText = "All channel windows";
        	break;

        case FILTER_ALL_MATCHING:
		    edit->SelText = "All windows matching " + WinMatching;
        	break;

        case FILTER_ALL_EXCLUDING:
		    edit->SelText = "All windows excluding " + WinExcluding;
        	break;
    }

    edit->SelText = "\n";

	edit->SelAttributes->Color = clBlack;

    edit->Lines->Add( "" );
    edit->Lines->Add( "Execute these actions:" );

	edit->Paragraph->Numbering = nsBullet;
	edit->SelAttributes->Color = clBlue;

    if( DoHighlight() )
	    edit->SelText = "Highlight the line\n";

	edit->SelAttributes->Color = clBlue;

    if( DoHide() )
	    edit->SelText = "Hide the line\n";

	edit->SelAttributes->Color = clBlue;

    if( DoChangeColor() )
	    edit->SelText = "Change the line colour\n";

	edit->SelAttributes->Color = clBlue;

	if( DoPrintOnActiveWin() )
	    edit->SelText = "Print the line on the active window\n";

	edit->SelAttributes->Color = clBlue;

    if( DoPlaySound() ) {

    	if( DoPlayBeep() )
		    edit->SelText = "Emit a beep\n";
        else
		    edit->SelText = "Play " + Sound + "\n";
	}

	edit->SelAttributes->Color = clBlue;

    if( DoFlashStatusBar() )
	    edit->SelText = "Flash the status bar button\n";

	edit->SelAttributes->Color = clBlue;

    if( DoStop() )
	    edit->SelText = "Stop processing filters\n";

	edit->Paragraph->Numbering = nsNone;
}
//---------------------------------------------------------------------------

