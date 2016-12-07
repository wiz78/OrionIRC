/* $Id: NotifyHandler.h,v 1.2 2005/04/19 18:19:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef NotifyHandlerH
#define NotifyHandlerH
//---------------------------------------------------------------------------

class TServerWin;
class NotifyItem;

class TNotifyHandler
{
public:
					TNotifyHandler();
                   ~TNotifyHandler();

	void			Clear( void );

	void			BeginUpdate( void );
    void			EndUpdate( TServerWin *server );

    void			IsOn( TServerWin *server, AnsiString nick );

private:
	TList		   *List;

    NotifyItem 	   *Get( int i );

    void 			DisplayEvent( TServerWin *server, AnsiString msg );
};

#endif
 