/* $Id: Alias.h,v 1.1 2005/01/23 19:16:43 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef AliasH
#define AliasH
//---------------------------------------------------------------------------

#include <registry.hpp>

class TServerWin;
class TChanWin;

class TAlias
{
public:
					TAlias();
					TAlias( const TAlias *copy );

	TAlias&			operator =( const TAlias& rhs );

    AnsiString		GetName( void ) const			{ return( Name ); }
    AnsiString		GetCommand( void ) const	   	{ return( Command ); }

    void			SetName( AnsiString str )		{ Name = str; }
    void			SetCommand( AnsiString str )	{ Command = str; }

	void			Load( TRegistry *reg, AnsiString key );
	void			Save( TRegistry *reg, AnsiString key );

    void			Invoke( TServerWin *server, TChanWin *channel,
    						AnsiString args, TStringList *words );

private:
	AnsiString		Name;
    AnsiString		Command;

	AnsiString 		GetNum( AnsiString str, int pos );
};

#endif
