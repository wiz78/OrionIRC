/* $Id: Commands.h,v 1.2 2003/12/31 16:16:04 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef CommandsH
#define CommandsH
//---------------------------------------------------------------------------

class TServerWin;
class TChanWin;

extern void HandleCommand( TServerWin *server, TChanWin *channel, AnsiString cmd );

#define IGNORE_CTCP		(1 << 0)
#define IGNORE_MSG		(1 << 1)
#define IGNORE_PUB		(1 << 2)
#define IGNORE_ALL		(IGNORE_CTCP | IGNORE_MSG | IGNORE_PUB)

#endif
