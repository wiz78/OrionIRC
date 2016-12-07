/* $Id: EditAliasWnd.cpp,v 1.1 2005/01/23 19:16:43 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditAliasWnd.h"
#include "Alias.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TEditAliasWin *EditAliasWin;
//---------------------------------------------------------------------------
__fastcall TEditAliasWin::TEditAliasWin( TComponent* Owner, TAlias *item )
	: TForm(Owner)
{
	Item = item;
}
//---------------------------------------------------------------------------
void __fastcall TEditAliasWin::FormShow(TObject *Sender)
{
	if( Item ) {

		Alias->Text   = Item->GetName();
        Command->Text = Item->GetCommand();
    }
}
//---------------------------------------------------------------------------
void __fastcall TEditAliasWin::CancelClick(TObject *Sender)
{
	Item = NULL;	
}
//---------------------------------------------------------------------------
void __fastcall TEditAliasWin::OkClick(TObject *Sender)
{
	if( !Item )
    	Item = new TAlias();

    Item->SetName( Alias->Text );
    Item->SetCommand( Command->Text );
}
//---------------------------------------------------------------------------
TAlias * __fastcall	TEditAliasWin::GetItem( void ) const
{
	return( Item );
}
//---------------------------------------------------------------------------

