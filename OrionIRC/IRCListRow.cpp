/* $Id: IRCListRow.cpp,v 1.34 2005/06/04 17:40:54 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "IRCListRow.h"
#include "IRCList.h"
#include "IRCUtils.h"
#include "irc.h"
#include "ColorSet.h"

const unsigned int MircColors[] = {
	0xffffff, 0x000000, 0x800000, 0x008000, 0x0000ff,
    0x000080, 0x800080, 0x0080ff, 0x00ffff, 0x00ff00,
    0x808000, 0xffff00, 0xff0000, 0xff00ff, 0x808080,
    0xc0c0c0
};

#define NUM_MIRC_COLORS	16

#define PRINTABLE(a)	IRCUtils::IsPrintable( a )

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
IRCListRow::IRCListRow( TIRCList *irclist, const char *nick, AnsiString txt, TIRCEvent event )
{
	IRCList   = irclist;
    TimeStamp = TDateTime::CurrentTime().FormatString( "hh:nn:ss" );
    Nick      = IRCUtils::GetNick( nick, event );
    Text      = txt;
    Event     = event;
    Lines     = new TStringList();
	SelFlags  = 0;
}
//---------------------------------------------------------------------------
IRCListRow::~IRCListRow()
{
	delete Lines;
}
//---------------------------------------------------------------------------
void IRCListRow::Reset( void )
{
	Lines->Clear();
}
//---------------------------------------------------------------------------
int IRCListRow::GetRows( TCanvas *canvas, int width )
{
	if( !Lines->Count ) {
	    int				n, len, x, start;
        IRCTextStyle	style;
        AnsiString		rest;
        int				hotspots[ 40 * 2 ], hsnum, curspot = 0, col = 0;

	    len   = Text.Length();
	    n     = 1;
        start = 1;
        x     = 0;

        memset( &style, 0, sizeof( style ));

       	hsnum = FindHotspots( hotspots, sizeof( hotspots ) / ( sizeof( hotspots[0] ) * 2 ));

		while( n <= len ) {
			char	txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;
            int		begin = 0;

        	do {

    	    	if( !PRINTABLE( Text[ n ] ))
                	n = GetStyle( Text, n, style );

		        while(( n <= len ) && ( PRINTABLE( Text[ n ])))
    		    	*to++ = Text[ n++ ];

        		*to = '\0';

                SetStyle( canvas, style );

        		x     += canvas->TextWidth( &txt[ begin ]);
                begin  = strlen( txt );

		    } while(( n <= len ) && ( x <= width ));

            if( x >= width ) { // doesn't fit
            	int l = strlen( txt );

        		x -= canvas->TextWidth( txt );

                while(( x + canvas->TextWidth( txt ) > width ) && ( to > txt )) {
                	char *ptr = strrchr( txt, ' ' );

                    // if possible, we split on spaces, otherwise
                    // just cut a character
                    if( ptr )
                    	*ptr = '\0';
                    else
                    	*--to = '\0';
                }

                n -= l - strlen( txt ) - 1;

                col  += AddLine( Text.SubString( start, n - start ), col,
                				 hotspots, hsnum, &curspot );
                start = n;
            }
    	}

        if( Text.Length() >= start )
	        AddLine( Text.SubString( start, Text.Length() - start + 1 ), col,
            		 hotspots, hsnum, &curspot );
    }

	return( Lines->Count );
}
//---------------------------------------------------------------------------
int IRCListRow::AddLine( AnsiString line, int start, int *hotspots, int hsnum, int *curspot )
{
	int pos = Lines->Add( line );

    if( IRCList && ( hsnum > 0 )) {
        AnsiString  stripped = IRCUtils::StripCodes( Text );
        int 		spot = *curspot;

        // ends before this part?
    	while(( spot < hsnum ) && ( hotspots[ ( spot * 2 ) + 1 ] < start ))
        	++spot;

        *curspot = spot;

        // are there hotspots on this line?
        while(( spot < hsnum ) && ( hotspots[ spot * 2 ] - start < line.Length() )) {
        	int col, len;

            col = hotspots[ spot * 2 ] + 1;
        	len = hotspots[ ( spot * 2 ) + 1 ] - col + 1;

        	IRCList->AddHotspot( this, pos, col - start, stripped.SubString( col, len ));

            *curspot = spot++;
        }
    }

    return( IRCUtils::StripCodes( line ).Length() );
}
//---------------------------------------------------------------------------
int IRCListRow::FindHotspots( int *hotspots, int max )
{
	int hsnum = 0;

	if( IRCList ) {
    	int 		hs[ 4 ], off = 0;
        AnsiString  txt = IRCUtils::StripCodes( Text );

        max *= 2;

    	while(( hsnum < max ) && ( IRCList->FindHotspots( txt, 0, hs, sizeof( hs )) > 0 )) {

        	hotspots[ hsnum++ ] = hs[ 2 ] + off;
        	hotspots[ hsnum++ ] = hs[ 3 ] + off;

            txt.Delete( 1, hs[ 3 ]);

            off += hs[ 3 ];
        }
    }

    return( hsnum / 2 );
}
//---------------------------------------------------------------------------
bool IRCListRow::NotFitted( void )
{
	return( Lines->Count == 0 );
}
//---------------------------------------------------------------------------
void IRCListRow::SetSelection( int flags, int start, int end )
{
	SelFlags = flags;
    SelStart = start;
    SelEnd   = end;
}
//---------------------------------------------------------------------------
void IRCListRow::PaintTimeStamp( TCanvas *canvas, Windows::TRect rect )
{
    canvas->Font->Style = canvas->Font->Style.Clear();

	if( SelFlags & IRCF_MARKING_TS ) {

		canvas->Font->Color  = clWhite;
		canvas->Brush->Color = clBlue;
		canvas->Brush->Style = bsSolid;

    } else {

		canvas->Font->Color  = IRCList ? IRCList->GetColors()->GetColor( IRC_TIMESTAMP_COLUMN ) : clBlack;
		canvas->Brush->Style = bsClear;
    }

	canvas->TextRect( rect, rect.Left, rect.Top, TimeStamp );
}
//---------------------------------------------------------------------------
void IRCListRow::PaintNick( TCanvas *canvas, Windows::TRect rect )
{
    canvas->Font->Style = canvas->Font->Style.Clear();

	if( SelFlags & ( IRCF_MARKING_NICK | IRCF_MARKING_TS )) {

		canvas->Font->Color  = clWhite;
		canvas->Brush->Color = clBlue;
		canvas->Brush->Style = bsSolid;

    } else {

    	if( IRCList )
			canvas->Font->Color = IRCList->GetColors()->GetColor(( Nick[1] == '«' ) ? IRC_EVENT : IRC_NICK_COLUMN );
        else
			canvas->Font->Color = clBlack;

		canvas->Brush->Style = bsClear;
    }

	canvas->TextRect( rect, rect.Right - canvas->TextWidth( Nick ) - 2, rect.Top, Nick );
}
//---------------------------------------------------------------------------
void IRCListRow::PaintText( TCanvas *canvas, Windows::TRect rect, int linenum )
{
	int				n, x, y, len, col;
    IRCTextStyle    style;
    AnsiString		line;

    x    = rect.Left;
    y    = rect.Top;
    line = NotFitted() ? Text : Lines->Strings[ linenum ];
    len  = line.Length();
	n    = 1;
    col  = 1;

    canvas->Font->Style = canvas->Font->Style.Clear();

    InitStyle( style, linenum );

	while( n <= len ) {
		char	txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;
        int		partial;

	    if( !PRINTABLE( line[ n ] ))
    		n = GetStyle( line, n, style );

        while(( n <= len ) && PRINTABLE( line[ n ]))
        	*to++ = line[ n++ ];

        *to = '\0';

        partial = strlen( txt );

        if(( SelFlags & IRCF_MARKING ) && ( SelStart <= col + partial ) && ( SelEnd >= col ))
        	x = TextSelected( canvas, style, rect, x, y, txt, col );
        else
        	x = TextNormal( canvas, style, rect, x, y, txt );

        col += partial;
    }
}
//---------------------------------------------------------------------------
int IRCListRow::GetNickWidth( TCanvas *canvas )
{
    canvas->Font->Style = canvas->Font->Style.Clear();

	return( canvas->TextWidth( Nick ) + 4 );
}
//---------------------------------------------------------------------------
int IRCListRow::TextNormal( TCanvas *canvas, IRCTextStyle& style, Windows::TRect& rect,
							int x, int y, const char *txt )
{
    SetStyle( canvas, style );

	if( style.HasBG ) {
    	tagSIZE	size = canvas->TextExtent( txt );

        canvas->Brush->Color = style.BG;
		canvas->Brush->Style = bsSolid;

		canvas->FillRect( Rect( x, y, x + size.cx, y + size.cy ));
	}

    canvas->Brush->Style = bsClear;

	canvas->TextRect( rect, x, y, txt );

	return( x + canvas->TextWidth( txt ));
}
//---------------------------------------------------------------------------
int IRCListRow::TextSelected( TCanvas *canvas, IRCTextStyle& style, Windows::TRect& rect,
							  int x, int y, char *txt, int col )
{
    char   *ptr = NULL, saved;

    SetStyle( canvas, style );

	if(( col >= SelStart ) && ( col <= SelEnd )) {

        if( SelEnd - col <= strlen( txt )) {
        	ptr   = txt + SelEnd - col;
        	saved = *ptr;
            *ptr  = '\0';
        }

    	x = PrintTextSelected( canvas, rect, x, y, txt );

        if( ptr )
        	*ptr = saved;

        col += strlen( txt );
        txt  = ptr;
    }

    if( txt ) {

    	if(( col < SelStart ) && ( col + strlen( txt ) > SelStart )) {

        	ptr   = txt + SelStart - col;
        	saved = *ptr;
            *ptr  = '\0';

        } else
        	ptr = NULL;

    	x    = TextNormal( canvas, style, rect, x, y, txt );
        col += strlen( txt );
        txt  = NULL;
    }

	if( ptr ) {

    	*ptr = saved;

    	if(( col <= SelEnd ) && ( col + strlen( ptr ) > SelEnd )) {
        	txt   = ptr + SelEnd - col;
        	saved = *txt;
            *txt  = '\0';
        }

    	x    = PrintTextSelected( canvas, rect, x, y, ptr );
        col += strlen( ptr );
    }

    if( txt ) {

    	*txt = saved;
    	x    = TextNormal( canvas, style, rect, x, y, txt );
//        col += strlen( txt );
    }

	return( x );
}
//---------------------------------------------------------------------------
int IRCListRow::PrintTextSelected( TCanvas *canvas, Windows::TRect& rect, int x, int y, char *txt )
{
	int		w = canvas->TextWidth( txt );
    TRect   bg = Rect( x, y, x + w, y + rect.Bottom - rect.Top );

	canvas->Font->Color  = (TColor)GetSysColor( COLOR_HIGHLIGHTTEXT );
    canvas->Brush->Color = (TColor)GetSysColor( COLOR_HIGHLIGHT );
	canvas->Brush->Style = bsSolid;

	canvas->FillRect( bg );

	canvas->Brush->Style = bsClear;

	canvas->TextRect( rect, x, y, txt );

	return( x + w );
}
//---------------------------------------------------------------------------
AnsiString IRCListRow::GetTimeStamp( void ) const
{
	return( TimeStamp );
}
//---------------------------------------------------------------------------
AnsiString IRCListRow::GetNick( void ) const
{
	return( Nick );
}
//---------------------------------------------------------------------------
AnsiString IRCListRow::GetText( void ) const
{
	return( Text );
}
//---------------------------------------------------------------------------
TIRCEvent IRCListRow::GetEvent( void ) const
{
	return( Event );
}
//---------------------------------------------------------------------------
AnsiString IRCListRow::GetText( int linenum, int start, int end )
{
	AnsiString		ret;

    if( linenum < Lines->Count ) {
        int				n, len;
        IRCTextStyle    style;
        AnsiString		line;

        line = Lines->Strings[ linenum ];
        len  = line.Length();
        n    = 1;

        while( n <= len ) {
            char	txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;

            if( !PRINTABLE( line[ n ] ))
                n = GetStyle( line, n, style );

            while(( n <= len ) && ( PRINTABLE( line[ n ])))
                *to++ = line[ n++ ];

            *to = '\0';

            ret += txt;
        }

		ret = ret.SubString( start, end - start );
    }

    return( ret );
}
//---------------------------------------------------------------------------
void IRCListRow::InitStyle( IRCTextStyle& style, int line )
{
	ResetStyle( style );

    for( int i = 0; i < line; i++ ) {
    	AnsiString	line;
        int			len, n;

	    line = Lines->Strings[ i ];
    	len  = line.Length();
		n    = 1;

		while( n <= len ) {

		    if( !PRINTABLE( line[ n ] ))
    			n = GetStyle( line, n, style );

        	while(( n <= len ) && PRINTABLE( line[ n ]))
        		n++;
        }
    }
}
//---------------------------------------------------------------------------
int IRCListRow::GetStyle( AnsiString& line, int n, IRCTextStyle& style )
{
	int len = line.Length();

	switch( line[ n++ ] ) {

    	case TEXT_COLOR:
        	if(( n <= len ) && IRCUtils::IsDigit( line[ n ])) {
            	unsigned int pen;

                pen = line[ n++ ] - '0';

                if(( n <= len ) && IRCUtils::IsDigit( line[ n ]))
                	pen = ( pen * 10 ) + line[ n++ ] - '0';

                if( pen < NUM_MIRC_COLORS )
                	style.FG = (TColor)MircColors[ pen ];

                if(( n + 1 <= len ) && ( line[ n ] == ',' ) && IRCUtils::IsDigit( line[ n + 1 ])) {

                	n++;
                    pen = line[ n++ ] - '0';

	                if(( n <= len ) && IRCUtils::IsDigit( line[ n ]))
    	            	pen = ( pen * 10 ) + line[ n++ ] - '0';

                    if( pen < NUM_MIRC_COLORS ) {
                    	style.HasBG = true;
                        style.BG    = (TColor)MircColors[ pen ];
   	                }
                }

            } else {
            	style.FG    = IRCList ? IRCList->GetColors()->GetColor( Event ) : clBlack;
				style.HasBG = false;
            }
            break;

        case TEXT_TRUE_COLOR:
        	if( n <= len ) {
            	AnsiString color = line.SubString( n, 6 );

	        	sscanf( color.c_str(), "%x", &style.FG );

    	    	n += 6;

            } else {
            	style.FG    = IRCList ? IRCList->GetColors()->GetColor( Event ) : clBlack;
                style.HasBG = false;
            }
        	break;

        case TEXT_BOLD:
        	style.Bold = !style.Bold;
            break;

        case TEXT_BEEP:
        	break;

        case TEXT_ITALIC:
        	style.Italic = !style.Italic;
            break;

        case TEXT_RESET:
        	ResetStyle( style );
            break;

        case TEXT_REVERSE:
        	if( style.Reverse ) {
            	TColor tmp = style.FG;

                style.FG = style.BG;

        		if( style.HasBG )
                	style.BG = tmp;

            } else {
            	TColor tmp = style.BG;

            	style.BG = style.FG;
                style.FG = style.HasBG ? tmp : ( IRCList ? IRCList->GetColors()->GetColor( IRC_BACKGROUND ) : clWhite );
            }

            style.Reverse = !style.Reverse;
        	break;

        case TEXT_UNDERLINE:
        	style.Underline = !style.Underline;
            break;
    }

    return( n );
}
//---------------------------------------------------------------------------
void IRCListRow::ResetStyle( IRCTextStyle& style )
{
	ColorSet   *set = NULL;

    if( IRCList )
    	set = IRCList->GetColors();

    style.Bold		= set ? set->IsBold( Event ) : false;
    style.Italic    = set ? set->IsItalic( Event ) : false;
    style.Underline = false;
    style.Reverse   = false;
    style.HasBG     = false;
    style.FG		= set ? set->GetColor( Event ) : clBlack;
}
//---------------------------------------------------------------------------
void IRCListRow::SetStyle( TCanvas *canvas, IRCTextStyle& style )
{
    canvas->Font->Style = canvas->Font->Style.Clear();

	if( style.Bold )
    	canvas->Font->Style = canvas->Font->Style << fsBold;

    if( style.Italic )
    	canvas->Font->Style = canvas->Font->Style << fsItalic;

    if( style.Underline )
    	canvas->Font->Style = canvas->Font->Style << fsUnderline;

    canvas->Font->Color = style.FG;
}
//---------------------------------------------------------------------------
int IRCListRow::GetTimeStampCol( TCanvas *canvas, int X )
{
	int 		left;
    AnsiString  tmp = TimeStamp;

    left = canvas->TextWidth( tmp );

    while( tmp.Length() && ( X < left )) {

    	tmp.Delete( tmp.Length(), 1 );

	    left = canvas->TextWidth( tmp );
    }

    return( tmp.Length() );
}
//---------------------------------------------------------------------------
int IRCListRow::GetNickCol( TCanvas *canvas, int X )
{
	int 		left; // XXX da rifare
    AnsiString  tmp = Nick;

    left = canvas->TextWidth( tmp );

    while( tmp.Length() && ( X < left )) {

    	tmp.Delete( tmp.Length(), 1 );

	    left = canvas->TextWidth( tmp );
    }

    return( tmp.Length() );
}
//---------------------------------------------------------------------------
int IRCListRow::GetTextCol( TCanvas *canvas, int linenum, int X, bool exact )
{
	int	col = 0;

    if( linenum < Lines->Count ) {
        int				n, x, len;
        IRCTextStyle    style;
        AnsiString		line;

        x    = 0;
        line = Lines->Strings[ linenum ];
        len  = line.Length();
        n    = 1;

        InitStyle( style, linenum );

        while(( X > x ) && ( n <= len )) {
            char		txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;
            AnsiString  tmp;
            int			right;

            if( !PRINTABLE( line[ n ] ))
                n = GetStyle( line, n, style );

            while(( n <= len ) && PRINTABLE( line[ n ]))
                *to++ = line[ n++ ];

            *to = '\0';

            SetStyle( canvas, style );

            to    = txt;
            right = 0;

            // add a char at a time until we reach the X
            while( *to && ( X > right )) {

                tmp   += AnsiString::StringOfChar( *to++, 1 );
                col   += 1;
                right  = x + canvas->TextWidth( tmp );
            }

            if( right > 0 )
	            x = right;
        }

        if(( n > len ) && ( X > x ))
        	col = exact ? - 1 : IRC_SEL_MAX;
    }

    return( col );
}
//---------------------------------------------------------------------------
