/* $Id: IgnoreWnd.h,v 1.3 2003/12/31 16:16:04 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IgnoreWndH
#define IgnoreWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TIgnore;
class TIgnoreWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TButton *Ok;
	TButton *Cancel;
	TBevel *Bevel1;
	TListView *List;
	TEdit *Mask;
	TLabel *Label1;
	TButton *Add;
	TButton *Remove;
	TCheckBox *Exclude;
	TBevel *Bevel2;
	TLabel *Label2;
	TCheckBox *Channel;
	TCheckBox *Private;
	TCheckBox *Notices;
	TCheckBox *CTCP;
	TCheckBox *Invite;
	TCheckBox *DCCFiles;
	TCheckBox *DCCChat;
	TCheckBox *Actions;
	TCheckBox *Page;
	TCheckBox *Codes;
	TCheckBox *Enabled;
	void __fastcall MaskChange(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall ListDeletion(TObject *Sender, TListItem *Item);
	void __fastcall RemoveClick(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall ExcludeClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	bool			IgnoreNotify;

	void __fastcall GetData( TIgnore *ign );

public:		// User declarations
	__fastcall 		TIgnoreWin( TComponent* Owner );
	void __fastcall AddItem( TIgnore *ign );
};
//---------------------------------------------------------------------------
extern PACKAGE TIgnoreWin *IgnoreWin;
//---------------------------------------------------------------------------
#endif
