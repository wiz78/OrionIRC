/* $Id: DCCMgrWnd.h,v 1.9 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DCCMgrWndH
#define DCCMgrWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "EnhListView.hpp"
#include "ExtListView.hpp"
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
#include "tcpsocket.h"

class TServerWin;
class TDCCTransfer;

class TDCCMgrWin : public TForm
{
__published:	// IDE-managed Components
	TdfsExtListView *List;
	TPopupMenu *ListPopup;
	TMenuItem *CancelMenu;
	TMenuItem *DeleteFileMenu;
	TMenuItem *N1;
	TMenuItem *OpenFileMenu;
	TMenuItem *N2;
	TMenuItem *TidyUpMenu;
	TFormStorage *FormStorage1;
	TMenuItem *ResendMenu;
	TPopupMenu *TaskbarPopup;
	TMenuItem *Tidyup1;
	TMenuItem *N3;
	TMenuItem *Closewindow1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OpenFileMenuClick(TObject *Sender);
	void __fastcall TidyUpMenuClick(TObject *Sender);
	void __fastcall CancelMenuClick(TObject *Sender);
	void __fastcall ResendMenuClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DeleteFileMenuClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	
	void __fastcall ListMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Closewindow1Click(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	TSpeedButton	   		   *TaskbarButton;

	void __fastcall 			Add( TDCCTransfer *dcc );
    TDCCTransfer * __fastcall	FindFromPort( int port );

public:		// User declarations
	__fastcall 			TDCCMgrWin(TComponent* Owner);

    void __fastcall 	Send( TServerWin *server, AnsiString target, AnsiString file );
    void __fastcall		Receive( TServerWin *server, AnsiString nick, AnsiString file,
    							 int size, Prom_Addr *addr, int port, bool resume );
    bool __fastcall		SetResumeFrom( int port, int from );
    void __fastcall		Resume( int port );
};
//---------------------------------------------------------------------------
extern PACKAGE TDCCMgrWin *DCCMgrWin;
//---------------------------------------------------------------------------
#endif
