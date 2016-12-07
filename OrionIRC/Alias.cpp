/* $Id: Alias.cpp,v 1.2 2005/04/08 17:31:58 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Alias.h"
#include "Commands.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TAlias::TAlias()
{
}
//---------------------------------------------------------------------------
TAlias::TAlias( const TAlias *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TAlias &TAlias::operator =( const TAlias& rhs )
{
	Name    = rhs.Name;
	Command = rhs.Command;

    return( *this );
}
//---------------------------------------------------------------------------
void TAlias::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Name    = reg->ReadString( "Name" );
    	Command = reg->ReadString( "Command" );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TAlias::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {
    		reg->WriteString( "Name", Name );
    		reg->WriteString( "Command", Command );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TAlias::Invoke( TServerWin *server, TChanWin *channel,
					 AnsiString args, TStringList *words )
{
	AnsiString cmd, tmp = Command;

    while( int pos = tmp.Pos( "%" )) {

    	cmd += tmp.SubString( 1, pos - 1 );

        if( pos + 1 <= tmp.Length() ) {

            switch( tmp[ ++pos ] ) {

                case '%':
                    cmd += "%";
                    break;

                case 's':
                    cmd += args;
                    break;

                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    AnsiString 	num = GetNum( tmp, pos );
                    int			idx = num.ToIntDef( 1 );

                    if(( idx > 0 ) && ( idx <= words->Count ))
                        cmd += words->Strings[ idx - 1 ];

                    pos += num.Length() - 1;
                }	break;
            }
        }

    	tmp.Delete( 1, pos );
    }

    HandleCommand( server, channel, cmd + tmp );
}
//---------------------------------------------------------------------------
AnsiString TAlias::GetNum( AnsiString str, int pos )
{
	int start = pos;

	while(( pos <= str.Length() ) && isdigit( str[ pos ] ))
    	pos++;

    return( str.SubString( start, pos - start ));
}
//---------------------------------------------------------------------------

