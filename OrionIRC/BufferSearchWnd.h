/* $Id: BufferSearchWnd.h,v 1.1 2005/02/20 18:41:02 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef BufferSearchWndH
#define BufferSearchWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TIRCList;

class TBufferSearchWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TLabel *Label1;
	TEdit *TextToFind;
	TCheckBox *RegExp;
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TIRCList	   *List;

	void __fastcall PrepareRegExp( pcre **re, pcre_extra **rex );
	void __fastcall DoSearch( pcre *re, pcre_extra *rex, TList *results );

public:		// User declarations
	__fastcall 		TBufferSearchWin( TComponent *Owner, TIRCList *list );
};
//---------------------------------------------------------------------------
extern PACKAGE TBufferSearchWin *BufferSearchWin;
//---------------------------------------------------------------------------
#endif
