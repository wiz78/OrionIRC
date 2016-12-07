/* $Id: User.h,v 1.3 2005/04/26 20:16:46 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef UserH
#define UserH
//---------------------------------------------------------------------------

#include "bitfield.h"

#define USER_OP			(1 << 0)
#define USER_VOICE		(1 << 1)
#define USER_AWAY		(1 << 2)
#define USER_OWNER		(1 << 3)
#define USER_CHANADMIN	(1 << 4)
#define USER_HALF_OP	(1 << 5)

class TUser
{
public:
					TUser() {}
					TUser( AnsiString nick );

	AnsiString		GetNick( void ) const  		{ return( Nick ); }
    AnsiString		GetHost( void ) const  		{ return( Host ); }
    AnsiString		GetAwayReason( void ) const { return( AwayReason ); }

	void			SetNick( AnsiString str ) 		{ Nick = str; }
    void			SetHost( AnsiString str ) 		{ Host = str; }
    void			SetAwayReason( AnsiString str ) { AwayReason = str; }

    bool			IsOp( void ) const 			{ return( Flags.IsSet( USER_OP        )); }
    bool			IsHalfOp( void ) const 		{ return( Flags.IsSet( USER_HALF_OP   )); }
    bool			IsOwner( void ) const 		{ return( Flags.IsSet( USER_OWNER     )); }
    bool			IsChanAdmin( void ) const 	{ return( Flags.IsSet( USER_CHANADMIN )); }
    bool			IsAway( void ) const 		{ return( Flags.IsSet( USER_AWAY      )); }
    bool			HasVoice( void ) const 		{ return( Flags.IsSet( USER_VOICE     )); }

    void			SetOp( bool set )  			{ Flags.Set( USER_OP,        set ); }
    void			SetHalfOp( bool set )  		{ Flags.Set( USER_HALF_OP,   set ); }
    void			SetOwner( bool set ) 		{ Flags.Set( USER_OWNER,     set ); }
    void			SetChanAdmin( bool set )	{ Flags.Set( USER_CHANADMIN, set ); }
    void			SetAway( bool set )  		{ Flags.Set( USER_AWAY,      set ); }
    void			SetVoice( bool set )  		{ Flags.Set( USER_VOICE,     set ); }

private:
	AnsiString		Nick;
    AnsiString		Host;
    AnsiString		AwayReason;
    BitField		Flags;
};


#endif
