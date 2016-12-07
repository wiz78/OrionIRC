/* $Id: IRCListRow.h,v 1.13 2005/06/04 17:40:54 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCListRowH
#define IRCListRowH
//---------------------------------------------------------------------------

#include "irc.h"

class IRCTextStyle
{
public:
	bool 	HasBG;
    TColor  FG;
    TColor	BG;
    bool	Italic;
    bool	Bold;
    bool	Underline;
    bool	Reverse;
};

class TIRCList;

class IRCListRow
{
public:
 					IRCListRow( TIRCList *irclist, const char *nick, AnsiString txt, TIRCEvent event );
                   ~IRCListRow();

	void			Reset( void );
	int				GetRows( TCanvas *canvas, int width );
    bool			NotFitted( void );

    void			SetSelection( int flags, int start, int end );

	void			PaintTimeStamp( TCanvas *canvas, Windows::TRect rect );
	void			PaintNick( TCanvas *canvas, Windows::TRect rect );
	void			PaintText( TCanvas *canvas, Windows::TRect rect, int linenum );

    int				GetNickWidth( TCanvas *canvas );

    AnsiString		GetTimeStamp( void ) const;
    AnsiString		GetNick( void ) const;
    AnsiString		GetText( void ) const ;
    AnsiString		GetText( int linenum, int start = 0, int end = 1000000 );
    TIRCEvent		GetEvent( void ) const;

	int				GetTimeStampCol( TCanvas *canvas, int X );
	int				GetNickCol( TCanvas *canvas, int X );
	int				GetTextCol( TCanvas *canvas, int linenum, int X, bool exact );

private:
	TIRCList	   *IRCList;
	AnsiString		Nick;
    AnsiString		TimeStamp;
    AnsiString		Text;
    TIRCEvent		Event;
    TStringList	   *Lines;
    int				SelFlags;
    int				SelStart;
    int				SelEnd;

	int 			TextNormal( TCanvas *canvas, IRCTextStyle& style, Windows::TRect& rect, int x, int y, const char *txt );
	int 			TextSelected( TCanvas *canvas, IRCTextStyle& style, Windows::TRect& rect, int x, int y, char *txt, int col );
	int				PrintTextSelected( TCanvas *canvas, Windows::TRect& rect, int x, int y, char *txt );

	void			InitStyle( IRCTextStyle& style, int line );
	int 			GetStyle( AnsiString& line, int n, IRCTextStyle& style );
	void 			ResetStyle( IRCTextStyle& style );
	void			SetStyle( TCanvas *canvas, IRCTextStyle& style );

    int				AddLine( AnsiString line, int start, int *hotspots, int hsnum, int *curspot );

	int				FindHotspots( int *hotspots, int max );
};

#endif
