/* $Id: DNSReq.h,v 1.1 2003/04/24 12:13:14 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DNSReqH
#define DNSReqH
//---------------------------------------------------------------------------

#include <winsock2.h>

class TServerWin;

class DNSReq
{
public:
					DNSReq( AnsiString host, TServerWin *form );
                   ~DNSReq();

	void			Done( void ) 	  		{ AsyncReq = NULL; }

    HANDLE			GetHandle( void ) 		{ return( AsyncReq ); }
    AnsiString		GetHost( void )   		{ return( Host ); }
    AnsiString		GetServer( void ) 		{ return( Server ); }
    AnsiString		GetResolvedName( void );

private:
	AnsiString		Host;
    AnsiString		Server;
    HANDLE			AsyncReq;
    char			HostEnt[ MAXGETHOSTSTRUCT ];
    bool			Reverse;
    int				AddrV4;
};

#endif
