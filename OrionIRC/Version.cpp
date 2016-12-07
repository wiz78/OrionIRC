/* $Id: Version.cpp,v 1.2 2003/03/16 15:48:11 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <windows.h>

#include "Version.h"

#define DEFAULT_LANG_ID			0x0409
#define DEFAULT_CHAR_SET_ID   	0x04E4
#define DEFAULT_LANG_CHAR_SET	"040904E4"

//---------------------------------------------------------------------------
VersionInfo	Version;

//---------------------------------------------------------------------------
VersionInfo::VersionInfo()
{
	char	file[ 1024 ];
    DWORD	size, dummy;

    Translations = new TStringList();

    GetModuleFileName( NULL, file, sizeof( file ));

	if( size = GetFileVersionInfoSize( file, &dummy )) {
    	char			   *buf = new char[ size ];
        VS_FIXEDFILEINFO   *info;
        UINT				len;
        struct {
        	unsigned short	Lang;
            unsigned short	Charset;
        }				   *ids;

		GetFileVersionInfo( file, NULL, size, buf );

        if( VerQueryValue( buf, "\\", &(void *)info, &len )) {
        	FILETIME	dt;
            SYSTEMTIME	systime;

			Version           = HIWORD( info->dwFileVersionMS );
			Revision          = LOWORD( info->dwFileVersionMS );
			Release           = HIWORD( info->dwFileVersionLS );
			Build             = LOWORD( info->dwFileVersionLS );
            dt.dwLowDateTime  = info->dwFileDateMS;
            dt.dwHighDateTime = info->dwFileDateLS;

            FileTimeToSystemTime( &dt, &systime );

            Date = TDateTime( systime.wYear, systime.wMonth, systime.wDay ).FormatString( "dd mmm yyyy" );
        }

		if( VerQueryValue( buf, "\\VarFileInfo\\Translation", &(void *)ids, &len )) {
        	int	num = len / sizeof( *ids );

            if( num > 0 ) {

	            for( int i = 0; i < num; i++ )
    	      		Translations->Add( Format( "%.4x%.4x", ARRAYOFCONST(( ids[ i ].Lang, ids[ i ].Charset ))));

            } else if( len > 0 ) {
        		// There was translation info, but there was not a full set. What's
          		// there is usually a char set, so we have to swap things around.

		        Translations->Add( Format( "%.4x%.4x", ARRAYOFCONST(( DEFAULT_LANG_ID, ids->Lang ))));
            }
        }

        Copyright   = GetString( buf, "LegalCopyright" );
        ProductName = GetString( buf, "ProductName" );

        delete[] buf;
    }
}
//---------------------------------------------------------------------------
VersionInfo::~VersionInfo()
{
	delete Translations;
}
//---------------------------------------------------------------------------
const char *VersionInfo::GetString( void *buf, AnsiString which )
{
	char   	   *str;
    UINT		len;
    AnsiString 	lang = DEFAULT_LANG_CHAR_SET;

    if( Translations->Count )
    	lang = Translations->Strings[ 0 ];

	which.Insert( "\\StringFileInfo\\" + lang + "\\", 1 );

	VerQueryValue( buf, which.c_str(), &(void *)str, &len );

    return( str );
}
//---------------------------------------------------------------------------

