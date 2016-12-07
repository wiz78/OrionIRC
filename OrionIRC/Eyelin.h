/* $Id: Eyelin.h,v 1.7 2005/01/09 17:37:53 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef EyelinH
#define EyelinH
//---------------------------------------------------------------------------

#include <EyelinAPI.h>

#include "tcpsocket.h"
#include "Hostmask.h"

class TServerWin;

class Eyelin
{
public:
								Eyelin();
                               ~Eyelin();

	static Eyelin 		   	   *GetInstance( void );

    inline bool					IsInstalled( void ) const { return( Installed && Handle ); }

    void						AcceptDCC( TServerWin *server, AnsiString nick );
    void						RequestDCC( TServerWin *server, AnsiString nick );

    void						HandleCTCP( TServerWin *server, HostMask& mask, AnsiString ctcp );
    bool						HandleDCC( TServerWin *server, AnsiString flag, AnsiString nick, Prom_Addr *addr );

private:
	EyelinHandle				Handle;
    HINSTANCE					Lib;
    bool						Installed;

	bool						Start( AnsiString nick );
    bool						Invite( AnsiString host );
    bool						Connect( AnsiString host );
    bool						IsConnected( void );
    bool						IsFull( void );
    bool						IsRunning( void );

    int							GetPort( void );

	void 						HandleCTCPRequest( TServerWin *server, HostMask& mask );
	void 						HandleCTCPAccept( TServerWin *server, AnsiString nick );

	void 						SendDCCAck( TServerWin *server, AnsiString nick, Prom_Addr *addr );
	void 						HandleDCCAccept( TServerWin *server, AnsiString nick, Prom_Addr *addr );

    EyelinHandle EYELINAPI_SPEC	( *_Create )( void );
	void EYELINAPI_SPEC		   	( *_Delete )( EyelinHandle handle );
	EyelinBool EYELINAPI_SPEC	( *_Start )( EyelinHandle handle );
	EyelinBool EYELINAPI_SPEC	( *_IsRunning )( EyelinHandle handle );
	EyelinBool EYELINAPI_SPEC	( *_IsConnected )( EyelinHandle handle );
	EyelinBool EYELINAPI_SPEC	( *_IsFull )( EyelinHandle handle );
	EyelinBool EYELINAPI_SPEC	( *_Invite )( EyelinHandle handle, const char *host );
    EyelinBool EYELINAPI_SPEC	( *_Connect )( EyelinHandle handle, const char *host );
    int EYELINAPI_SPEC			( *_GetPort )( EyelinHandle handle );
    EyelinBool EYELINAPI_SPEC	( *_Show )( EyelinHandle handle );
    EyelinBool EYELINAPI_SPEC	( *_SetNick )( EyelinHandle handle, const char *nick );
};

#endif
 