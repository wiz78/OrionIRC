/* $Id: DCCChatOfferWnd.h,v 1.2 2004/12/19 18:46:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DCCChatOfferWndH
#define DCCChatOfferWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "socket.h"
#include "Hostmask.h"

class TServerWin;

class TDCCChatOfferWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TButton *Ignore;
	TLabel *Label;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall IgnoreClick(TObject *Sender);
private:
	TServerWin *Server;
	Prom_Addr	Addr;
    int			Port;
    AnsiString	Nick;
    int			Type;

public:
	enum { CHAT, EYELIN }; // types

	__fastcall TDCCChatOfferWin( TComponent* Owner, int type, Prom_Addr *addr, int port, HostMask& mask );
};
//---------------------------------------------------------------------------
extern PACKAGE TDCCChatOfferWin *DCCChatOfferWin;
//---------------------------------------------------------------------------
#endif
