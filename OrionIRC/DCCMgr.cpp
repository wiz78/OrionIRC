/* $Id: DCCMgr.cpp,v 1.4 2003/05/05 17:16:23 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DCCMgr.h"
#include "DCCTransfer.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TDCCMgr DCCMgr;

//---------------------------------------------------------------------------
TDCCMgr::TDCCMgr()
{
	DCC      = new TList();
    Listview = NULL;
}
//---------------------------------------------------------------------------
TDCCMgr::~TDCCMgr()
{
	for( int i = 0; i < DCC->Count; i++ )
		delete reinterpret_cast<TDCCTransfer *>( DCC->Items[ i ] );

	delete DCC;
}
//---------------------------------------------------------------------------
void TDCCMgr::Add( TDCCTransfer *dcc )
{
	DCC->Add( dcc );

    if( Listview )
	    AddToListview( dcc );
}
//---------------------------------------------------------------------------
void TDCCMgr::Remove( TDCCTransfer *dcc )
{
	DCC->Remove( dcc );

    delete dcc;
}
//---------------------------------------------------------------------------
void TDCCMgr::SetListView( TdfsExtListView *list )
{
	Listview = list;

    if( list ) {

	    Listview->Items->BeginUpdate();
        Listview->Items->Clear();

    	for( int i = 0; i < DCC->Count; i++ )
    		AddToListview( reinterpret_cast<TDCCTransfer *>( DCC->Items[ i ] ));

	    Listview->Items->EndUpdate();

    } else {

    	for( int i = 0; i < DCC->Count; i++ )
    		reinterpret_cast<TDCCTransfer *>( DCC->Items[ i ] )->SetListItem( NULL );
    }
}
//---------------------------------------------------------------------------
void TDCCMgr::AddToListview( TDCCTransfer *dcc )
{
	TListItem  *item = Listview->Items->Add();

    item->Data    = dcc;
    item->Caption = dcc->GetServer();

    item->SubItems->Add( dcc->GetTarget()   );
    item->SubItems->Add( dcc->GetStatus()   );
    item->SubItems->Add( FloatToStrF( dcc->GetSize(), ffNumber, 20, 0 ));
    item->SubItems->Add( dcc->GetProgress() );
    item->SubItems->Add( "" );
    item->SubItems->Add( dcc->GetFile() );

    dcc->SetListItem( item );
}
//---------------------------------------------------------------------------
