/* $Id: ChanPropWnd.h,v 1.2 2003/05/10 13:32:45 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ChanPropWndH
#define ChanPropWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Placemnt.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <RXSpin.hpp>
//---------------------------------------------------------------------------
#include "irc.h"

class TChanWin;
class TChanPropWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TPageControl *Pages;
	TTabSheet *Page1;
	TTabSheet *TabSheet2;
	TTabSheet *ExceptionPage;
	TTabSheet *InvitePage;
	TLabel *Label12;
	TBevel *Bevel5;
	TComboBox *Topic;
	TLabel *Label1;
	TLabel *TopicSetBy;
	TLabel *Label2;
	TLabel *TopicSetTime;
	TBevel *Bevel2;
	TLabel *Label3;
	TCheckBox *NoExtMsgs;
	TCheckBox *Secret;
	TCheckBox *Private;
	TCheckBox *InviteOnly;
	TCheckBox *Moderated;
	TCheckBox *TopicProt;
	TCheckBox *AnonOps;
	TCheckBox *UseKeyword;
	TEdit *Keyword;
	TCheckBox *LimitUsers;
	TRxSpinEdit *UserLimit;
	TCheckBox *Quiet;
	TCheckBox *Reop;
	TListView *Bans;
	TListView *Excepts;
	TListView *Invites;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall UseKeywordClick(TObject *Sender);	
	void __fastcall LimitUsersClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TChanWin	   *Channel;

	void __fastcall ProcessList( TListView *list, AnsiString flag );
    
public:		// User declarations
	__fastcall 		TChanPropWin(TComponent* Owner, TChanWin *channel);
    void __fastcall AddListItem( TIRCChanList list, AnsiString item, AnsiString setby, AnsiString settime );
};
//---------------------------------------------------------------------------
extern PACKAGE TChanPropWin *ChanPropWin;
//---------------------------------------------------------------------------
#endif
