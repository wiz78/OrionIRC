/* $Id: AliasesMgr.h,v 1.1 2005/01/23 19:16:43 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef AliasesMgrH
#define AliasesMgrH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "WizStrHash.h"

class TAlias;
class TServerWin;
class TChanWin;

class AliasesMgr
{
public:
				AliasesMgr();
               ~AliasesMgr();

	AliasesMgr&	operator =( const AliasesMgr& rhs );

	int 		GetCount( void ) const;
    TAlias     *Get( int i ) const;

    void		Clear( void );

    void		Add( TAlias *rec );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

    void		ReHash( void );
    bool		Invoke( TServerWin *server, TChanWin *channel,
    					AnsiString cmd, AnsiString args, TStringList *words );

private:
	TList	   *Records;
    WizStrHash	Aliases;
    int			Recursion;
};

#endif
 