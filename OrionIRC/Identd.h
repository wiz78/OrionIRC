/* $Id: Identd.h,v 1.1 2003/04/15 15:29:34 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IdentdH
#define IdentdH
//---------------------------------------------------------------------------

#include "tcpsocket.h"

class IdentCon;

class TIdentd
{
public:
					TIdentd();
                   ~TIdentd();

	void			AddInfo( int port, AnsiString ident );
    void			RemInfo( int port );
    AnsiString		FindInfo( int port );

    void			RemConn( IdentCon *conn );

private:
	TcpSocket	   *Sock;
    TStringList	   *Idents;
    TList		   *Connections;

	void 			Setup( void );
	static void 	SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata );
};

#endif
