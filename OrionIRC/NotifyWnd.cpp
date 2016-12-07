/* $Id: NotifyWnd.cpp,v 1.5 2004/12/28 16:47:36 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "NotifyWnd.h"
#include "EditNotifyWnd.h"
#include "NotifyMgr.h"
#include "Notify.h"
#include "Prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EnhListView"
#pragma link "ExtListView"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TNotifyWin *NotifyWin;
//---------------------------------------------------------------------------
__fastcall TNotifyWin::TNotifyWin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::FormShow(TObject *Sender)
{
	NotifyMgr  *mgr = Prefs.GetNotifies();

    List->Items->BeginUpdate();

    for( int i = 0; i < mgr->GetCount(); i++ )
    	AddItem( new TNotify( mgr->Get( i )));

    List->AlphaSort();
    List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::FormClose( TObject *Sender, TCloseAction &Action )
{
	Store();

	Action    = caFree;
    NotifyWin = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::CloseButtonClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::PanelBottomResize(TObject *Sender)
{
	Bevel->Width      = PanelBottom->Width - 8;
    CloseButton->Left = PanelBottom->Width - 81;
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::ListClick(TObject *Sender)
{
	bool enable = List->Selected != NULL;

    Edit->Enabled   = enable;
    Delete->Enabled = enable;
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::Store( void )
{
	NotifyMgr  *mgr = Prefs.GetNotifies();

    mgr->Clear();

	for( int i = 0; i < List->Items->Count; i++ ) {
     	TListItem  *item = List->Items->Item[ i ];
		TNotify    *rec = reinterpret_cast<TNotify *>( item->Data );

    	mgr->Add( new TNotify( rec ));
    }

    Prefs.Save();
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::DeleteClick(TObject *Sender)
{
	for( int i = List->Items->Count - 1; i >= 0; i-- ) {
     	TListItem *item = List->Items->Item[ i ];

        if( item->Selected )
			item->Delete();
    }

	Store();
    ListClick( List );
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::EditClick(TObject *Sender)
{
	TListItem *item = List->Selected;

    if( item ) {
		TNotify 	   *rec = reinterpret_cast<TNotify *>( item->Data );
        TEditNotifyWin *win = new TEditNotifyWin( NULL, rec );

        if( win->ShowModal() == mrOk ) {

        	if( item->Caption.AnsiCompareIC( rec->GetNick() ))
            	rec->SetIsOn( false );

        	item->Caption                = rec->GetNick();
            item->SubItems->Strings[ 0 ] = rec->IsOn() ? "yes" : "no";

            item->Update();

            Store();
        }

        delete win;
    }
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::AddClick(TObject *Sender)
{
	TNotify		   *rec = new TNotify();
	TEditNotifyWin *win = new TEditNotifyWin( NULL, rec );

    if(( win->ShowModal() == mrOk ) && !IsInList( rec )) {

        AddItem( rec );
        List->AlphaSort();

        Store();

    } else
    	delete rec;

	delete win;
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::AddItem( TNotify *rec )
{
	TListItem *item = List->Items->Add();

    item->Caption = rec->GetNick();
    item->Data    = rec;

    item->SubItems->Add( rec->IsOn() ? "yes" : "no" );
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::UpdateItem( TNotify *rec )
{
	for( int i = List->Items->Count - 1; i >= 0; i-- ) {
     	TListItem *item = List->Items->Item[ i ];
		TNotify   *tmp = reinterpret_cast<TNotify *>( item->Data );

        // tmp and rec could be different objects
        if( !tmp->GetNick().AnsiCompareIC( rec->GetNick() )) {

			tmp->SetIsOn( rec->IsOn() );

		    item->SubItems->Strings[ 0 ] = rec->IsOn() ? "yes" : "no";

            item->Update();
        }
    }
}
//---------------------------------------------------------------------------
bool __fastcall TNotifyWin::IsInList( TNotify *rec )
{
	bool ret = false;

	for( int i = List->Items->Count - 1; !ret && ( i >= 0 ); i-- ) {
     	TListItem *item = List->Items->Item[ i ];
		TNotify   *tmp = reinterpret_cast<TNotify *>( item->Data );

    	if( !tmp->GetNick().AnsiCompareIC( rec->GetNick() ))
        	ret = true;
    }

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TNotifyWin::ListDeletion(TObject *Sender, TListItem *Item)
{
	delete reinterpret_cast<TNotify *>( Item->Data );
}
//---------------------------------------------------------------------------

