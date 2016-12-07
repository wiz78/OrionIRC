/* $Id: SplashWnd.cpp,v 1.2 2003/04/15 20:56:21 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SplashWnd.h"
#include "Version.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplashWin *SplashWin;
//---------------------------------------------------------------------------
__fastcall TSplashWin::TSplashWin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplashWin::TimerTimer(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TSplashWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TSplashWin::FormShow(TObject *Sender)
{
	Ver->Caption       = "v" + Version.GetVersionStr();
    Copyright->Caption = "Copyright " + Version.GetCopyright();
}
//---------------------------------------------------------------------------


