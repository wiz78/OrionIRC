/* $Id: DCCSendOfferWnd.cpp,v 1.7 2003/05/11 17:30:47 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DCCSendOfferWnd.h"
#include "DCCMgrWnd.h"
#include "ServerWnd.h"
#include "DCCTransfer.h"
#include "Main.h"
#include "Prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ToolEdit"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TDCCSendOfferWin *DCCSendOfferWin;
//---------------------------------------------------------------------------
__fastcall TDCCSendOfferWin::TDCCSendOfferWin(TComponent* Owner, Prom_Addr *addr, int port, HostMask& mask, AnsiString file, int size )
	: TForm(Owner)
{
	Server = (TServerWin *)Owner;
	Addr   = *addr;
    Port   = port;
    Nick   = mask.GetNick();
    Size   = size;

    Label->Caption = Nick + " (" + mask.GetUserHost() + ") would like to send you a file.";

	FileOffered->Caption = file;
    SizeLabel->Caption   = FloatToStrF( size, ffNumber, 20, 0 );
    File->Text           = Prefs.GetDownloadsDir( file ) + file;

    Top  = MainWin->Top + (( MainWin->Height - Height ) / 2 );
    Left = MainWin->Left + (( MainWin->Width - Width ) / 2 );
}
//---------------------------------------------------------------------------
void __fastcall TDCCSendOfferWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TDCCSendOfferWin::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TDCCSendOfferWin::OkClick(TObject *Sender)
{
	MainWin->NeedDCCMgr();
	DCCMgrWin->Receive( Server, Nick, GetFile(), Size, &Addr, Port, Resume->Checked );
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TDCCSendOfferWin::FileChange(TObject *Sender)
{
	Resume->Visible = FileExists( GetFile() );
    Resume->Checked = Resume->Visible && ( TDCCTransfer::GetFileSize( GetFile() ) != Size );
}
//---------------------------------------------------------------------------
AnsiString __fastcall TDCCSendOfferWin::GetFile( void )
{
	AnsiString file = File->Text;

	if(( file[1] == '"' ) && ( file[ file.Length() ] == '"' ))
    	file = file.SubString( 2, file.Length() - 2 );

    return( file );
}
//---------------------------------------------------------------------------

