/* $Id: FavWnd.h,v 1.2 2003/05/25 10:48:29 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FavWndH
#define FavWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFavourite;
class TFavWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TEdit *Channel;
	TEdit *Description;
	TEdit *Network;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *Password;
	void __fastcall OkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	TFavourite	   *Fav;
public:		// User declarations
	__fastcall 		TFavWin( TComponent* Owner, TFavourite *fav );
};
//---------------------------------------------------------------------------
extern PACKAGE TFavWin *FavWin;
//---------------------------------------------------------------------------
#endif
