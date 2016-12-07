/* $Id: NotifyMgr.h,v 1.2 2003/07/14 18:56:20 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef NotifyMgrH
#define NotifyMgrH
//---------------------------------------------------------------------------

class TNotify;
class TServerWin;

class NotifyMgr
{
public:
				NotifyMgr();
               ~NotifyMgr();

	int 		GetCount( void );
    TNotify    *Get( int i );

    void		Clear( void );

    void		Add( AnsiString nick );
    void		Add( TNotify *fav );

    void		Remove( AnsiString nick );
    void		Remove( TNotify *fav );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

    void		SendIsON( TServerWin *server );

    void		IsOn( AnsiString nick, bool on );

private:
    TList	   *Notify;
};

#endif
 