/* $Id: FilterMgr.h,v 1.1 2003/05/04 20:41:01 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FilterMgrH
#define FilterMgrH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "irc.h"

class TFilter;

class FilterMgr
{
public:
				FilterMgr();
               ~FilterMgr();

	int 		GetCount( void );
    TFilter	   *Get( int i );

    void		Clear( void );

    void		Add( TFilter *filter );

    bool		Process( const char *nick, AnsiString& msg, TIRCEvent& type, TForm *wnd );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

private:
    TList	   *Filters;
};

#endif
 