/* $Id: URLCatcher.h,v 1.4 2005/04/27 17:33:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef URLCatcherH
#define URLCatcherH
//---------------------------------------------------------------------------

#include <pcre.h>

#include "WizStrHash.h"

class TURL;

class TURLCatcher
{
public:
					TURLCatcher();
                   ~TURLCatcher();

    void			Clear( void );

    int				Count( void );
    TURL		   *Get( int index );

	void			Add( TURL *url );
    void			Delete( TURL *url );

	void			Filter( AnsiString nick, AnsiString line );

    void			Load( AnsiString file );
    void			Save( AnsiString file );

private:
	TList		   *Items;
    pcre		   *Regex;
    pcre_extra	   *RegexExtra;
    WizStrHash		Hash;

	void			Add( AnsiString nick, AnsiString line );

    AnsiString		GetHashKey( AnsiString url ) const;
};

#endif
