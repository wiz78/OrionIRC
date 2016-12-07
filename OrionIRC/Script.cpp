/* $Id: Script.cpp,v 1.1 2005/01/23 02:14:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "Script.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TScript::TScript()
{
	Enabled = true;
}
//---------------------------------------------------------------------------
TScript::TScript( const TScript *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TScript &TScript::operator =( const TScript& rhs )
{
	File    = rhs.File;
	Enabled = rhs.Enabled;

    return( *this );
}
//---------------------------------------------------------------------------
void TScript::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	File = reg->ReadString( "File" );

        try{ Enabled = reg->ReadBool( "Enabled" ); } catch(...) {}

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TScript::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {

    		reg->WriteString( "File", File );

        	reg->WriteBool( "Enabled", Enabled );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
