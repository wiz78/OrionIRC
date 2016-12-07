/* $Id: ScriptingWnd.h,v 1.2 2005/01/23 19:47:15 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ScriptingWndH
#define ScriptingWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TIRCList;

class TScriptingWin : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TSplitter *Splitter1;
	TMemo *Code;
	TPanel *PanelExecute;
	TButton *btExecute;
	TToolBar *ToolBar;
	TToolButton *Constants;
	TToolButton *ToolButton1;
	TToolButton *Variables;
	TToolButton *ToolButton3;
	TToolButton *Functions;
	TToolButton *ToolButton5;
	TToolButton *Classes;
	TToolButton *ToolButton2;
	TToolButton *Interfaces;
	void __fastcall PanelExecuteResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btExecuteClick(TObject *Sender);
	
	
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ConstantsClick(TObject *Sender);
	void __fastcall VariablesClick(TObject *Sender);
	void __fastcall FunctionsClick(TObject *Sender);
	void __fastcall ClassesClick(TObject *Sender);
	void __fastcall InterfacesClick(TObject *Sender);
private:	// User declarations
	TIRCList	   *Display;
    TSpeedButton   *TaskbarButton;

public:		// User declarations
	__fastcall 		TScriptingWin(TComponent* Owner);
    void __fastcall	AddRow( AnsiString str );
};
//---------------------------------------------------------------------------
extern PACKAGE TScriptingWin *ScriptingWin;
//---------------------------------------------------------------------------
#endif
