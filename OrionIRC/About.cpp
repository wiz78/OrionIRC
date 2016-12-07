/* $Id: About.cpp,v 1.7 2003/08/01 20:22:01 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <shellapi.h>

#include "About.h"
#include "Version.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TAboutWin *AboutWin;
//---------------------------------------------------------------------------
__fastcall TAboutWin::TAboutWin(TComponent* Owner)
    : TForm(Owner)
{
	Ver->Caption       = "v" + Version.GetVersionStr();
	Build->Caption     = Format( "Release: %d Build: %d",
                                 ARRAYOFCONST(( Version.GetRelease(), Version.GetBuild() )));
    Copyright->Caption = "Copyright " + Version.GetCopyright();
}
//---------------------------------------------------------------------------
void __fastcall TAboutWin::HomepageClick(TObject *Sender)
{
    ShellExecute( Handle, "open", "http://tellini.info/index_en.html", NULL, NULL, FALSE );
}
//---------------------------------------------------------------------------
void __fastcall TAboutWin::OrionHotspotClick(TObject *Sender)
{
    ShellExecute( Handle, "open", "http://tellini.info/software/orion/", NULL, NULL, FALSE );
}
//---------------------------------------------------------------------------
void __fastcall TAboutWin::OkClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAboutWin::FormShow(TObject *Sender)
{
	Caption = "About - " + Version.GetProductName();
}
//---------------------------------------------------------------------------

