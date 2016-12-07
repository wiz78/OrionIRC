/* $Id: Hotspot.cpp,v 1.9 2005/04/19 18:19:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <shellapi.h>

#include "Hotspot.h"
#include "Prefs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
Hotspot::Hotspot( IRCListRow *row, int lineidx, int column, AnsiString str )
{
	str = str.Trim();

    switch( str[ 1 ] ) {

    	case '"':
        case '(':
        case '[':
        case '<':
        	str.Delete( 1, 1 );
            break;
    }

    if( !str.SubString( 1, 4 ).AnsiCompareIC( "www." ) ||
        !str.SubString( 1, 4 ).AnsiCompareIC( "ftp." ) ||
        str.Pos( "://" ))
        Type = WEB;
    else if( !str.SubString( 1, 7 ).AnsiCompareIC( "telnet:" ))
    	Type = TELNET;
    else if( !str.SubString( 1, 5 ).AnsiCompareIC( "news:" ))
    	Type = NEWS;
    else if( !str.SubString( 1, 7 ).AnsiCompareIC( "mailto:" ) ||
    		 str.Pos( "@" ))
    	Type = EMAIL;
    else
    	Type = CHANNEL;

    switch( str[ str.Length() ]) {

    	case '.':
        case ',':
        case '?':
    	case '"':
    	case '>':
	    	str.Delete( str.Length(), 1 );
        	break;
    }

	Row         = row;
    LineIdx     = lineidx;
    StartColumn = column;
    EndColumn   = column + str.Length();
    Text        = str;
}
//---------------------------------------------------------------------------
bool Hotspot::IsInside( IRCListRow *row, int lineidx, int column )
{
	return(( row == Row ) && ( lineidx == LineIdx ) &&
    	   ( column >= StartColumn ) && ( column <= EndColumn ));
}
//---------------------------------------------------------------------------
AnsiString Hotspot::GetURL( void )
{
	AnsiString	url = Text;

	switch( Type ) {

    	case WEB:
        	if( !Text.SubString( 1, 3 ).AnsiCompareIC( "www" ))
            	url = "http://" + Text;
        	else if( !Text.SubString( 1, 3 ).AnsiCompareIC( "ftp" ))
            	url = "ftp://" + Text;
            break;

        case EMAIL:
        	if( Text.SubString( 1, 6 ).AnsiCompareIC( "mailto" ))
            	url = "mailto:" + Text;
            break;
	}

	return( url );
}
//---------------------------------------------------------------------------
void Hotspot::Launch( void )
{
	AnsiString	url = GetURL();

    if( url.SubString( 1, 7 ).AnsiCompareIC( "http://" ))
		ShellExecute( Application->Handle, "open", url.c_str(), NULL, NULL, SW_NORMAL );
    else
    	Prefs.LaunchURL( url );
}
//---------------------------------------------------------------------------
