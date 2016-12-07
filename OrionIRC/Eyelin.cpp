/* $Id: Eyelin.cpp,v 1.9 2005/04/15 21:44:21 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Eyelin.h"
#include "Prefs.h"
#include "ServerWnd.h"
#include "DCCChatOfferWnd.h"
#include "irc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

static Eyelin *Instance = NULL;

//---------------------------------------------------------------------------
Eyelin::Eyelin()
{
    Instance  = this;
	Handle    = NULL;
    Installed = false;
    Lib		  = LoadLibrary( "EyelinAPI.dll" );

	if( Lib ) {

    	(FARPROC)_Create      = GetProcAddress( Lib, "Eyelin_Create" );
		(FARPROC)_Delete      = GetProcAddress( Lib, "Eyelin_Delete" );
		(FARPROC)_Start       = GetProcAddress( Lib, "Eyelin_Start" );
		(FARPROC)_IsRunning   = GetProcAddress( Lib, "Eyelin_IsRunning" );
		(FARPROC)_IsConnected = GetProcAddress( Lib, "Eyelin_IsConnected" );
		(FARPROC)_IsFull      = GetProcAddress( Lib, "Eyelin_IsFull" );
		(FARPROC)_Invite      = GetProcAddress( Lib, "Eyelin_Invite" );
    	(FARPROC)_Connect     = GetProcAddress( Lib, "Eyelin_Connect" );
    	(FARPROC)_GetPort     = GetProcAddress( Lib, "Eyelin_GetPort" );
    	(FARPROC)_Show        = GetProcAddress( Lib, "Eyelin_Show" );
    	(FARPROC)_SetNick     = GetProcAddress( Lib, "Eyelin_SetNick" );

        if( _Create && _Delete && _Start && _IsRunning && _IsConnected &&
            _IsFull && _Invite && _Connect && _GetPort && _Show && _SetNick ) {

            Installed = true;
	    	Handle    = _Create();
        }
    }
}
//---------------------------------------------------------------------------
Eyelin::~Eyelin()
{
	if( Handle )
    	_Delete( Handle );

    if( Lib )
	    FreeLibrary( Lib );
}
//---------------------------------------------------------------------------
Eyelin *Eyelin::GetInstance( void )
{
	return( Instance );
}
//---------------------------------------------------------------------------
bool Eyelin::Start( AnsiString nick )
{
	bool ret = false;

    if( IsInstalled() ) {

    	ret = _IsRunning( Handle );

		if( !ret )
    		ret = _Start( Handle );
    }

    if( ret )
    	_SetNick( Handle, nick.c_str() );

	return( ret );
}
//---------------------------------------------------------------------------
bool Eyelin::Invite( AnsiString host )
{
	bool ret = false;

    if( IsInstalled() )
		ret = _Invite( Handle, host.c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
bool Eyelin::Connect( AnsiString host )
{
	bool ret = false;

    if( IsInstalled() )
		ret = _Connect( Handle, host.c_str() );

    return( ret );
}
//---------------------------------------------------------------------------
bool Eyelin::IsConnected( void )
{
	bool ret = false;

    if( IsInstalled() )
		ret = _IsConnected( Handle );

    return( ret );
}
//---------------------------------------------------------------------------
bool Eyelin::IsFull( void )
{
	bool ret = false;

    if( IsInstalled() )
		ret = _IsFull( Handle );

    return( ret );
}
//---------------------------------------------------------------------------
bool Eyelin::IsRunning( void )
{
	bool ret = false;

    if( IsInstalled() )
		ret = _IsRunning( Handle );

    return( ret );
}
//---------------------------------------------------------------------------
int Eyelin::GetPort( void )
{
	int ret = -1;

    if( IsInstalled() )
		ret = _GetPort( Handle );

    return( ret );
}
//---------------------------------------------------------------------------
void Eyelin::HandleCTCP( TServerWin *server, HostMask& mask, AnsiString ctcp )
{
	try {

        if( ctcp == "R" )
			HandleCTCPRequest( server, mask );
        else if( ctcp == "A" ) {
		    server->DisplayOnActiveWin( NULL, mask.GetNick() + " has accepted your invite on Eyelin", IRC_DCC );
            HandleCTCPAccept( server, mask.GetNick() );
        } else if( ctcp == "C" )
		    server->DisplayOnActiveWin( NULL, mask.GetNick() + " has accepted your invite on Eyelin", IRC_DCC );
    }
    catch( const char *err ) {
        server->DisplayOnActiveWin( NULL, err, IRC_ERROR );
    }
}
//---------------------------------------------------------------------------
void Eyelin::HandleCTCPRequest( TServerWin *server, HostMask& mask )
{
	AnsiString tmp;

    if( !Prefs.GetIgnore()->Ignore( mask, tmp, IGN_DCC_VIDEO ))
        switch( Prefs.GetEyelinReqAction() ) {

            case DCC_ACCEPT:
                AcceptDCC( server, mask.GetNick() );
                break;

            case DCC_ASK:
				Prefs.GetSounds()->Play( SoundsMgr::DCC_OFFER );
                new TDCCChatOfferWin( server, TDCCChatOfferWin::EYELIN,
                                      NULL, 0, mask )->Show();
                break;
        }
}
//---------------------------------------------------------------------------
void Eyelin::HandleCTCPAccept( TServerWin *server, AnsiString nick )
{
    if( !IsInstalled() )
        throw "Eyelin doesn't seem to be properly installed on this machine.";

    if( !Start( server->Session.Nick ))
        throw "Cannot launch Eyelin!";

    server->SendCmd( "PRIVMSG " + nick + " :\001DCC CHAT EYELIN_R " +
                     server->GetMyIPForDCC() + " " +
                     IntToStr( GetPort() ) + "\001" );
}
//---------------------------------------------------------------------------
void Eyelin::RequestDCC( TServerWin *server, AnsiString nick )
{
    server->SendCmd( "PRIVMSG " + nick + " :\001EYELIN R\001" );
}
//---------------------------------------------------------------------------
void Eyelin::AcceptDCC( TServerWin *server, AnsiString nick )
{
	AnsiString reply = IsConnected() ? "C" : "A";

    server->SendCmd( "PRIVMSG " + nick + " :\001EYELIN " + reply + "\001" );

    if( reply == "C" )
    	HandleCTCPAccept( server, nick );
}
//---------------------------------------------------------------------------
bool Eyelin::HandleDCC( TServerWin *server, AnsiString flag, AnsiString nick, Prom_Addr *addr )
{
	bool malformed = true;

    try {

        if( !flag.AnsiCompareIC( "EYELIN_R" )) {

            malformed = false;

        	SendDCCAck( server, nick, addr );

        } else if( !flag.AnsiCompareIC( "EYELIN_A" )) {

            malformed = false;

        	HandleDCCAccept( server, nick, addr );
        }
    }
    catch( const char *err ) {
        server->DisplayOnActiveWin( NULL, err, IRC_ERROR );
    }
    catch( AnsiString err ) {
        server->DisplayOnActiveWin( NULL, err, IRC_ERROR );
    }

    return( malformed );
}
//---------------------------------------------------------------------------
void Eyelin::SendDCCAck( TServerWin *server, AnsiString nick, Prom_Addr *addr )
{
    char *name;

    if( !Start( server->Session.Nick ))
        throw "Cannot launch Eyelin!";

    server->SendCmd( "PRIVMSG " + nick + " :\001DCC CHAT EYELIN_A " +
                     server->GetMyIPForDCC() + " " +
                     IntToStr( GetPort() ) + "\001" );

    name = TcpSocket::AddrToName( addr );

    Connect( name );
    free( name );

    _Show( Handle );
}
//---------------------------------------------------------------------------
void Eyelin::HandleDCCAccept( TServerWin *server, AnsiString nick, Prom_Addr *addr )
{
   	char 	   *name = TcpSocket::AddrToName( addr );
    AnsiString 	ex;

	try {

		if( !IsRunning() )
        	throw AnsiString( "Eyelin has been stopped!" );

        if( !Invite( name )) {
        	AnsiString err = "Eyelin: cannot add ";

            err += nick;
            err += " among the allowed clients: ";

            if( IsFull() )
            	err += "you have invited too many people.";
            else
            	err += "an unexpected error occurred.";

            throw err;
        }

        _Show( Handle );
    }
    catch( AnsiString err ) {
        ex = err;
    }

	free( name );

    if( !ex.IsEmpty() )
    	throw ex;
}
//---------------------------------------------------------------------------

