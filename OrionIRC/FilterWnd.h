/* $Id: FilterWnd.h,v 1.4 2005/06/06 18:34:31 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FilterWndH
#define FilterWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "RxCombos.hpp"
#include "ToolEdit.hpp"
#include <Buttons.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFilter;

class TFilterWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TPageControl *Pages;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TLabel *Label1;
	TEdit *FilterName;
	TLabel *Label2;
	TEdit *FilterWhat;
	TLabel *Label3;
	TRadioButton *FilterAllWindows;
	TRadioButton *FilterChanWindows;
	TRadioButton *FilterQueryWindows;
	TRadioButton *FilterAllMatching;
	TRadioButton *FilterAllExcluding;
	TCheckBox *ActionNop;
	TCheckBox *ActionHide;
	TCheckBox *ActionChangeColor;
	TCheckBox *ActionPrintActive;
	TCheckBox *ActionStop;
	TEdit *AllMatchingText;
	TEdit *AllExcludingText;
	TCheckBox *ActionHighlight;
	TCheckBox *ActionPlaySound;
	TPanel *PanelSound;
	TRadioButton *PlaySoundBeep;
	TRadioButton *PlaySoundFile;
	TFilenameEdit *PlaySoundFileName;
	TSpeedButton *PlaySoundTest;
	TColorDialog *ColorDlg;
	TPanel *ActionColor;
	TCheckBox *ActionFlashStatusBar;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FilterAllWindowsClick(TObject *Sender);
	void __fastcall ActionNopClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall ActionPlaySoundClick(TObject *Sender);
	void __fastcall PlaySoundFileClick(TObject *Sender);
	void __fastcall PlaySoundTestClick(TObject *Sender);
	void __fastcall ActionChangeColorClick(TObject *Sender);
	void __fastcall ActionColorClick(TObject *Sender);
private:	// User declarations
	TFilter		   *Filter;

public:		// User declarations
	__fastcall 		TFilterWin( TComponent* Owner, TFilter *filter );
};
//---------------------------------------------------------------------------
extern PACKAGE TFilterWin *FilterWin;
//---------------------------------------------------------------------------
#endif
