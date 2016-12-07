/* $Id: MyIniFile.cpp,v 1.2 2006/04/16 19:38:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <fstream.h>

#include "MyIniFile.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
MyIniFile::MyIniFile( AnsiString file, bool load )
{
    File = file;
    Keys = new TStringList();
    Data = new TStringList();

    if( load )
    	Load();
}
//---------------------------------------------------------------------------
MyIniFile::~MyIniFile()
{
	if( Update ) {
    	ofstream fh( File.c_str(), ios::trunc );

        if( fh ) {
        	AnsiString section = "none";

        	for( int i = 0; i < Keys->Count; i++ ) {
            	AnsiString	str = Keys->Strings[ i ];
                int			pos = str.Pos( "§" );
                AnsiString 	sect = str.SubString( 1, pos - 1 );
                AnsiString 	key = str.SubString( pos + 1, str.Length() );

                if( section.AnsiCompareIC( sect )) {
                	fh << "[" << sect.c_str() << "]" << endl;
                    section = sect;
                }

                fh << key.c_str() << "=" << Data->Strings[ i ].c_str() << endl;
            }

            fh.close();
        }
    }

	delete Keys;
    delete Data;
}
//---------------------------------------------------------------------------
void MyIniFile::Clear( void )
{
	Keys->Clear();
    Data->Clear();
}
//---------------------------------------------------------------------------
void MyIniFile::Load( void )
{
	ifstream fh( File.c_str() );

    if( fh ) {
    	AnsiString section = "none";

		while( !fh.eof() ) {
        	char line[ 1024 * 64 ];

            fh.getline( line, sizeof( line ));

            if( int len = fh.gcount() ) {

            	if( line[0] == '[' ) {

                	section = &line[1];

                    section.Delete( section.Length(), 1 );

                    section += "§";

                } else {
					AnsiString 	str = line;
					int			pos = str.Pos( '=' );

                    if( pos ) {

                        Keys->Add( section + str.SubString( 1, pos - 1 ));
                        Data->Add( str.SubString( pos + 1, str.Length() ));
                    }
                }
            }
        }

        fh.close();
    }
}
//---------------------------------------------------------------------------
int MyIniFile::ReadInteger( AnsiString section, AnsiString key, int def )
{
	int i = Keys->IndexOf( section + "§" + key );

    if( i >= 0 )
    	def = Data->Strings[ i ].ToIntDef( def );

    return( def );
}
//---------------------------------------------------------------------------
AnsiString MyIniFile::ReadString( AnsiString section, AnsiString key, AnsiString def )
{
	int i = Keys->IndexOf( section + "§" + key );

    if( i >= 0 )
    	def = Data->Strings[ i ];

    return( def );
}
//---------------------------------------------------------------------------
TDateTime MyIniFile::ReadDateTime( AnsiString section, AnsiString key, TDateTime def )
{
	int i = Keys->IndexOf( section + "§" + key );

    if( i >= 0 )
    	def = TDateTime( Data->Strings[ i ] );

    return( def );
}
//---------------------------------------------------------------------------
void MyIniFile::WriteInteger( AnsiString section, AnsiString key, int value )
{
	WriteString( section, key, IntToStr( value ));
}
//---------------------------------------------------------------------------
void MyIniFile::WriteString( AnsiString section, AnsiString key, AnsiString value )
{
	int i = Keys->IndexOf( section + "§" + key );

    if( i < 0 ) {

    	Keys->Add( section + "§" + key );
        Data->Add( value );

	} else
    	Data->Strings[ i ] = value;

    Update = true;
}
//---------------------------------------------------------------------------
void MyIniFile::WriteDateTime( AnsiString section, AnsiString key, TDateTime value )
{
	WriteString( section, key, value.DateTimeString() );
}
//---------------------------------------------------------------------------
