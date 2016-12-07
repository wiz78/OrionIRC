/* $Id: FavouritesMgr.cpp,v 1.1 2003/05/09 21:50:52 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "FavouritesMgr.h"
#include "Favourite.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
FavouritesMgr::FavouritesMgr()
{
    Favourites = new TList();
}
//---------------------------------------------------------------------------
FavouritesMgr::~FavouritesMgr()
{
	Clear();

    delete Favourites;
}
//---------------------------------------------------------------------------
int FavouritesMgr::GetCount( void )
{
	return( Favourites->Count );
}
//---------------------------------------------------------------------------
TFavourite *FavouritesMgr::Get( int i )
{
	TFavourite *ret = NULL;

    if( i < Favourites->Count )
		ret = reinterpret_cast<TFavourite *>( Favourites->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void FavouritesMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Favourites->Clear();
}
//---------------------------------------------------------------------------
void FavouritesMgr::Add( TFavourite *fav )
{
	Favourites->Add( fav );
}
//---------------------------------------------------------------------------
void FavouritesMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TFavourite	*fav = new TFavourite();

            fav->Load( reg, key + "\\Favourite" + IntToStr( i ));

            Add( fav );
        }
    }
}
//---------------------------------------------------------------------------
void FavouritesMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Favourite" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------