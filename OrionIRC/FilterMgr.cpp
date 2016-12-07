/* $Id: FilterMgr.cpp,v 1.3 2005/06/06 18:34:31 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FilterMgr.h"
#include "Filter.h"
#include "Main.h"
#include "IRCUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
FilterMgr::FilterMgr()
{
    Filters = new TList();
}
//---------------------------------------------------------------------------
FilterMgr::~FilterMgr()
{
	Clear();

    delete Filters;
}
//---------------------------------------------------------------------------
int FilterMgr::GetCount( void )
{
	return( Filters->Count );
}
//---------------------------------------------------------------------------
TFilter *FilterMgr::Get( int i )
{
	TFilter *ret = NULL;

    if( i < Filters->Count )
		ret = reinterpret_cast<TFilter *>( Filters->Items[ i ]);

    return( ret );
}
//---------------------------------------------------------------------------
void FilterMgr::Clear( void )
{
    for( int i = 0; i < GetCount(); i++ )
		delete Get( i );

    Filters->Clear();
}
//---------------------------------------------------------------------------
void FilterMgr::Add( TFilter *filter )
{
	Filters->Add( filter );
}
//---------------------------------------------------------------------------
bool FilterMgr::Process( const char *nick, AnsiString& msg, TIRCEvent& type, TForm *wnd )
{
	bool	ret = true, go = true;

    for( int i = 0; go && ( i < GetCount() ); i++ ) {
    	TFilter	*filter = Get( i );

        if( filter->IsEnabled() && filter->Matches( msg, wnd )) {

    		if( filter->DoHighlight() )
            	type = IRC_HIGHLIGHT;

    		if( filter->DoHide() )
            	ret = false;

    		if( filter->DoChangeColor() )
            	msg = Format( "\004%06x", ARRAYOFCONST(( filter->GetColor() ))) +
                	  IRCUtils::StripColors( msg );

    		if( filter->DoPrintOnActiveWin() )
				MainWin->ForwardToActiveWin( nick, msg, type, wnd );

    		if( filter->DoPlaySound() )
				filter->PlaySnd();

            if( filter->DoFlashStatusBar() )
            	MainWin->FlashStatusBar();

        	if( filter->DoStop() )
            	go = false;
        }
    }

    return( ret );
}
//---------------------------------------------------------------------------
void FilterMgr::Load( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, false )) {
    	int num = 0;

        try{ num = reg->ReadInteger( "Count" ); } catch(...) {}

        reg->CloseKey();

        for( int i = 0; i < num; i++ ) {
			TFilter	*filter = new TFilter();

            filter->Load( reg, key + "\\Filter" + IntToStr( i ));

            Add( filter );
        }
    }
}
//---------------------------------------------------------------------------
void FilterMgr::Save( TRegistry *reg, AnsiString key )
{
    if( reg->OpenKey( key, true )) {

        try{
        	reg->WriteInteger( "Count", GetCount() );
        }
        catch(...) {
        }

        reg->CloseKey();

        for( int i = 0; i < GetCount(); i++ )
            Get( i )->Save( reg, key + "\\Filter" + IntToStr( i ));
    }
}
//---------------------------------------------------------------------------
