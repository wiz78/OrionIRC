/* $Id: ColorSet.cpp,v 1.5 2005/06/04 17:40:54 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ColorSet.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
ColorSet::ColorSet()
{
	Colors[ IRC_NORMAL               ] = (TColor)0;
	Colors[ IRC_MSG                  ] = (TColor)0;
	Colors[ IRC_OWN_MSG              ] = (TColor)0;
	Colors[ IRC_JOIN                 ] = (TColor)16711935;
	Colors[ IRC_PART                 ] = (TColor)32768;
	Colors[ IRC_QUIT                 ] = (TColor)8388608;
	Colors[ IRC_CTCP                 ] = (TColor)16711680;
	Colors[ IRC_DCC                  ] = (TColor)8388608;
    Colors[ IRC_DCC_CHAT             ] = (TColor)0;
	Colors[ IRC_ERROR                ] = (TColor)8388608;
	Colors[ IRC_NICK                 ] = (TColor)32768;
    Colors[ IRC_MOTD                 ] = (TColor)0;
	Colors[ IRC_ACTION               ] = (TColor)8388736;
	Colors[ IRC_WALLOP               ] = (TColor)128;
	Colors[ IRC_TOPIC                ] = (TColor)32768;
	Colors[ IRC_KICK                 ] = (TColor)8388736;
	Colors[ IRC_MODE                 ] = (TColor)32768;
	Colors[ IRC_NOTIFY               ] = (TColor)33023;
	Colors[ IRC_NOTICE               ] = (TColor)128;
	Colors[ IRC_INVITE               ] = (TColor)16711680;
	Colors[ IRC_WHOIS                ] = (TColor)0;
	Colors[ IRC_HIGHLIGHT            ] = (TColor)255;
    Colors[ IRC_LOCAL                ] = (TColor)0;

    Colors[ IRC_BACKGROUND           ] = (TColor)0xffffff;
    Colors[ IRC_NICK_BACKGROUND      ] = (TColor)0xdddddd;
    Colors[ IRC_TIMESTAMP_BACKGROUND ] = (TColor)0xcccccc;
    Colors[ IRC_NICK_COLUMN			 ] = clBlack;
    Colors[ IRC_TIMESTAMP_COLUMN     ] = clBlack;
    Colors[ IRC_EVENT                ] = clBlack;
    Colors[ IRC_SCRIPT               ] = (TColor)0x222222;

    memset( Styles, 0, sizeof( Styles ));
}
//---------------------------------------------------------------------------
TColor ColorSet::GetColor( TIRCEvent type )
{
	return( Colors[ type ]);
}
//---------------------------------------------------------------------------
void ColorSet::SetColor( TIRCEvent type, TColor col )
{
	Colors[ type ] = col;
}
//---------------------------------------------------------------------------
bool ColorSet::IsBold( TIRCEvent type ) const
{
	return( Styles[ type ] & S_BOLD );
}
//---------------------------------------------------------------------------
void ColorSet::SetBold( TIRCEvent type, bool set )
{
	if( set )
    	Styles[ type ] |= S_BOLD;
    else
    	Styles[ type ] &= ~S_BOLD;
}
//---------------------------------------------------------------------------
bool ColorSet::IsItalic( TIRCEvent type ) const
{
	return( Styles[ type ] & S_ITALIC );
}
//---------------------------------------------------------------------------
void ColorSet::SetItalic( TIRCEvent type, bool set )
{
	if( set )
    	Styles[ type ] |= S_ITALIC;
    else
    	Styles[ type ] &= ~S_ITALIC;
}
//---------------------------------------------------------------------------
void ColorSet::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

        	for( int i = 0; i < IRC_NUM_EVENTS; i++ ) {
            	reg->WriteInteger( "Color" + IntToStr( i ), Colors[ i ]);
            	reg->WriteInteger( "Style" + IntToStr( i ), Styles[ i ]);
            }
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void ColorSet::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

        for( int i = 0; i < IRC_NUM_EVENTS; i++ ) {
            try { Colors[ i ] = (TColor)reg->ReadInteger( "Color" + IntToStr( i )); } catch(...) {}
            try { Styles[ i ] = (char)reg->ReadInteger( "Style" + IntToStr( i )); } catch(...) {}
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void ColorSet::Save( TIniFile *ini, AnsiString section )
{
    for( int i = 0; i < IRC_NUM_EVENTS; i++ ) {
        ini->WriteInteger( section, "Color" + IntToStr( i ), Colors[ i ]);
        ini->WriteInteger( section, "Style" + IntToStr( i ), Styles[ i ]);
    }
}
//---------------------------------------------------------------------------
void ColorSet::Load( TIniFile *ini, AnsiString section )
{
    for( int i = 0; i < IRC_NUM_EVENTS; i++ ) {
        Colors[ i ] = (TColor)ini->ReadInteger( section, "Color" + IntToStr( i ), Colors[ i ]);
        Styles[ i ] = (char)ini->ReadInteger( section, "Style" + IntToStr( i ), Styles[ i ]);
    }
}
//---------------------------------------------------------------------------

