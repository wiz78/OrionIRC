/* $Id: About.h,v 1.6 2003/04/15 20:56:21 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TAboutWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TLabel *Copyright;
	TLabel *Ver;
	TImage *Image1;
	TLabel *Homepage;
	TLabel *Build;
	TLabel *OrionHotspot;
    void __fastcall HomepageClick(TObject *Sender);
    void __fastcall OkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OrionHotspotClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TAboutWin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutWin *AboutWin;
//---------------------------------------------------------------------------
#endif
