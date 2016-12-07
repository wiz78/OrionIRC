/* $Id: FavouritesWnd.cpp,v 1.5 2003/07/27 14:21:18 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FavouritesWnd.h"
#include "FavWnd.h"
#include "Favourite.h"
#include "Prefs.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EnhListView"
#pragma link "ExtListView"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TFavouritesWin *FavouritesWin;
//---------------------------------------------------------------------------
__fastcall TFavouritesWin::TFavouritesWin( TComponent* Owner, AnsiString server )
	: TForm(Owner)
{
	Server = server;  
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::PanelBottomResize(TObject *Sender)
{
	Bevel->Width      = PanelBottom->Width - 8;
    CloseButton->Left = PanelBottom->Width - 81;
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::ListClick(TObject *Sender)
{
	bool	enable = List->Selected != NULL;

    Edit->Enabled   = enable;
    Delete->Enabled = enable;
    Join->Enabled   = enable;
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::DeleteClick(TObject *Sender)
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
		TListItem  *item = List->Items->Item[ i ];

    	if( item->Selected )
    		item->Delete();
    }

	List->Items->EndUpdate();
    ListClick( List );
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::ListDeletion(TObject *Sender,
      TListItem *Item)
{
	delete reinterpret_cast<TFavourite *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::AddClick(TObject *Sender)
{
	TFavourite *fav = new TFavourite();
    TFavWin    *wnd = new TFavWin( NULL, fav );

    if( wnd->ShowModal() == mrOk ) {
    	TListItem  *item = List->Items->Add();

        item->Data    = fav;
        item->Caption = fav->GetChannel();

        item->SubItems->Add( fav->GetDescription() );
        item->SubItems->Add( fav->GetNetwork() );

    } else
    	delete fav;

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::EditClick(TObject *Sender)
{
	TListItem  *item = List->Selected;

    if( item ) {
    	TFavourite *fav = reinterpret_cast<TFavourite *>( item->Data );
        TFavWin    *wnd = new TFavWin( NULL, fav );

        if( wnd->ShowModal() == mrOk ) {

            item->Caption = fav->GetChannel();

            item->SubItems->Strings[ 0 ] = fav->GetDescription();
            item->SubItems->Strings[ 1 ] = fav->GetNetwork();

            item->Update();
        }

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caFree;

	List->WriteSettings();

    Prefs.GetFavourites()->Clear();

	for( int i = 0; i < List->Items->Count; i++ )
		Prefs.GetFavourites()->Add( new TFavourite( reinterpret_cast<TFavourite *>( List->Items->Item[ i ]->Data )));

    Prefs.SetPopupFavOnConnect( ShowOnConnect->Checked );
    Prefs.Save();

    FavouritesWin = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::FormShow(TObject *Sender)
{
	List->ReadSettings();
	List->Items->BeginUpdate();

	for( int i = 0; i < Prefs.GetFavourites()->GetCount(); i++ ) {
     	TListItem  *item = List->Items->Add();
        TFavourite *fav = new TFavourite( Prefs.GetFavourites()->Get( i ));

        item->Data    = fav;
        item->Caption = fav->GetChannel();

        item->SubItems->Add( fav->GetDescription() );
        item->SubItems->Add( fav->GetNetwork() );
    }

	List->Items->EndUpdate();

    Join->Enabled          = !Server.IsEmpty();
    ShowOnConnect->Checked = Prefs.PopupFavOnConnect();
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::SetServer( AnsiString server )
{
	Server = server;

    Join->Enabled = !Server.IsEmpty();
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::JoinClick(TObject *Sender)
{
	TListItem  *item = List->Selected;

    if( item ) {
    	TFavourite *fav = reinterpret_cast<TFavourite *>( item->Data );
		TServerWin *server = MainWin->FindServerWin( Server );

    	if( server ) {
        	AnsiString	cmd, pwd;

            cmd = "JOIN " + fav->GetChannel();
            pwd = fav->GetPassword();

            if( !pwd.IsEmpty() )
				cmd += " " + pwd;

    		server->SendCmd( cmd );
        }

		Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TFavouritesWin::ListDblClick(TObject *Sender)
{
	if( Join->Enabled )
    	JoinClick( Join );
    else
    	EditClick( Edit );
}
//---------------------------------------------------------------------------

