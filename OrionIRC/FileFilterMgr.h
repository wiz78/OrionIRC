/* $Id: FileFilterMgr.h,v 1.2 2003/05/11 21:25:29 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FileFilterMgrH
#define FileFilterMgrH
//---------------------------------------------------------------------------

#include <registry.hpp>

class TFileFilter;

class FileFilterMgr
{
public:
					FileFilterMgr();
               	   ~FileFilterMgr();

	int 			GetCount( void );
    TFileFilter	   *Get( int i );

    void			Clear( void );

    void			Add( TFileFilter *filter );

    TFileFilter    *Find( AnsiString file );
    bool			IsAcceptable( AnsiString file );

    void 			Load( TRegistry *reg, AnsiString key );
    void 			Save( TRegistry *reg, AnsiString key );

private:
    TList	   *Filters;
};

#endif
