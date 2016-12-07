/* $Id: KickBanWnd.h,v 1.1 2003/05/15 17:43:51 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef KickBanWndH
#define KickBanWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "HostMask.h"

class TKickBanWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TButton *KickBan;
	TButton *Kick;
	TButton *Ban;
	TEdit *Reason;
	TLabel *Label1;
	TLabel *Label2;
	TRadioButton *Mask1;
	TRadioButton *Mask2;
	TRadioButton *Mask3;
	TRadioButton *Mask4;
	TRadioButton *Mask5;
	TRadioButton *Mask6;void __fastcall BanClick(TObject *Sender);
	void __fastcall KickBanClick(TObject *Sender);
	void __fastcall KickClick(TObject *Sender);
private:	// User declarations
	int					Result;
public:		// User declarations
	__fastcall 			TKickBanWin(TComponent* Owner);
    MaskType __fastcall GetMaskType( void );
    int __fastcall		Execute( void );
};

enum { KB_KICK, KB_KICK_BAN, KB_BAN, KB_CANCEL };

//---------------------------------------------------------------------------
extern PACKAGE TKickBanWin *KickBanWin;
//---------------------------------------------------------------------------
#endif
