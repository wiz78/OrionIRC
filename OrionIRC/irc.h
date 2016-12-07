/* $Id: irc.h,v 1.24 2005/04/13 19:19:36 wiz Exp $ */
#ifndef IRC_H
#define IRC_H

#include "BitField.h"

#define	IRC_MAX_MSG_LENGTH	512

// USER cmd mode flags
#define IRC_USER_WALLOPS	(1 << 2)
#define IRC_USER_INVISIBLE	(1 << 3)

typedef enum {
	IRC_NORMAL = 0,
	IRC_MSG,
    IRC_OWN_MSG,
	IRC_JOIN,
    IRC_PART,
    IRC_QUIT,
    IRC_CTCP,
    IRC_DCC,
    IRC_DCC_CHAT,
    IRC_INFO,
    IRC_ERROR,
    IRC_NICK,
    IRC_MOTD,
    IRC_ACTION,
    IRC_WALLOP,
    IRC_TOPIC,
    IRC_KICK,
    IRC_MODE,
    IRC_NOTIFY,
    IRC_NOTICE,
    IRC_INVITE,
    IRC_WHOIS,
    IRC_HIGHLIGHT,
    IRC_LOCAL,

    IRC_BACKGROUND,
    IRC_NICK_BACKGROUND,
    IRC_TIMESTAMP_BACKGROUND,
    IRC_NICK_COLUMN,
    IRC_TIMESTAMP_COLUMN,
    IRC_EVENT,

    IRC_SCRIPT,

    IRC_NUM_EVENTS
} TIRCEvent;

class TServerWin;

class TIRCSession
{
public:
	AnsiString		Nick;
    AnsiString		PreviousNick; // used by /away etc...
    AnsiString		Hostmask;
    AnsiString		Server;
    AnsiString		Network;
    AnsiString		ChanTypes;
    AnsiString		ChanModes;
    unsigned short	Port;
    BitField		Flags;
    TServerWin	   *ServerWin;
    TForm		   *ActiveWin;
    int				ModesCount; // num args per MODE cmd
    int				Lag;
};

#define IRC_SESS_CONNECTING		(1 << 0)
#define IRC_SESS_CONNECTED		(1 << 1)
#define IRC_SESS_AWAY			(1 << 2)
#define IRC_SESS_AWAY_NICK		(1 << 3)
#define IRC_SESS_DONT_RECONNECT	(1 << 4)
#define IRC_SESS_NO_WHO			(1 << 5)

#include "IRCChanData.h"

typedef enum {
	IRC_CHAN_LIST_BAN,
    IRC_CHAN_LIST_EXCEPT,
    IRC_CHAN_LIST_INVITE
} TIRCChanList;


// control codes
#define TEXT_BOLD		'\002'
#define TEXT_COLOR 		'\003'
#define TEXT_TRUE_COLOR	'\004'
#define TEXT_BEEP 		'\007'
#define TEXT_ITALIC		'\011'
#define TEXT_RESET 		'\017'
#define TEXT_REVERSE 	'\026'
#define TEXT_ESCAPE    	'\033'
#define TEXT_UNDERLINE	'\037'

#endif /* IRC_H */