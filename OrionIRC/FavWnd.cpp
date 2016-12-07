/* $Id: FavWnd.cpp,v 1.2 2003/05/25 10:48:29 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FavWnd.h"
#include "Favourite.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TFavWin *FavWin;
//---------------------------------------------------------------------------
__fastcall TFavWin::TFavWin( TComponent* Owner, TFavourite *fav )
	: TForm(Owner)
{
	Fav = fav;
}
//---------------------------------------------------------------------------
void __fastcall TFavWin::OkClick(TObject *Sender)
{
	Fav->SetChannel( Channel->Text );
    Fav->SetDescription( Description->Text );
    Fav->SetNetwork( Network->Text );
    Fav->SetPassword( Password->Text );
}
//---------------------------------------------------------------------------
void __fastcall TFavWin::FormShow(TObject *Sender)
{
	Channel->Text     = Fav->GetChannel();
	Description->Text = Fav->GetDescription();
	Network->Text     = Fav->GetNetwork();
    Password->Text    = Fav->GetPassword();
}
//---------------------------------------------------------------------------
