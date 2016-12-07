/* $Id: FilterWnd.cpp,v 1.5 2005/06/06 18:34:31 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <mmsystem.h>

#include "FilterWnd.h"
#include "Filter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "RxCombos"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TFilterWin *FilterWin;
//---------------------------------------------------------------------------
__fastcall TFilterWin::TFilterWin( TComponent* Owner, TFilter *filter )
	: TForm(Owner)
{
	Filter = filter;
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::FormShow(TObject *Sender)
{
	Pages->ActivePage = Pages->Pages[ 0 ];

	FilterName->Text        = Filter->GetName();
	FilterWhat->Text        = Filter->GetFilter();
   	AllMatchingText->Text   = Filter->GetWinMatching();
   	AllExcludingText->Text  = Filter->GetWinExcluding();
    PlaySoundFileName->Text = Filter->GetSound();

    switch( Filter->GetApply() ) {

    	case FILTER_ALL:
        	FilterAllWindows->Checked = true;
            break;

    	case FILTER_CHANNEL:
        	FilterChanWindows->Checked = true;
            break;

    	case FILTER_QUERY:
        	FilterQueryWindows->Checked = true;
            break;

    	case FILTER_ALL_MATCHING:
        	FilterAllMatching->Checked = true;
            break;

    	case FILTER_ALL_EXCLUDING:
        	FilterAllExcluding->Checked = true;
            break;
    }

	ActionNop->Checked            = Filter->DoNothing();
	ActionHide->Checked           = Filter->DoHide();
	ActionHighlight->Checked      = Filter->DoHighlight();
	ActionChangeColor->Checked    = Filter->DoChangeColor();
    ActionColor->Color   	      = Filter->GetColor();
	ActionPrintActive->Checked    = Filter->DoPrintOnActiveWin();
	ActionPlaySound->Checked      = Filter->DoPlaySound();
    PlaySoundBeep->Checked        = Filter->DoPlayBeep();
    PlaySoundFile->Checked        = !Filter->DoPlayBeep();
    ActionFlashStatusBar->Checked = Filter->DoFlashStatusBar();
	ActionStop->Checked           = Filter->DoStop();

    ActionNopClick( ActionNop );
    ActionPlaySoundClick( ActionPlaySound );
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::OkClick(TObject *Sender)
{
	Filter->SetName( FilterName->Text );
	Filter->SetFilter( FilterWhat->Text );
   	Filter->SetWinMatching( AllMatchingText->Text );
   	Filter->SetWinExcluding( AllExcludingText->Text );
    Filter->SetSound( PlaySoundFileName->Text );

    if( FilterAllWindows->Checked )
    	Filter->SetApply( FILTER_ALL );
	else if( FilterChanWindows->Checked )
    	Filter->SetApply( FILTER_CHANNEL );
    else if( FilterQueryWindows->Checked )
    	Filter->SetApply( FILTER_QUERY );
    else if( FilterAllMatching->Checked )
    	Filter->SetApply( FILTER_ALL_MATCHING );
    else if( FilterAllExcluding->Checked )
    	Filter->SetApply( FILTER_ALL_EXCLUDING );

	Filter->SetNothing( ActionNop->Checked );
	Filter->SetHide( ActionHide->Checked );
	Filter->SetHighlight( ActionHighlight->Checked );
	Filter->SetChangeColor( ActionChangeColor->Checked );
	Filter->SetColor( ActionColor->Color );
	Filter->SetPrintOnActiveWin( ActionPrintActive->Checked );
	Filter->SetPlaySound( ActionPlaySound->Checked );
	Filter->SetPlayBeep( PlaySoundBeep->Checked );
    Filter->SetFlashStatusBar( ActionFlashStatusBar->Checked );
	Filter->SetStop( ActionStop->Checked );
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::FilterAllWindowsClick(TObject *Sender)
{
	AllMatchingText->Enabled  = FilterAllMatching->Checked;
	AllExcludingText->Enabled = FilterAllExcluding->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::ActionNopClick(TObject *Sender)
{
	bool enable = !ActionNop->Checked;

    ActionHide->Enabled           = enable;
    ActionHighlight->Enabled      = enable;
    ActionChangeColor->Enabled    = enable;
    ActionPrintActive->Enabled    = enable;
    ActionPlaySound->Enabled      = enable;
    ActionFlashStatusBar->Enabled = enable;

    if( enable ) {

	    ActionPlaySoundClick( ActionPlaySound );
        ActionChangeColorClick( ActionChangeColor );

    } else {

		PlaySoundBeep->Enabled     = false;
		PlaySoundFile->Enabled     = false;
		PlaySoundFileName->Enabled = false;
		PlaySoundTest->Enabled     = false;
		ActionColor->Enabled       = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::ActionPlaySoundClick(TObject *Sender)
{
	PlaySoundBeep->Enabled = ActionPlaySound->Checked;
	PlaySoundFile->Enabled = ActionPlaySound->Checked;

    if( ActionPlaySound->Checked )
    	PlaySoundFileClick( PlaySoundFile );
	else {
		PlaySoundFileName->Enabled = false;
		PlaySoundTest->Enabled     = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::PlaySoundFileClick(TObject *Sender)
{
	PlaySoundFileName->Enabled = PlaySoundFile->Checked;
	PlaySoundTest->Enabled     = PlaySoundFile->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::PlaySoundTestClick(TObject *Sender)
{
	PlaySound( PlaySoundFileName->Text.c_str(), NULL, SND_FILENAME | SND_ASYNC ); 
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::ActionChangeColorClick(TObject *Sender)
{
	ActionColor->Enabled = ActionChangeColor->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFilterWin::ActionColorClick(TObject *Sender)
{
	ColorDlg->Color = ActionColor->Color;

    if( ColorDlg->Execute() )
    	ActionColor->Color = ColorDlg->Color;
}
//---------------------------------------------------------------------------

