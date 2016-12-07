/* $Id: IRCChanData.cpp,v 1.2 2005/01/25 21:16:16 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IRCChanData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TIRCChanData::TIRCChanData()
{
    Topics = new TStringList();

    Reset();
}
//---------------------------------------------------------------------------
TIRCChanData::~TIRCChanData()
{
    delete Topics;
}
//---------------------------------------------------------------------------
void TIRCChanData::Reset( void )
{
    Keyword      = "";
    Limit        = 0;
    TopicSetBy   = "";
    TopicSetTime = "";

    Modes.Clear();
    Flags.Clear();
    Flags.Set( IRC_CHAN_FLAG_JOINED );
}
//---------------------------------------------------------------------------
AnsiString TIRCChanData::GetModesStr( void ) const
{
	AnsiString ret = "";

    if( Modes.IsSet( IRC_CHAN_MODE_SECRET ))
    	ret += "s";

    if( Modes.IsSet( IRC_CHAN_MODE_PRIVATE ))
    	ret += "p";

    if( Modes.IsSet( IRC_CHAN_MODE_TOPIC_PROT ))
    	ret += "t";

    if( Modes.IsSet( IRC_CHAN_MODE_ANONYMOUS ))
    	ret += "a";

    if( Modes.IsSet( IRC_CHAN_MODE_INVITE ))
    	ret += "i";

    if( Modes.IsSet( IRC_CHAN_MODE_MODERATED ))
    	ret += "m";

    if( Modes.IsSet( IRC_CHAN_MODE_QUIET ))
    	ret += "q";

	if( Modes.IsSet( IRC_CHAN_MODE_NO_MSGS ))
    	ret += "n";

    if( Modes.IsSet( IRC_CHAN_MODE_REOP	))
    	ret += "r";

	if( Modes.IsSet( IRC_CHAN_MODE_LIMIT ))
    	ret += "l";

    if( Modes.IsSet( IRC_CHAN_MODE_KEY ))
    	ret += "k";

    return( ret );
}
//---------------------------------------------------------------------------
