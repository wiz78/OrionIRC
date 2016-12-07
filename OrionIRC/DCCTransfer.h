/* $Id: DCCTransfer.h,v 1.9 2005/04/08 17:31:58 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DCCTransferH
#define DCCTransferH
//---------------------------------------------------------------------------

#include <time.h>

#include "tcpsocket.h"

typedef enum { DCC_WAITING = 0, DCC_RESUMING, DCC_CONNECTING, DCC_SENDING,
			   DCC_RECVING, DCC_DONE, DCC_ABORTED, DCC_ERROR } TDCCState;

class TServerWin;

class TDCCTransfer
{
public:

					TDCCTransfer( AnsiString server, AnsiString nick, AnsiString file );
                   ~TDCCTransfer();

	static int		GetFileSize( AnsiString file );

	AnsiString		GetServer( void )  { return( Server ); }
	AnsiString		GetTarget( void )  { return( Target ); }
	AnsiString		GetFile( void )    { return( File ); }
	int				GetSize( void )    { return( Size ); }
	TDCCState		GetState( void )   { return( State ); }
    int				GetPort( void )    { return( Port ); }
	AnsiString		GetStatus( void );
	AnsiString		GetProgress( void );

    void			SetListItem( TListItem *item ) { Item = item; }
    void			SetResumeFrom( int from )      { ResumeFrom = from; }

	bool 			Offer( void );
	bool			Receive( Prom_Addr *addr, int port, int size );
	void			PrepareToResume( Prom_Addr *addr, int port, int size, unsigned int resumefrom );
	void			Resume( void );
    void			Close( bool abort = false );

	void 			Update( void );

private:
	TcpSocket	   *Sock;
    Prom_Addr		Addr;
    int				Port;
	AnsiString		Target;
    AnsiString		File;
    AnsiString		Server;
    TDCCState		State;
    int				Size;
    int				Done;
    unsigned int	ResumeFrom;
    bool			IsSend;
    TListItem	   *Item;
    time_t			StartTime;
    char			Buffer[ 1024 * 16 ];
    int				Ack;
    HANDLE		    FileHandle;

    void			Reset( void );
    void			CreateSocket( void );
	bool 			Bind( void );
	static void		SocketCB( Socket *socket, Prom_SC_Reason reason, int data, void *userdata );
	void 			Accept( TcpSocket *socket );
    void			Error( AnsiString err );
	AnsiString 		CalcCPS( void );
	void 			HandleIncomingData( int data );
	void			SendChunk( void );
	void			StartReceive( void );
    void			WriteComment( void );
};

#endif
