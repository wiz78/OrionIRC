/* $Id: KickBanWnd.cpp,v 1.1 2003/05/15 17:43:51 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "KickBanWnd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TKickBanWin *KickBanWin;
//---------------------------------------------------------------------------
__fastcall TKickBanWin::TKickBanWin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
MaskType __fastcall TKickBanWin::GetMaskType( void )
{
	MaskType ret;

	if( Mask1->Checked )
    	ret = MASK_NICK_USER_HOST;
    else if( Mask2->Checked )
    	ret = MASK_NICK_USER_TLD;
    else if( Mask3->Checked )
    	ret = MASK_USER_HOST;
    else if( Mask4->Checked )
    	ret = MASK_USER_TLD;
    else if( Mask5->Checked )
    	ret = MASK_HOST;
    else if( Mask6->Checked )
    	ret = MASK_TLD;

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TKickBanWin::BanClick(TObject *Sender)
{
	Result = KB_BAN;
}
//---------------------------------------------------------------------------
void __fastcall TKickBanWin::KickBanClick(TObject *Sender)
{
	Result = KB_KICK_BAN;
}
//---------------------------------------------------------------------------
void __fastcall TKickBanWin::KickClick(TObject *Sender)
{
	Result = KB_KICK;
}
//---------------------------------------------------------------------------
int __fastcall TKickBanWin::Execute( void )
{
	if( ShowModal() == mrCancel )
    	Result = KB_CANCEL;

    return( Result );
}
//---------------------------------------------------------------------------
