/* $Id: URLWnd.cpp,v 1.1 2003/05/13 22:16:36 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "URLWnd.h"
#include "URL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TURLWin *URLWin;
//---------------------------------------------------------------------------
__fastcall TURLWin::TURLWin( TComponent* Owner, TURL *url )
	: TForm(Owner)
{
	URL = url;
}
//---------------------------------------------------------------------------
void __fastcall TURLWin::FormShow(TObject *Sender)
{
	Address->Text     = URL->GetURL();
    Description->Text = URL->GetDescription();
	User->Text        = URL->GetUser();
}
//---------------------------------------------------------------------------
void __fastcall TURLWin::OkClick(TObject *Sender)
{
	URL->SetURL( Address->Text );
    URL->SetDescription( Description->Text );
	URL->SetUser( User->Text );
}
//---------------------------------------------------------------------------
