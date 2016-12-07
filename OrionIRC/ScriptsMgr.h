/* $Id: ScriptsMgr.h,v 1.1 2005/01/23 02:14:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ScriptsMgrH
#define ScriptsMgrH
//---------------------------------------------------------------------------

#include <registry.hpp>

class TScript;

class ScriptsMgr
{
public:
				ScriptsMgr();
               ~ScriptsMgr();

	ScriptsMgr&	operator =( const ScriptsMgr& rhs );

	int 		GetCount( void ) const;
    TScript    *Get( int i ) const;

    void		Clear( void );

    void		Add( TScript *rec );
    void		Remove( TScript *rec );

    void 		Load( TRegistry *reg, AnsiString key );
    void 		Save( TRegistry *reg, AnsiString key );

    void		Compile( void );

private:
	TList	   *Records;
};

#endif
