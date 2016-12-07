/* $Id: Commands.cpp,v 1.54 2006/01/15 12:38:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Commands.h"
#include "Main.h"
#include "ServerWnd.h"
#include "ChanWnd.h"
#include "QueryWnd.h"
#include "Eyelin.h"
#include "Prefs.h"
#include "OrionScriptInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

static void SplitWords( AnsiString cmd, TStringList *words );

static void CmdAway( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdBan( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdCall( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdClear( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdClearSendQ( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdCTCP( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdCycle( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdDCC( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdDCCAllow( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdDeop( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdDNS( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdIgnore( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdInvite( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdJoin( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdKick( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdKnock( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdList( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdMe( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdMode( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdMsg( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdNames( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdNick( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdNotice( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdNotify( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdNServer( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdOp( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdPage( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdPart( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdQuery( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdQuit( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdRaw( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdServer( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdSlap( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdTopic( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdUnban( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdUnvoice( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdUserIP( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdVoice( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdWho( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdWhois( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdWhowas( TServerWin *server, TChanWin *channel, TStringList *words );
static void CmdHelp( TServerWin *server, TChanWin *channel, TStringList *words );
static void SetUserModes( TServerWin *server, TChanWin *channel, TStringList *words,
						  AnsiString mode, AnsiString action );

typedef void ( *CmdHandler )( TServerWin *server, TChanWin *channel, TStringList *words );

#define C_NEED_SERVER		(1 << 0)
#define C_NEED_CHAN      	(1 << 1)
#define C_ORIGINAL_ARGS     (1 << 2)

static const struct Cmd {
	const char 	   *Cmd;
    CmdHandler		Handler;
    short			MinArgs;
    unsigned int	Flags;
    const char 	   *Help;
} CmdTable[] = {

	{ "?", CmdHelp, 0, 0,
      "/? [command], shows the help for [command], or the command list" },

	{ "AWAY", CmdAway, 0, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/AWAY [away message], toggles away status" },

	{ "BAN", CmdBan, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/BAN <mask> [<mask> <mask> ...], sets a ban for <mask>" },

	{ "CALL", CmdCall, 1, C_ORIGINAL_ARGS,
      "/CALL <function>, calls the script <function>" },

	{ "CLEAR", CmdClear, 0, 0,
      "/CLEAR, clears the buffer of the current window" },

	{ "CLEARSENDQ", CmdClearSendQ, 0, 0,
      "/CLEARSENDQ, clears the send queue for the server (useful if you pasted the whole English dictionary by mistake :)" },

	{ "CTCP", CmdCTCP, 2, C_NEED_SERVER,
      "/CTCP <target> <command> [args], sends a CTCP <command> to <target> with [args], if provided" },

	{ "CYCLE", CmdCycle, 0, C_NEED_SERVER | C_NEED_CHAN,
      "/CYCLE, leaves and joins again the current channel" },

	{ "DCC", CmdDCC, 2, C_NEED_SERVER,
      "/DCC SEND <nick> <file>, sends <file> to <nick> - /DCC CHAT <nick>, establishes a direct chat line with <nick>" },

	{ "DCCALLOW", CmdDCCAllow, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/DCCALLOW [<+|->nick[,<+|->nick, ...]] [list] [help], specifies which people can send you files. See /DCCALLOW HELP for more info." },

	{ "DEOP", CmdDeop, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/DEOP <nick> [<nick> <nick> ...], deops <nick>" },

	{ "DNS", CmdDNS, 1, C_ORIGINAL_ARGS,
      "/DNS <host or nick>, tries to resolve the hostname specified or associated with the specified nick" },

	{ "HELP", CmdHelp, 0, 0,
      "/HELP [command], shows the help for [command], or the command list" },

	{ "IGNORE", CmdIgnore, 1, C_NEED_SERVER,
      "/IGNORE <nick> [PUB] [MSG] [CTCP], ignores messages coming from <nick>. You can specify which type of messages should be ignored. A finer control is available through the ignore window." },

	{ "INVITE", CmdInvite, 1, C_NEED_SERVER,
      "/INVITE <nick> [channel], invites <nick> in the current channel, or in [channel]" },

	{ "JOIN", CmdJoin, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/JOIN <channel>, enters the channel" },

	{ "KICK", CmdKick, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/KICK <nick> [<reason>], kicks <nick> from the current channel" },

	{ "KNOCK", CmdKnock, 1, C_NEED_SERVER,
      "/KNOCK <channel> [<reason>], if <channel> is invite-only, asks its ops to be let in" },

	{ "LEAVE", CmdPart, 0, C_NEED_SERVER,
      "/LEAVE [channel] [part msg], leaves the channel specified, or the current one" },

	{ "LIST", CmdList, 0, C_NEED_SERVER,
      "/LIST, lists the channels available on the server" },

	{ "ME", CmdMe, 1, C_NEED_SERVER | C_NEED_CHAN | C_ORIGINAL_ARGS,
      "/ME <message>, says message prefixed by your nick" },

	{ "MODE", CmdMode, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/MODE <channel> <mode> [nick], changes channel modes" },

	{ "MSG", CmdMsg, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/MSG <target> <message>, sends <message> to <target>" },

	{ "NAMES", CmdNames, 0, C_NEED_SERVER,
      "/NAMES [channel], lists the users of [channel]" },

	{ "NICK", CmdNick, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/NICK <new nick>, changes your current nickname" },

	{ "NOTICE", CmdNotice, 2, C_NEED_SERVER,
      "/NOTICE <target> <msg>, sends <msg> to <target> as a notice" },

	{ "NOTIFY", CmdNotify, 1, C_NEED_SERVER,
      "/NOTIFY <nick> [<nick> <nick> ...], adds <nick> to the list of nicks to monitor" },

	{ "NSERVER", CmdNServer, 1, 0,
      "/NSERVER <server> [<port>], connects to server <nick> on <port> in a new window" },

	{ "OP", CmdOp, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/OP <nick> [<nick> <nick> ...], ops <nick>" },

	{ "PAGE", CmdPage, 1, C_NEED_SERVER,
      "/PAGE <nick>, sends a CTCP PAGE to <nick>" },

	{ "PART", CmdPart, 0, C_NEED_SERVER,
      "/PART [channel] [part msg], leaves the channel specified, or the current one" },

	{ "QUERY", CmdQuery, 1, 0,
      "/QUERY <nick>, opens a query to <nick>" },

	{ "QUIT", CmdQuit, 0, C_NEED_SERVER,
      "/QUIT [quit msg], disconnects from the server" },

	{ "QUOTE", CmdRaw, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/QUOTE <command>, sends <command> as-is to the server" },

	{ "RAW", CmdRaw, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/RAW <command>, sends <command> as-is to the server" },

	{ "SERVER", CmdServer, 1, 0,
      "/SERVER <server> [<port>], connects to server <nick> on <port>" },

	{ "SLAP", CmdSlap, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/SLAP <nick>, slaps <nick>" },

	{ "TOPIC", CmdTopic, 0, C_NEED_SERVER | C_NEED_CHAN | C_ORIGINAL_ARGS,
      "/TOPIC [topic], sets channel topic" },

	{ "UNBAN", CmdUnban, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/UNBAN <mask> [<mask> <mask> ...], removes the ban for <mask>" },

	{ "UNVOICE", CmdUnvoice, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/UNVOICE <nick> [<nick> <nick> ...], removes voice from <nick>" },

	{ "USERIP", CmdUserIP, 1, C_NEED_SERVER | C_ORIGINAL_ARGS,
      "/USERIP <nick> [<nick> <nick> ...], shows the ip of <nick>" },

	{ "VOICE", CmdVoice, 1, C_NEED_SERVER | C_NEED_CHAN,
      "/VOICE <nick> [<nick> <nick> ...], gives voice to <nick>" },

	{ "WHO", CmdWho, 0, C_NEED_SERVER,
      "/WHO [channel], lists the users of [channel]" },

	{ "WHOIS", CmdWhois, 1, C_NEED_SERVER,
      "/WHOIS <nick>[,<nick>], shows information about <nick>" },

	{ "WHOWAS", CmdWhowas, 1, C_NEED_SERVER,
      "/WHOWAS <nick>[,<nick>], tells who <nick> was" },
};

#define NUM_CMDS	( sizeof( CmdTable ) / sizeof( CmdTable[0] ))

//---------------------------------------------------------------------------
void HandleCommand( TServerWin *server, TChanWin *channel, AnsiString cmd )
{
	TStringList	   *words = new TStringList();
    AnsiString		command;
    bool			found = false;

    if( int pos = cmd.Pos( " " )) {

    	command = cmd.SubString( 1, pos - 1 );

        cmd.Delete( 1, pos );

    } else {

    	command = cmd;
        cmd     = "";
    }

    for( int i = 0; !found && ( i < NUM_CMDS ); i++ ) {
    	const struct Cmd *c = &CmdTable[ i ];

    	if( !command.AnsiCompareIC( c->Cmd )) {

        	if( c->Flags & C_ORIGINAL_ARGS )
            	words->Add( cmd );
            else
            	SplitWords( cmd.Trim(), words );

        	if( c->MinArgs > words->Count )
	           	server->DisplayOnActiveWin( NULL, "I need more arguments for " + command, IRC_ERROR );
        	else if(( c->Flags & C_NEED_SERVER ) && !server->Session.Flags.IsSet( IRC_SESS_CONNECTED ))
	           	server->DisplayOnActiveWin( NULL, "Not connected to the server!", IRC_ERROR );
            else if(( c->Flags & C_NEED_CHAN ) && !channel )
            	server->DisplayOnActiveWin( NULL, "Not in a channel!", IRC_ERROR );
            else
	        	c->Handler( server, channel, words );

            found = true;
        }
    }

    if( !found ) {
        AnsiString	origArgs = cmd;

        SplitWords( cmd, words );

        found = Prefs.GetAliases()->Invoke( server, channel, command, origArgs, words );

	    if( !found )
			server->DisplayOnActiveWin( NULL, "Sorry, I don't know what " + command + " means...", IRC_ERROR );
    }

    delete words;
}
//---------------------------------------------------------------------------
static void SplitWords( AnsiString cmd, TStringList *words )
{
    while( int pos = cmd.Pos( " " )) {

        words->Add( cmd.SubString( 1, pos - 1 ));

        cmd = cmd.Delete( 1, pos ).TrimLeft();
    }

    if( !cmd.IsEmpty() )
        words->Add( cmd.Trim() );
}
//---------------------------------------------------------------------------
static void CmdAway( TServerWin *server, TChanWin *channel, TStringList *words )
{
	if( server->Session.Flags.IsSet( IRC_SESS_AWAY )) {

		server->SendCmd( "AWAY" );

		if( server->Session.Flags.IsSet( IRC_SESS_AWAY_NICK ))
			server->SendCmd( "NICK " + server->Session.PreviousNick );

    } else {
		AnsiString	msg;

    	if( words->Count > 0 )
    		msg = words->Strings[ 0 ];

	    if( msg.IsEmpty() )
    		msg = server->Server.GetAwayMsg();

		server->SendCmd( "AWAY :" + msg );

        if( server->Server.Flags.IsSet( SERVF_SET_AWAY_NICK ) &&
        	!server->Server.AwayNick.IsEmpty() ) {

            server->Session.PreviousNick = server->Session.Nick;

			server->SendCmd( "NICK " + server->Server.AwayNick );

            server->Session.Flags.Set( IRC_SESS_AWAY_NICK );
        }
    }
}
//---------------------------------------------------------------------------
static void CmdBan( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "b", "+" );
}
//---------------------------------------------------------------------------
static void CmdCall( TServerWin *server, TChanWin *channel, TStringList *words )
{
    OrionScript::Eval( words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdClear( TServerWin *server, TChanWin *channel, TStringList *words )
{
	TIRCList   *list = NULL;

    if( channel ) {

	    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel )))
        	list = channel->GetDisplay();
        else if( TQueryWin *query = dynamic_cast<TQueryWin *>( static_cast<TForm *>( channel )))
        	list = query->GetDisplay();

    } else
    	list = server->GetDisplay();

    if( list ) {
	    list->Clear();
        list->Repaint();
    }
}
//---------------------------------------------------------------------------
static void CmdClearSendQ( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->ClearSendQueue();
}
//---------------------------------------------------------------------------
static void CmdCTCP( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString cmd;

    cmd = "PRIVMSG " + words->Strings[ 0 ] + " :\001" + words->Strings[ 1 ].UpperCase();

    for( int i = 2; i < words->Count; i++ )
    	cmd += " " + words->Strings[ i ];

    cmd += "\001";

    server->SendCmd( cmd );
}
//---------------------------------------------------------------------------
static void CmdCycle( TServerWin *server, TChanWin *channel, TStringList *words )
{
    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {
		AnsiString	target;

	   	target = channel->GetChannel();

	    channel->ChanData.Flags.Set( IRC_CHAN_FLAG_DONT_CLOSE | IRC_CHAN_FLAG_LEFT );

        channel->AddRow( NULL, "Leaving " + target + "...", IRC_INFO, true );

		server->SendCmd( "PART " + target + " :brb" );
		server->SendCmd( "JOIN " + target );
    }
}
//---------------------------------------------------------------------------
static void CmdDCC( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target = words->Strings[ 1 ];

	if( !words->Strings[ 0 ].AnsiCompareIC( "SEND" )) {
    	AnsiString	file;

        if( words->Count >= 3 )
        	file = words->Strings[ 2 ];

        for( int i = 3; i < words->Count; i++ )
        	file += " " + words->Strings[ i ];

    	server->DCCSend( target, file );

    } else if( !words->Strings[ 0 ].AnsiCompareIC( "CHAT" )) {
		TQueryWin  *win = server->FindQueryWin( target );

        win->InitDCC();

    } else if( !words->Strings[ 0 ].AnsiCompareIC( "EYELIN" )) {

        Eyelin::GetInstance()->RequestDCC( server, target );

    } else
    	server->DisplayOnActiveWin( NULL,
        							"DCC " + words->Strings[ 0 ] +
        							" is something I don't know how to handle",
                                    IRC_ERROR );
}
//---------------------------------------------------------------------------
static void CmdDCCAllow( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "DCCALLOW " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdDeop( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "o", "-" );
}
//---------------------------------------------------------------------------
static void CmdDNS( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	host = words->Strings[ 0 ].Trim();

    if( host.Pos( "." ) || host.Pos( ":" ))
		MainWin->DoDNS( host, server );
    else if( server->Session.Flags.IsSet( IRC_SESS_CONNECTED ))
    	server->GetHostForDNS( host );
    else
    	server->DisplayOnActiveWin( NULL, "Not connected to a server, cannot find the nick " + host, IRC_ERROR );
}
//---------------------------------------------------------------------------
static void CmdIgnore( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	nick = words->Strings[ 0 ];
    int			flags = 0;

    for( int i = 1; i < words->Count; i++ ) {
    	AnsiString f = words->Strings[ i ];

        if( !f.AnsiCompareIC( "CTCP" ))
        	flags |= IGNORE_CTCP;
        else if( !f.AnsiCompareIC( "MSG" ))
        	flags |= IGNORE_MSG;
        else if( !f.AnsiCompareIC( "PUB" ))
        	flags |= IGNORE_PUB;
    }

    if( !flags )
    	flags = IGNORE_ALL;

    if( nick.Pos( "." ) || nick.Pos( "!" ) || nick.Pos( "@" ))
		server->IgnoreUser( HostMask( nick ), flags );
    else if( server->Session.Flags.IsSet( IRC_SESS_CONNECTED ))
    	server->GetHostForIgnore( nick, flags );
    else
    	server->DisplayOnActiveWin( NULL, "Not connected to a server, cannot find the nick " + nick, IRC_ERROR );
}
//---------------------------------------------------------------------------
static void CmdInvite( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target, nick;

    nick = words->Strings[ 0 ];

    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel )))
	   	target = channel->GetChannel();

    if( words->Count >= 2 )
    	target = words->Strings[ 1 ];

    if( target.IsEmpty() )
    	server->DisplayOnActiveWin( NULL, "You must specify a channel to invite " + nick + " to", IRC_ERROR );
    else
    	server->SendCmd( "INVITE " + nick + " " + target );
}
//---------------------------------------------------------------------------
static void CmdJoin( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "JOIN " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdKick( TServerWin *server, TChanWin *channel, TStringList *words )
{
    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {
		AnsiString	target, msg;

        target = words->Strings[ 0 ];

        for( int i = 1; i < words->Count; i++ )
            msg += " " + words->Strings[ i ];

        if( !msg.IsEmpty() )
            target += " :" + msg.SubString( 2, msg.Length() );

        server->SendUrgCmd( "KICK " + channel->GetChannel() + " " + target );
    }
}
//---------------------------------------------------------------------------
static void CmdKnock( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	cmd = "KNOCK " + words->Strings[ 0 ] + " :";

    for( int i = 1; i < words->Count; i++ ) {

    	if( i > 1 )
        	cmd += " ";

    	cmd += words->Strings[ i ];
    }

    server->SendCmd( cmd );
}
//---------------------------------------------------------------------------
static void CmdList( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "LIST" );
}
//---------------------------------------------------------------------------
static void CmdMe( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target, msg = " " + words->Strings[ 0 ];

    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {

	   	target = channel->GetChannel();

        channel->AddRow( NULL, server->Session.Nick + msg, IRC_ACTION );

	} else {

    	target = ((TQueryWin *)channel )->GetTargetNick();

        ((TQueryWin *)channel )->AddRow( NULL, server->Session.Nick + msg, IRC_ACTION, false, true );
    }

	server->SendCmd( "PRIVMSG " + target + " :\001ACTION" + msg + "\001" );
}
//---------------------------------------------------------------------------
static void CmdMode( TServerWin *server, TChanWin *channel, TStringList *words )
{
    server->SendCmd( "MODE " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdMsg( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString 	cmd, msg, nick;
    int			pos;

    cmd  = "PRIVMSG ";
    nick = words->Strings[ 0 ];
    pos  = nick.Pos( " " );

    if( pos ) {

    	msg = nick.SubString( pos + 1, nick.Length() );

        nick.Delete( pos, nick.Length() );
    }

    cmd += nick + " :" + msg;

    server->SendCmd( cmd );
    server->DisplayOnActiveWin( server->Session.Nick.c_str(),
    						    "[to " + nick + "] " + msg,
                                IRC_OWN_MSG );
}
//---------------------------------------------------------------------------
static void CmdNames( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target;

    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel )))
	   	target = channel->GetChannel();

    if( words->Count >= 1 )
    	target = words->Strings[ 0 ];

    if( target.IsEmpty() )
    	server->DisplayOnActiveWin( NULL, "You must specify a channel", IRC_ERROR );
    else
    	server->SendCmd( "NAMES " + target );
}
//---------------------------------------------------------------------------
static void CmdNick( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "NICK " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdNotice( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	msg;

    for( int i = 1; i < words->Count; i++ )
    	msg += " " + words->Strings[ i ];

	server->SendCmd( "NOTICE " + words->Strings[ 0 ] + " :" + msg.SubString( 2, msg.Length() ));
}
//---------------------------------------------------------------------------
static void CmdNotify( TServerWin *server, TChanWin *channel, TStringList *words )
{
    for( int i = 0; i < words->Count; i++ )
    	Prefs.GetNotifies()->Add( words->Strings[ i ]);

    Prefs.Save();
}
//---------------------------------------------------------------------------
static void CmdNServer( TServerWin *server, TChanWin *channel, TStringList *words )
{
	int    		port = 6667;
    TServer	   *newserv = new TServer( &server->Server );

	if( words->Count >= 2 )
    	port = words->Strings[ 1 ].ToIntDef( 6667 );

    newserv->Host = words->Strings[ 0 ];
    newserv->Port = IntToStr( port );

    newserv->ClearChannels();

	new TServerWin( Application, newserv );

    delete newserv;
}
//---------------------------------------------------------------------------
static void CmdOp( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "o", "+" );
}
//---------------------------------------------------------------------------
static void CmdPage( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->DisplayOnActiveWin( "«PAGE»", "Paging " + words->Strings[0] + "...", IRC_CTCP );

	server->SendCmd( "PRIVMSG " + words->Strings[0] + " :\001PAGE\001" );
}
//---------------------------------------------------------------------------
static void CmdPart( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target, reason;

    if( words->Count )
    	target = words->Strings[ 0 ];

    if( !target.IsEmpty() && !server->Session.ChanTypes.Pos( target.SubString( 1, 1 ))) {

    	reason = target;
    	target = "";

        for( int i = 1; i < words->Count; i++ )
        	reason += " " + words->Strings[ i ];

    } else
    	reason = server->Server.GetPartReason();

    if( channel && target.IsEmpty() )
    	target = channel->GetChannel();

    if( target.IsEmpty() )
       	server->DisplayOnActiveWin( NULL, "You must specify a channel to part", IRC_ERROR );
    else
		server->SendCmd( "PART " + target + " :" + reason );
}
//---------------------------------------------------------------------------
static void CmdQuery( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->FindQueryWin( words->Strings[ 0 ] );
    server->WhoForQuery( words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdQuit( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	reason;

    for( int i = 0; i < words->Count; i++ )
    	reason += " " + words->Strings[ i ];

    if( reason.IsEmpty() )
    	reason = server->Server.GetQuitReason();
    else
    	reason.Delete( 1, 1 );

    server->Session.Flags.Set( IRC_SESS_DONT_RECONNECT );
    
    server->SendCmd( "QUIT :" + reason );
}
//---------------------------------------------------------------------------
static void CmdRaw( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdServer( TServerWin *server, TChanWin *channel, TStringList *words )
{
	int port = 6667;

	if( words->Count >= 2 )
    	port = words->Strings[ 1 ].ToIntDef( 6667 );

    server->ConnectTo( words->Strings[ 0 ], port );
}
//---------------------------------------------------------------------------
static void CmdSlap( TServerWin *server, TChanWin *channel, TStringList *words )
{
	static const char *What[] = {
    	"a large trout",
        "a flying window",
        "Tux, the Linux penguin",
        "a huge Unix manual",
        "a rambone (ask reycava)",
    };
#define WHAT_NUM	(sizeof( What ) / sizeof( What[0] ))
	AnsiString	target, msg;

    msg = " slaps " + words->Strings[ 0 ] + " around a bit with " + What[ rand() % WHAT_NUM ];

    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {

	   	target = channel->GetChannel();

        channel->AddRow( NULL, server->Session.Nick + msg, IRC_ACTION );

	} else {

    	target = ((TQueryWin *)channel )->GetTargetNick();

        ((TQueryWin *)channel )->AddRow( NULL, server->Session.Nick + msg, IRC_ACTION, false, true );
    }

	server->SendCmd( "PRIVMSG " + target + " :\001ACTION" + msg + "\001" );
}
//---------------------------------------------------------------------------
static void CmdTopic( TServerWin *server, TChanWin *channel, TStringList *words )
{
    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {
		AnsiString	msg;

    	if( words->Count > 0 )
	    	msg = words->Strings[ 0 ];

        if( !msg.IsEmpty() )
        	msg.Insert( " :", 1 );

		server->SendCmd( "TOPIC " + channel->GetChannel() + msg );

	} else
       	server->DisplayOnActiveWin( NULL, "Not in a channel", IRC_ERROR );
}
//---------------------------------------------------------------------------
static void CmdUnban( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "b", "-" );
}
//---------------------------------------------------------------------------
static void CmdUnvoice( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "v", "-" );
}
//---------------------------------------------------------------------------
static void CmdUserIP( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "USERIP " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdVoice( TServerWin *server, TChanWin *channel, TStringList *words )
{
	SetUserModes( server, channel, words, "v", "+" );
}
//---------------------------------------------------------------------------
static void CmdWho( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	target;

    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel )))
	   	target = channel->GetChannel();

    if( words->Count >= 1 )
    	target = words->Strings[ 0 ];

    if( target.IsEmpty() )
    	server->DisplayOnActiveWin( NULL, "You must specify a channel", IRC_ERROR );
    else
    	server->SendCmd( "WHO " + target );
}
//---------------------------------------------------------------------------
static void CmdWhois( TServerWin *server, TChanWin *channel, TStringList *words )
{
	AnsiString	nick = words->Strings[ 0 ];

	if( server->Session.Flags.IsSet( IRC_SESS_NO_WHO ) || nick.Pos( "," ))
		server->SendCmd( "WHOIS " + nick );
	else
		server->WhoForWhois( nick );
}
//---------------------------------------------------------------------------
static void CmdWhowas( TServerWin *server, TChanWin *channel, TStringList *words )
{
	server->SendCmd( "WHOWAS " + words->Strings[ 0 ] );
}
//---------------------------------------------------------------------------
static void CmdHelp( TServerWin *server, TChanWin *channel, TStringList *words )
{
	if( words->Count ) {
    	bool		found = false;
    	AnsiString  command = words->Strings[ 0 ];

    	for( int i = 0; !found && ( i < NUM_CMDS ); i++ ) {
	    	const struct Cmd *c = &CmdTable[ i ];

	    	if( !command.AnsiCompareIC( c->Cmd )) {

            	server->DisplayOnActiveWin( "«HELP»", c->Help, IRC_LOCAL );

            	found = true;
            }
        }

        if( !found )
           	server->DisplayOnActiveWin( NULL, command + ": command not found", IRC_ERROR );

	} else {

	    for( int i = 0; i < NUM_CMDS; i++ ) {
    		const struct Cmd *c = &CmdTable[ i ];

           	server->DisplayOnActiveWin( "«HELP»", c->Help, IRC_LOCAL );
		}
    }
}
//---------------------------------------------------------------------------
static void SetUserModes( TServerWin *server, TChanWin *channel, TStringList *words,
						  AnsiString mode, AnsiString action )
{
    if( dynamic_cast<TChanWin *>( static_cast<TForm *>( channel ))) {

        for( int i = 0; i < words->Count; ) {
            AnsiString	modes, nicks;

            for( int j = 0; ( j < server->Session.ModesCount ) && ( i < words->Count ); i++ ) {

                if( j++ )
                    nicks += " ";

                nicks += words->Strings[ i ];
                modes += mode;
            }

            if( !nicks.IsEmpty() )
                server->SendCmd( "MODE " + channel->GetChannel() + " " + action + modes + " " + nicks );
        }

	} else
       	server->DisplayOnActiveWin( NULL, "Not in a channel", IRC_ERROR );
}
//---------------------------------------------------------------------------
