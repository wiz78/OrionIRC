/* $Id: MatchStr.cpp,v 1.1 2003/07/26 17:18:49 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MatchStr.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
bool MatchStr( char *string, char *pattern, bool wildcard )
{
    bool    ret = false;
    char   *ptr;

    if( ptr = strchr( pattern, ';' )) {

        do {

            if( ptr )
                *ptr = '\0';

            if( !ret )
                ret = MatchStr( string, pattern, wildcard );

            if( ptr )
                *ptr++ = ';';

            pattern = ptr;

            if( pattern )
                ptr = strchr( pattern, ';' );

        } while( pattern && pattern[0] );

    } else if( string && pattern && *pattern ) {

        ret = true;
        ptr = strchr( pattern, '*' );

        if( !ptr ) {

            if( *string ) {

                if( wildcard ) // *a case, where * has already been removed from the pattern
                    string = strstr( string, pattern );

                ret = string && ( stricmp( string, pattern ) == 0 );

            } else
                ret = wildcard; // a* matches "a"

        } else {
            int len;

            *ptr = '\0';
            len  = strlen( pattern );

            if( len ) {

                if( wildcard ) {
                    char    *p, buffer[ 1024 ], patt[ 1024 ];

                    strncpy( buffer, string,  sizeof( buffer ));
                    strncpy( patt,   pattern, sizeof( patt   ));

                    buffer[ sizeof( buffer ) - 1 ] = '\0';
                    patt[ sizeof( patt ) - 1 ]     = '\0';

                    p = buffer;

                    while( *p )
                        *p++ = tolower( *p );

                    p = buffer;

                    while( *p )
                        *p++ = tolower( *p );

                    if( p = strstr( buffer, patt ))
                        string += ( p - buffer ) + strlen( patt );
                    else
                        ret = false;

                } else {

                    if( strncmpi( string, pattern, len ))
                        ret = false;
                    else
                        string += len;
                }
            }

            *ptr++  = '*';
            pattern = ptr;

            if( *string == '\0' )
                while( ret && *ptr ) {  // only case when string can match a shorter pattern is
                    if( *ptr++ != '*' ) // when the pattern ends with a(t least one) *
                       ret = false;
                }

            if( ret && *pattern )
                ret = MatchStr( string, pattern, true );
        }
    }

    return( ret );
}
//---------------------------------------------------------------------------
