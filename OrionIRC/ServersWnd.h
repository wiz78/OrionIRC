/* $Id: ServersWnd.h,v 1.9 2005/04/19 18:19:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ServersWndH
#define ServersWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "RXSpin.hpp"
#include "ToolEdit.hpp"
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TServer;
class TServersWin : public TForm
{
__published:	// IDE-managed Components
	TTreeView *ServerTree;
	TBitBtn *ServerAdd;
	TBitBtn *ServerClone;
	TBitBtn *ServerDelete;
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TButton *Connect;
	TPageControl *Pages;
	TTabSheet *FirstPage;
	TLabel *Label4;
	TEdit *ServerDescr;
	TLabel *Label1;
	TEdit *ServerHost;
	TLabel *Label2;
	TEdit *ServerPort;
	TLabel *Label3;
	TEdit *ServerPwd;
	TTabSheet *TabSheet2;
	TCheckBox *ServerUserInherit;
	TEdit *ServerUsername;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *ServerRealName;
	TEdit *ServerNick;
	TLabel *Label7;
	TTabSheet *TabSheet1;
	TCheckBox *InheritConnection;
	TLabel *Label8;
	TBevel *Bevel2;
	TCheckBox *AutoConnect;
	TCheckBox *ShowMOTD;
	TCheckBox *BeInvisible;
	TCheckBox *RecvWallops;
	TLabel *Label9;
	TBevel *Bevel3;
	TRadioButton *ConnectRetry;
	TRxSpinEdit *ConnectRetryDelay;
	TLabel *Label10;
	TRadioButton *ConnectNext;
	TRadioButton *DoNothing;
	TTabSheet *TabSheet3;
	TBevel *Bevel5;
	TLabel *Label12;
	TCheckBox *InheritMessages;
	TPanel *PanelMsgsPart;
	TRadioButton *MsgsPartFixed;
	TRadioButton *MsgsPartRandom;
	TEdit *MsgsPart;
	TFilenameEdit *MsgsPartFile;
	TPanel *PanelMsgsQuit;
	TRadioButton *MsgsQuitFixed;
	TRadioButton *MsgsQuitRandom;
	TEdit *MsgsQuit;
	TFilenameEdit *MsgsQuitFile;
	TBevel *Bevel6;
	TLabel *Label13;
	TPanel *PanelMsgsFinger;
	TRadioButton *MsgsFingerFixed;
	TRadioButton *MsgsFingerRandom;
	TEdit *MsgsFinger;
	TFilenameEdit *MsgsFingerFile;
	TBevel *Bevel7;
	TLabel *Label14;
	TPanel *PanelMsgsAway;
	TRadioButton *MsgsAwayFixed;
	TRadioButton *MsgsAwayRandom;
	TEdit *MsgsAway;
	TFilenameEdit *MsgsAwayFile;
	TBevel *Bevel8;
	TLabel *Label15;
	TCheckBox *MsgsAwayChangeNick;
	TEdit *MsgsAwayNick;
	TButton *Switch;
	TTabSheet *TabSheet4;
	TLabel *Label11;
	TBevel *Bevel4;
	TMemo *AutoJoinChannels;
	TLabel *Label16;
	TBevel *Bevel9;
	TMemo *AutoExecuteCmd;
	TCheckBox *InheritOnConnect;
	TTabSheet *EventsPage;
	TComboBox *EventNicks;
	TLabel *Label35;
	TLabel *Label34;
	TComboBox *EventKicks;
	TComboBox *EventQuits;
	TLabel *Label33;
	TLabel *Label32;
	TComboBox *EventParts;
	TComboBox *EventJoins;
	TLabel *Label31;
	TLabel *Label30;
	TBevel *Bevel11;
	TCheckBox *InheritEvents;
	TLabel *Label17;
	TComboBox *EventInvites;
	TLabel *Label18;
	TComboBox *EventNotices;
	TComboBox *EventNotify;
	TLabel *Label19;
	void __fastcall ConnectClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall ServerTreeDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall ServerAddClick(TObject *Sender);
	void __fastcall ServerDeleteClick(TObject *Sender);
	void __fastcall ServerCloneClick(TObject *Sender);
	void __fastcall ServerUserInheritClick(TObject *Sender);
	void __fastcall ServerDescrChange(TObject *Sender);
	void __fastcall ServerTreeChanging(TObject *Sender, TTreeNode *Node,
          bool &AllowChange);
	void __fastcall ServerTreeChange(TObject *Sender, TTreeNode *Node);
	void __fastcall ServerTreeExpanded(TObject *Sender, TTreeNode *Node);
	void __fastcall ServerTreeDblClick(TObject *Sender);
	void __fastcall InheritConnectionClick(TObject *Sender);
	void __fastcall MsgsAwayChangeNickClick(TObject *Sender);
	void __fastcall InheritMessagesClick(TObject *Sender);
	void __fastcall MsgsPartFixedClick(TObject *Sender);
	void __fastcall MsgsQuitFixedClick(TObject *Sender);
	void __fastcall MsgsFingerFixedClick(TObject *Sender);
	void __fastcall MsgsAwayFixedClick(TObject *Sender);
	void __fastcall SwitchClick(TObject *Sender);
	void __fastcall InheritOnConnectClick(TObject *Sender);
	void __fastcall InheritEventsClick(TObject *Sender);
	void __fastcall ServerHostChange(TObject *Sender);
private:	// User declarations
	TServer		  	   *FirstServer;
	TServer		   	   *TmpServers;
    AnsiString			CurrentServer;

	void __fastcall 	GetServerTree( void );
	void __fastcall		GetServerChildren( TTreeNode *node, TServer *parent );
	void __fastcall 	GetServer( void );
	void __fastcall 	GetServer( TServer *serv );

public:		// User declarations
	__fastcall 			TServersWin(TComponent* Owner);
	void __fastcall		SetCurrentServer( AnsiString server );
};
//---------------------------------------------------------------------------
extern PACKAGE TServersWin *ServersWin;
//---------------------------------------------------------------------------
#endif
