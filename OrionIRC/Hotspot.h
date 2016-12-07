/* $Id: Hotspot.h,v 1.3 2003/04/15 15:27:47 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef HotspotH
#define HotspotH
//---------------------------------------------------------------------------

class IRCListRow;

class Hotspot
{
public:
	typedef enum { CHANNEL, WEB, TELNET, NEWS, EMAIL } HotspotType;

				Hotspot( IRCListRow *row, int lineidx, int column, AnsiString str );

	bool		IsInside( IRCListRow *row, int lineidx, int column );

    void		Launch( void );

    HotspotType	GetType( void ) { return( Type ); }
    AnsiString	GetText( void ) { return( Text ); }
	AnsiString 	GetURL( void );

private:
	IRCListRow *Row;
    int			LineIdx;
    int			StartColumn;
    int			EndColumn;
    AnsiString	Text;
    HotspotType	Type;
};

#endif
