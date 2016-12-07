/* $Id: FileFilter.h,v 1.1 2003/05/11 17:30:47 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FileFilterH
#define FileFilterH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "bitfield.h"

typedef enum { FILE_SAVE, FILE_IGNORE } TFileAction;

#define FFF_ENABLED		(1 << 0)

class TFileFilter
{
public:
					TFileFilter();
                    TFileFilter( const TFileFilter *copy );

	TFileFilter&	operator=( const TFileFilter &rhs );

	void 			Save( TRegistry *reg, AnsiString key );
	void 			Load( TRegistry *reg, AnsiString key );

    bool			Matches( AnsiString file );

	AnsiString		GetName( void )			{ return( Name       ); }
    AnsiString		GetExtensions( void )	{ return( Extensions ); }
    AnsiString		GetDirectory( void )    { return( Directory  ); }
	TFileAction		GetAction( void )       { return( Action     ); }
    bool			IsEnabled( void )		{ return( Flags.IsSet( FFF_ENABLED )); }

	void			SetName( AnsiString str )		{ Name       = str; }
    void			SetExtensions( AnsiString str )	{ Extensions = str.LowerCase(); }
    void			SetDirectory( AnsiString str )  { Directory  = str; }
	void			SetAction( TFileAction act )    { Action     = act; }
    void			SetEnabled( bool set )			{ Flags.Set( FFF_ENABLED, set ); }

private:
	AnsiString		Name;
    AnsiString		Extensions;
    AnsiString		Directory;
	TFileAction		Action;
    BitField		Flags;
};

#endif
