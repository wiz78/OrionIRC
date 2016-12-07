/* $Id: URLCatcherWnd.h,v 1.3 2005/09/17 15:40:15 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef URLCatcherWndH
#define URLCatcherWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EnhListView.hpp"
#include "ExtListView.hpp"
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "DropSource.h"
#include "DropURLSource.hpp"
//---------------------------------------------------------------------------
class TURL;
class TURLCatcherWin : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TBevel *Bevel;
	TButton *CloseButton;
	TFormStorage *FormStorage1;
	TPanel *PanelRight;
	TButton *Add;
	TButton *Edit;
	TButton *Delete;
	TButton *Launch;
	TdfsExtListView *List;
	TButton *Clear;
	TDropURLSource *URLSrc;
	void __fastcall PanelBottomResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall ClearClick(TObject *Sender);
	void __fastcall LaunchClick(TObject *Sender);
	void __fastcall ListMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ListMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall ListMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall EditClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
	bool			Dragging;

	void __fastcall SelectAll( void );

public:		// User declarations
	__fastcall 		TURLCatcherWin(TComponent* Owner);
	void __fastcall AddItem( TURL *url );
};
//---------------------------------------------------------------------------
extern PACKAGE TURLCatcherWin *URLCatcherWin;
//---------------------------------------------------------------------------
#endif
