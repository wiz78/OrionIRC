/* $Id: Prefs.h,v 1.40 2005/09/08 18:58:50 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef PrefsH
#define PrefsH
//---------------------------------------------------------------------------

#include "BitField.h"
#include "ColorSet.h"
#include "DblClickAction.h"
#include "FilterMgr.h"
#include "FileFilterMgr.h"
#include "FavouritesMgr.h"
#include "NotifyMgr.h"
#include "IgnoreMgr.h"
#include "ScriptsMgr.h"
#include "AliasesMgr.h"
#include "SoundsMgr.h"

typedef enum { DCC_ACCEPT, DCC_ASK, DCC_IGNORE } TDCCAction;
typedef enum { DCC_RECV_ASK, DCC_RECV_OVERWRITE, DCC_RECV_RESUME } TDCCRecvAction;
typedef enum { PAGE_NEVER, PAGE_ALWAYS, PAGE_AWAY } TPageWhen;
typedef enum { SORT_ALPHA, SORT_MODES } TNickSort;
typedef enum { TAB_SORT_NONE, TAB_SORT_SERVER, TAB_SORT_TYPE } TTabBarSort;
typedef enum { DBL_CLICK_NICK = 0, DBL_CLICK_CHANNEL, DBL_CLICK_QUERY, DBL_CLICK_SERVER,
    	   	   DBL_CLICK_COUNT } TDblClickAct;


#define PREFS_MINIMIZE_TO_TRAY			(1 << 0)
#define PREFS_FAVOURITES_ON_CONNECT		(1 << 1)
#define PREFS_SHOW_CHAN_MODES			(1 << 2)
#define PREFS_LOG_TIMESTAMP				(1 << 3)
#define PREFS_LOG_STRIP_CODES			(1 << 4)
#define PREFS_LOG_CHANNELS				(1 << 5)
#define PREFS_LOG_QUERIES				(1 << 6)
#define PREFS_LOG_SERVERS				(1 << 7)
#define PREFS_ADAPTIVE_NICK				(1 << 8)
#define PREFS_SHOW_TIMESTAMP			(1 << 9)
#define PREFS_URL_CATCHER				(1 << 10)
#define PREFS_OPEN_IN_NEW_BROWSER		(1 << 11)
#define PREFS_SAVE_URLS					(1 << 12)
#define PREFS_WHO_ON_JOIN				(1 << 13)
#define PREFS_REJOIN_ON_KICK			(1 << 14)
#define PREFS_CONFIRM_QUIT				(1 << 15)
#define PREFS_STRIP_COLORS				(1 << 16)
#define PREFS_MAXIMIZE_ON_START			(1 << 17)
#define PREFS_LIMIT_DCC_PORTS			(1 << 18)
#define PREFS_DISABLE_LAG_METER			(1 << 19)
#define PREFS_RESTORE_ON_PRIV_MSG		(1 << 20)
#define PREFS_FLASH_STATUS_BAR			(1 << 21)

class TPrefs
{
public:
    				TPrefs();
    		   	   ~TPrefs();

	void			Clear( void );

	TPrefs&			operator =( const TPrefs& rhs );

    void			Load( void );
    void			Save( void );

	void			LaunchURL( AnsiString url );

    TDCCAction		GetChatReqAction( void ) const 		{ return( DCCChatAction ); }
    TDCCAction		GetSendReqAction( void ) const 		{ return( DCCSendAction ); }
    TDCCAction		GetEyelinReqAction( void ) const 	{ return( DCCEyelinAction ); }
    TDCCRecvAction	GetRecvAction( void ) const 		{ return( DCCRecvAction ); }

    void			SetChatReqAction( TDCCAction act ) 		{ DCCChatAction = act; }
    void			SetSendReqAction( TDCCAction act ) 		{ DCCSendAction = act; }
    void			SetEyelinReqAction( TDCCAction act ) 	{ DCCEyelinAction = act; }
    void			SetRecvAction( TDCCRecvAction act )		{ DCCRecvAction = act; }

    DblClickAction&	GetDblClickAction( TDblClickAct act ) { return( DblClickActions[ act ] ); }

    int				GetWhoOnJoinMaxUsers( void )    { return( WhoOnJoinMaxUsers ); }
    void			SetWhoOnJoinMaxUsers( int set ) { WhoOnJoinMaxUsers = set; }

    int				GetNotifyInterval( void )    	{ return( NotifyInterval ); }
    void			SetNotifyInterval( int set ) 	{ NotifyInterval = set; }

    int				GetDCCPortFrom( void )			{ return( DCCPortFrom ); }
    int				GetDCCPortTo( void )			{ return( DCCPortTo ); }
    void			SetDCCPortFrom( int set )		{ DCCPortFrom = set; }
    void			SetDCCPortTo( int set )			{ DCCPortTo = set; }

    AnsiString		GetDownloadsDir( AnsiString name );
    AnsiString		GetLogsDir( void ) const			{ return( LogsDir ); }
    AnsiString		GetLogNameFormat( void ) const		{ return( LogNameFormat ); }
    AnsiString		GetLogName( AnsiString server, AnsiString target ) const;
    AnsiString		GetBrowser( void ) const			{ return( Browser ); }
    AnsiString		GetURLCatcherFile( void ) const		{ return( URLCatcherFile ); }
    AnsiString		GetPagerSound( void ) const			{ return( PagerSound ); }

    void			SetDownloadsDir( AnsiString str )  	{ DownloadsDir   = str; }
    void			SetLogsDir( AnsiString str )  		{ LogsDir        = str; }
    void			SetLogNameFormat( AnsiString str )  { LogNameFormat  = str; }
    void			SetBrowser( AnsiString str )  	    { Browser        = str; }
    void			SetURLCatcherFile( AnsiString str ) { URLCatcherFile = str; }
    void			SetPagerSound( AnsiString str ) 	{ PagerSound     = str; }

    TPageWhen		GetPageWhen( void ) const			{ return( PageWhen ); }
    void			SetPageWhen( TPageWhen set ) 		{ PageWhen = set; }

    TNickSort		GetNickSort( void ) const			{ return( NickSort ); }
    void			SetNickSort( TNickSort set )		{ NickSort = set; }

    TTabBarSort		GetTabBarSort( void ) const			{ return( TabBarSort ); }
    void			SetTabBarSort( TTabBarSort set )	{ TabBarSort = set; }

    TFont 	   	   *GetStatusFont( void )	 	{ return( StatusFont ); }
    TFont 	   	   *GetChannelFont( void )   	{ return( ChannelFont ); }
    TFont 	   	   *GetQueryFont( void )     	{ return( QueryFont ); }
    TFont 	   	   *GetTopicFont( void )     	{ return( TopicFont ); }
    TFont 	   	   *GetNicklistFont( void )  	{ return( NicklistFont ); }
    TFont 	   	   *GetInputFont( void )     	{ return( InputFont ); }

    ColorSet   	   *GetColorSet( void ) 	 	{ return( &Colors ); }
    FilterMgr  	   *GetFilters( void ) 		 	{ return( &Filters ); }
    FileFilterMgr  *GetFileFilters( void ) 		{ return( &FileFilters ); }
    FavouritesMgr  *GetFavourites( void ) 	   	{ return( &Favourites ); }
    NotifyMgr  	   *GetNotifies( void ) 	   	{ return( &Notify ); }
    IgnoreMgr  	   *GetIgnore( void ) 	   		{ return( &Ignore ); }
    ScriptsMgr     *GetScripts( void ) 	   		{ return( &Scripts ); }
    AliasesMgr     *GetAliases( void ) 	   		{ return( &Aliases ); }
    SoundsMgr      *GetSounds( void ) 	   		{ return( &Sounds ); }

    bool			MinimizeToTray( void )		{ return( Flags.IsSet( PREFS_MINIMIZE_TO_TRAY      )); }
    bool			PopupFavOnConnect( void )	{ return( Flags.IsSet( PREFS_FAVOURITES_ON_CONNECT )); }
    bool			ShowChanModes( void )		{ return( Flags.IsSet( PREFS_SHOW_CHAN_MODES       )); }
    bool			TimestampLogs( void )		{ return( Flags.IsSet( PREFS_LOG_TIMESTAMP         )); }
    bool			StripCodesFromLogs( void )	{ return( Flags.IsSet( PREFS_LOG_STRIP_CODES       )); }
    bool			LogChannels( void )			{ return( Flags.IsSet( PREFS_LOG_CHANNELS          )); }
    bool			LogQueries( void )			{ return( Flags.IsSet( PREFS_LOG_QUERIES           )); }
    bool			LogServers( void )			{ return( Flags.IsSet( PREFS_LOG_SERVERS           )); }
    bool			AdaptiveNick( void )		{ return( Flags.IsSet( PREFS_ADAPTIVE_NICK         )); }
    bool			ShowTimestamp( void )		{ return( Flags.IsSet( PREFS_SHOW_TIMESTAMP        )); }
    bool			CatchURLs( void )			{ return( Flags.IsSet( PREFS_URL_CATCHER           )); }
    bool			OpenNewBrowser( void )		{ return( Flags.IsSet( PREFS_OPEN_IN_NEW_BROWSER   )); }
    bool			SaveURLs( void )			{ return( Flags.IsSet( PREFS_SAVE_URLS             )); }
    bool			DoWhoOnJoin( void )			{ return( Flags.IsSet( PREFS_WHO_ON_JOIN		   )); }
    bool			RejoinOnKick( void )		{ return( Flags.IsSet( PREFS_REJOIN_ON_KICK        )); }
    bool			ConfirmQuit( void )			{ return( Flags.IsSet( PREFS_CONFIRM_QUIT		   )); }
    bool			StripColors( void )			{ return( Flags.IsSet( PREFS_STRIP_COLORS		   )); }
    bool			MaximizeOnStart( void )	 	{ return( Flags.IsSet( PREFS_MAXIMIZE_ON_START	   )); }
    bool			LimitDCCPorts( void )	 	{ return( Flags.IsSet( PREFS_LIMIT_DCC_PORTS	   )); }
    bool			UseLagMeter( void )	 		{ return( !Flags.IsSet( PREFS_DISABLE_LAG_METER	   )); }
    bool			RestoreOnPrivMsg( void )	{ return( Flags.IsSet( PREFS_RESTORE_ON_PRIV_MSG   )); }
    bool			FlashStatusBar( void )		{ return( Flags.IsSet( PREFS_FLASH_STATUS_BAR      )); }

    void			SetMinimizeToTray( bool set )		{ Flags.Set( PREFS_MINIMIZE_TO_TRAY,      set ); }
    void			SetPopupFavOnConnect( bool set )	{ Flags.Set( PREFS_FAVOURITES_ON_CONNECT, set ); }
    void			SetShowChanModes( bool set )		{ Flags.Set( PREFS_SHOW_CHAN_MODES,       set ); }
    void			SetTimestampLogs( bool set )	 	{ Flags.Set( PREFS_LOG_TIMESTAMP,         set ); }
    void			SetStripCodesFromLogs( bool set )	{ Flags.Set( PREFS_LOG_STRIP_CODES,       set ); }
    void			SetLogChannels( bool set )	 		{ Flags.Set( PREFS_LOG_CHANNELS,          set ); }
    void			SetLogQueries( bool set )	 		{ Flags.Set( PREFS_LOG_QUERIES,           set ); }
    void			SetLogServers( bool set )	 		{ Flags.Set( PREFS_LOG_SERVERS,           set ); }
    void			SetAdaptiveNick( bool set )	 		{ Flags.Set( PREFS_ADAPTIVE_NICK,         set ); }
    void			SetShowTimestamp( bool set )	   	{ Flags.Set( PREFS_SHOW_TIMESTAMP,        set ); }
    void			SetCatchURLs( bool set )	   		{ Flags.Set( PREFS_URL_CATCHER,    	      set ); }
    void			SetOpenNewBrowser( bool set )	  	{ Flags.Set( PREFS_OPEN_IN_NEW_BROWSER,   set ); }
    void			SetSaveURLs( bool set )	  			{ Flags.Set( PREFS_SAVE_URLS,             set ); }
    void			SetWhoOnJoin( bool set )			{ Flags.Set( PREFS_WHO_ON_JOIN,           set ); }
    void			SetRejoinOnKick( bool set )			{ Flags.Set( PREFS_REJOIN_ON_KICK,        set ); }
    void			SetConfirmQuit( bool set )			{ Flags.Set( PREFS_CONFIRM_QUIT, 		  set ); }
    void			SetStripColors( bool set )			{ Flags.Set( PREFS_STRIP_COLORS, 		  set ); }
    void			SetMaximizeOnStart( bool set )	 	{ Flags.Set( PREFS_MAXIMIZE_ON_START,	  set ); }
    void			SetLimitDCCPorts( bool set )	 	{ Flags.Set( PREFS_LIMIT_DCC_PORTS,	      set ); }
    void			SetUseLagMeter( bool set )	 		{ Flags.Set( PREFS_DISABLE_LAG_METER,	 !set ); }
    void			SetRestoreOnPrivMsg( bool set )		{ Flags.Set( PREFS_RESTORE_ON_PRIV_MSG,	  set ); }
    void			SetFlashStatusBar( bool set )		{ Flags.Set( PREFS_FLASH_STATUS_BAR,	  set ); }

	static void		RecursiveDeleteKey( AnsiString Key );

private:
    AnsiString		DownloadsDir;
    AnsiString		LogsDir;
    AnsiString		LogNameFormat;
    AnsiString		Browser;
    AnsiString		URLCatcherFile;
    AnsiString		PagerSound;
    TPageWhen		PageWhen;
    TNickSort		NickSort;
    TTabBarSort		TabBarSort;
    TDCCAction		DCCChatAction;
    TDCCAction		DCCSendAction;
    TDCCAction		DCCEyelinAction;
    TDCCRecvAction	DCCRecvAction;
    TFont	   	   *StatusFont;
    TFont	   	   *ChannelFont;
    TFont	   	   *QueryFont;
    TFont	   	   *NicklistFont;
    TFont	   	   *TopicFont;
    TFont	   	   *InputFont;
    ColorSet		Colors;
    FilterMgr		Filters;
    FileFilterMgr	FileFilters;
    FavouritesMgr   Favourites;
    NotifyMgr		Notify;
    IgnoreMgr		Ignore;
    ScriptsMgr		Scripts;
    AliasesMgr		Aliases;
    SoundsMgr		Sounds;
    BitField		Flags;
    int				WhoOnJoinMaxUsers;
    int				NotifyInterval;
    int				DCCPortFrom;
    int				DCCPortTo;
    DblClickAction	DblClickActions[ DBL_CLICK_COUNT ];

	void 			LoadGeneralOptions( TRegistry *reg );
	void 			LoadDCCOptions( TRegistry *reg );
	void 			LoadPagerOptions( TRegistry *reg );
	void 			LoadFontOptions( TRegistry *reg );
	void 			LoadFont( TRegistry *reg, AnsiString name, TFont *font );

	void 			SaveGeneralOptions( TRegistry *reg );
	void 			SaveDCCOptions( TRegistry *reg );
	void 			SavePagerOptions( TRegistry *reg );
	void 			SaveFontOptions( TRegistry *reg );
	void 			SaveFont( TRegistry *reg, AnsiString name, TFont *font );

    void			GetDefaultBrowser( void );
};

extern TPrefs Prefs;

#endif
