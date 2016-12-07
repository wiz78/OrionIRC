/* $Id: Server.h,v 1.15 2005/04/19 18:19:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ServerH
#define ServerH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "bitfield.h"

class TServer
{
public:
	enum { SHOW_CHANNEL, SHOW_STATUS, HIDE };
    enum { EV_JOIN, EV_PART, EV_QUIT, EV_KICK, EV_NICK,
           EV_INVITE, EV_NOTICE, EV_NOTIFY,
    	   EV_COUNT };

	AnsiString		Descr;
    AnsiString		Host;
    AnsiString		Port;
    AnsiString		Password;
    AnsiString		Username;
    AnsiString		Realname;
    BitField		Flags;
    int				ReconnectDelay;
    enum {
    	RECONNECT,
        CONNECT_NEXT,
        NOTHING
    } 				FailureAction;
	AnsiString		PartReason;
	AnsiString		QuitReason;
	AnsiString		FingerMsg;
	AnsiString		AwayMsg;
	AnsiString		PartFile;
	AnsiString		QuitFile;
	AnsiString		FingerFile;
	AnsiString		AwayFile;
    AnsiString		AwayNick;

    				TServer();
    				TServer( const TServer *copy );
                   ~TServer();

	TServer 	   *GetChild( int i ) const;
    void			AddChild( TServer *serv );
	void			ClearChildrenList( void );
	void			ClearChildren( void );

    TServer		   *GetParent( void ) const    { return( Parent ); }
    void			SetParent( TServer *serv ) { Parent = serv; }

    void			InsertIntoTree( TTreeView *tree, TTreeNode *parent = NULL );

    void			SetNicks( AnsiString nicks );
	int				GetNicksCount( void );
    void			AddNick( AnsiString nick );
    AnsiString		GetNick( int num );
    AnsiString		GetNicks( void );

    void			SetChannels( TStrings *chans );
    void			ClearChannels( void );
	int				GetChannelsCount( void );
    AnsiString		GetChannel( int num );

    void			SetCommandsOnConnect( TStrings *chans );
    void			ClearCommandsOnConnect( void );
	int				GetCommandsOnConnectCount( void );
    AnsiString		GetCommandOnConnect( int num );

    AnsiString		GetPartReason( void );
    AnsiString		GetQuitReason( void );
    AnsiString		GetFingerMsg( void );
    AnsiString		GetAwayMsg( void );
    void			SetPartReason( AnsiString str ) 	{ PartReason = str; }
    void			SetQuitReason( AnsiString str ) 	{ QuitReason = str; }
    void			SetFingerMsg( AnsiString str )  	{ FingerMsg = str; }
    void			SetAwayMsg( AnsiString str )    	{ AwayMsg = str; }
    void			SetPartFile( AnsiString str )   	{ PartFile = str; }
    void			SetQuitFile( AnsiString str )   	{ QuitFile = str; }
    void			SetFingerFile( AnsiString str ) 	{ FingerFile = str; }
    void			SetAwayFile( AnsiString str )   	{ AwayFile = str; }

    inline int		GetEventDisplay( int ev ) const		{ return( EventDisplay[ ev ] ); }
    inline void		SetEventDisplay( int ev, int disp ) { EventDisplay[ ev ] = disp; }

    inline bool		CanConnect( void ) const 			{ return( !Host.IsEmpty() ); }

    TServer&		operator =( const TServer& rhs );

    void			PropagateSettings( void );

    void			Load( TRegistry *reg, AnsiString key );
    void			Save( TRegistry *reg, AnsiString key );

    void			AutoConnect( void );

    void			FindNext( void );

private:
	TList		   *Children;
    TServer		   *Parent;
    TStringList	   *Nicks;
    TStringList	   *Channels;
    TStringList	   *ConnectCmds;
    int				EventDisplay[ EV_COUNT ];

	void			Init( void );

	void			CopyUserStuff( const TServer& rhs );
	void			CopyConnectionStuff( const TServer& rhs );
	void			CopyOnConnectStuff( const TServer& rhs );
	void			CopyMessagesStuff( const TServer& rhs );
	void 			CopyEventDisplayStuff( const TServer& rhs );

	void 			LoadChannels( TRegistry *reg, AnsiString key );
	void 			LoadMessages( TRegistry *reg, AnsiString key );
	void 			LoadCommands( TRegistry *reg, AnsiString key );
	void 			LoadEventDisplay( TRegistry *reg, AnsiString key );

	void 			SaveChannels( TRegistry *reg, AnsiString key );
	void 			SaveMessages( TRegistry *reg, AnsiString key );
	void 			SaveCommands( TRegistry *reg, AnsiString key );
	void 			SaveEventDisplay( TRegistry *reg, AnsiString key );

    AnsiString		GetRandomLine( AnsiString file );

    TServer 	   *FindNext( TServer& serv );
};

#define SERVF_INHERIT_USER			(1 << 0)
#define SERVF_EXPANDED				(1 << 1)
#define SERVF_INHERIT_CONNECTION	(1 << 2)
#define SERVF_AUTO_CONNECT			(1 << 3)
#define SERVF_SHOW_MOTD				(1 << 4)
#define SERVF_INVISIBLE				(1 << 5)
#define SERVF_WALLOPS				(1 << 6)
#define SERVF_MSG_FROM_FILE_PART	(1 << 7)
#define SERVF_MSG_FROM_FILE_QUIT	(1 << 8)
#define SERVF_MSG_FROM_FILE_FINGER	(1 << 9)
#define SERVF_MSG_FROM_FILE_AWAY	(1 << 10)
#define SERVF_INHERIT_MESSAGES		(1 << 11)
#define SERVF_SET_AWAY_NICK			(1 << 12)
#define SERVF_INHERIT_ON_CONNECT	(1 << 13)
#define SERVF_INHERIT_EVENT_DISPLAY (1 << 14)

#define SERVF_MESSAGES_MASK			( SERVF_MSG_FROM_FILE_PART | SERVF_MSG_FROM_FILE_QUIT |\
    			 					  SERVF_MSG_FROM_FILE_FINGER | SERVF_MSG_FROM_FILE_AWAY |\
                 					  SERVF_SET_AWAY_NICK )

#endif
