/* $Id: AskRemoveWnd.cpp,v 1.2 2003/05/06 14:45:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AskRemoveWnd.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAskRemoveWin *AskRemoveWin;
//---------------------------------------------------------------------------
__fastcall TAskRemoveWin::TAskRemoveWin(TComponent* Owner)
    : TForm(Owner)
{
    ReturnCode = ARR_CANCEL;
}
//---------------------------------------------------------------------------
void __fastcall TAskRemoveWin::CancelClick(TObject *Sender)
{
    ReturnCode = ARR_CANCEL;
}
//---------------------------------------------------------------------------
void __fastcall TAskRemoveWin::RemoveClick(TObject *Sender)
{
    ReturnCode = ARR_YES;
}
//---------------------------------------------------------------------------
void __fastcall TAskRemoveWin::NoClick(TObject *Sender)
{
    ReturnCode = ARR_NO;
}
//---------------------------------------------------------------------------
void __fastcall TAskRemoveWin::YesAllClick(TObject *Sender)
{
    ReturnCode = ARR_YES_ALL;
}
//---------------------------------------------------------------------------
int __fastcall TAskRemoveWin::Execute( void )
{
    ShowModal();

    return( ReturnCode );
}
//---------------------------------------------------------------------------
void __fastcall TAskRemoveWin::FormShow(TObject *Sender)
{
    Top  = MainWin->Top + (( MainWin->Height - Height ) / 2 );
    Left = MainWin->Left + (( MainWin->Width - Width ) / 2 );
}
//---------------------------------------------------------------------------

