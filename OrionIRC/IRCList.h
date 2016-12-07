/* $Id: IRCList.h,v 1.18 2005/04/18 19:17:04 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCListH
#define IRCListH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
#include <pcre.h>

#include "bitfield.h"
#include "IRCListRow.h"

class TServerWin;
class ColorSet;

class PACKAGE TIRCList : public TCustomControl
{
private:
	TServerWin			   *Server;
    ColorSet			   *Colors;
	Graphics::TBitmap	   *BackBuffer;
    TCustomEdit			   *InputCtrl;
    int                     VisibleRows;
    int						BottomRow;
    int						OldWidth;
    TMouseEvent             FOnCellMouseUp;
    TMouseEvent             FOnCellMouseDown;
    TScrollBar             *VertBar;
    TList                  *Rows;
    TList				   *RowsIndex;
    BitField				Flags;
    int						MaxRows;
    int						TimeStampWidth;
    int						NickWidth;
    int						LineHeight;
    struct TSelection {
    	int	StartRow;
    	int	StartColumn;
    	int EndRow;
    	int EndColumn;
    }						Selection;
    pcre				   *HotspotRegex;
    pcre_extra			   *HotspotRegexExtra;
    TList				   *Hotspots;
    short					MouseDownX;
    short					MouseDownY;
    int						WheelDelta;

	void __fastcall 		ClearRowsIndex( void );
    void __fastcall         ComputeVisibleItems( TCanvas *canvas );
	int __fastcall 			ComputeNickWidth( TCanvas *canvas, int idx );
	void __fastcall 		SetSelection( IRCListRow *row, int idx );
	void 					GetSelectionBoundaries( int& ssr, int& ser, int& ssc, int& sec );
    void __fastcall         VertScroll( TObject *Sender, TScrollCode ScrollCode, int &ScrollPos );

	void __fastcall 		MouseWheel( TMessage& msg );
    void __fastcall 		MouseDown( TMouseButton Button, Classes::TShiftState Shift, int X, int Y );
    void __fastcall			MouseUp( TMouseButton Button, Classes::TShiftState Shift, int X, int Y );
    void __fastcall			MouseMove( Classes::TShiftState Shift, int X, int Y );
	void __fastcall 		DblClick( void );

	int __fastcall 			GetRowAt( int Y, bool exact = false );
	int __fastcall 			GetColumnAt( int rownum, int X, bool exact = false );

	void __fastcall 		CopyToClipboard( void );

    void __fastcall			CheckHotspots( int row, int col );

	void __fastcall 		SaveBuffer( AnsiString file );

    // avoid flickering
    void __fastcall         WMEraseBkgnd(TMessage &Message) { Message.Result = 0; };

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_ERASEBKGND, TMessage, WMEraseBkgnd )
    MESSAGE_HANDLER( WM_MOUSEWHEEL, TMessage, MouseWheel )
END_MESSAGE_MAP( TCustomControl )

protected:
    virtual void __fastcall Paint( void );
    virtual Windows::TRect __fastcall GetClientRect( void );

public:
    __fastcall              TIRCList( TComponent* Owner, TServerWin *server, ColorSet *colors );
    __fastcall             ~TIRCList();

	void __fastcall 		AddRow( const char *nick, AnsiString text, TIRCEvent event );
    IRCListRow * __fastcall	GetRow( int index );
    void __fastcall         Clear( void );

    void __fastcall         ShowTimestamp( bool show );
    void __fastcall         ShowNick( bool show );
    void __fastcall         SetAdaptToNick( bool set );

    void __fastcall         SetInputCtrl( TCustomEdit *ctrl );

    int __fastcall			FindHotspots( AnsiString str, int start, int *ovector, int ovecsize );
    void __fastcall			AddHotspot( IRCListRow *row, int lineidx, int startcol, AnsiString txt );

    ColorSet * __fastcall	GetColors( void ) { return( Colors ); }

	void __fastcall 		SaveBuffer( void );

    void __fastcall			Refresh( void );

__published:
    __property              Align;
    __property              ShowHint;
    __property              ParentShowHint;
    __property              Visible;
    __property              Font;
    __property              PopupMenu;
    __property              OnClick;
    __property              OnDblClick;
    __property              OnMouseDown;
    __property              OnMouseMove;
    __property              OnMouseUp;
};

// Flags
#define IRCF_SHOW_NICK			(1 << 0)
#define IRCF_SHOW_TIMESTAMP		(1 << 1)
#define IRCF_MARKING			(1 << 2)
#define IRCF_MARKING_TS			(1 << 3)
#define IRCF_MARKING_NICK		(1 << 4)
#define IRCF_DBL_CLICK			(1 << 5)
#define IRCF_ADAPT_TO_NICK		(1 << 6)

#define IRCF_MARK_MASK			( IRCF_MARKING | IRCF_MARKING_TS | IRCF_MARKING_NICK )

// it means it's selected till the end of the line
#define IRC_SEL_MAX				1000000

extern unsigned int IRCListColors[ IRC_NUM_EVENTS ];

//---------------------------------------------------------------------------
#endif
 