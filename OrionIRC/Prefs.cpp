/* $Id: Prefs.cpp,v 1.38 2005/09/08 18:58:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "Prefs.h"
#include "Filter.h"
#include "FileFilter.h"
#include "Favourite.h"
#include "Notify.h"
#include "ScriptsMgr.h"
#include "Version.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TPrefs	Prefs;

#define PREFS_DEFAULT	( PREFS_MINIMIZE_TO_TRAY | PREFS_SHOW_CHAN_MODES | \
						  PREFS_LOG_TIMESTAMP | PREFS_LOG_STRIP_CODES | \
                          PREFS_ADAPTIVE_NICK | PREFS_SHOW_TIMESTAMP | \
                          PREFS_URL_CATCHER | PREFS_SAVE_URLS | \
                          PREFS_WHO_ON_JOIN | PREFS_REJOIN_ON_KICK | \
                          PREFS_CONFIRM_QUIT )

//---------------------------------------------------------------------------
TPrefs::TPrefs()
{
	Flags.Set( PREFS_DEFAULT );

	DCCChatAction     = DCC_ASK;
    DCCSendAction     = DCC_ASK;
    DCCRecvAction     = DCC_RECV_ASK;
	DCCEyelinAction   = DCC_ASK;
    DCCPortFrom	      = 1024;
    DCCPortTo		  = 65535;
    WhoOnJoinMaxUsers = 150;
    NotifyInterval	  = 30;
    PageWhen          = PAGE_NEVER;
    NickSort		  = SORT_ALPHA;
    TabBarSort		  = TAB_SORT_SERVER;
    LogNameFormat	  = "%s_%t.txt";

    StatusFont   = new TFont();
    ChannelFont  = new TFont();
    QueryFont    = new TFont();
    NicklistFont = new TFont();
    TopicFont    = new TFont();
    InputFont    = new TFont();

    StatusFont->Name = "MS Sans Serif";
    StatusFont->Size = 10;

    ChannelFont->Assign( StatusFont );
    QueryFont->Assign( StatusFont );
    TopicFont->Assign( StatusFont );
    InputFont->Assign( StatusFont );
    NicklistFont->Assign( StatusFont );
}
//---------------------------------------------------------------------------
TPrefs::~TPrefs()
{
	Clear();

    delete StatusFont;
    delete ChannelFont;
    delete QueryFont;
    delete NicklistFont;
    delete TopicFont;
    delete InputFont;
}
//---------------------------------------------------------------------------
void TPrefs::Clear( void )
{
	Filters.Clear();
	FileFilters.Clear();
    Favourites.Clear();
    Flags.Clear();
    Notify.Clear();
    Ignore.Clear();
    Scripts.Clear();
    Aliases.Clear();
    Sounds.Clear();
}
//---------------------------------------------------------------------------
TPrefs&	TPrefs::operator =( const TPrefs& rhs )
{
	Clear();

    Flags             = rhs.Flags;
    DownloadsDir      = rhs.DownloadsDir;
    LogsDir           = rhs.LogsDir;
    Browser           = rhs.Browser;
	DCCChatAction     = rhs.DCCChatAction;
    DCCSendAction     = rhs.DCCSendAction;
    DCCRecvAction     = rhs.DCCRecvAction;
    WhoOnJoinMaxUsers = rhs.WhoOnJoinMaxUsers;
    URLCatcherFile    = rhs.URLCatcherFile;
    PagerSound        = rhs.PagerSound;
    PageWhen          = rhs.PageWhen;
    NickSort		  = rhs.NickSort;
    TabBarSort		  = rhs.TabBarSort;

    StatusFont->Assign( rhs.StatusFont );
    ChannelFont->Assign( rhs.ChannelFont );
    QueryFont->Assign( rhs.QueryFont );
    TopicFont->Assign( rhs.TopicFont );
    InputFont->Assign( rhs.InputFont );
    NicklistFont->Assign( rhs.NicklistFont );

    Colors = rhs.Colors;

    for( int i = 0; i < rhs.Filters.GetCount(); i++ )
        Filters.Add( new TFilter( rhs.Filters.Get( i )));

    for( int i = 0; i < rhs.FileFilters.GetCount(); i++ )
        FileFilters.Add( new TFileFilter( rhs.FileFilters.Get( i )));

    for( int i = 0; i < rhs.Favourites.GetCount(); i++ )
        Favourites.Add( new TFavourite( rhs.Favourites.Get( i )));

    for( int i = 0; i < rhs.Notify.GetCount(); i++ )
        Notify.Add( new TNotify( rhs.Notify.Get( i )));

    for( int i = 0; i < DBL_CLICK_COUNT; i++ )
    	DblClickActions[ i ] = rhs.DblClickActions[ i ];

    Ignore  = rhs.Ignore;
    Scripts = rhs.Scripts;
    Aliases = rhs.Aliases;
    Sounds  = rhs.Sounds;

	return( *this );
}
//---------------------------------------------------------------------------
void TPrefs::Load( void )
{
	TRegistry	*reg = new TRegistry();

    Clear();

	LoadGeneralOptions( reg );
    LoadDCCOptions( reg );
    LoadPagerOptions( reg );
    LoadFontOptions( reg );

    if( URLCatcherFile.IsEmpty() )
    	URLCatcherFile = ExtractFilePath( ParamStr( 0 )) + "Data\\Urls.ini";

    Colors.Load( reg, "Software\\WizSoftware\\Orion\\Colors" );
    Filters.Load( reg, "Software\\WizSoftware\\Orion\\Filters" );
    FileFilters.Load( reg, "Software\\WizSoftware\\Orion\\DCC\\Filters" );
    Favourites.Load( reg, "Software\\WizSoftware\\Orion\\Favourites" );
    Notify.Load( reg, "Software\\WizSoftware\\Orion\\Notify" );
    Ignore.Load( reg, "Software\\WizSoftware\\Orion\\Ignore" );
    Scripts.Load( reg, "Software\\WizSoftware\\Orion\\Scripts" );
    Aliases.Load( reg, "Software\\WizSoftware\\Orion\\Aliases" );
    Sounds.Load( reg, "Software\\WizSoftware\\Orion\\Sounds" );

    DblClickActions[ DBL_CLICK_NICK    ].Load( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\NickList" );
    DblClickActions[ DBL_CLICK_QUERY   ].Load( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Query" );
    DblClickActions[ DBL_CLICK_CHANNEL ].Load( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Channel" );
    DblClickActions[ DBL_CLICK_SERVER  ].Load( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Server" );

    delete reg;
}
//---------------------------------------------------------------------------
void TPrefs::LoadGeneralOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion", false )) {

    	try{
        	Flags.Set( reg->ReadInteger( "Flags" ));
        }
        catch(...) {
        	Flags.Set( PREFS_DEFAULT );
		}

        try{ WhoOnJoinMaxUsers = reg->ReadInteger( "WhoOnJoinMaxUsers" ); } catch(...) {}
        try{ NotifyInterval    = reg->ReadInteger( "NotifyInterval" ); } catch(...) {}
        try{ NickSort          = static_cast<TNickSort>( reg->ReadInteger( "NickSort" )); } catch(...) {}
        try{ TabBarSort        = static_cast<TTabBarSort>( reg->ReadInteger( "TabBarSort" )); } catch(...) {}

        LogsDir        = reg->ReadString( "LogsDir" );
        LogNameFormat  = reg->ReadString( "LogNameFormat" );
        Browser        = reg->ReadString( "Browser" );
        URLCatcherFile = reg->ReadString( "URLCatcherFile" );

        reg->CloseKey();

    } else
		Flags.Set( PREFS_DEFAULT );

    if( DownloadsDir.IsEmpty() )
	    DownloadsDir = "Downloads";

    if( Browser.IsEmpty() )
    	GetDefaultBrowser();

    if( LogsDir.IsEmpty() )
	    LogsDir = "Logs";

    if( LogNameFormat.IsEmpty() )
	    LogNameFormat = "%s_%t.txt";
}
//---------------------------------------------------------------------------
void TPrefs::LoadDCCOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\DCC", false )) {

    	try{ DCCChatAction   = (TDCCAction)reg->ReadInteger( "OnChatRequest" ); } catch(...) {}
    	try{ DCCSendAction   = (TDCCAction)reg->ReadInteger( "OnSendRequest" ); } catch(...) {}
    	try{ DCCEyelinAction = (TDCCAction)reg->ReadInteger( "OnEyelinRequest" ); } catch(...) {}
    	try{ DCCRecvAction   = (TDCCRecvAction)reg->ReadInteger( "IfExists" ); } catch(...) {}
    	try{ DCCPortFrom     = reg->ReadInteger( "PortRangeFrom" ); } catch(...) {}
    	try{ DCCPortTo       = reg->ReadInteger( "PortRangeTo" ); } catch(...) {}

        DownloadsDir = reg->ReadString( "DownloadsDir" );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::LoadPagerOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\Pager", false )) {

        PagerSound = reg->ReadString( "PagerSound" );

        try{ PageWhen = (TPageWhen)reg->ReadInteger( "PageWhen" ); } catch(...) {}

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::LoadFontOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\Fonts", false )) {

    	LoadFont( reg, "Status",   StatusFont   );
    	LoadFont( reg, "Channel",  ChannelFont  );
    	LoadFont( reg, "Query",    QueryFont    );
    	LoadFont( reg, "Nicklist", NicklistFont );
    	LoadFont( reg, "Topic",    TopicFont    );
    	LoadFont( reg, "Input",    InputFont    );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::Save( void )
{
	TRegistry	*reg = new TRegistry();

	SaveGeneralOptions( reg );
    SaveDCCOptions( reg );
    SavePagerOptions( reg );
    SaveFontOptions( reg );

    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Filters" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\DCC\\Filters" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Colors" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Favourites" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Notify" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Ignore" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Scripts" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Aliases" );
    RecursiveDeleteKey( "Software\\WizSoftware\\Orion\\Sounds" );

    Filters.Save( reg, "Software\\WizSoftware\\Orion\\Filters" );
    FileFilters.Save( reg, "Software\\WizSoftware\\Orion\\DCC\\Filters" );
    Colors.Save( reg, "Software\\WizSoftware\\Orion\\Colors" );
    Favourites.Save( reg, "Software\\WizSoftware\\Orion\\Favourites" );
    Notify.Save( reg, "Software\\WizSoftware\\Orion\\Notify" );
    Ignore.Save( reg, "Software\\WizSoftware\\Orion\\Ignore" );
    Scripts.Save( reg, "Software\\WizSoftware\\Orion\\Scripts" );
    Aliases.Save( reg, "Software\\WizSoftware\\Orion\\Aliases" );
    Sounds.Save( reg, "Software\\WizSoftware\\Orion\\Sounds" );

    DblClickActions[ DBL_CLICK_NICK    ].Save( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\NickList" );
    DblClickActions[ DBL_CLICK_QUERY   ].Save( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Query" );
    DblClickActions[ DBL_CLICK_CHANNEL ].Save( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Channel" );
    DblClickActions[ DBL_CLICK_SERVER  ].Save( reg, "Software\\WizSoftware\\Orion\\DoubleClickActions\\Server" );

    delete reg;
}
//---------------------------------------------------------------------------
void TPrefs::SaveGeneralOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion", true )) {

    	try {
        	reg->WriteInteger( "Flags", 			Flags.Get()       );
            reg->WriteInteger( "WhoOnJoinMaxUsers", WhoOnJoinMaxUsers );
            reg->WriteInteger( "NotifyInterval", 	NotifyInterval    );
            reg->WriteInteger( "NickSort",			NickSort          );
            reg->WriteInteger( "TabBarSort",		TabBarSort        );

            reg->WriteString( "LogsDir",        LogsDir        );
            reg->WriteString( "LogNameFormat",	LogNameFormat  );
            reg->WriteString( "Browser",        Browser        );
        	reg->WriteString( "URLCatcherFile", URLCatcherFile );
        }
        catch(...) {
        }

        reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::SaveDCCOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\DCC", true )) {

    	try {

        	reg->WriteInteger( "OnChatRequest",   DCCChatAction );
    		reg->WriteInteger( "OnSendRequest",   DCCSendAction );
    		reg->WriteInteger( "OnEyelinRequest", DCCEyelinAction );
    		reg->WriteInteger( "IfExists", 		  DCCRecvAction );
    		reg->WriteInteger( "PortRangeFrom",   DCCPortFrom );
    		reg->WriteInteger( "PortRangeTo",	  DCCPortTo );

            reg->WriteString( "DownloadsDir", DownloadsDir );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::SavePagerOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\Pager", true )) {

    	try {

        	reg->WriteString( "PagerSound", PagerSound );

        	reg->WriteInteger( "PageWhen", PageWhen );
        }
        catch(...) {
        }

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::SaveFontOptions( TRegistry *reg )
{
    if( reg->OpenKey( "Software\\WizSoftware\\Orion\\Fonts", true )) {

    	SaveFont( reg, "Status",   StatusFont   );
    	SaveFont( reg, "Channel",  ChannelFont  );
    	SaveFont( reg, "Query",    QueryFont    );
    	SaveFont( reg, "Nicklist", NicklistFont );
    	SaveFont( reg, "Topic",    TopicFont    );
    	SaveFont( reg, "Input",    InputFont    );

    	reg->CloseKey();
    }
}
//---------------------------------------------------------------------------
void TPrefs::LoadFont( TRegistry *reg, AnsiString name, TFont *font )
{
	try {
		font->Name = reg->ReadString( name + "Name" );
		font->Size = reg->ReadInteger( name + "Size" );
    }
    catch(...) {
    }
}
//---------------------------------------------------------------------------
void TPrefs::SaveFont( TRegistry *reg, AnsiString name, TFont *font )
{
	try {
		reg->WriteString( name + "Name", font->Name );
		reg->WriteInteger( name + "Size", font->Size );
    }
    catch(...) {
    }
}
//---------------------------------------------------------------------------
AnsiString TPrefs::GetDownloadsDir( AnsiString name )
{
	AnsiString	ret = DownloadsDir;

    if( !name.IsEmpty() ) {
        TFileFilter	*filter;

        filter = FileFilters.Find( name );

        if( filter && ( filter->GetAction() == FILE_SAVE ))
            ret = filter->GetDirectory();
    }

    if(( ret[1] == '"' ) && ( ret[ ret.Length() ] == '"' ))
    	ret = ret.SubString( 2, ret.Length() - 2 );

    if( ret[ ret.Length() ] != '\\' )
    	ret += "\\";

	return( ret );
}
//---------------------------------------------------------------------------
void TPrefs::GetDefaultBrowser( void )
{
	TRegistry  *reg = new TRegistry();

    reg->RootKey = HKEY_CLASSES_ROOT;

    if( reg->OpenKey( "http\\shell\\open\\command", false )) {

    	Browser = reg->ReadString( "" );

    	reg->CloseKey();
    }

    delete reg;
}
//---------------------------------------------------------------------------
void TPrefs::LaunchURL( AnsiString url )
{
	if( OpenNewBrowser() ) {
    	AnsiString 	cmd = Browser.Trim(), args;
        int			split;

        if( cmd[1] == '"' ) {
        	int pos;

        	cmd.Delete( 1, 1 );

            pos  = cmd.Pos( "\"" );
			args = cmd.SubString( pos + 1, cmd.Length() );

            cmd.Delete( pos, cmd.Length() );

        } else {
        	int pos = cmd.Pos( " " );

        	if( pos ) {

            	args = cmd.SubString( pos + 1, cmd.Length() );

                cmd.Delete( pos, cmd.Length() );
            }
        }

        if( !args.IsEmpty() )
        	args += " ";

        args += url;

    	ShellExecute( Application->Handle, "open", cmd.c_str(), args.c_str(), NULL, SW_SHOWNORMAL );

    } else
    	ShellExecute( Application->Handle, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL );
}
//---------------------------------------------------------------------------
void TPrefs::RecursiveDeleteKey( AnsiString Key )
{
    TRegistry  *reg = new TRegistry();

    if( reg->OpenKey( Key, false )) {

        if( reg->HasSubKeys() ) {
            TRegKeyInfo info;

            reg->GetKeyInfo( info );

            DWORD   len = info.MaxSubKeyLen + 1;
            char   *s = new char[ len ];

            for( int i = 0; i < info.NumSubKeys; i++ ) {

                RegEnumKey( reg->CurrentKey, i, s, len );

                RecursiveDeleteKey( Key + "\\" + AnsiString( s ));
            }

            delete[] s;
        }

        reg->CloseKey();

        reg->DeleteKey( Key );
    }

    delete reg;
}
//---------------------------------------------------------------------------
AnsiString TPrefs::GetLogName( AnsiString server, AnsiString target ) const
{
	AnsiString	ret, tmp = LogNameFormat;
    TDateTime	now = TDateTime::CurrentDateTime();

    while( int pos = tmp.Pos( "%" )) {

    	ret += tmp.SubString( 1, pos - 1 );

        if( pos + 1 <= tmp.Length() ) {

            switch( tmp[ ++pos ] ) {

                case '%':
                    ret += "%";
                    break;

                case 't':
					ret += target;
                    break;

                case 's':
                	ret += server;
                    break;

                case 'd':
                	ret += now.FormatString( "dd" );
                	break;

                case 'm':
                	ret += now.FormatString( "mm" );
                	break;

                case 'y':
                	ret += now.FormatString( "yyyy" );
                	break;
            }
        }

    	tmp.Delete( 1, pos );
    }

    return( ret + tmp );
}
//---------------------------------------------------------------------------

