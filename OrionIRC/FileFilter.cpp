/* $Id: FileFilter.cpp,v 1.2 2003/05/11 21:25:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FileFilter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TFileFilter::TFileFilter()
{
	Action = FILE_IGNORE;

    Flags.Set( FFF_ENABLED );
}
//---------------------------------------------------------------------------
TFileFilter::TFileFilter( const TFileFilter *copy )
{
	*this = *copy;
}
//---------------------------------------------------------------------------
TFileFilter& TFileFilter::operator =( const TFileFilter &rhs )
{
	Name       = rhs.Name;
    Extensions = rhs.Extensions;
    Directory  = rhs.Directory;
    Action     = rhs.Action;
    Flags      = rhs.Flags;

    return( *this );
}
//---------------------------------------------------------------------------
void TFileFilter::Save( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, true )) {

    	try {
	    	reg->WriteString( "Name",       Name       );
	    	reg->WriteString( "Extensions", Extensions );
	    	reg->WriteString( "Directory",  Directory  );

	    	reg->WriteInteger( "Action", Action      );
            reg->WriteInteger( "Flags",  Flags.Get() );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TFileFilter::Load( TRegistry *reg, AnsiString key )
{
	if( reg->OpenKey( key, false )) {

    	Name       = reg->ReadString( "Name"       );
    	Extensions = reg->ReadString( "Extensions" );
        Directory  = reg->ReadString( "Directory"  );

    	try{ Action = (TFileAction)reg->ReadInteger( "Action" ); } catch(...) {}
    	try{ Flags.Set( reg->ReadInteger( "Flags" )); } catch(...) {}

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
bool TFileFilter::Matches( AnsiString file )
{
	bool	ret = false;
	int		idx = file.LastDelimiter( "." );

    if( idx ) {
    	AnsiString	ext = file.SubString( idx + 1, file.Length() ).LowerCase();

        ret = Extensions.Pos( ext ) > 0;
    }

    return( ret );
}
//---------------------------------------------------------------------------

