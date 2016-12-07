/* $Id: IRCChanData.h,v 1.3 2005/04/27 17:33:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCChanDataH
#define IRCChanDataH
//---------------------------------------------------------------------------

#include "BitField.h"

class TIRCChanData
{
public:
					TIRCChanData();
                   ~TIRCChanData();

	void			Reset( void );

	AnsiString 		GetModesStr( void ) const;

    BitField		Flags;
    BitField		Modes;
    AnsiString		Keyword;
    int				Limit;
    AnsiString		TopicSetBy;
    AnsiString		TopicSetTime;
    TStringList	   *Topics;
};

#define IRC_CHAN_FLAG_OP			(1 << 0)
#define IRC_CHAN_FLAG_VOICE			(1 << 1)
#define IRC_CHAN_FLAG_DONT_CLOSE	(1 << 2)
#define IRC_CHAN_FLAG_JOINED		(1 << 3)
#define IRC_CHAN_FLAG_LEFT			(1 << 4)

#define IRC_CHAN_MODE_SECRET		(1 << 0)
#define IRC_CHAN_MODE_PRIVATE		(1 << 1)
#define IRC_CHAN_MODE_TOPIC_PROT	(1 << 2)
#define IRC_CHAN_MODE_ANONYMOUS		(1 << 3)
#define IRC_CHAN_MODE_INVITE		(1 << 4)
#define IRC_CHAN_MODE_MODERATED		(1 << 5)
#define IRC_CHAN_MODE_QUIET			(1 << 6)
#define IRC_CHAN_MODE_NO_MSGS		(1 << 7)
#define IRC_CHAN_MODE_REOP			(1 << 8)
#define IRC_CHAN_MODE_LIMIT			(1 << 9)
#define IRC_CHAN_MODE_KEY			(1 << 10)

#endif
 