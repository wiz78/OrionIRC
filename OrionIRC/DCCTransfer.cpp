/* $Id: DCCTransfer.cpp,v 1.20 2005/04/15 21:44:21 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <fstream>

#include "DCCTransfer.h"
#include "ServerWnd.h"
#include "Prefs.h"
#include "Main.h"
#include "OrionScriptInterface.h"
#include "Version.h"
#include "FileProps/FileProps.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define DCC_TIMEOUT	 		180
#define DCC_TIMEOUT_ACK		180

//---------------------------------------------------------------------------
TDCCTransfer::TDCCTransfer( AnsiString server, AnsiString nick, AnsiString file )
{
    Server     = server;
	Target     = nick;
    File       = file;
    Item	   = NULL;
    IsSend     = false;
    FileHandle = INVALID_HANDLE_VALUE;

    Reset();
    CreateSocket();
}
//---------------------------------------------------------------------------
TDCCTransfer::~TDCCTransfer()
{
	delete Sock;

    if( FileHandle != INVALID_HANDLE_VALUE )
    	CloseHandle( FileHandle );
}
//---------------------------------------------------------------------------
void TDCCTransfer::Reset( void )
{
    Done       = 0;
    ResumeFrom = 0;
    State      = DCC_WAITING;

    if( File.IsEmpty() )
    	Size = 0;
    else
    	Size = GetFileSize( File );
}
//---------------------------------------------------------------------------
void TDCCTransfer::CreateSocket( void )
{
	Sock = new TcpSocket();

    Sock->UseDispatcher( &MainWin->IO );
   	Sock->SetAsyncCallback( SocketCB, this );
}
//---------------------------------------------------------------------------
int TDCCTransfer::GetFileSize( AnsiString file )
{
	ifstream	fh( file.c_str(), ios::binary );
    int			size = 0;

    if( fh ) {

        fh.seekg( 0, ios::end );

        size = fh.tellg();

        fh.close();
    }

    return( size );
}
//---------------------------------------------------------------------------
AnsiString TDCCTransfer::GetStatus( void )
{
	static const char *States[] = { "waiting", "resuming", "connecting", "sending",
			   					    "receiving", "done", "aborted", "error" };

	return( AnsiString( States[ State ]));
}
//---------------------------------------------------------------------------
AnsiString TDCCTransfer::GetProgress( void )
{
	float pos = ( Done > Size ) ? Size : Done, sz = Size;

    if( sz < 1. )
    	sz = 1.;

	return( FloatToStrF(( pos * 100. ) / sz, ffFixed, 20, 1 ) + "%" );
}
//---------------------------------------------------------------------
void TDCCTransfer::Update( void )
{
	if( Item ) {

	    Item->SubItems->Strings[ 1 ] = GetStatus();
    	Item->SubItems->Strings[ 2 ] = FloatToStrF( GetSize(), ffNumber, 20, 0 );
    	Item->SubItems->Strings[ 3 ] = GetProgress();
    	Item->SubItems->Strings[ 4 ] = CalcCPS();

        Item->Update();
    }
}
//---------------------------------------------------------------------------
bool TDCCTransfer::Offer( void )
{
	bool 		ret = false;
	TServerWin *server = MainWin->FindServerWin( Server );

    if( server ) {

    	if( !Sock )
		    CreateSocket();

     	ret  = Sock->IsValid() && Bind() && Sock->Listen();
        Port = Sock->GetLocalPort();

	    IsSend     = true;
    	FileHandle = CreateFile( File.c_str(), GENERIC_READ, FILE_SHARE_READ,
        						 NULL, OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
                                 NULL );

	    if( Size && ret && ( FileHandle != INVALID_HANDLE_VALUE )) {
    	    AnsiString	file3 = ExtractFileName( File );
    		char 	   *file2 = new char[ file3.Length() + 1 ];

	        for( int i = 0; i < file3.Length(); i++ ) {
    	     	char ch = file3[ i + 1 ];

        	    switch( ch ) {

            		case ' ':
                		file2[ i ] = '_';
                    	break;

	            	default:
    	            	file2[ i ] = ch;
        	            break;
            	}
	        }

    	    file2[ file3.Length() ] = '\0';

			server->DisplayOnActiveWin( NULL, "Offering \"" + AnsiString( file2 ) + "\" to " + Target, IRC_DCC );

	    	server->SendCmd( "PRIVMSG " + Target + " :\001DCC SEND " + AnsiString( file2 ) + " " +
    						 server->GetMyIPForDCC() + " " +
    						 IntToStr( Port ) + " " +
            	             IntToStr( Size ) + "\001" );

        	Reset();
		    Update();

    		delete[] file2;

	    } else
    		Error( "file is either empty or not accessible or a socket couldn't be created" );

    } else {

    	State = DCC_ERROR;

        Update();
    }

    return( ret );
}
//---------------------------------------------------------------------------
bool TDCCTransfer::Bind( void )
{
	bool ret;

    if( Prefs.LimitDCCPorts() ) {

    	ret = false;

        for( int i = Prefs.GetDCCPortFrom(); !ret && ( i <= Prefs.GetDCCPortTo() ); i++ )
        	ret = Sock->Bind( i );

    } else
		ret = Sock->Bind( 0 );

    return( ret );
}
//---------------------------------------------------------------------------
bool TDCCTransfer::Receive( Prom_Addr *addr, int port, int size )
{
	bool	ret = Sock->IsValid() && Sock->AsyncConnect( addr, port );

    Size = size;

    if( ret ) {

    	if( Size ) {

	    	State = DCC_CONNECTING;

		    Update();

        } else
        	Close();

    } else
    	Error( "cannot create a socket" );

    return( ret );
}
//---------------------------------------------------------------------
void TDCCTransfer::PrepareToResume( Prom_Addr *addr, int port, int size, unsigned int resumefrom )
{
	Addr       = *addr;
    Port       = port;
    Size       = size;
    ResumeFrom = resumefrom;
    Done       = resumefrom;
    State      = DCC_RESUMING;

    Update();
}
//---------------------------------------------------------------------
void TDCCTransfer::Resume( void )
{
	Receive( &Addr, Port, Size );
}
//---------------------------------------------------------------------
void TDCCTransfer::SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata )
{
	TDCCTransfer *dcc = (TDCCTransfer *)userdata;

    switch( reason ) {

    	case PROM_SOCK_ACCEPT:
        	dcc->Accept((TcpSocket *)data );
        	break;

    	case PROM_SOCK_READ:
        	if( data > 0 )
            	dcc->HandleIncomingData( data );
            else
            	dcc->Close();
        	break;

    	case PROM_SOCK_WRITTEN:
        	if( dcc->IsSend )
            	dcc->SendChunk();
            else if( dcc->Done >= dcc->Size )
            	dcc->Close();
        	break;

        case PROM_SOCK_CONNECTED:
        	if( data )
	        	dcc->Error( "cannot connect!" );
            else
	        	dcc->StartReceive();
            break;

        case PROM_SOCK_ERROR:
        	dcc->Error( "socket error!" );
            break;

        case PROM_SOCK_TIMEOUT:
			dcc->Error( "timeout." );
        	break;
    }
}
//---------------------------------------------------------------------
void TDCCTransfer::Accept( TcpSocket *socket )
{
	delete Sock;

    Sock = socket;

    if( Sock->IsValid() ) {

    	Sock->UseDispatcher( &MainWin->IO );
	   	Sock->SetAsyncCallback( SocketCB, this );

        SendChunk();

        State = DCC_SENDING;
        Done  = ResumeFrom;

        if( ResumeFrom )
	        SetFilePointer( FileHandle, ResumeFrom, NULL, FILE_BEGIN );

	    Sock->AsyncRecv( &Ack, sizeof( Ack ), 0, DCC_TIMEOUT_ACK );

        time( &StartTime );

        Update();

    } else
    	Error( "failed to accept an incoming connection" );
}
//---------------------------------------------------------------------
void TDCCTransfer::Error( AnsiString err )
{
	delete Sock;

    Sock  = NULL;
	State = DCC_ERROR;

    if( TServerWin *serv = MainWin->FindServerWin( Server )) {
		AnsiString	dir, verb;

        if( IsSend ) {
        	dir  = "to";
            verb = "send";
        } else {
        	dir  = "from";
            verb = "receive";
        }

   		serv->DisplayOnActiveWin( NULL,
        						  "DCC " + verb + " of \"" + File + "\" " +
        						  dir + " " + Target + " failed: " + err,
                                  IRC_ERROR );
    }

    Update();
	Prefs.GetSounds()->Play( SoundsMgr::DCC_FAIL );

    OrionScript::FireEvent( IsSend ? SE_SENDFAIL : SE_GETFAIL, this );
}
//---------------------------------------------------------------------
void TDCCTransfer::Close( bool abort )
{
	if( State != DCC_DONE ) {

        if( FileHandle != INVALID_HANDLE_VALUE ) {

            CloseHandle( FileHandle );

            FileHandle = INVALID_HANDLE_VALUE;
        }

        delete Sock;

        Sock  = NULL;
        State = DCC_DONE;

        if( TServerWin *serv = MainWin->FindServerWin( Server )) {
            AnsiString	dir, verb;

            if( IsSend ) {

                dir  = "to";
                verb = "send";

            } else {

                dir  = "from";
                verb = "receive";

                WriteComment();
            }

            if( abort )
                serv->DisplayOnActiveWin( NULL,
                                          "DCC " + verb + " of \"" + File + "\" " +
                                          dir + " " + Target + " aborted.",
                                          IRC_DCC );
            else {

                serv->DisplayOnActiveWin( NULL,
                                          "DCC " + verb + " of \"" + File + "\" " +
                                          dir + " " + Target + " complete (" + CalcCPS() + ")",
                                          IRC_DCC );

				Prefs.GetSounds()->Play( IsSend ? SoundsMgr::DCC_SEND_COMPLETE : SoundsMgr::DCC_RECV_COMPLETE );

			    OrionScript::FireEvent( IsSend ? SE_FILESENT : SE_FILERCVD, this );
            }
        }

        Update();
    }
}
//---------------------------------------------------------------------
AnsiString TDCCTransfer::CalcCPS( void )
{
	AnsiString	speed;
    float		spd;
    int			secs;

    secs = time( NULL ) - StartTime;

    if( secs < 1 )
    	secs = 1;

	spd = (float)( Done - ResumeFrom ) / (float)secs;

    if( spd > 1024 )
    	speed = FloatToStrF( spd / 1024.0, ffFixed, 20, 2 ) + " KB/s";
    else
    	speed = FloatToStrF( spd, ffFixed, 20, 0 ) + " cps";

    return( speed );
}
//---------------------------------------------------------------------
void TDCCTransfer::HandleIncomingData( int data )
{
	if( IsSend ) {

    	Done = ntohl( Ack );

        if( Done < Size )
	    	Sock->AsyncRecv( &Ack, sizeof( Ack ), 0, DCC_TIMEOUT_ACK );

    } else {
    	int		ack;
        DWORD   written;

    	Done += data;

        WriteFile( FileHandle, Buffer, data, &written, NULL );

        ack = htonl( Done );

        Sock->AsyncSend( &ack, sizeof( ack ));
    	Sock->AsyncRecv( Buffer, sizeof( Buffer ), 0, DCC_TIMEOUT );
    }

    if( IsSend && ( Done >= Size ))
		Close();
    else
	    Update();
}
//---------------------------------------------------------------------------
void TDCCTransfer::SendChunk( void )
{
	DWORD len;

    ReadFile( FileHandle, Buffer, sizeof( Buffer ), &len, NULL );

    if( len > 0 )
   		Sock->AsyncSend( Buffer, len );
}
//---------------------------------------------------------------------------
void TDCCTransfer::StartReceive( void )
{
   	FileHandle = CreateFile( File.c_str(),
    						 GENERIC_WRITE, FILE_SHARE_READ,
       						 NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                             NULL );

    if( FileHandle != INVALID_HANDLE_VALUE ) {

    	if( ResumeFrom )
        	SetFilePointer( FileHandle, ResumeFrom, NULL, FILE_BEGIN );
        else
	    	SetEndOfFile( FileHandle );

		State = DCC_RECVING;

	    Sock->AsyncRecv( Buffer, sizeof( Buffer ), 0, DCC_TIMEOUT );

    	time( &StartTime );

        Update();

    } else
    	Error( "cannot write to the file \"" + File + "\"!" );
}
//---------------------------------------------------------------------------
void TDCCTransfer::WriteComment( void )
{
    AnsiString	comment = "Received from " + Target + " on " + Server;
    wchar_t	   *name = new wchar_t[ File.WideCharBufSize() ];

    File.WideChar( name, File.WideCharBufSize() );

	FP_WriteProperties( name, Version.GetProductName().c_str(), comment.c_str() );

    delete[] name;
}
//---------------------------------------------------------------------------


