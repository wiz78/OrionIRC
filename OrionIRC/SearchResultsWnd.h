/* $Id: SearchResultsWnd.h,v 1.1 2005/02/20 18:41:02 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef SearchResultsWndH
#define SearchResultsWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TIRCList;

class TSearchResultsWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TPopupMenu *PopupMenu;
	TMenuItem *Savebuffer1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Savebuffer1Click(TObject *Sender);
private:	// User declarations
	TIRCList   	   	   *Display;

	void __fastcall		Fill( TList *rows );

public:
	__fastcall 			TSearchResultsWin( TComponent *Owner, TList *rows );
};
//---------------------------------------------------------------------------
extern PACKAGE TSearchResultsWin *SearchResultsWin;
//---------------------------------------------------------------------------
#endif
