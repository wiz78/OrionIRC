/* $Id: FavouritesMgr.h,v 1.1 2003/05/09 21:50:52 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FavouritesMgrH
#define FavouritesMgrH
//---------------------------------------------------------------------------

class TFavourite;

class FavouritesMgr
{
public:
				FavouritesMgr();
               ~FavouritesMgr();

	int 		GetCount( void );
    TFavourite *Get( int i );

    void		Clear( void );

    void		Add( TFavourite *fav );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

private:
    TList	   *Favourites;
};

#endif
