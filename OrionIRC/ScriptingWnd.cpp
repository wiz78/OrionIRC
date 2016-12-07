/* $Id: ScriptingWnd.cpp,v 1.4 2005/01/23 20:04:11 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ScriptingWnd.h"
#include "Main.h"
#include "OrionScriptInterface.h"
#include "Prefs.h"
#include "IRCList.h"
#include "irc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TScriptingWin *ScriptingWin;
//---------------------------------------------------------------------------
__fastcall TScriptingWin::TScriptingWin(TComponent* Owner)
	: TForm(Owner)
{
	Display         = new TIRCList( this, NULL, Prefs.GetColorSet() );
    Display->Parent = this;
    Display->Align  = alClient;

    Display->SetInputCtrl( Code );
    Display->ShowTimestamp( false );
    Display->ShowNick( true );
    Display->SetAdaptToNick( true );

    ActiveControl = Code;
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::PanelExecuteResize(TObject *Sender)
{
	btExecute->Top  = ( PanelExecute->Height - btExecute->Height ) / 2;
	btExecute->Left = ( PanelExecute->Width - btExecute->Width ) / 2;
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::FormShow(TObject *Sender)
{
    TaskbarButton = MainWin->AddTaskbarButton( "Scripting console", this, TB_SCRIPT, NULL );

	PanelExecuteResize( PanelExecute );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::FormClose(TObject *Sender, TCloseAction &Action)
{
    Action       = caFree;
    ScriptingWin = NULL;

    MainWin->RemoveTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::FormActivate(TObject *Sender)
{
    MainWin->EnableTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::btExecuteClick(TObject *Sender)
{
	if( !Code->Text.IsEmpty() )
		OrionScript::CodeEval( Code->Text );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::AddRow( AnsiString str )
{
	Display->AddRow( NULL, str, IRC_SCRIPT );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::ConstantsClick(TObject *Sender)
{
	OrionScript::CodeEval( "print( \"\n-------------------------------------------------\n\" );"
    					   "print_r( get_defined_constants() );"
                           "print( \"\nDone.\n\" );" );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::VariablesClick(TObject *Sender)
{
	OrionScript::CodeEval( "print( \"\n-------------------------------------------------\n\" );"
    					   "print_r( get_defined_vars() );"
                           "print( \"\nDone.\n\" );" );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::FunctionsClick(TObject *Sender)
{
	OrionScript::CodeEval( "print( \"\n-------------------------------------------------\n\" );"
    					   "print_r( get_defined_functions() );"
                           "print( \"\nDone.\n\" );" );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::ClassesClick(TObject *Sender)
{
	OrionScript::CodeEval( "print( \"\n-------------------------------------------------\n\" );"
    					   "print_r( get_declared_classes() );"
                           "print( \"\nDone.\n\" );" );
}
//---------------------------------------------------------------------------
void __fastcall TScriptingWin::InterfacesClick(TObject *Sender)
{
	OrionScript::CodeEval( "print( \"\n-------------------------------------------------\n\" );"
    					   "print_r( get_declared_interfaces() );"
                           "print( \"\nDone.\n\" );" );
}
//---------------------------------------------------------------------------

