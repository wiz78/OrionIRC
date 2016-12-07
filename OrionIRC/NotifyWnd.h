/* $Id: NotifyWnd.h,v 1.4 2004/12/28 16:47:36 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef NotifyWndH
#define NotifyWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EnhListView.hpp"
#include "ExtListView.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "Placemnt.hpp"
//---------------------------------------------------------------------------
class TNotify;
class TNotifyWin : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelBottom;
	TBevel *Bevel;
	TButton *CloseButton;
	TPanel *PanelRight;
	TButton *Add;
	TButton *Edit;
	TButton *Delete;
	TdfsExtListView *List;
	TFormStorage *FormStorage1;
	void __fastcall PanelBottomResize(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall CloseButtonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall EditClick(TObject *Sender);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	
	void __fastcall ListDeletion(TObject *Sender, TListItem *Item);
private:
	void __fastcall AddItem( TNotify *rec );
	void __fastcall Store( void );
	bool __fastcall IsInList( TNotify *rec );

public:
	__fastcall 		TNotifyWin(TComponent* Owner);
    void __fastcall UpdateItem( TNotify *rec );
};
//---------------------------------------------------------------------------
extern PACKAGE TNotifyWin *NotifyWin;
//---------------------------------------------------------------------------
#endif
