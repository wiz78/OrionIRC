/* $Id: SoundsMgr.h,v 1.2 2005/05/26 15:06:08 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef SoundsMgrH
#define SoundsMgrH
//---------------------------------------------------------------------------

#include <registry.hpp>

class SoundsMgr
{
public:
	typedef enum {
    	INVITE, NOTICE, KICK, DISCONNECT, QUERY,
    	DCC_OFFER, DCC_FAIL, DCC_SEND_COMPLETE, DCC_RECV_COMPLETE,
        NOTIFY,
        COUNT }	EventType;

					SoundsMgr();

	SoundsMgr&		operator =( const SoundsMgr& rhs );

    void			Clear( void );

    void			Play( EventType ev ) const;

    AnsiString		GetSound( EventType ev ) const;
    bool			IsSoundEnabled( EventType ev ) const;
    void			SetSound( EventType ev, AnsiString file, bool enabled );

    void			Save( TRegistry *reg, AnsiString key ) const;
    void			Load( TRegistry *reg, AnsiString key );

private:
	AnsiString		Files[ COUNT ];
    bool			Enabled[ COUNT ];
};

#endif
