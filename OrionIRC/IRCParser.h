/* $Id: IRCParser.h,v 1.3 2003/03/16 15:48:11 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCParserH
#define IRCParserH
//---------------------------------------------------------------------------

#include "Hostmask.h"

class TServerWin;

class IRCParser
{
public:
					IRCParser( TServerWin *wnd );
				   ~IRCParser();

	bool			Parse( char *cmd );

    HostMask&		GetOrigin( void )  { return( Origin ); }
    AnsiString&		GetCommand( void ) { return( Command ); }

    bool			IsNumeric( void );
    int				GetNumeric( void );

    int				GetArgsCount( void ) { return( Args->Count ); }
    AnsiString		GetArg( int index );

private:
	TServerWin	   *ServerWnd;
	HostMask		Origin;
    AnsiString		Command;
    TStringList    *Args;
};

#endif
 