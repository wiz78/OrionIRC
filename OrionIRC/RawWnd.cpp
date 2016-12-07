/* $Id: RawWnd.cpp,v 1.3 2004/12/19 18:46:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RawWnd.h"

#define MAX_LINES 500

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TRawWin *RawWin;
//---------------------------------------------------------------------------
__fastcall TRawWin::TRawWin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRawWin::AddLine( const char *line )
{
	Display->Lines->Add( line );

    if( Display->Lines->Count > MAX_LINES )
    	Display->Lines->Delete( 0 );

    Display->SelStart = Display->Lines->Count - 1;
}
//---------------------------------------------------------------------------
void __fastcall TRawWin::Button1Click(TObject *Sender)
{
	Display->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TRawWin::Button2Click(TObject *Sender)
{
	if( SaveDlg->Execute() )
    	Display->Lines->SaveToFile( SaveDlg->FileName );
}
//---------------------------------------------------------------------------

