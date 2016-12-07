/* $Id: IRCList.cpp,v 1.42 2006/11/25 23:52:40 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <Clipbrd.hpp>

#include "IRCList.h"
#include "IRCEdit.h"
#include "Hotspot.h"
#include "ServerWnd.h"
#include "ColorSet.h"
#include "Prefs.h"

#define LINE_SPC	1
#define COL_SPC		2

class RowPtr
{
public:
				RowPtr( IRCListRow *row, short idx )
                {
                	Row   = row;
                    Index = idx;
                }

	IRCListRow *Row;
    short		Index;
};

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TIRCList::TIRCList( TComponent* Owner, TServerWin *server, ColorSet *colors )
    : TCustomControl(Owner)
{
	const char *error;
    int    		erroff;

    Server         = server;
    Colors         = colors;
	InputCtrl      = NULL;
    Rows		   = new TList();
    RowsIndex	   = new TList();
    VertBar        = new TScrollBar( this );
    BackBuffer     = new Graphics::TBitmap();
    Hotspots	   = new TList();
    OldWidth       = 0;
    MaxRows        = 1000;
    WheelDelta     = 0;

    Font->Size = 10;

    VertBar->Parent      = this;
    VertBar->Align       = alRight;
    VertBar->Kind        = sbVertical;
    VertBar->OnScroll    = VertScroll;
    VertBar->Min         = 0;
    VertBar->Max         = 0;
    VertBar->Position    = 0;

    ControlStyle = ControlStyle << csOpaque;

    HotspotRegex = pcre_compile( "(?:.*"
    								"(?:[ <>\\[\\(])"
    							 ")?("
    							 "(?:"												// URLs
                                 	"(?:"
	                                 	"(?:"
		    								 "(?:[[:alnum:]]+://|telnet:|mailto:)"	// scheme
			    							 "(?:[^@]+@)?"	  						// user, pass
    	                                ")|(?:"
                                        	"www\\.|ftp\\."
                                        ")"
	                                ")"
    								 "\\|?[[:alnum:]]+(?:[\\|\\.\\-[:alnum:]]+)*"	// hostname
                                     "(?::[[:digit:]]+)?"							// port
            	                     "(?:/[^ /\\x02\\x03\\x1f\\t]*)*" 				// path
                                 ")|(?:"											// channels
                                 	"#[[:alnum:]]+"
                                 ")|(?:"											// emails
                                 	"[[:alnum:]\\-_\\.]+@[[:alnum:]]+(?:\\.[\\-[:alnum:]]+)"
                                 ")"
                                 ")(?=[ <>\"\\]\\)]|$)",
    							 PCRE_CASELESS | PCRE_UNGREEDY, &error, &erroff, NULL );

    HotspotRegexExtra = pcre_study( HotspotRegex, 0, &error );

    if( !HotspotRegexExtra ) {
    	HotspotRegexExtra        = reinterpret_cast<pcre_extra *>( pcre_malloc( sizeof( pcre_extra )));
        HotspotRegexExtra->flags = PCRE_EXTRA_MATCH_LIMIT;
    }

    HotspotRegexExtra->match_limit = 100000;

    Flags.Set( IRCF_SHOW_TIMESTAMP | IRCF_SHOW_NICK | IRCF_ADAPT_TO_NICK );
}
//---------------------------------------------------------------------------
__fastcall TIRCList::~TIRCList()
{
	Clear();

    pcre_free( HotspotRegex );
    pcre_free( HotspotRegexExtra );

    delete VertBar;
    delete Rows;
    delete RowsIndex;
    delete BackBuffer;
    delete Hotspots;
}
//---------------------------------------------------------------------------
/*void __fastcall TMainWin::InitMouseWheel( void )
{
	HWND wheelwnd = FindWindow( "MouseZ", "Magellan MSWHEEL" );

    if( wheelwnd ) {
		UINT wheelsupport, scrolllines;

	    MSWheelMsg   = RegisterWindowMessage( "MSWHEEL_ROLLMSG" );
        wheelsupport = RegisterWindowMessage( "MSH_WHEELSUPPORT_MSG" );
        scrolllines  = RegisterWindowMessage( "MSH_SCROLL_LINES_MSG" );

        if( wheelsupport )
			SendMessage( wheelwnd, wheelsupport, 0, 0 );

        if( scrolllines )
        	scrolllines = SendMessage( wheelwnd, scrolllines, 0, 0 );

	} else
    	MSWheelMsg = 0;
} */
//---------------------------------------------------------------------------
void __fastcall TIRCList::AddRow( const char *nick, AnsiString text, TIRCEvent event )
{
	IRCListRow *row = new IRCListRow( this, nick, text, event );
    bool		bottom = VertBar->Position == VertBar->Max;

    Rows->Add( row );
    RowsIndex->Add( new RowPtr( row, 0 ));

    VertBar->Max = VertBar->Max + 1;

    if( Rows->Count > MaxRows ) {
        bool    go;

		row = GetRow( 0 );

		do {
        	RowPtr *ptr = (RowPtr *)RowsIndex->Items[ 0 ];

            go = ptr->Row == row;

            if( go ) {

			    VertBar->Max = VertBar->Max - 1;

            	RowsIndex->Delete( 0 );

            	delete ptr;
            }

        } while( go );

        Rows->Delete( 0 );

        delete row;
    }

    if( bottom )
	    VertBar->Position = VertBar->Max;

    Paint();
}
//---------------------------------------------------------------------------
IRCListRow * __fastcall TIRCList::GetRow( int index )
{
	IRCListRow *ret = NULL;

	if( index < Rows->Count )
    	ret = reinterpret_cast<IRCListRow *>( Rows->Items[ index ] );

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::Clear( void )
{
	int i = 0;

    while( IRCListRow *row = GetRow( i++ ))
    	delete row;

    Rows->Clear();

    ClearRowsIndex();

    VertBar->Min      = 0;
    VertBar->Max      = 0;
    VertBar->Position = 0;

    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::ClearRowsIndex( void )
{
    for( int i = 0; i < RowsIndex->Count; i++ )
    	delete reinterpret_cast<RowPtr *>( RowsIndex->Items[ i ] );

	for( int i = 0; i < Hotspots->Count; i++ )
    	delete reinterpret_cast<Hotspot *>( Hotspots->Items[ i ] );

    RowsIndex->Clear();
    Hotspots->Clear();
}
//---------------------------------------------------------------------------
Windows::TRect __fastcall TIRCList::GetClientRect( void )
{
    return( Rect( 0, 0, Width - 1, Height - 1 ));
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::ComputeVisibleItems( TCanvas *canvas )
{
    VisibleRows = Height / canvas->TextHeight( "TypjQ" );

    if( Rows->Count < VisibleRows )
        VisibleRows = Rows->Count;

    VertBar->LargeChange = VisibleRows - 1;
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::Paint( void )
{
    Windows::TRect  rect = Rect( 0, 0, 0, Height );
    int				idx, textwidth;
    RowPtr 	   	   *bottomrow = NULL;
    TCanvas	   	   *canvas = BackBuffer->Canvas;

    if( Width - VertBar->Width - 1 <= 0 )
    	return;

    BackBuffer->Width  = Width - VertBar->Width - 2;
    BackBuffer->Height = Height;

    canvas->Font = Font;
    LineHeight   = canvas->TextHeight( "TypjQ" ) + LINE_SPC;

    ComputeVisibleItems( canvas );

    canvas->Brush->Style = bsSolid;

    idx = VertBar->Position - 1;

    if( idx >= RowsIndex->Count )
    	idx = RowsIndex->Count - 1;

    if( idx >= 0 )
    	bottomrow = (RowPtr *)RowsIndex->Items[ idx ];

    if( Flags.IsSet( IRCF_SHOW_TIMESTAMP )) {

    	canvas->Brush->Color = Colors->GetColor( IRC_TIMESTAMP_BACKGROUND );
	    TimeStampWidth       = canvas->TextWidth( "22.59.59" ) + ( COL_SPC * 2 );
	    rect.Right           = TimeStampWidth;

	    canvas->FillRect( rect );

    } else
    	TimeStampWidth = 0;

    if( Flags.IsSet( IRCF_SHOW_NICK )) {

    	canvas->Brush->Color = Colors->GetColor( IRC_NICK_BACKGROUND );
        NickWidth			 = ComputeNickWidth( canvas, idx );
	    rect.Left            = rect.Right;
    	rect.Right          += NickWidth;

	    canvas->FillRect( rect );

    } else
    	NickWidth = 0;

    canvas->Brush->Color = Colors->GetColor( IRC_BACKGROUND );
    rect.Left            = rect.Right;
    rect.Right           = BackBuffer->Width;
    textwidth			 = rect.Right - rect.Left - ( COL_SPC * 2 );

    canvas->FillRect( rect );

    rect.Top    = Height - 2;
    rect.Bottom = Height;

    if( textwidth != OldWidth ) {
        int   		i = 0;
        IRCListRow *row = bottomrow ? bottomrow->Row : NULL;

        ClearRowsIndex();

	    while( IRCListRow *row = GetRow( i++ )) {
    		row->Reset();
            RowsIndex->Add( new RowPtr( row, 0 ));
        }

        OldWidth     = textwidth;
        VertBar->Max = Rows->Count;
        idx          = Rows->IndexOf( row );

	    if( idx >= 0 )
    		bottomrow = (RowPtr *)RowsIndex->Items[ idx ];
	}

    // output the text from the bottom row
    while(( idx >= 0 ) && ( rect.Bottom > 0 )) {
    	RowPtr     *rowptr = (RowPtr *)RowsIndex->Items[ idx ];
    	IRCListRow *row = rowptr->Row;

        // has the text already been fitted in the available width?
        if( row->NotFitted() ) {
        	int rows = row->GetRows( canvas, textwidth );

			for( int i = 1; i < rows; i++ )
            	RowsIndex->Insert( ++idx, new RowPtr( row, i ));

            if( rowptr == bottomrow )
            	bottomrow = (RowPtr *)RowsIndex->Items[ idx ];

        	VertBar->Max      = RowsIndex->Count;
            VertBar->Position = RowsIndex->IndexOf( bottomrow ) + 1;

    		rowptr = (RowPtr *)RowsIndex->Items[ idx ];
        }

        SetSelection( row, idx );

	    rect.Left  = COL_SPC;
        rect.Right = 0;
        rect.Top  -= LineHeight;

	    if( Flags.IsSet( IRCF_SHOW_TIMESTAMP )) {

        	rect.Right = TimeStampWidth;

            if( !rowptr->Index )
    	    	row->PaintTimeStamp( canvas, rect );
        }

	    if( Flags.IsSet( IRCF_SHOW_NICK )) {

    		canvas->Brush->Color = clTeal;
	    	rect.Left            = rect.Right + COL_SPC;
	    	rect.Right          += NickWidth;

            if( !rowptr->Index )
		    	row->PaintNick( canvas, rect );
	    }

        rect.Left  = rect.Right + COL_SPC;
	    rect.Right = Width - VertBar->Width;

        row->PaintText( canvas, rect, rowptr->Index );

        rect.Bottom -= LineHeight;
        idx--;
    }

    canvas->Pen->Color = clBtnFace;
    canvas->Pen->Style = psSolid;

    canvas->MoveTo( 0, Height - 1 );
    canvas->LineTo( Width - 1, Height - 1 );

    Canvas->Draw( 0, 0, BackBuffer );

    BottomRow = VertBar->Position - 1;

    if( BottomRow >= RowsIndex->Count )
    	BottomRow = RowsIndex->Count - 1;
}
//---------------------------------------------------------------------------
int __fastcall TIRCList::ComputeNickWidth( TCanvas *canvas, int idx )
{
	int	width = 0;

    if(( idx >= 0 ) && Flags.IsSet( IRCF_ADAPT_TO_NICK )) {
    	int h = Height;

        idx = Rows->IndexOf( reinterpret_cast<RowPtr *>( RowsIndex->Items[ idx ])->Row );

        while(( idx >= 0 ) && ( h >= 0 )) {
            IRCListRow *row = reinterpret_cast<IRCListRow *>( Rows->Items[ idx ]);
            int			w;

            w = row->GetNickWidth( canvas );

            if( w > width )
            	width = w;

            idx--;

            h -= LineHeight;
        }

    } else
    	width = canvas->TextWidth( "WWWWWWWWW" ) + ( COL_SPC * 2 );

    return( width );
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::SetSelection( IRCListRow *row, int idx )
{
    int ssr, ser, ssc, sec;

    GetSelectionBoundaries( ssr, ser, ssc, sec );

	if( Flags.IsSet( IRCF_MARKING ) && ( idx >= ssr ) && ( idx <= ser )) {

        if(( idx == ssr ) && ( idx != ser ))
        	sec = IRC_SEL_MAX;

        if(( idx == ser ) && ( idx != ssr ))
        	ssc = 0;

        if(( idx > ssr ) && ( idx < ser )) {
        	ssc = 0;
        	sec = IRC_SEL_MAX;
        }

        row->SetSelection( Flags.Get() & IRCF_MARK_MASK, ssc, sec );

    } else
    	row->SetSelection( 0, 0, 0 );
}
//---------------------------------------------------------------------------
void TIRCList::GetSelectionBoundaries( int& ssr, int& ser, int& ssc, int& sec )
{
    if( Selection.StartRow == Selection.EndRow ) {

    	ssr = ser = Selection.StartRow;
        ssc = ( Selection.StartColumn < Selection.EndColumn ) ? Selection.StartColumn : Selection.EndColumn;
        sec = ( Selection.StartColumn > Selection.EndColumn ) ? Selection.StartColumn : Selection.EndColumn;

    } else if( Selection.StartRow < Selection.EndRow ) {

    	ssr = Selection.StartRow;
        ser = Selection.EndRow;
        ssc = Selection.StartColumn;
        sec = Selection.EndColumn;

    } else {

    	ssr = Selection.EndRow;
        ser = Selection.StartRow;
        ssc = Selection.EndColumn;
        sec = Selection.StartColumn;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::VertScroll( TObject *Sender, TScrollCode ScrollCode, int &ScrollPos )
{
    Paint();

    if( InputCtrl )
    	InputCtrl->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::ShowTimestamp( bool show )
{
	Flags.Set( IRCF_SHOW_TIMESTAMP, show );

    Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::ShowNick( bool show )
{
	Flags.Set( IRCF_SHOW_NICK, show );

    Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::SetAdaptToNick( bool set )
{
	Flags.Set( IRCF_ADAPT_TO_NICK, set );

    Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::MouseWheel( TMessage& msg )
{
    WheelDelta -= (short)HIWORD( msg.WParam );

    if( abs( WheelDelta ) >= WHEEL_DELTA ) {
		UINT	lines;
	    int		toscroll;

		SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &lines, 0 );

    	toscroll = WheelDelta / WHEEL_DELTA;

        if( msg.WParam & MK_CONTROL ) // page scroll
        	toscroll *= VertBar->LargeChange;
        else
        	toscroll *= lines;

    	VertBar->Position = VertBar->Position + toscroll;

        WheelDelta %= WHEEL_DELTA;

    	VertScroll( VertBar, (TScrollCode)0, VertBar->Position );
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::MouseDown( TMouseButton Button, Classes::TShiftState Shift, int X, int Y )
{
	if( Flags.IsSet( IRCF_DBL_CLICK ))
		Flags.Clear( IRCF_DBL_CLICK );

	else if(( Button == mbLeft ) && Rows->Count &&
    		( X >= 0 ) && ( X <= Width ) && ( Y >= 0 ) && ( Y <= Height )) {

    	MouseDownX = X;
    	MouseDownY = Y;

    	Flags.Set( IRCF_MARKING );

	    if( Flags.IsSet( IRCF_SHOW_TIMESTAMP ) && ( X < TimeStampWidth ))
    		Flags.Set( IRCF_MARKING_TS );
	    else if( Flags.IsSet( IRCF_SHOW_NICK ) && ( X < TimeStampWidth + NickWidth ))
			Flags.Set( IRCF_MARKING_NICK );

        Selection.StartRow    = GetRowAt( Y );
        Selection.StartColumn = GetColumnAt( Selection.StartRow, X );
        Selection.EndRow      = Selection.StartRow;
        Selection.EndColumn   = Selection.StartColumn;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::MouseUp( TMouseButton Button, Classes::TShiftState Shift, int X, int Y )
{
	if(( Button == mbLeft ) && Flags.IsSet( IRCF_MARKING )) {

    	CopyToClipboard();

	    Flags.Clear( IRCF_MARKING | IRCF_MARKING_TS | IRCF_MARKING_NICK );

        Paint();
    }

    if( InputCtrl )
    	InputCtrl->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::MouseMove( Classes::TShiftState Shift, int X, int Y )
{
	if( Flags.IsSet( IRCF_MARKING )) {

    	if( X < 0 )
        	X = 0;

        if( Y < 0 ) {

        	if( VertBar->Position > VertBar->Min ) {
            	VertBar->Position = VertBar->Position - 1;
                Paint();
            }

        	Y = 0;

        } else if( Y > Height ) {

        	if( VertBar->Position < VertBar->Max ) {
            	VertBar->Position = VertBar->Position + 1;
                Paint();
            }

        	Y = Height - 1;
        }

        Selection.EndRow    = GetRowAt( Y );
        Selection.EndColumn = GetColumnAt( Selection.EndRow, X );

    	Paint();

    } else if(( X >= TimeStampWidth + NickWidth ) && ( X <= Width ) &&
              ( Y >= 0 ) && ( Y <= Height )) {
    	int	line = GetRowAt( Y, true );

    	CheckHotspots( line, GetColumnAt( line, X, true ));
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::DblClick( void )
{
    int		rownum = GetRowAt( MouseDownY, true );
    bool    inhotspot = false;

    Flags.Clear( IRCF_MARK_MASK );
	Flags.Set( IRCF_DBL_CLICK );

    if(( rownum >= 0 ) && ( rownum < RowsIndex->Count )) {
		RowPtr     *rowptr = (RowPtr *)RowsIndex->Items[ rownum ];
        IRCListRow *row = rowptr->Row;
        int			col = GetColumnAt( rownum, MouseDownX, true );

        for( int i = 0; !inhotspot && ( i < Hotspots->Count ); i++ ) {
            Hotspot *hot = reinterpret_cast<Hotspot *>( Hotspots->Items[ i ] );

            if( hot->IsInside( row, rowptr->Index, col )) {

            	switch( hot->GetType() ) {

                	case Hotspot::CHANNEL:
                    	if( Server )
	                    	Server->SendCmd( "JOIN " + hot->GetText() );
                    	break;

                    default:
                    	hot->Launch();
                        break;
                }

                inhotspot = true;
            }
        }
    }

    if( !inhotspot )
    	TCustomControl::DblClick();
}
//---------------------------------------------------------------------------
int __fastcall TIRCList::GetRowAt( int Y, bool exact )
{
	int	top = Height - LineHeight, line = BottomRow;

    while(( line >= 0 ) && (  Y < top )) {
    	top  -= LineHeight;
        line -= 1;
    }

    if( !exact && ( line < 0 ))
    	line = 0;

    return( line );
}
//---------------------------------------------------------------------------
int __fastcall TIRCList::GetColumnAt( int rownum, int X, bool exact )
{
	int col = 0;

	if(( rownum >= 0 ) && ( rownum < RowsIndex->Count ) &&
       ( !Flags.IsSet( IRCF_MARKING ) || ( X >= NickWidth ))) {
   		RowPtr     *rowptr = (RowPtr *)RowsIndex->Items[ rownum ];
	   	IRCListRow *row = rowptr->Row;

	    Canvas->Font = Font;

    	col = row->GetTextCol( Canvas, rowptr->Index, X - NickWidth - TimeStampWidth, exact );
    }

    return( col );
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::CopyToClipboard( void )
{
    int 		ssr, ser, ssc, sec;
    AnsiString	txt;
    IRCListRow *prev = NULL;

    GetSelectionBoundaries( ssr, ser, ssc, sec );

	for( int idx = ssr; ( idx <= ser ) && ( idx < RowsIndex->Count ); idx++ ) {
    	int 		start = ssc, end = sec;
   		RowPtr     *rowptr = (RowPtr *)RowsIndex->Items[ idx ];
	   	IRCListRow *row = rowptr->Row;

        if(( idx == ssr ) && ( idx != ser ))
        	end = IRC_SEL_MAX;

        if(( idx == ser ) && ( idx != ssr ))
        	start = 0;

        if(( idx > ssr ) && ( idx < ser )) {
        	start = 0;
        	end   = IRC_SEL_MAX;
        }

        if( prev != row ) {

        	if( prev )
	        	txt += "\r\n";

			if( Flags.IsSet( IRCF_MARKING_TS ))
        		txt += row->GetTimeStamp() + " ";

			if( Flags.IsSet( IRCF_MARKING_TS | IRCF_MARKING_NICK ) &&
            	Flags.IsSet( IRCF_SHOW_NICK ))
        		txt += row->GetNick() + " ";
		}

        txt  += row->GetText( rowptr->Index, start, end );
        prev  = row;
    }

    if( !txt.IsEmpty() )
	    Clipboard()->SetTextBuf( txt.c_str() );
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::SetInputCtrl( TCustomEdit *ctrl )
{
	InputCtrl = ctrl;
}
//---------------------------------------------------------------------------
int __fastcall TIRCList::FindHotspots( AnsiString str, int start, int *ovector, int ovecsize )
{
	return( pcre_exec( HotspotRegex, HotspotRegexExtra, str.c_str(), str.Length(), start, 0, ovector, ovecsize ));
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::AddHotspot( IRCListRow *row, int lineidx, int startcol, AnsiString txt )
{
	Hotspots->Add( new Hotspot( row, lineidx, startcol, txt ));
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::CheckHotspots( int rownum, int col )
{
	bool in = false;

    if(( rownum >= 0 ) && ( col >= 0 ) && ( rownum < RowsIndex->Count )) {
		RowPtr     *rowptr = (RowPtr *)RowsIndex->Items[ rownum ];
        IRCListRow *row = rowptr->Row;

        for( int i = 0; i < Hotspots->Count; i++ ) {
            Hotspot *hot = reinterpret_cast<Hotspot *>( Hotspots->Items[ i ] );

            if( hot->IsInside( row, rowptr->Index, col )) {

                Cursor = crHandPoint;
                in     = true;
                Hint   = "Double click to open " + hot->GetURL();

                break;
            }
        }
    }

    if( !in ) {
    	Cursor = crDefault;
        Hint   = "";
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::SaveBuffer( AnsiString file )
{
	Logger log;

    log.Open( file );

    for( int i = 0; i < Rows->Count; i++ ) {
    	IRCListRow *row = reinterpret_cast<IRCListRow *>( Rows->Items[ i ]);

        log.Log( row->GetNick().c_str(), row->GetText(), row->GetEvent() );
    }

    log.Close();
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::SaveBuffer( void )
{
	TSaveDialog	*savedlg = new TSaveDialog( NULL );

    savedlg->InitialDir = Prefs.GetLogsDir();
    savedlg->Filter     = "Text files (*.txt)|*.txt";
    savedlg->Title      = "Save buffer as...";

    if( savedlg->Execute() )
    	SaveBuffer( savedlg->FileName );

    delete savedlg;
}
//---------------------------------------------------------------------------
void __fastcall TIRCList::Refresh( void )
{
	OldWidth = -1; // re-layout

    Invalidate();
}
//---------------------------------------------------------------------------

