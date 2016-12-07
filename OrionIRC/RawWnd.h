/* $Id: RawWnd.h,v 1.2 2003/02/27 18:39:01 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef RawWndH
#define RawWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TRawWin : public TForm
{
__published:	// IDE-managed Components
	TMemo *Display;
	TPanel *PanelTop;
	TButton *Button1;
	TButton *Button2;
	TSaveDialog *SaveDlg;
	TFormStorage *FormStorage1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall 		TRawWin(TComponent* Owner);
	void __fastcall AddLine( const char *line );
};
//---------------------------------------------------------------------------
extern PACKAGE TRawWin *RawWin;
//---------------------------------------------------------------------------
#endif
