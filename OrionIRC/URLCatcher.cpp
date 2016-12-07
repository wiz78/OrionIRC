/* $Id: URLCatcher.cpp,v 1.12 2006/04/16 19:38:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "URLCatcher.h"
#include "URL.h"
#include "URLCatcherWnd.h"
#include "IRCUtils.h"
#include "Hotspot.h"
#include "Prefs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TURLCatcher::TURLCatcher() : Hash( 1027 )
{
	const char *error;
    int    		erroff;

	Items = new TList();
    Regex = pcre_compile( "(?:.*"
                             "(?=[ <>\\(\\[])"
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
                              "(?:/[^ /]*)*" 								// path
                          ")|(?:"											// emails
                             "[[:alnum:]\\-_\\.]+@[[:alnum:]]+(?:\\.[\\-[:alnum:]]+)"
                          ")"
                          ")(?=[ ><\"\\]\\)]|$)",
                          PCRE_CASELESS | PCRE_UNGREEDY, &error, &erroff, NULL );

    RegexExtra = pcre_study( Regex, 0, &error );

    if( !RegexExtra ) {
    	RegexExtra        = reinterpret_cast<pcre_extra *>( pcre_malloc( sizeof( pcre_extra )));
        RegexExtra->flags = PCRE_EXTRA_MATCH_LIMIT;
    }

    RegexExtra->match_limit = 100000;
}
//---------------------------------------------------------------------------
TURLCatcher::~TURLCatcher()
{
	Clear();

	delete Items;

    pcre_free( Regex );
    pcre_free( RegexExtra );
}
//---------------------------------------------------------------------------
void TURLCatcher::Clear( void )
{
	int i = 0;

	while( TURL *url = Get( i++ ))
    	delete url;

    Items->Clear();
    Hash.Clear();
}
//---------------------------------------------------------------------------
int TURLCatcher::Count( void )
{
	return( Items->Count );
}
//---------------------------------------------------------------------------
TURL *TURLCatcher::Get( int index )
{
	TURL   *ret = NULL;

	if( index < Count() )
    	ret = reinterpret_cast<TURL *>( Items->Items[ index ]);

    return( ret );
}
//---------------------------------------------------------------------------
void TURLCatcher::Delete( TURL *url )
{
	Items->Remove( url );
    Hash.Remove( url->GetURL().c_str() );

	delete url;
}
//---------------------------------------------------------------------------
void TURLCatcher::Filter( AnsiString nick, AnsiString line )
{
	if( Prefs.CatchURLs() ) {
        int 		hs[ 3 ];
        AnsiString  txt = IRCUtils::StripCodes( line );

        while( pcre_exec( Regex, RegexExtra, txt.c_str(), txt.Length(), 0, 0, hs, sizeof( hs )) > 0 ) {

            Add( nick, txt.SubString( hs[ 2 ], hs[ 3 ] - hs[ 2 ] + 1 ));

            txt.Delete( 1, hs[ 3 ]);
        }
    }
}
//---------------------------------------------------------------------------
void TURLCatcher::Add( AnsiString nick, AnsiString line )
{
	Hotspot	 	hot( NULL, 0, 0, line );
    AnsiString  url = hot.GetURL();

    if( !Hash.Contains( GetHashKey( url ).c_str() ))
	    Add( new TURL( nick, url ));
}
//---------------------------------------------------------------------------
void TURLCatcher::Add( TURL *url )
{
    if( !Hash.Contains( url->GetURL().c_str() )) {

	    Items->Add( url );
    	Hash.Add( GetHashKey( url->GetURL() ).c_str(), NULL );

	    if( URLCatcherWin )
    		URLCatcherWin->AddItem( url );
    }
}
//---------------------------------------------------------------------------
AnsiString TURLCatcher::GetHashKey( AnsiString url ) const
{
    if( !url.IsEmpty() && ( url[ url.Length() ] == '/' ))
    	url.Delete( url.Length(), 1 );

    return( url );
}
//---------------------------------------------------------------------------
void TURLCatcher::Load( AnsiString file )
{
	MyIniFile  *ini = new MyIniFile( file );
    int			count;

    count = ini->ReadInteger( "Count", "Count", 0 );

    for( int i = 0; i < count; i++ ) {
    	TURL   *url = new TURL();

        url->Load( ini, "URL" + IntToStr( i ));

        Add( url );
    }

    delete ini;
}
//---------------------------------------------------------------------------
void TURLCatcher::Save( AnsiString file )
{
	MyIniFile   *ini = new MyIniFile( file, false );

    ini->WriteInteger( "Count", "Count", Count() );

    for( int i = 0; i < Count(); i++ )
        Get( i )->Save( ini, "URL" + IntToStr( i ));

    delete ini;
}
//---------------------------------------------------------------------------
