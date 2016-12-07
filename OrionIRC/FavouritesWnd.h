/* $Id: FavouritesWnd.h,v 1.1 2003/05/09 21:50:52 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FavouritesWndH
#define FavouritesWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "EnhListView.hpp"
#include "ExtListView.hpp"
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFavouritesWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TPanel *PanelBottom;
	TBevel *Bevel;
	TButton *CloseButton;
	TCheckBox *ShowOnConnect;
	TPanel *PanelRight;
	TdfsExtListView *List;
	TButton *Add;
	TButton *Edit;
	TButton *Delete;
	TButton *Join;
	void __fastcall PanelBottomResize(TObject *Sender);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall ListDeletion(TObject *Sender, TListItem *Item);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall EditClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall JoinClick(TObject *Sender);
	void __fastcall ListDblClick(TObject *Sender);
private:	// User declarations
	AnsiString	Server;

public:		// User declarations
	__fastcall 		TFavouritesWin( TComponent* Owner, AnsiString server );
	void __fastcall SetServer( AnsiString server );
};
//---------------------------------------------------------------------------
extern PACKAGE TFavouritesWin *FavouritesWin;
//---------------------------------------------------------------------------
#endif
