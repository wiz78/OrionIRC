/* $Id: URL.h,v 1.3 2004/11/21 00:16:08 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef URLH
#define URLH
//---------------------------------------------------------------------------

#include "MyIniFile.h"

class TURL
{
public:
					TURL();
					TURL( AnsiString user, AnsiString url );

	void			Load( MyIniFile *ini, AnsiString section );
	void			Save( MyIniFile *ini, AnsiString section );

	AnsiString		GetDescription( void ) const { return( Description ); }
	AnsiString		GetUser( void ) const 		 { return( User ); }
    AnsiString		GetURL( void ) const  	     { return( URL  ); }
    TDateTime		GetDate( void ) const 		 { return( Date ); }

	void			SetDescription( AnsiString str ) { Description = str; }
	void			SetUser( AnsiString str ) 		 { User        = str; }
    void			SetURL( AnsiString str )   	     { URL         = str; }

private:
	AnsiString		Description;
	AnsiString		User;
    AnsiString		URL;
    TDateTime		Date;
};

#endif
 