/* $Id: OptionsWnd.h,v 1.34 2005/09/08 18:58:50 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef OptionsWndH
#define OptionsWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "PageMngr.hpp"
#include "RxCombos.hpp"
#include "ToolEdit.hpp"
#include <Mask.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
#include "Prefs.h"
#include "RXSpin.hpp"

class TIRCList;
class TFileFilter;

class TOptionsWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TPageControl *Pages;
	TTabSheet *PageFonts;
	TTabSheet *PageColours;
	TTabSheet *PageFilters;
	TComboEdit *FontStatus;
	TLabel *Label1;
	TComboEdit *FontChannel;
	TLabel *Label2;
	TComboEdit *FontQuery;
	TLabel *Label3;
	TComboEdit *FontNicklist;
	TLabel *Label4;
	TComboEdit *FontTopic;
	TLabel *Label5;
	TComboEdit *FontInput;
	TLabel *Label6;
	TFontDialog *FontDlg;
	TPanel *PanelColoursExample;
	TListBox *ColoursList;
	TColorDialog *ColorDlg;
	TLabel *Label7;
	TPanel *PanelColour;
	TLabel *Label8;
	TBevel *Bevel2;
	TComboBox *ColoursSetName;
	TButton *ColoursLoad;
	TButton *ColoursSave;
	TButton *ColoursDelete;
	TListView *Filters;
	TRichEdit *FiltersShow;
	TButton *FiltersAdd;
	TButton *FiltersMod;
	TButton *FiltersDel;
	TButton *FiltersUp;
	TButton *FiltersDown;
	TTabSheet *PageLogging;
	TDirectoryEdit *LogDirectory;
	TLabel *Label11;
	TCheckBox *LogTimestamp;
	TCheckBox *LogStripCodes;
	TLabel *Label12;
	TCheckBox *LogChannels;
	TCheckBox *LogServers;
	TCheckBox *LogQueries;
	TTabSheet *PageDCCSend;
	TLabel *Label13;
	TDirectoryEdit *DCCSendDir;
	TListBox *List;
	TPanel *PanelOnDCCSendOffer;
	TLabel *Label14;
	TRadioButton *DCCSendAsk;
	TRadioButton *DCCSendAccept;
	TRadioButton *DCCSendIgnore;
	TLabel *Label15;
	TPanel *Panel1;
	TRadioButton *DCCSendAsk2;
	TRadioButton *DCCSendOverwrite;
	TRadioButton *DCCSendResume;
	TLabel *Label16;
	TBevel *Bevel5;
	TLabel *Label17;
	TBevel *Bevel6;
	TLabel *Label18;
	TBevel *Bevel7;
	TListView *DCCSendFilters;
	TButton *DCCSendAdd;
	TButton *DCCSendEdit;
	TButton *DCCSendDel;
	TTabSheet *PageDCCChat;
	TBevel *Bevel8;
	TLabel *Label19;
	TLabel *Label20;
	TPanel *Panel2;
	TRadioButton *DCCChatAsk;
	TRadioButton *DCCChatAccept;
	TRadioButton *DCCChatIgnore;
	TTabSheet *PageAppearance;
	TCheckBox *MinimizeTray;
	TCheckBox *ShowModes;
	TCheckBox *ShowTimestamp;
	TCheckBox *AdaptiveNick;
	TTabSheet *PageMisc;
	TCheckBox *URLCatcherEnable;
	TCheckBox *URLNewBrowser;
	TLabel *Label9;
	TFilenameEdit *URLBrowser;
	TFilenameEdit *URLFile;
	TCheckBox *URLSave;
	TLabel *Label10;
	TBevel *Bevel3;
	TLabel *Label21;
	TBevel *Bevel4;
	TCheckBox *WhoOnJoin;
	TRxSpinEdit *WhoOnJoinMaxUsers;
	TLabel *Label22;
	TCheckBox *MiscRejoin;
	TBevel *Bevel9;
	TLabel *Label23;
	TFilenameEdit *PagerSound;
	TSpeedButton *PagerSoundTest;
	TLabel *Label24;
	TPanel *PagerPanel;
	TRadioButton *PagerNever;
	TRadioButton *PagerAlways;
	TRadioButton *PagerAway;
	TCheckBox *ConfirmQuit;
	TCheckBox *StripColors;
	TTabSheet *PageDCC;
	TBevel *Bevel10;
	TLabel *Label25;
	TPanel *PanelDCCPorts;
	TRadioButton *DCCBindAny;
	TRadioButton *DCCBindRange;
	TRxSpinEdit *DCCBindFrom;
	TRxSpinEdit *DCCBindTo;
	TLabel *Label26;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label29;
	TTabSheet *PageScripts;
	TLabel *Label30;
	TBevel *Bevel11;
	TListView *Scripts;
	TButton *ScriptsAdd;
	TButton *ScriptsRemove;
	TOpenDialog *OpenDlg;
	TTabSheet *PageAliases;
	TLabel *Label31;
	TBevel *Bevel12;
	TListView *Aliases;
	TButton *AliasesEdit;
	TButton *AliasesRemove;
	TButton *AliasesAdd;
	TBevel *Bevel13;
	TLabel *Label32;
	TLabel *Label33;
	TLabel *Label34;
	TCheckBox *UseLagMeter;
	TComboBox *SortNickList;
	TLabel *Label35;
	TCheckBox *RestoreOnPrivMsg;
	TTabSheet *PageSounds;
	TBevel *Bevel14;
	TLabel *Label36;
	TFilenameEdit *SoundsInviteFile;
	TSpeedButton *SoundsInvitePlay;
	TFilenameEdit *SoundsNoticeFile;
	TSpeedButton *SoundsNoticePlay;
	TFilenameEdit *SoundsKickFile;
	TSpeedButton *SoundsKickPlay;
	TFilenameEdit *SoundsDisconnectFile;
	TSpeedButton *SoundsDisconnectPlay;
	TFilenameEdit *SoundsQueryFile;
	TSpeedButton *SoundsQueryPlay;
	TFilenameEdit *SoundsDCCOfferFile;
	TSpeedButton *SoundsDCCOfferPlay;
	TFilenameEdit *SoundsDCCFailFile;
	TSpeedButton *SoundsDCCFailPlay;
	TFilenameEdit *SoundsDCCSendCompleteFile;
	TSpeedButton *SoundsDCCSendCompletePlay;
	TCheckBox *SoundsInvite;
	TCheckBox *SoundsNotice;
	TCheckBox *SoundsKick;
	TCheckBox *SoundsDisconnect;
	TCheckBox *SoundsQuery;
	TCheckBox *SoundsDCCOffer;
	TCheckBox *SoundsDCCFail;
	TCheckBox *SoundsDCCSendComplete;
	TCheckBox *SoundsDCCRecvComplete;
	TFilenameEdit *SoundsDCCRecvCompleteFile;
	TSpeedButton *SoundsDCCRecvCompletePlay;
	TLabel *Label37;
	TEdit *LogNameFormat;
	TLabel *Label38;
	TBevel *Bevel15;
	TBevel *Bevel16;
	TLabel *Label39;
	TGroupBox *GroupBox1;
	TLabel *Label40;
	TLabel *Label41;
	TCheckBox *SoundsNotify;
	TFilenameEdit *SoundsNotifyFile;
	TSpeedButton *SoundsNotifyPlay;
	TCheckBox *ColoursBold;
	TCheckBox *ColoursItalic;
	TCheckBox *FlashStatusBar;
	TTabSheet *PageDoubleClick;
	TLabel *Label42;
	TBevel *Bevel17;
	TPanel *PanelDblClickNickList;
	TRadioButton *DblClickNickWhois;
	TRadioButton *DblClickNickQuery;
	TRadioButton *DblClickNickCallCmd;
	TEdit *DblClickNickCmd;
	TPanel *PanelDblClickChannel;
	TRadioButton *DblClickChannelEditProps;
	TRadioButton *DblClickChannelCallCmd;
	TEdit *DblClickChannelCmd;
	TBevel *Bevel18;
	TLabel *Label43;
	TBevel *Bevel19;
	TLabel *Label44;
	TPanel *PanelDblClickQuery;
	TRadioButton *DblClickQueryWhois;
	TRadioButton *DblClickQueryCallCmd;
	TEdit *DblClickQueryCmd;
	TBevel *Bevel20;
	TLabel *Label45;
	TPanel *PanelDblClickServer;
	TRadioButton *DblClickServerStats;
	TRadioButton *DblClickServerCallCmd;
	TEdit *DblClickServerCmd;
	TLabel *Label46;
	TComboBox *SortTabBar;
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FontStatusButtonClick(TObject *Sender);
	void __fastcall FontStatusKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FontStatusKeyPress(TObject *Sender, char &Key);
	void __fastcall ColoursListClick(TObject *Sender);
	void __fastcall PanelColourClick(TObject *Sender);
	void __fastcall ColoursSetNameChange(TObject *Sender);
	void __fastcall ColoursDeleteClick(TObject *Sender);
	void __fastcall ColoursSaveClick(TObject *Sender);
	void __fastcall ColoursLoadClick(TObject *Sender);
	void __fastcall FiltersDeletion(TObject *Sender, TListItem *Item);
	void __fastcall FiltersClick(TObject *Sender);
	void __fastcall FiltersDelClick(TObject *Sender);
	void __fastcall FiltersModClick(TObject *Sender);
	void __fastcall FiltersAddClick(TObject *Sender);
	void __fastcall FiltersUpClick(TObject *Sender);
	void __fastcall FiltersDownClick(TObject *Sender);
	void __fastcall ListClick(TObject *Sender);
	void __fastcall DCCSendFiltersClick(TObject *Sender);
	void __fastcall DCCSendDelClick(TObject *Sender);
	void __fastcall DCCSendFiltersDeletion(TObject *Sender, TListItem *Item);
	void __fastcall DCCSendAcceptClick(TObject *Sender);
	void __fastcall DCCSendAddClick(TObject *Sender);
	void __fastcall URLNewBrowserClick(TObject *Sender);
	void __fastcall URLCatcherEnableClick(TObject *Sender);
	void __fastcall URLSaveClick(TObject *Sender);
	void __fastcall WhoOnJoinClick(TObject *Sender);
	void __fastcall PagerNeverClick(TObject *Sender);
	void __fastcall PagerAlwaysClick(TObject *Sender);
	void __fastcall PagerSoundTestClick(TObject *Sender);
	void __fastcall DCCSendEditClick(TObject *Sender);
	void __fastcall DCCBindRangeClick(TObject *Sender);
	void __fastcall ScriptsRemoveClick(TObject *Sender);
	void __fastcall ScriptsAddClick(TObject *Sender);
	void __fastcall ScriptsDeletion(TObject *Sender, TListItem *Item);
	void __fastcall ScriptsClick(TObject *Sender);
	void __fastcall AliasesClick(TObject *Sender);
	void __fastcall AliasesDeletion(TObject *Sender, TListItem *Item);
	void __fastcall AliasesRemoveClick(TObject *Sender);
	void __fastcall AliasesAddClick(TObject *Sender);
	void __fastcall AliasesEditClick(TObject *Sender);
	void __fastcall SoundsInviteClick(TObject *Sender);
	void __fastcall SoundsNoticeClick(TObject *Sender);
	void __fastcall SoundsKickClick(TObject *Sender);
	void __fastcall SoundsDisconnectClick(TObject *Sender);
	void __fastcall SoundsQueryClick(TObject *Sender);
	void __fastcall SoundsDCCOfferClick(TObject *Sender);
	void __fastcall SoundsDCCFailClick(TObject *Sender);
	void __fastcall SoundsDCCSendCompleteClick(TObject *Sender);
	void __fastcall SoundsDCCRecvCompleteClick(TObject *Sender);
	void __fastcall SoundsInvitePlayClick(TObject *Sender);
	void __fastcall SoundsNoticePlayClick(TObject *Sender);
	void __fastcall SoundsKickPlayClick(TObject *Sender);
	void __fastcall SoundsDisconnectPlayClick(TObject *Sender);
	void __fastcall SoundsQueryPlayClick(TObject *Sender);
	void __fastcall SoundsDCCOfferPlayClick(TObject *Sender);
	void __fastcall SoundsDCCFailPlayClick(TObject *Sender);
	void __fastcall SoundsDCCSendCompletePlayClick(TObject *Sender);
	void __fastcall SoundsDCCRecvCompletePlayClick(TObject *Sender);
	void __fastcall SoundsNotifyPlayClick(TObject *Sender);
	void __fastcall SoundsNotifyClick(TObject *Sender);
	void __fastcall ColoursBoldClick(TObject *Sender);
	void __fastcall ColoursItalicClick(TObject *Sender);
	void __fastcall DblClickNickCallCmdClick(TObject *Sender);
	void __fastcall DblClickChannelCallCmdClick(TObject *Sender);
	void __fastcall DblClickQueryCallCmdClick(TObject *Sender);
	void __fastcall DblClickServerCallCmdClick(TObject *Sender);
private:	// User declarations
	TPrefs			   	   *TmpPrefs;
    TIRCList		   	   *ColoursDisplay;

	void __fastcall 		SetFont( TComboEdit *from, TFont *font );
	void __fastcall 		AddDCCSendFilter( TFileFilter *filter );
	TListItem * __fastcall 	AddFilter( TFilter *filter );
	void __fastcall 		AddScript( TScript *script );
	void __fastcall 		AddAlias( TAlias *alias );
    void __fastcall			FillSounds( void );
    void __fastcall			StoreSounds( void );
    void __fastcall			FillDblClickActions( void );
    void __fastcall			StoreDblClickActions( void );

public:		// User declarations
	__fastcall 				TOptionsWin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionsWin *OptionsWin;
//---------------------------------------------------------------------------
#endif
