/* $Id: EditAliasWnd.h,v 1.1 2005/01/23 19:16:43 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef EditAliasWndH
#define EditAliasWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAlias;

class TEditAliasWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TBevel *Bevel13;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label34;
	TEdit *Alias;
	TEdit *Command;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TAlias	   		   *Item;

public:		// User declarations
	__fastcall 			TEditAliasWin( TComponent* Owner, TAlias *item );
    TAlias * __fastcall	GetItem( void ) const;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditAliasWin *EditAliasWin;
//---------------------------------------------------------------------------
#endif
