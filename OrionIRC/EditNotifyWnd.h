/* $Id: EditNotifyWnd.h,v 1.1 2003/07/14 18:56:20 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef EditNotifyWndH
#define EditNotifyWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TNotify;
class TEditNotifyWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TLabel *Label1;
	TButton *Ok;
	TButton *Cancel;
	TEdit *Nick;
	TFormStorage *FormStorage1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TNotify		   *Item;
public:		// User declarations
	__fastcall 		TEditNotifyWin( TComponent* Owner, TNotify *rec );
};
//---------------------------------------------------------------------------
extern PACKAGE TEditNotifyWin *EditNotifyWin;
//---------------------------------------------------------------------------
#endif
