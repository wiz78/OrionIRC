/* $Id: Logger.h,v 1.4 2005/05/10 17:23:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef LoggerH
#define LoggerH
//---------------------------------------------------------------------------

#include "irc.h"

class Logger
{
public:
					Logger();
                   ~Logger();

	void			Open( AnsiString server, AnsiString target );
    void			Open( AnsiString name );
    void			Close( void );

    AnsiString		GetLogFile( void ) { return( FileName ); }

    bool			IsOpen( void ) { return( FileHandle != INVALID_HANDLE_VALUE ); }

	void			Log( const char *nick, AnsiString msg, TIRCEvent type );

private:
	AnsiString		FileName;
    HANDLE			FileHandle;

    void			Write( AnsiString msg );
};

#endif
