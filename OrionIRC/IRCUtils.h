/* $Id: IRCUtils.h,v 1.5 2005/04/15 21:44:21 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCUtilsH
#define IRCUtilsH
//---------------------------------------------------------------------------

#include "irc.h"

class IRCUtils
{
public:
	static bool 			IsPrintable( char ch );
    static bool				IsDigit( char ch );
	static AnsiString		StripCodes( AnsiString str );
	static AnsiString		StripColors( AnsiString str );
	static AnsiString 		GetNick( const char *nick, TIRCEvent Event );

private:
	static int 				SkipStyle( AnsiString& line, int n );
};

#endif
