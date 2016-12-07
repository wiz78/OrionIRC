/* $Id: Hostmask.h,v 1.4 2003/12/31 16:16:04 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef HostmaskH
#define HostmaskH
//---------------------------------------------------------------------------

typedef enum {
	MASK_NICK_USER_HOST = 0,
    MASK_NICK_USER_TLD,
    MASK_USER_HOST,
    MASK_USER_TLD,
    MASK_HOST,
    MASK_TLD
} MaskType;

class HostMask
{
public:
	    		HostMask();
	    		HostMask( AnsiString mask );
				HostMask( AnsiString nick, AnsiString user, AnsiString host );

	AnsiString	Get( void );
	AnsiString	GetUserHost( void );

	AnsiString	GetNick( void ) { return( Nick ); }
	AnsiString	GetUser( void ) { return( User ); }
	AnsiString	GetHost( void ) { return( Host ); }

    AnsiString	GetMask( MaskType type );

    void		Set( AnsiString mask );

	void		SetNick( AnsiString str ) { Nick = str; }
	void		SetUser( AnsiString str ) { User = str; }
	void		SetHost( AnsiString str ) { Host = str; }

	void		Clear( void );

    bool		Match( AnsiString pattern );

private:
	AnsiString	Nick;
    AnsiString	User;
    AnsiString	Host;

	AnsiString 	GetTldMask( void );
};

#endif
