/* $Id: Logger.cpp,v 1.4 2005/05/10 17:23:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Logger.h"
#include "IRCUtils.h"
#include "Prefs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
Logger::Logger()
{
    FileHandle = INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------
Logger::~Logger()
{
	Close();
}
//---------------------------------------------------------------------------
void Logger::Open( AnsiString server, AnsiString target )
{
	Open( Prefs.GetLogsDir() + "\\" + Prefs.GetLogName( server, target ));
}
//---------------------------------------------------------------------------
void Logger::Open( AnsiString name )
{
	if( FileHandle == INVALID_HANDLE_VALUE ) {

        FileName   = name;
        FileHandle = CreateFile( FileName.c_str(),
                                 GENERIC_WRITE, FILE_SHARE_READ,
                                 NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL,
                                 NULL );

        if( FileHandle != INVALID_HANDLE_VALUE ) {

            SetFilePointer( FileHandle, 0, NULL, FILE_END );

            Write( "\r\nSession started at " + TDateTime::CurrentDateTime().FormatString( "dddddd tt" ));
        }
    }
}
//---------------------------------------------------------------------------
void Logger::Close( void )
{
    if( FileHandle != INVALID_HANDLE_VALUE ) {

    	Write( "Session closed at " + TDateTime::CurrentDateTime().FormatString( "dddddd tt" ));

    	CloseHandle( FileHandle );

        FileHandle = INVALID_HANDLE_VALUE;
        FileName   = "";
    }
}
//---------------------------------------------------------------------------
void Logger::Write( AnsiString line )
{
    if( FileHandle != INVALID_HANDLE_VALUE ) {
     	DWORD	written;

        line += "\r\n";

        WriteFile( FileHandle, line.c_str(), line.Length(), &written, NULL );
    }
}
//---------------------------------------------------------------------------
void Logger::Log( const char *nick, AnsiString msg, TIRCEvent type )
{
    if( FileHandle != INVALID_HANDLE_VALUE ) {
        AnsiString	line;

        if( Prefs.StripCodesFromLogs() )
            msg = IRCUtils::StripCodes( msg );

        if( Prefs.TimestampLogs() )
            line += "[" + TDateTime::CurrentDateTime().FormatString( "c" ) + "] ";

        line += IRCUtils::GetNick( nick, type ) + " " + msg;

        Write( line );
    }
}
//---------------------------------------------------------------------------
