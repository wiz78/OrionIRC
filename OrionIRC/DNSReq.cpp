/* $Id: DNSReq.cpp,v 1.1 2003/04/24 12:13:14 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DNSReq.h"
#include "Main.h"
#include "ServerWnd.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
DNSReq::DNSReq( AnsiString host, TServerWin *form )
{
	Host    = host;
    Server  = form->Session.Server;
	AddrV4  = inet_addr( Host.c_str() );
	Reverse = ( AddrV4 != INADDR_NONE ) /* || host.Pos( ":" )*/;

    if( Reverse ) {
		void   *addr;
        int		type, len = 0;

//        if( !host.Pos( ":" )) {

        	addr = &AddrV4;
            len  = sizeof( AddrV4 );
            type = AF_INET;

/*        } else {

        	addr   = &addrv6;

        	type   = AF_INET6;
        }
*/
	    AsyncReq = WSAAsyncGetHostByAddr( MainWin->Handle, WM_RESOLVED, (char *)addr, len, type, HostEnt, sizeof( HostEnt ));

    } else
	    AsyncReq = WSAAsyncGetHostByName( MainWin->Handle, WM_RESOLVED, Host.c_str(), HostEnt, sizeof( HostEnt ));
}
//---------------------------------------------------------------------------
DNSReq::~DNSReq()
{
	if( AsyncReq )
   		WSACancelAsyncRequest( AsyncReq );
}
//---------------------------------------------------------------------------
AnsiString DNSReq::GetResolvedName( void )
{
    struct hostent *ent = (struct hostent *)HostEnt;
    AnsiString		ret = "N/A";

    if( Reverse )
    	ret = ent->h_name;
    else
    	ret = inet_ntoa( *((struct in_addr *)ent->h_addr ));

    return( ret );
}
//---------------------------------------------------------------------------
