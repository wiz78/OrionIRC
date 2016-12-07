/* $Id: DCCChatOfferWnd.cpp,v 1.8 2004/12/24 14:48:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DCCChatOfferWnd.h"
#include "ServerWnd.h"
#include "QueryWnd.h"
#include "Commands.h"
#include "Eyelin.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma resource "*.dfm"
TDCCChatOfferWin *DCCChatOfferWin;
//---------------------------------------------------------------------------
__fastcall TDCCChatOfferWin::TDCCChatOfferWin( TComponent* Owner, int type,
											   Prom_Addr *addr, int port, HostMask& mask )
	: TForm(Owner)
{
	Server = reinterpret_cast<TServerWin *>( Owner );
    Port   = port;
    Nick   = mask.GetNick();
    Type   = type;

    if( addr )
		Addr = *addr;

    Label->Caption = Nick + " (" + mask.GetUserHost() + ") would like to establish a";

    switch( Type ) {

    	case CHAT:
		    Label->Caption = Label->Caption + " DCC Chat";
        	break;

        case EYELIN:
        	Caption        = "Eyelin request";
		    Label->Caption = Label->Caption + "n Eyelin";
        	break;
    }

    Label->Caption = Label->Caption + " session with you.\n\nDo you wish to accept?";

    Top  = MainWin->Top + (( MainWin->Height - Height ) / 2 );
    Left = MainWin->Left + (( MainWin->Width - Width ) / 2 );
}
//---------------------------------------------------------------------------
void __fastcall TDCCChatOfferWin::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TDCCChatOfferWin::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TDCCChatOfferWin::OkClick(TObject *Sender)
{
    switch( Type ) {

    	case CHAT:
			Server->FindQueryWin( Nick, true, true )->ConnectDCC( Addr, Port );
            break;

        case EYELIN:
        	Eyelin::GetInstance()->AcceptDCC( Server, Nick );
        	break;
    }

    Close();
}
//---------------------------------------------------------------------------
void __fastcall TDCCChatOfferWin::IgnoreClick(TObject *Sender)
{
   	Server->GetHostForIgnore( Nick, IGNORE_CTCP );
    Close();
}
//---------------------------------------------------------------------------

