/* $Id: EditNotifyWnd.cpp,v 1.2 2004/12/21 17:01:20 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditNotifyWnd.h"
#include "Notify.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TEditNotifyWin *EditNotifyWin;
//---------------------------------------------------------------------------
__fastcall TEditNotifyWin::TEditNotifyWin(TComponent* Owner, TNotify *rec)
	: TForm(Owner)
{
	Item = rec;
}
//---------------------------------------------------------------------------
void __fastcall TEditNotifyWin::FormShow(TObject *Sender)
{
	Nick->Text = Item->GetNick();

    Nick->SetFocus();	
}
//---------------------------------------------------------------------------
void __fastcall TEditNotifyWin::OkClick(TObject *Sender)
{
	if( Nick->Text.Trim().IsEmpty() )
    	ModalResult = mrCancel;
    else
		Item->SetNick( Nick->Text );	
}
//---------------------------------------------------------------------------
