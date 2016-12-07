/* $Id: IgnoreMgr.h,v 1.4 2004/12/20 21:57:38 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IgnoreMgrH
#define IgnoreMgrH
//---------------------------------------------------------------------------

class TIgnore;
class HostMask;

#define IGN_CHANNEL			(1 << 0)
#define IGN_PRIVATE			(1 << 1)
#define IGN_NOTICE			(1 << 2)
#define IGN_CTCP			(1 << 3)
#define IGN_INVITE			(1 << 4)
#define IGN_DCC_FILE		(1 << 5)
#define IGN_DCC_CHAT		(1 << 6)
#define IGN_ACTION			(1 << 7)
#define IGN_PAGE			(1 << 8)
#define IGN_DCC_VIDEO		(1 << 9)

#define IGN_CONTROL_CODE	(1 << 31)

class IgnoreMgr
{
public:
				IgnoreMgr();
               ~IgnoreMgr();

	IgnoreMgr&	operator =( const IgnoreMgr& rhs );

	int 		GetCount( void ) const;
    TIgnore    *Get( int i ) const;

    void		Clear( void );

    void		Add( TIgnore *ign );
    void		Remove( TIgnore *ign );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

    bool		Ignore( HostMask& mask, AnsiString& msg, int event );

    bool		IsEnabled( void ) const { return( Enabled ); }
    void		SetEnabled( bool set )  { Enabled = set; }

private:
    TList	   *Records;
    bool		Enabled;
};


#endif
 