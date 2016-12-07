/* $Id: URLCatcherWnd.cpp,v 1.5 2005/09/17 15:40:15 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "URLCatcherWnd.h"
#include "URLWnd.h"
#include "URL.h"
#include "Main.h"
#include "Prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EnhListView"
#pragma link "ExtListView"
#pragma link "Placemnt"
#pragma link "DropSource"
#pragma link "DropURLSource"
#pragma resource "*.dfm"
TURLCatcherWin *URLCatcherWin;
//---------------------------------------------------------------------------
__fastcall TURLCatcherWin::TURLCatcherWin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::PanelBottomResize(TObject *Sender)
{
	Bevel->Width      = PanelBottom->Width - 8;
    CloseButton->Left = PanelBottom->Width - 81;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::FormShow(TObject *Sender)
{
	Dragging = false;

	PanelBottomResize( PanelBottom );

    for( int i = 0; i < MainWin->URLCatcher.Count(); i++ )
    	AddItem( MainWin->URLCatcher.Get( i ));

	List->ReadSettings();
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	List->WriteSettings();

	Action        = caFree;
    URLCatcherWin = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::CloseButtonClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::AddItem( TURL *url )
{
	TListItem  *item = List->Items->Add();

    item->Data    = url;
    item->Caption = url->GetDescription();

    item->SubItems->Add( url->GetUser() );
    item->SubItems->Add( url->GetDate().FormatString( "ddddd t" ));
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::DeleteClick(TObject *Sender)
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
		TListItem  *item = List->Items->Item[ i ];

	    if( item->Selected ) {

    		MainWin->URLCatcher.Delete( reinterpret_cast<TURL *>( item->Data ));

        	item->Delete();
        }
    }

	List->Items->EndUpdate();
    ListClick( List );
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::ListClick(TObject *Sender)
{
	bool	enable = List->Selected != NULL;

	Launch->Enabled = enable;
	Edit->Enabled   = enable;
	Delete->Enabled = enable;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::ClearClick(TObject *Sender)
{
	List->Items->Clear();
    MainWin->URLCatcher.Clear();
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::LaunchClick(TObject *Sender)
{
	TListItem  *item = List->Selected;

    if( item ) {
		TURL *url = reinterpret_cast<TURL *>( item->Data );

        Prefs.LaunchURL( url->GetURL() );
    }
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::ListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TListItem  *item = List->Selected;

    if( item )
		Dragging = true;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::ListMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Dragging = false;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::ListMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	TListItem  *item = List->Selected;

    if( item && Dragging ) {
		TURL *url = reinterpret_cast<TURL *>( item->Data );

    	URLSrc->Title = url->GetDescription();
    	URLSrc->URL   = url->GetURL();

        URLSrc->Execute();

        Dragging = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::AddClick(TObject *Sender)
{
	TURL   	   *url = new TURL();
    TURLWin    *wnd = new TURLWin( NULL, url );

    if( wnd->ShowModal() == mrOk )
	   	MainWin->URLCatcher.Add( url );
    else
    	delete url;

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::EditClick(TObject *Sender)
{
	TListItem  *item = List->Selected;

    if( item  ) {
		TURL 	*url = reinterpret_cast<TURL *>( item->Data );
	    TURLWin *wnd = new TURLWin( NULL, url );

        if( wnd->ShowModal() == mrOk ) {

        	item->Caption                = url->GetDescription();
            item->SubItems->Strings[ 0 ] = url->GetUser();
        }

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch( MapVirtualKey( Key, 2 )) {

    	case 'A':
        case 'a':
        	if( Shift.Contains( ssCtrl ))
            	SelectAll();
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TURLCatcherWin::SelectAll( void )
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
		TListItem  *item = List->Items->Item[ i ];

        item->Selected = true;
    }

	List->Items->EndUpdate();
    ListClick( List );
}
//---------------------------------------------------------------------------
