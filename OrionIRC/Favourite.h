/* $Id: Favourite.h,v 1.2 2003/05/25 10:48:29 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FavouriteH
#define FavouriteH
//---------------------------------------------------------------------------

class TFavourite
{
public:
					TFavourite() {}
					TFavourite( TFavourite *copy );

	TFavourite&		operator=( const TFavourite& rhs );

	void 			Save( TRegistry *reg, AnsiString key );
	void 			Load( TRegistry *reg, AnsiString key );

	AnsiString		GetChannel( void )     { return( Channel ); }
    AnsiString		GetDescription( void ) { return( Description ); }
    AnsiString		GetNetwork( void )     { return( Network ); }
    AnsiString		GetPassword( void )	   { return( Password ); }

	void			SetChannel( AnsiString str )     { Channel = str; }
    void			SetDescription( AnsiString str ) { Description = str; }
    void			SetNetwork( AnsiString str )     { Network = str; }
    void			SetPassword( AnsiString str )    { Password = str; }

private:
	AnsiString		Channel;
    AnsiString		Description;
    AnsiString		Network;
    AnsiString		Password;
};

#endif
