/* $Id: DblClickAction.h,v 1.1 2005/06/08 20:45:14 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DblClickActionH
#define DblClickActionH
//---------------------------------------------------------------------------

#include <registry.hpp>

class TUser;
class TChanWin;
class TQueryWin;
class TServerWin;

class DblClickAction
{
public:
	typedef enum { DEFAULT = 0,
    			   WHOIS = DEFAULT, OPEN_QUERY, CALL_CMD,
    			   SERVER_STATS = DEFAULT,
                   CHANNEL_PROPS = DEFAULT } TDblClickActionType;

						DblClickAction();
                        DblClickAction( const DblClickAction *copy );

	DblClickAction&		operator =( const DblClickAction& copy );

	TDblClickActionType	GetAction( void ) const 	{ return( Action ); }
    AnsiString			GetCommand( void ) const    { return( Command ); }

    void				SetAction( TDblClickActionType act )	{ Action = act; }
    void				SetCommand( AnsiString cmd )			{ Command = cmd; }

    void				Execute( TChanWin *wnd ) const;
    void				Execute( TServerWin *wnd ) const;
    void				Execute( TQueryWin *wnd ) const;
    void				Execute( TChanWin *wnd, TUser *user ) const;

    void				Load( TRegistry *reg, AnsiString key );
    void				Save( TRegistry *reg, AnsiString key ) const;

private:
	TDblClickActionType	Action;
    AnsiString			Command;

	AnsiString 			FormatCmd( AnsiString arg ) const;
};

#endif
