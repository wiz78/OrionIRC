/* $Id: Identd.cpp,v 1.4 2003/04/24 12:13:14 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>

#include "Identd.h"
#include "Main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

class IdentCon
{
public:
					IdentCon( TIdentd *ident, TcpSocket *sock );
                   ~IdentCon();

	TIdentd		   *Ident;
	TcpSocket	   *Sock;
    AnsiString		Request;
    char			Buffer[ 256 ];

private:
	static void 	SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata );
	void 			HandleIncomingData( int len );
	void 			Close( void );
};

//---------------------------------------------------------------------------
TIdentd::TIdentd()
{
	Sock        = NULL;
    Idents      = new TStringList();
    Connections = new TList();
}
//---------------------------------------------------------------------------
TIdentd::~TIdentd()
{
	for( int i = 0; i < Connections->Count; i++ )
    	delete reinterpret_cast<IdentCon *>( Connections->Items[ i ] );

	delete Sock;
    delete Idents;
    delete Connections;
}
//---------------------------------------------------------------------------
void TIdentd::AddInfo( int port, AnsiString ident )
{
	Idents->AddObject( ident, reinterpret_cast<System::TObject *>( port ));

    if( !Sock )
    	Setup();
}
//---------------------------------------------------------------------------
void TIdentd::RemInfo( int port )
{
	for( int i = 0; i < Idents->Count; i++ )
    	if( port == reinterpret_cast<int>( Idents->Objects[ i ])) {
        	Idents->Delete( i );
            break;
        }

    if( !Idents->Count ) {

    	delete Sock;

        Sock = NULL;
    }
}
//---------------------------------------------------------------------------
AnsiString TIdentd::FindInfo( int port )
{
	for( int i = 0; i < Idents->Count; i++ )
    	if( port == reinterpret_cast<int>( Idents->Objects[ i ]))
        	return( Idents->Strings[ i ]);

    return( "" );
}
//---------------------------------------------------------------------------
void TIdentd::Setup( void )
{
	bool	ok;

	Sock = new TcpSocket();

   	Sock->UseDispatcher( &MainWin->IO );
   	Sock->SetAsyncCallback( SocketCB, this );

    ok = Sock->Bind( 113 ) && Sock->Listen();

    if( !ok ) {

    	delete Sock;

        Sock = NULL;
    }
}
//---------------------------------------------------------------------------
void TIdentd::SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata )
{
	TIdentd *id = reinterpret_cast<TIdentd *>( userdata );

    if( reason == PROM_SOCK_ACCEPT ) {
    	TcpSocket *sock = reinterpret_cast<TcpSocket *>( data );

        if( sock->IsValid() )
        	id->Connections->Add( new IdentCon( id, sock ));
        else
        	delete sock;
    }
}
//---------------------------------------------------------------------
void TIdentd::RemConn( IdentCon *conn )
{
	Connections->Remove( conn );
}
//---------------------------------------------------------------------
IdentCon::IdentCon( TIdentd *ident, TcpSocket *sock )
{
	Ident = ident;
	Sock  = sock;

    Sock->UseDispatcher( &MainWin->IO );
    Sock->SetAsyncCallback( SocketCB, this );
    Sock->AsyncRecv( Buffer, sizeof( Buffer ));
}
//---------------------------------------------------------------------
IdentCon::~IdentCon()
{
	delete Sock;
}
//---------------------------------------------------------------------
void IdentCon::SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata )
{
	IdentCon *con = reinterpret_cast<IdentCon *>( userdata );

    switch( reason ) {

    	case PROM_SOCK_READ:
        	if( data )
            	con->HandleIncomingData( data );
            else
            	con->Close();
        	break;

        case PROM_SOCK_ERROR:
        case PROM_SOCK_TIMEOUT:
        	con->Close();
        	break;
    }
}
//---------------------------------------------------------------------
void IdentCon::HandleIncomingData( int len )
{
	bool	done = false;
    char   *ptr;

	Buffer[ len ] = '\0';

    ptr = strchr( Buffer, '\r' );

    if( !ptr )
    	ptr = strchr( Buffer, '\n' );

    if( ptr ) {
    	*ptr = '\0';
    	done = true;
    }

    Request += Buffer;

    if( done ) {
    	int		local, remote;
        bool    ok = false;

        if( sscanf( Request.c_str(), "%d , %d", &local, &remote ) == 2 ) {
        	AnsiString	user = Ident->FindInfo( local );

            if( !user.IsEmpty() ) {

		        Sock->AsyncPrintf( "%s : USERID : %s : %s\r\n",
    		    				   Request.c_str(), "Win32", user.c_str() );

            	ok = true;
            }
        }

        if( !ok )
        	Sock->AsyncPrintf( "%s : ERROR : NO-USER\r\n", Request.c_str() );

        Request = "";
    }

    Sock->AsyncRecv( Buffer, sizeof( Buffer ));
}
//---------------------------------------------------------------------
void IdentCon::Close( void )
{
	Ident->RemConn( this );

    delete this;
}
//---------------------------------------------------------------------
