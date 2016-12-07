/* $Id: FileFilterMgr.cpp,v 1.2 2003/05/11 21:25:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FileFilterMgr.h"
#include "FileFilter.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
FileFilterMgr::FileFilterMgr()
{
    Filters = new TList();
}
//---------------------------------------------------------------------------
FileFilterMgr::~FileFilterMgr()
{
	Clear();

    delete Filters;
}
//---------------------------------------------------------------------------
int FileFilterMgr::GetCount( void )
{
	return( Filters->Count );
}
//---------------------------------------------------------------------------
TFileFilter *FileFilterMgr::Get( int i )
{
	TFileFilter *ret = NULL;

    if( i < Filters->Count )
		ret = reinterpret_cast<TFileFilter *>( Filters->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void FileFilterMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Filters->Clear();
}
//---------------------------------------------------------------------------
void FileFilterMgr::Add( TFileFilter *filter )
{
	Filters->Add( filter );
}
//---------------------------------------------------------------------------
TFileFilter *FileFilterMgr::Find( AnsiString name )
{
	TFileFilter *ret = NULL;

    for( int i = 0; !ret && ( i < GetCount() ); i++ ) {
    	TFileFilter	*filter = Get( i );

        if( filter->IsEnabled() && filter->Matches( name ))
        	ret = filter;
    }

    return( ret );
}
//---------------------------------------------------------------------------
bool FileFilterMgr::IsAcceptable( AnsiString file )
{
	TFileFilter *filter = Find( file );
    bool		 ret = true;

	if( filter && ( filter->GetAction() == FILE_IGNORE ))
		ret = false;

    return( ret );
}
//---------------------------------------------------------------------------
void FileFilterMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TFileFilter	*filter = new TFileFilter();

            filter->Load( reg, key + "\\Filter" + IntToStr( i ));

            Add( filter );
        }
    }
}
//---------------------------------------------------------------------------
void FileFilterMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Filter" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------