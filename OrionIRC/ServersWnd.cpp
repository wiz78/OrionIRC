/* $Id: ServersWnd.cpp,v 1.15 2005/04/19 18:19:26 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <registry.hpp>

#include "ServersWnd.h"
#include "ServerWnd.h"
#include "Server.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "RXSpin"
#pragma link "ToolEdit"
#pragma resource "*.dfm"
TServersWin *ServersWin;
//---------------------------------------------------------------------------
__fastcall TServersWin::TServersWin(TComponent* Owner)
	: TForm(Owner)
{
	TRegistry	*reg = new TRegistry();

	FirstServer           = new TServer();
    FirstServer->Descr    = "Default";
    FirstServer->Port     = "6667";
    FirstServer->Username = "OrionUser";
    FirstServer->Realname = "I'm too lazy to setup my irc client properly";

    FirstServer->AddNick( "OrionUser" );

	FirstServer->Load( reg, "Software\\WizSoftware\\Orion\\Servers" );

    delete reg;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::FormDestroy(TObject *Sender)
{
	delete FirstServer;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ConnectClick(TObject *Sender)
{
	if( ServerTree->Selected ) {
		TServer	*serv = (TServer *)ServerTree->Selected->Data;

		OkClick( Sender );

		new TServerWin( Application, serv );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::SwitchClick(TObject *Sender)
{
    TServerWin *server = MainWin->FindServerWin( CurrentServer );

    if( server ) {
		TServer	*serv = reinterpret_cast<TServer *>( ServerTree->Selected->Data );

        OkClick( Sender );

        server->ConnectTo( serv );
	}
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::FormShow(TObject *Sender)
{
	ServerTree->Items->BeginUpdate();
	ServerTree->Items->Clear();

    TmpServers = new TServer( FirstServer );

    TmpServers->InsertIntoTree( ServerTree );

	ServerTree->Items->EndUpdate();

	Connect->Enabled      = false;
    Switch->Enabled       = false;
    ServerClone->Enabled  = false;
    ServerDelete->Enabled = false;
    Pages->ActivePage     = FirstPage;

    ServerTreeChange( ServerTree, NULL );
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::OkClick(TObject *Sender)
{
	TRegistry	*reg = new TRegistry();

	delete FirstServer;

    GetServerTree();

	FirstServer->Save( reg, "Software\\WizSoftware\\Orion\\Servers" );

	Close();

    delete reg;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::CancelClick(TObject *Sender)
{
	Close();	
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::GetServerTree( void )
{
    TTreeNode  *node = ServerTree->Items->GetFirstNode();
    TServer	   *serv = (TServer *)node->Data;

	GetServer();
    GetServerChildren( node, serv );

    serv->PropagateSettings();

   	FirstServer = serv;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::GetServerChildren( TTreeNode *node, TServer *parent )
{
    parent->ClearChildrenList();

	for( int i = 0; i < node->Count; i++ ) {
    	TTreeNode  *child = node->Item[ i ];
        TServer	   *s = (TServer *)child->Data;

        parent->AddChild( s );

        GetServerChildren( child, s );
    }

    node->Data = NULL; //avoid double-free
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::GetServer( void )
{
    TTreeNode  *node = ServerTree->Selected;

    if( node ) {
    	TServer *serv = reinterpret_cast<TServer *>( node->Data );

        GetServer( serv );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::GetServer( TServer *serv )
{
    serv->Descr          = ServerDescr->Text;
    serv->Host           = ServerHost->Text;
    serv->Port           = ServerPort->Text;
    serv->Password       = ServerPwd->Text;
    serv->Username       = ServerUsername->Text;
    serv->Realname       = ServerRealName->Text;
    serv->ReconnectDelay = ConnectRetryDelay->Value;
    serv->AwayNick       = MsgsAwayNick->Text;

    serv->SetNicks( ServerNick->Text );
    serv->SetChannels( AutoJoinChannels->Lines );
    serv->SetCommandsOnConnect( AutoExecuteCmd->Lines );
    serv->SetPartReason( MsgsPart->Text );
    serv->SetQuitReason( MsgsQuit->Text );
    serv->SetFingerMsg( MsgsFinger->Text );
    serv->SetAwayMsg( MsgsAway->Text );
    serv->SetPartFile( MsgsPartFile->Text );
    serv->SetQuitFile( MsgsQuitFile->Text );
    serv->SetFingerFile( MsgsFingerFile->Text );
    serv->SetAwayFile( MsgsAwayFile->Text );

    serv->SetEventDisplay( TServer::EV_JOIN,   EventJoins->ItemIndex   );
    serv->SetEventDisplay( TServer::EV_PART,   EventParts->ItemIndex   );
    serv->SetEventDisplay( TServer::EV_QUIT,   EventQuits->ItemIndex   );
    serv->SetEventDisplay( TServer::EV_KICK,   EventKicks->ItemIndex   );
    serv->SetEventDisplay( TServer::EV_NICK,   EventNicks->ItemIndex   );
    serv->SetEventDisplay( TServer::EV_INVITE, EventInvites->ItemIndex );
    serv->SetEventDisplay( TServer::EV_NOTICE, EventNotices->ItemIndex );
    serv->SetEventDisplay( TServer::EV_NOTIFY, EventNotify->ItemIndex  );

    serv->Flags.Set( SERVF_INHERIT_USER,         	ServerUserInherit->Checked  );
    serv->Flags.Set( SERVF_INHERIT_CONNECTION,   	InheritConnection->Checked  );
    serv->Flags.Set( SERVF_INHERIT_ON_CONNECT,		InheritOnConnect->Checked   );
    serv->Flags.Set( SERVF_INHERIT_MESSAGES,      	InheritMessages->Checked    );
    serv->Flags.Set( SERVF_INHERIT_EVENT_DISPLAY, 	InheritEvents->Checked      );
    serv->Flags.Set( SERVF_AUTO_CONNECT, 	     	AutoConnect->Checked        );
    serv->Flags.Set( SERVF_SHOW_MOTD, 	   	     	ShowMOTD->Checked           );
    serv->Flags.Set( SERVF_INVISIBLE, 	         	BeInvisible->Checked        );
    serv->Flags.Set( SERVF_WALLOPS, 	   	     	RecvWallops->Checked        );
    serv->Flags.Set( SERVF_MSG_FROM_FILE_PART, 	 	MsgsPartRandom->Checked     );
    serv->Flags.Set( SERVF_MSG_FROM_FILE_QUIT, 	 	MsgsQuitRandom->Checked     );
    serv->Flags.Set( SERVF_MSG_FROM_FILE_FINGER, 	MsgsFingerRandom->Checked   );
    serv->Flags.Set( SERVF_MSG_FROM_FILE_AWAY,   	MsgsAwayRandom->Checked     );
    serv->Flags.Set( SERVF_SET_AWAY_NICK,   	 	MsgsAwayChangeNick->Checked );

    if( ConnectRetry->Checked )
        serv->FailureAction = TServer::RECONNECT;
    else if( ConnectNext->Checked )
        serv->FailureAction = TServer::CONNECT_NEXT;
    else
        serv->FailureAction = TServer::NOTHING;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerHostChange(TObject *Sender)
{
	TServer	serv;

    GetServer( &serv );

	Connect->Enabled = serv.CanConnect();
	Switch->Enabled  = serv.CanConnect() && !CurrentServer.IsEmpty();
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerAddClick(TObject *Sender)
{
    TTreeNode  *node = ServerTree->Selected;
	TServer	   *serv = new TServer();

    GetServer();

    serv->Descr = "New server";
    serv->Port  = "6667";

    if( node ) {
    	TServer *parent = (TServer *)node->Data;

        serv->Username = parent->Username;
        serv->Realname = parent->Realname;

        for( int i = 0; i < parent->GetNicksCount(); i++ )
        	serv->AddNick( parent->GetNick( i ));

    	ServerTree->Selected = ServerTree->Items->AddChildObject( node, serv->Descr, serv );

        serv->Flags.Set( SERVF_INHERIT_USER );

    } else
    	ServerTree->Selected = ServerTree->Items->AddObject( NULL, serv->Descr, serv );

    ServerTreeChange( Sender, ServerTree->Selected );
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerTreeDeletion(TObject *Sender,
      TTreeNode *Node)
{
    TServer *serv = (TServer *)Node->Data;

    if( serv ) {

    	serv->ClearChildrenList();

		delete serv;
    }
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerDeleteClick(TObject *Sender)
{
    TTreeNode  *node = ServerTree->Selected;

    if( node )
		ServerTree->Items->Delete( node );
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerCloneClick(TObject *Sender)
{
    TTreeNode  *node = ServerTree->Selected;

    if( node ) {
    	TServer	*serv = new TServer();

        *serv       = *((TServer *)node->Data );
        serv->Descr = "Copy of " + serv->Descr;

        serv->ClearChildren();

    	ServerTree->Items->AddObject( node, serv->Descr, serv );
	}
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerUserInheritClick(TObject *Sender)
{
	ServerUsername->Enabled = !ServerUserInherit->Checked;
	ServerRealName->Enabled = !ServerUserInherit->Checked;
	ServerNick->Enabled     = !ServerUserInherit->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerDescrChange(TObject *Sender)
{
    TTreeNode  *node = ServerTree->Selected;

    if( node )
        node->Text  = ServerDescr->Text;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerTreeChanging(TObject *Sender,
      TTreeNode *Node, bool &AllowChange)
{
	GetServer();

    // avoid some confusion in the click handler
    if( Node == ServerTree->Selected )
    	AllowChange = false;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerTreeChange(TObject *Sender,
      TTreeNode *Node)
{
    bool		enable;
    TTreeNode  *node = ServerTree->Selected;

    if( node && node->Data ) {
    	TServer *serv = reinterpret_cast<TServer *>( node->Data );

        ServerDescr->Text           = serv->Descr;
        ServerHost->Text            = serv->Host;
        ServerPort->Text            = serv->Port;
        ServerPwd->Text             = serv->Password;
        ServerUsername->Text        = serv->Username;
        ServerRealName->Text        = serv->Realname;
        ServerNick->Text            = serv->GetNicks();
        ServerUserInherit->Checked  = serv->Flags.IsSet( SERVF_INHERIT_USER          );
        InheritConnection->Checked  = serv->Flags.IsSet( SERVF_INHERIT_CONNECTION    );
        InheritOnConnect->Checked   = serv->Flags.IsSet( SERVF_INHERIT_ON_CONNECT    );
        InheritMessages->Checked    = serv->Flags.IsSet( SERVF_INHERIT_MESSAGES      );
        InheritEvents->Checked      = serv->Flags.IsSet( SERVF_INHERIT_EVENT_DISPLAY );
        AutoConnect->Checked	    = serv->Flags.IsSet( SERVF_AUTO_CONNECT          );
        ShowMOTD->Checked           = serv->Flags.IsSet( SERVF_SHOW_MOTD             );
        BeInvisible->Checked        = serv->Flags.IsSet( SERVF_INVISIBLE	         );
        RecvWallops->Checked        = serv->Flags.IsSet( SERVF_WALLOPS     	         );
        MsgsPartRandom->Checked     = serv->Flags.IsSet( SERVF_MSG_FROM_FILE_PART    );
        MsgsQuitRandom->Checked     = serv->Flags.IsSet( SERVF_MSG_FROM_FILE_QUIT    );
        MsgsFingerRandom->Checked   = serv->Flags.IsSet( SERVF_MSG_FROM_FILE_FINGER  );
        MsgsAwayRandom->Checked     = serv->Flags.IsSet( SERVF_MSG_FROM_FILE_AWAY    );
        MsgsAwayChangeNick->Checked = serv->Flags.IsSet( SERVF_SET_AWAY_NICK   	     );
        ConnectRetryDelay->Value    = serv->ReconnectDelay;
        MsgsPart->Text              = serv->PartReason;
        MsgsQuit->Text              = serv->QuitReason;
        MsgsFinger->Text            = serv->FingerMsg;
        MsgsAway->Text              = serv->AwayMsg;
        MsgsPartFile->Text          = serv->PartFile;
        MsgsQuitFile->Text          = serv->QuitFile;
        MsgsFingerFile->Text        = serv->FingerFile;
        MsgsAwayFile->Text          = serv->AwayFile;
        MsgsAwayNick->Text          = serv->AwayNick;
        EventJoins->ItemIndex		= serv->GetEventDisplay( TServer::EV_JOIN   );
        EventParts->ItemIndex		= serv->GetEventDisplay( TServer::EV_PART   );
        EventQuits->ItemIndex		= serv->GetEventDisplay( TServer::EV_QUIT   );
        EventKicks->ItemIndex		= serv->GetEventDisplay( TServer::EV_KICK   );
        EventNicks->ItemIndex		= serv->GetEventDisplay( TServer::EV_NICK   );
        EventInvites->ItemIndex		= serv->GetEventDisplay( TServer::EV_INVITE );
        EventNotices->ItemIndex		= serv->GetEventDisplay( TServer::EV_NOTICE );
        EventNotify->ItemIndex		= serv->GetEventDisplay( TServer::EV_NOTIFY );

        switch( serv->FailureAction ) {

        	case TServer::RECONNECT:
            	ConnectRetry->Checked = true;
                break;

        	case TServer::CONNECT_NEXT:
            	ConnectNext->Checked = true;
                break;

        	case TServer::NOTHING:
            	DoNothing->Checked = true;
                break;
        }

        AutoJoinChannels->Lines->Clear();

        for( int i = 0; i < serv->GetChannelsCount(); i++ )
        	AutoJoinChannels->Lines->Add( serv->GetChannel( i ));

        AutoExecuteCmd->Lines->Clear();

        for( int i = 0; i < serv->GetCommandsOnConnectCount(); i++ )
        	AutoExecuteCmd->Lines->Add( serv->GetCommandOnConnect( i ));

        ServerUserInheritClick( Sender );
        InheritConnectionClick( Sender );
        InheritOnConnectClick( Sender );
        InheritEventsClick( Sender );

	    Connect->Enabled = serv->CanConnect();
        Switch->Enabled  = serv->CanConnect() && !CurrentServer.IsEmpty();
    }

    enable                     = ( node != NULL ) && node->AbsoluteIndex;
    ServerUserInherit->Enabled = enable;
    InheritConnection->Enabled = enable;
    InheritMessages->Enabled   = enable;
    InheritEvents->Enabled     = enable;
	ServerClone->Enabled       = enable;
    ServerDelete->Enabled      = enable;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerTreeExpanded(TObject *Sender, TTreeNode *Node)
{
	TServer	*serv = (TServer *)Node->Data;

    serv->Flags.Set( SERVF_EXPANDED );
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::ServerTreeDblClick(TObject *Sender)
{
    TTreeNode  *node = ServerTree->Selected;

    if( node ) {
    	TServer *serv = (TServer *)node->Data;

	    if( !serv->Host.IsEmpty() )
    		ConnectClick( Sender );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::InheritConnectionClick(TObject *Sender)
{
	ShowMOTD->Enabled          = !InheritConnection->Checked;
	BeInvisible->Enabled       = !InheritConnection->Checked;
	RecvWallops->Enabled       = !InheritConnection->Checked;
	ConnectRetry->Enabled      = !InheritConnection->Checked;
	ConnectRetryDelay->Enabled = !InheritConnection->Checked;
	ConnectNext->Enabled       = !InheritConnection->Checked;
	DoNothing->Enabled         = !InheritConnection->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::InheritMessagesClick(TObject *Sender)
{
	if( InheritMessages->Checked ) {

    	MsgsPartFixed->Enabled      = false;
		MsgsPartRandom->Enabled     = false;
		MsgsPart->Enabled           = false;
		MsgsPartFile->Enabled       = false;
		MsgsQuitFixed->Enabled      = false;
		MsgsQuitRandom->Enabled     = false;
		MsgsQuit->Enabled           = false;
		MsgsQuitFile->Enabled       = false;
		MsgsFingerFixed->Enabled    = false;
		MsgsFingerRandom->Enabled   = false;
		MsgsFinger->Enabled         = false;
		MsgsFingerFile->Enabled     = false;
		MsgsAwayFixed->Enabled      = false;
		MsgsAwayRandom->Enabled     = false;
		MsgsAway->Enabled           = false;
		MsgsAwayFile->Enabled       = false;
		MsgsAwayNick->Enabled       = false;
        MsgsAwayChangeNick->Enabled = false;

    } else {

    	MsgsPartFixed->Enabled      = true;
		MsgsPartRandom->Enabled     = true;
		MsgsQuitFixed->Enabled      = true;
		MsgsQuitRandom->Enabled     = true;
		MsgsFingerFixed->Enabled    = true;
		MsgsFingerRandom->Enabled   = true;
		MsgsAwayFixed->Enabled      = true;
		MsgsAwayRandom->Enabled     = true;
        MsgsAwayChangeNick->Enabled = true;

		MsgsPartFixedClick( NULL );
		MsgsQuitFixedClick( NULL );
		MsgsFingerFixedClick( NULL );
		MsgsAwayFixedClick( NULL );
    	MsgsAwayChangeNickClick( NULL );
    }
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::MsgsAwayChangeNickClick(TObject *Sender)
{
	MsgsAwayNick->Enabled = MsgsAwayChangeNick->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::MsgsPartFixedClick(TObject *Sender)
{
	MsgsPart->Enabled     = MsgsPartFixed->Checked;
    MsgsPartFile->Enabled = !MsgsPartFixed->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::MsgsQuitFixedClick(TObject *Sender)
{
	MsgsQuit->Enabled     = MsgsQuitFixed->Checked;
    MsgsQuitFile->Enabled = !MsgsQuitFixed->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::MsgsFingerFixedClick(TObject *Sender)
{
	MsgsFinger->Enabled     = MsgsFingerFixed->Checked;
    MsgsFingerFile->Enabled = !MsgsFingerFixed->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::MsgsAwayFixedClick(TObject *Sender)
{
	MsgsAway->Enabled     = MsgsAwayFixed->Checked;
    MsgsAwayFile->Enabled = !MsgsAwayFixed->Checked;
}
//---------------------------------------------------------------------------
void __fastcall	TServersWin::SetCurrentServer( AnsiString server )
{
	CurrentServer = server;

    if( CurrentServer.IsEmpty() )
    	Switch->Enabled = false;
    else
	    ServerTreeChange( ServerTree, NULL );
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::InheritOnConnectClick(TObject *Sender)
{
    AutoExecuteCmd->Enabled   = !InheritOnConnect->Checked;
    AutoJoinChannels->Enabled = !InheritOnConnect->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TServersWin::InheritEventsClick(TObject *Sender)
{
	bool enable = !InheritEvents->Checked;

	EventJoins->Enabled   = enable;
	EventParts->Enabled   = enable;
	EventQuits->Enabled   = enable;
	EventKicks->Enabled   = enable;
	EventNicks->Enabled   = enable;
	EventInvites->Enabled = enable;
	EventNotices->Enabled = enable;
	EventNotify->Enabled  = enable;
}
//---------------------------------------------------------------------------

