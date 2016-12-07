/* $Id: ScriptsMgr.cpp,v 1.1 2005/01/23 02:14:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ScriptsMgr.h"
#include "Script.h"
#include "OrionScriptInterface.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
ScriptsMgr::ScriptsMgr()
{
	Records = new TList();
}
//---------------------------------------------------------------------------
ScriptsMgr::~ScriptsMgr()
{
	Clear();

	delete Records;
}
//---------------------------------------------------------------------------
ScriptsMgr& ScriptsMgr::operator =( const ScriptsMgr& rhs )
{
	Clear();

    for( int i = 0; i < rhs.GetCount(); i++ )
    	Add( new TScript( rhs.Get( i )));

    return( *this );
}
//---------------------------------------------------------------------------
int ScriptsMgr::GetCount( void ) const
{
	return( Records->Count );
}
//---------------------------------------------------------------------------
TScript *ScriptsMgr::Get( int i ) const
{
	TScript *ret = NULL;

    if( i < GetCount() )
		ret = reinterpret_cast<TScript *>( Records->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void ScriptsMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Records->Clear();
}
//---------------------------------------------------------------------------
void ScriptsMgr::Add( TScript *rec )
{
	Records->Add( rec );
}
//---------------------------------------------------------------------------
void ScriptsMgr::Remove( TScript *rec )
{
	Records->Remove( rec );
}
//---------------------------------------------------------------------------
void ScriptsMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TScript	*rec = new TScript();

            rec->Load( reg, key + "\\Record" + IntToStr( i ));

            Add( rec );
        }
    }
}
//---------------------------------------------------------------------------
void ScriptsMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Record" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------
void ScriptsMgr::Compile( void )
{
	for( int i = 0; i < GetCount(); i++ ) {
    	TScript *rec = Get( i );

        if( rec->IsEnabled() )
	    	OrionScript::CodeEval( "include_once( '" +
    	    					    OrionScript::EscapeTicks( rec->GetFile() ) +
        	                        "' )" );
    }
}
//---------------------------------------------------------------------------

