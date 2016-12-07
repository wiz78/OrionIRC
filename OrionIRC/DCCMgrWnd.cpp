/* $Id: DCCMgrWnd.cpp,v 1.11 2005/09/09 22:52:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "DCCMgrWnd.h"
#include "ServerWnd.h"
#include "DCCTransfer.h"
#include "DCCMgr.h"
#include "AskRemoveWnd.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "EnhListView"
#pragma link "ExtListView"
#pragma link "Placemnt"
#pragma resource "*.dfm"
TDCCMgrWin *DCCMgrWin;
//---------------------------------------------------------------------------
__fastcall TDCCMgrWin::TDCCMgrWin(TComponent* Owner)
	: TForm(Owner)
{
	TaskbarButton = NULL;

	DCCMgr.SetListView( List );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::FormShow(TObject *Sender)
{
    TaskbarButton = MainWin->AddTaskbarButton( "DCC Transfers", this, TB_DCC, TaskbarPopup );

	List->ReadSettings();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::FormClose(TObject *Sender, TCloseAction &Action)
{
	List->WriteSettings();

	DCCMgr.SetListView( NULL );

    DCCMgrWin = NULL;
    Action    = caFree;

    MainWin->RemoveTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::FormActivate(TObject *Sender)
{
    MainWin->EnableTaskbarButton( TaskbarButton );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::Send( TServerWin *server, AnsiString target, AnsiString file )
{
	TDCCTransfer   *dcc = new TDCCTransfer( server->Session.Server, target, file );

    DCCMgr.Add( dcc );

    if( !dcc->Offer() )
    	server->DisplayOnActiveWin( NULL, "DCC send error: cannot setup the socket!", IRC_ERROR );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::Receive( TServerWin *server, AnsiString nick, AnsiString file,
									 int size, Prom_Addr *addr, int port, bool resume )
{
	TDCCTransfer   *dcc = new TDCCTransfer( server->Session.Server, nick, file );

    DCCMgr.Add( dcc );

    if( resume ) {
    	int from = TDCCTransfer::GetFileSize( file );

    	dcc->PrepareToResume( addr, port, size, from );

        server->SendCmd( "PRIVMSG " + nick + " :\001DCC RESUME file.ext " + IntToStr( port ) + " " + IntToStr( from ) + "\001" );

    } else
	    dcc->Receive( addr, port, size );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::OpenFileMenuClick(TObject *Sender)
{
	TListItem  *item = List->Selected;

    if( item ) {
        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

        ShellExecute( Handle, "open", dcc->GetFile().c_str(), NULL, NULL, SW_SHOWNORMAL );
    }
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::TidyUpMenuClick(TObject *Sender)
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
    	TListItem	 *item = List->Items->Item[ i ];
        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

		switch( dcc->GetState() ) {

        	case DCC_DONE:
            case DCC_ERROR:
            case DCC_ABORTED:
            	item->Delete();
                DCCMgr.Remove( dcc );
            	break;
        }
    }

	List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::CancelMenuClick(TObject *Sender)
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
		TListItem  *item = List->Items->Item[ i ];

        if( item->Selected ) {
	        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

    		dcc->Close( true );
        }
    }

	List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::ResendMenuClick(TObject *Sender)
{
	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; i >= 0; i-- ) {
		TListItem  *item = List->Items->Item[ i ];

        if( item->Selected ) {
	        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

    		dcc->Offer();
        }
    }

	List->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::DeleteFileMenuClick(TObject *Sender)
{
	TAskRemoveWin  *wnd = new TAskRemoveWin( NULL );
    bool			all = false, proceed = true;
    int				result;

	List->Items->BeginUpdate();

    for( int i = List->Items->Count - 1; proceed && ( i >= 0 ); i-- ) {
    	TListItem *item = List->Items->Item[ i ];

        if( item->Selected ) {
	        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

            if( !all ) {

            	wnd->Msg->Caption = Format( "Do you really want to delete the file\n'%s'?",
                                            ARRAYOFCONST(( dcc->GetFile() )));

                result = wnd->Execute();
            }

			switch( result ) {

                case ARR_YES_ALL:
                    all = true;
                    // fall through

                case ARR_YES:
                	DeleteFile( dcc->GetFile() );

                	item->Delete();
            	    DCCMgr.Remove( dcc );
                    break;

                case ARR_CANCEL:
                	proceed = false;
                	break;
            }
        }
    }

	List->Items->EndUpdate();

    delete wnd;
}
//---------------------------------------------------------------------------
TDCCTransfer * __fastcall TDCCMgrWin::FindFromPort( int port )
{
	TDCCTransfer *ret = NULL;

    for( int i = List->Items->Count - 1; !ret && ( i >= 0 ); i-- ) {
		TListItem  	   *item = List->Items->Item[ i ];
        TDCCTransfer   *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

        if( dcc->GetPort() == port )
        	ret = dcc;
    }

    return( ret );
}
//---------------------------------------------------------------------------
bool __fastcall TDCCMgrWin::SetResumeFrom( int port, int from )
{
	TDCCTransfer *dcc = FindFromPort( port );

    if( dcc )
    	dcc->SetResumeFrom( from );

    return( dcc != NULL );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::Resume( int port )
{
	TDCCTransfer *dcc = FindFromPort( port );

    if( dcc )
    	dcc->Resume();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::ListMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TListItem  *item;

    if( item = List->Selected ) {
        TDCCTransfer *dcc = reinterpret_cast<TDCCTransfer *>( item->Data );

		CancelMenu->Enabled     = ( dcc->GetState() != DCC_DONE ) &&
        						  ( dcc->GetState() != DCC_ERROR ) &&
                                  ( dcc->GetState() != DCC_ABORTED );
        ResendMenu->Enabled     = !CancelMenu->Enabled;
        DeleteFileMenu->Enabled = !CancelMenu->Enabled;
        OpenFileMenu->Enabled   = true;

    } else {

		CancelMenu->Enabled     = false;
        ResendMenu->Enabled     = false;
        OpenFileMenu->Enabled   = false;
        DeleteFileMenu->Enabled = false;
    }

    TidyUpMenu->Enabled = List->Items->Count > 0;
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::Closewindow1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( Shift.Contains( ssAlt ))
    	MainWin->AltDown( true, Key );
}
//---------------------------------------------------------------------------
void __fastcall TDCCMgrWin::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if( !Shift.Contains( ssAlt ))
    	MainWin->AltDown( false, Key );
}
//---------------------------------------------------------------------------

