/* $Id: Ignore.h,v 1.2 2003/07/27 13:32:14 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IgnoreH
#define IgnoreH
//---------------------------------------------------------------------------

#include "bitfield.h"

class HostMask;

class TIgnore
{
public:
					TIgnore();
                    TIgnore( TIgnore *copy );

	TIgnore&		operator =( const TIgnore& rhs );

    AnsiString		GetMask( void ) const			{ return( Mask ); }
    bool			IsExcluded( void ) const		{ return( Exclude ); }
    bool			MatchEvent( int event ) const   { return( Events.IsSet( event )); }

    void			SetMask( AnsiString str )		{ Mask = str; }
    void			SetExcluded( bool set )	 		{ Exclude = set; }
    void			SetEvents( int events )			{ Events = events; }

    bool			Match( HostMask& mask );
    bool			Ignore( AnsiString& msg, int event );

	void			Load( TRegistry *reg, AnsiString key );
	void			Save( TRegistry *reg, AnsiString key );

private:
	AnsiString		Mask;
    BitField		Events;
    bool			Exclude;
};

#endif
 