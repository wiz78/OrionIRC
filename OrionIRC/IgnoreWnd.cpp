/* $Id: IgnoreWnd.cpp,v 1.3 2003/12/31 16:16:04 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IgnoreWnd.h"
#include "Ignore.h"
#include "Prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TIgnoreWin *IgnoreWin;
//---------------------------------------------------------------------------
__fastcall TIgnoreWin::TIgnoreWin(TComponent* Owner)
	: TForm(Owner)
{
	IgnoreNotify = false;
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::FormShow(TObject *Sender)
{
	IgnoreMgr *mgr = Prefs.GetIgnore();

    Enabled->Checked = mgr->IsEnabled();

    List->Items->BeginUpdate();

    for( int i = 0; i < mgr->GetCount(); i++ )
    	AddItem( new TIgnore( mgr->Get( i )));

    List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action    = caFree;
	IgnoreWin = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::AddItem( TIgnore *ign )
{
	TListItem  *item = List->Items->Add();

    item->Caption = ign->GetMask();
    item->Data    = ign;
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::MaskChange(TObject *Sender)
{
	TListItem *item = List->Selected;

    if( item ) {
    	TIgnore	*ign = reinterpret_cast<TIgnore *>( item->Data );

        ign->SetMask( Mask->Text );

        item->Caption = Mask->Text;
    }

	Add->Enabled = !Mask->Text.IsEmpty();
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::ListClick(TObject *Sender)
{
	TListItem *item = List->Selected;

    if( item ) {
    	TIgnore	*ign = reinterpret_cast<TIgnore *>( item->Data );

        IgnoreNotify      = true;

    	Mask->Text 		  = ign->GetMask();
        Exclude->Checked  = ign->IsExcluded();
    	Channel->Checked  = ign->MatchEvent( IGN_CHANNEL      );
    	Private->Checked  = ign->MatchEvent( IGN_PRIVATE      );
		Notices->Checked  = ign->MatchEvent( IGN_NOTICE       );
    	CTCP->Checked     = ign->MatchEvent( IGN_CTCP         );
    	Invite->Checked   = ign->MatchEvent( IGN_INVITE       );
    	DCCFiles->Checked = ign->MatchEvent( IGN_DCC_FILE     );
		DCCChat->Checked  = ign->MatchEvent( IGN_DCC_CHAT     );
    	Actions->Checked  = ign->MatchEvent( IGN_ACTION       );
    	Page->Checked     = ign->MatchEvent( IGN_PAGE         );
    	Codes->Checked    = ign->MatchEvent( IGN_CONTROL_CODE );

        IgnoreNotify      = false;
    }

    Remove->Enabled = item != NULL;
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::ListDeletion(TObject *Sender, TListItem *Item)
{
	delete reinterpret_cast<TIgnore *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::RemoveClick(TObject *Sender)
{
	TListItem *item = List->Selected;

    if( item )
    	item->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::AddClick(TObject *Sender)
{
	TIgnore *ign = new TIgnore();

    ign->SetMask( Mask->Text );

    GetData( ign );
    AddItem( ign );
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::GetData( TIgnore *ign )
{
    if( !IgnoreNotify ) {
		int events = 0;

        if( Channel->Checked )
            events |= IGN_CHANNEL;

        if( Private->Checked )
            events |= IGN_PRIVATE;

        if( Notices->Checked )
            events |= IGN_NOTICE;

        if( CTCP->Checked )
            events |= IGN_CTCP;

        if( Invite->Checked )
            events |= IGN_INVITE;

        if( DCCFiles->Checked )
            events |= IGN_DCC_FILE;

        if( DCCChat->Checked )
            events |= IGN_DCC_CHAT;

        if( Actions->Checked )
            events |= IGN_ACTION;

        if( Page->Checked )
            events |= IGN_PAGE;

        if( Codes->Checked )
            events |= IGN_CONTROL_CODE;

        ign->SetEvents( events );
        ign->SetExcluded( Exclude->Checked );
    }
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::ExcludeClick(TObject *Sender)
{
	TListItem *item = List->Selected;

    if( item ) {
    	TIgnore	*ign = reinterpret_cast<TIgnore *>( item->Data );

        GetData( ign );
    }
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TIgnoreWin::OkClick(TObject *Sender)
{
	IgnoreMgr *mgr = Prefs.GetIgnore();

    Close();
    
    mgr->Clear();
    mgr->SetEnabled( Enabled->Checked );

    for( int i = 0; i < List->Items->Count; i++ )
    	mgr->Add( new TIgnore( reinterpret_cast<TIgnore *>( List->Items->Item[ i ]->Data )));

    Prefs.Save();
}
//---------------------------------------------------------------------------
