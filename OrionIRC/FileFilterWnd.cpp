/* $Id: FileFilterWnd.cpp,v 1.1 2003/05/11 17:30:47 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FileFilterWnd.h"
#include "FileFilter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TFileFilterWin *FileFilterWin;
//---------------------------------------------------------------------------
__fastcall TFileFilterWin::TFileFilterWin( TComponent *Owner, TFileFilter *filter )
	: TForm(Owner)
{
	Filter = filter;
}
//---------------------------------------------------------------------------
void __fastcall TFileFilterWin::SaveClick(TObject *Sender)
{
	Directory->Enabled = Save->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFileFilterWin::FormShow(TObject *Sender)
{
	Name->Text       = Filter->GetName();
    Extensions->Text = Filter->GetExtensions();
    Directory->Text  = Filter->GetDirectory();

    if( Filter->GetAction() == FILE_IGNORE )
    	Ignore->Checked = true;
    else
    	Save->Checked = true;

    SaveClick( Save );
}
//---------------------------------------------------------------------------
void __fastcall TFileFilterWin::OkClick(TObject *Sender)
{
	Filter->SetName( Name->Text );
    Filter->SetExtensions( Extensions->Text );
    Filter->SetDirectory( Directory->Text );
    Filter->SetAction( Ignore->Checked ? FILE_IGNORE : FILE_SAVE );
}
//---------------------------------------------------------------------------

