/* $Id: OptionsWnd.cpp,v 1.39 2005/09/08 18:58:50 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <mmsystem.h>

#include "OptionsWnd.h"
#include "Main.h"
#include "IRCList.h"
#include "Filter.h"
#include "Script.h"
#include "Alias.h"
#include "FileFilter.h"
#include "FilterWnd.h"
#include "FileFilterWnd.h"
#include "EditAliasWnd.h"
#include "OrionScriptInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Placemnt"
#pragma link "PageMngr"
#pragma link "RxCombos"
#pragma link "ToolEdit"
#pragma link "RXSpin"
#pragma resource "*.dfm"
TOptionsWin *OptionsWin;
//---------------------------------------------------------------------------
__fastcall TOptionsWin::TOptionsWin(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FormShow(TObject *Sender)
{
	TmpPrefs  = new TPrefs();
    *TmpPrefs = Prefs;

#if !SCRIPTING
    int idx = List->Items->IndexOf( "Scripts" );

    List->Items->Delete( idx );
#endif

	TIniFile   *ini = new TIniFile( GetCurrentDir() + "\\Data\\Colors.ini" );

    ini->ReadSections( ColoursSetName->Items );

    delete ini;

   	ColoursDisplay = new TIRCList( this, NULL, TmpPrefs->GetColorSet() );

   	ColoursDisplay->Parent = PanelColoursExample;
   	ColoursDisplay->Align  = alClient;

    ColoursDisplay->ShowNick( true );
    ColoursDisplay->ShowTimestamp( true );

    ColoursDisplay->AddRow( NULL, "You are now talking in #testchannel", IRC_INFO );
    ColoursDisplay->AddRow( NULL, "Modes for #testchannel are secret, no external messages", IRC_MODE );
    ColoursDisplay->AddRow( NULL, "Topic for #testchannel is \"Test\"", IRC_TOPIC );
    ColoursDisplay->AddRow( "User1", "Hi!", IRC_OWN_MSG );
    ColoursDisplay->AddRow( NULL, "User2 has joined #testchannel", IRC_JOIN );
    ColoursDisplay->AddRow( "User2", "Hello User1!", IRC_MSG );
    ColoursDisplay->AddRow( NULL, "User3 feels a little bored", IRC_ACTION );
    ColoursDisplay->AddRow( NULL, "Received CTCP VERSION from User3", IRC_CTCP );
    ColoursDisplay->AddRow( NULL, "User2 left #testchannel (Bye)", IRC_PART );
    ColoursDisplay->AddRow( NULL, "Offering file to User3", IRC_DCC );
    ColoursDisplay->AddRow( NULL, "User3 has quit IRC (puff!)", IRC_QUIT );
    ColoursDisplay->AddRow( NULL, "User4 has been kicked from #testchannel by User1 (out!)", IRC_KICK );

    FontStatus->Text   = Prefs.GetStatusFont()->Name + "/" + IntToStr( Prefs.GetStatusFont()->Size );
    FontChannel->Text  = Prefs.GetChannelFont()->Name + "/" + IntToStr( Prefs.GetChannelFont()->Size );
    FontQuery->Text    = Prefs.GetQueryFont()->Name + "/" + IntToStr( Prefs.GetQueryFont()->Size );
    FontTopic->Text    = Prefs.GetTopicFont()->Name + "/" + IntToStr( Prefs.GetTopicFont()->Size );
    FontNicklist->Text = Prefs.GetNicklistFont()->Name + "/" + IntToStr( Prefs.GetNicklistFont()->Size );
    FontInput->Text    = Prefs.GetInputFont()->Name + "/" + IntToStr( Prefs.GetInputFont()->Size );

    ColoursList->ItemIndex = 0;

    ColoursListClick( ColoursList );

    for( int i = 0; i < Prefs.GetFilters()->GetCount(); i++ )
    	AddFilter( new TFilter( Prefs.GetFilters()->Get( i )));

    for( int i = 0; i < Prefs.GetFileFilters()->GetCount(); i++ )
    	AddDCCSendFilter( new TFileFilter( Prefs.GetFileFilters()->Get( i )));

    for( int i = 0; i < Prefs.GetScripts()->GetCount(); i++ )
    	AddScript( new TScript( Prefs.GetScripts()->Get( i )));

    for( int i = 0; i < Prefs.GetAliases()->GetCount(); i++ )
    	AddAlias( new TAlias( Prefs.GetAliases()->Get( i )));

    SortNickList->ItemIndex   = Prefs.GetNickSort();
    SortTabBar->ItemIndex     = Prefs.GetTabBarSort();
    ConfirmQuit->Checked      = Prefs.ConfirmQuit();
    MinimizeTray->Checked     = Prefs.MinimizeToTray();
    RestoreOnPrivMsg->Checked = Prefs.RestoreOnPrivMsg();
    ShowModes->Checked        = Prefs.ShowChanModes();
    ShowTimestamp->Checked    = Prefs.ShowTimestamp();
    AdaptiveNick->Checked     = Prefs.AdaptiveNick();
    StripColors->Checked	  = Prefs.StripColors();
    UseLagMeter->Checked	  = Prefs.UseLagMeter();
    FlashStatusBar->Checked	  = Prefs.FlashStatusBar();
    LogTimestamp->Checked     = Prefs.TimestampLogs();
    LogStripCodes->Checked    = Prefs.StripCodesFromLogs();
    LogChannels->Checked      = Prefs.LogChannels();
    LogQueries->Checked       = Prefs.LogQueries();
    LogServers->Checked       = Prefs.LogServers();
    LogDirectory->Text        = Prefs.GetLogsDir();
    LogNameFormat->Text		  = Prefs.GetLogNameFormat();
    DCCSendDir->Text          = Prefs.GetDownloadsDir( "" );
    URLNewBrowser->Checked    = Prefs.OpenNewBrowser();
    URLCatcherEnable->Checked = Prefs.CatchURLs();
    URLBrowser->Text          = Prefs.GetBrowser();
    URLSave->Checked          = Prefs.SaveURLs();
    URLFile->Text             = Prefs.GetURLCatcherFile();
    WhoOnJoin->Checked		  = Prefs.DoWhoOnJoin();
    WhoOnJoinMaxUsers->Value  = Prefs.GetWhoOnJoinMaxUsers();
    MiscRejoin->Checked 	  = Prefs.RejoinOnKick();
    PagerSound->Text		  = Prefs.GetPagerSound();
   	DCCBindRange->Checked     = Prefs.LimitDCCPorts();
   	DCCBindAny->Checked       = !Prefs.LimitDCCPorts();
    DCCBindFrom->Value        = Prefs.GetDCCPortFrom();
   	DCCBindTo->Value          = Prefs.GetDCCPortTo();

    URLNewBrowserClick( URLNewBrowser );
	URLCatcherEnableClick( URLCatcherEnable );
    WhoOnJoinClick( WhoOnJoin );
    DCCBindRangeClick( DCCBindRange );

    switch( Prefs.GetSendReqAction() ) {

    	case DCC_ASK:
        	DCCSendAsk->Checked = true;
            break;

    	case DCC_ACCEPT:
        	DCCSendAccept->Checked = true;
            DCCSendAcceptClick( DCCSendAccept );
            break;

    	case DCC_IGNORE:
        	DCCSendIgnore->Checked = true;
            break;
    }

    switch( Prefs.GetRecvAction() ) {

    	case DCC_RECV_ASK:
        	DCCSendAsk2->Checked = true;
            break;

    	case DCC_RECV_RESUME:
        	DCCSendResume->Checked = true;
            break;

    	case DCC_RECV_OVERWRITE:
        	DCCSendOverwrite->Checked = true;
            break;
    }

    switch( Prefs.GetChatReqAction() ) {

    	case DCC_ASK:
        	DCCChatAsk->Checked = true;
            break;

    	case DCC_ACCEPT:
        	DCCChatAccept->Checked = true;
            break;

    	case DCC_IGNORE:
        	DCCChatIgnore->Checked = true;
            break;
    }

    switch( Prefs.GetPageWhen() ) {

    	case PAGE_NEVER:
        	PagerNever->Checked = true;
            PagerNeverClick( PagerNever );
        	break;

    	case PAGE_ALWAYS:
        	PagerAlways->Checked = true;
            PagerAlwaysClick( PagerAlways );
        	break;

    	case PAGE_AWAY:
        	PagerAway->Checked = true;
            PagerAlwaysClick( PagerAway );
        	break;
    }

    FillSounds();
    FillDblClickActions();

    List->ItemIndex   = 0;
    Pages->ActivePage = Pages->Pages[ 0 ];
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete TmpPrefs;

    OptionsWin = NULL;

    Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::CancelClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::OkClick(TObject *Sender)
{
	Prefs = *TmpPrefs;

    SetFont( FontStatus,   Prefs.GetStatusFont() );
    SetFont( FontChannel,  Prefs.GetChannelFont() );
    SetFont( FontQuery,    Prefs.GetQueryFont() );
    SetFont( FontTopic,    Prefs.GetTopicFont() );
    SetFont( FontNicklist, Prefs.GetNicklistFont() );
    SetFont( FontInput,    Prefs.GetInputFont() );

    Prefs.GetFilters()->Clear();

    for( int i = 0; i < Filters->Items->Count; i++ ) {
    	TListItem  *item = Filters->Items->Item[ i ];
        TFilter	   *filter = reinterpret_cast<TFilter *>( item->Data );

    	filter->SetEnabled( item->Checked );

    	Prefs.GetFilters()->Add( new TFilter( filter ));
    }

    Prefs.GetFileFilters()->Clear();

    for( int i = 0; i < DCCSendFilters->Items->Count; i++ ) {
    	TListItem  	*item = DCCSendFilters->Items->Item[ i ];
        TFileFilter	*filter = reinterpret_cast<TFileFilter *>( item->Data );

    	filter->SetEnabled( item->Checked );

    	Prefs.GetFileFilters()->Add( new TFileFilter( filter ));
    }

    Prefs.GetScripts()->Clear();

    for( int i = 0; i < Scripts->Items->Count; i++ ) {
    	TListItem  	*item = Scripts->Items->Item[ i ];
        TScript		*script = reinterpret_cast<TScript *>( item->Data );

    	script->SetEnabled( item->Checked );

    	Prefs.GetScripts()->Add( new TScript( script ));
    }

    Prefs.GetAliases()->Clear();

    for( int i = 0; i < Aliases->Items->Count; i++ ) {
    	TListItem  	*item = Aliases->Items->Item[ i ];
        TAlias		*alias = reinterpret_cast<TAlias *>( item->Data );

    	Prefs.GetAliases()->Add( new TAlias( alias ));
    }

    Prefs.GetAliases()->ReHash();

    Prefs.SetNickSort( static_cast<TNickSort>( SortNickList->ItemIndex ));
    Prefs.SetTabBarSort( static_cast<TTabBarSort>( SortTabBar->ItemIndex ));
    Prefs.SetConfirmQuit( ConfirmQuit->Checked );
    Prefs.SetMinimizeToTray( MinimizeTray->Checked );
    Prefs.SetRestoreOnPrivMsg( RestoreOnPrivMsg->Checked );
    Prefs.SetShowChanModes( ShowModes->Checked );
    Prefs.SetShowTimestamp( ShowTimestamp->Checked );
    Prefs.SetAdaptiveNick( AdaptiveNick->Checked );
    Prefs.SetStripColors( StripColors->Checked );
    Prefs.SetUseLagMeter( UseLagMeter->Checked );
    Prefs.SetFlashStatusBar( FlashStatusBar->Checked );
    Prefs.SetTimestampLogs( LogTimestamp->Checked );
    Prefs.SetStripCodesFromLogs( LogStripCodes->Checked );
    Prefs.SetLogChannels( LogChannels->Checked );
    Prefs.SetLogQueries( LogQueries->Checked );
    Prefs.SetLogServers( LogServers->Checked );
    Prefs.SetLogsDir( LogDirectory->Text );
    Prefs.SetLogNameFormat( LogNameFormat->Text );
    Prefs.SetDownloadsDir( DCCSendDir->Text );
    Prefs.SetOpenNewBrowser( URLNewBrowser->Checked );
    Prefs.SetCatchURLs( URLCatcherEnable->Checked );
    Prefs.SetSaveURLs( URLSave->Checked );
    Prefs.SetBrowser( URLBrowser->Text );
    Prefs.SetURLCatcherFile( URLFile->Text );
    Prefs.SetWhoOnJoin( WhoOnJoin->Checked );
    Prefs.SetWhoOnJoinMaxUsers( WhoOnJoinMaxUsers->Value );
    Prefs.SetRejoinOnKick( MiscRejoin->Checked );
    Prefs.SetPagerSound( PagerSound->FileName );
    Prefs.SetLimitDCCPorts( DCCBindRange->Checked );

    if( DCCBindFrom->Value < DCCBindTo->Value ) {
        Prefs.SetDCCPortFrom( DCCBindFrom->Value );
        Prefs.SetDCCPortTo( DCCBindTo->Value );
    } else {
        Prefs.SetDCCPortFrom( DCCBindTo->Value );
        Prefs.SetDCCPortTo( DCCBindFrom->Value );
    }

   	if( DCCSendAsk->Checked )
    	Prefs.SetSendReqAction( DCC_ASK );
    else if( DCCSendAccept->Checked )
        Prefs.SetSendReqAction( DCC_ACCEPT );
    else
    	Prefs.SetSendReqAction( DCC_IGNORE );

    if( DCCSendAsk2->Checked )
    	Prefs.SetRecvAction( DCC_RECV_ASK );
    else if( DCCSendResume->Checked )
    	Prefs.SetRecvAction( DCC_RECV_RESUME );
    else
    	Prefs.SetRecvAction( DCC_RECV_OVERWRITE );

	if( DCCChatAsk->Checked )
    	Prefs.SetChatReqAction( DCC_ASK );
    else if( DCCChatAccept->Checked )
    	Prefs.SetChatReqAction( DCC_ACCEPT );
	else
    	Prefs.SetChatReqAction( DCC_IGNORE );

    if( PagerNever->Checked )
    	Prefs.SetPageWhen( PAGE_NEVER );
    else if( PagerAlways->Checked )
    	Prefs.SetPageWhen( PAGE_ALWAYS );
    else
    	Prefs.SetPageWhen( PAGE_AWAY );

    StoreSounds();
    StoreDblClickActions();

    Prefs.Save();
	Close();

    OrionScript::Reload( Prefs.GetScripts() );
    MainWin->PrefsChanged();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SetFont( TComboEdit *from, TFont *font )
{
    font->Name = from->Text.SubString( 1, from->Text.Pos( "/" ) - 1 );
    font->Size = from->Text.SubString( from->Text.Pos( "/" ) + 1, from->Text.Length() ).ToIntDef( 11 );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FontStatusButtonClick(TObject *Sender)
{
	TComboEdit *edit = reinterpret_cast<TComboEdit *>( Sender );

    SetFont( edit, FontDlg->Font );

    if( FontDlg->Execute() )
    	edit->Text = FontDlg->Font->Name + "/" + IntToStr( FontDlg->Font->Size );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FontStatusKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FontStatusKeyPress(TObject *Sender, char &Key)
{
	Key = '\0';
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursListClick(TObject *Sender)
{
	if( ColoursList->ItemIndex >= 0 ) {
    	ColorSet   *set = TmpPrefs->GetColorSet();

		PanelColour->Color     = set->GetColor( (TIRCEvent)ColoursList->ItemIndex );
		ColoursBold->Checked   = set->IsBold( (TIRCEvent)ColoursList->ItemIndex );
		ColoursItalic->Checked = set->IsItalic( (TIRCEvent)ColoursList->ItemIndex );
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::PanelColourClick(TObject *Sender)
{
	ColorDlg->Color = PanelColour->Color;

	if( ColorDlg->Execute() ) {

    	PanelColour->Color = ColorDlg->Color;

    	TmpPrefs->GetColorSet()->SetColor( (TIRCEvent)ColoursList->ItemIndex, ColorDlg->Color );

        ColoursDisplay->Repaint();
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursSetNameChange(TObject *Sender)
{
	bool enable = !ColoursSetName->Text.IsEmpty();

    ColoursLoad->Enabled   = enable;
    ColoursSave->Enabled   = enable;
    ColoursDelete->Enabled = enable;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursDeleteClick(TObject *Sender)
{
	TIniFile   *ini = new TIniFile( GetCurrentDir() + "\\Data\\Colors.ini" );
    int			idx;

    try {
    	ini->EraseSection( ColoursSetName->Text );
    }
    catch(...) {
    }

    if(( idx = ColoursSetName->Items->IndexOf( ColoursSetName->Text )) >= 0 )
		ColoursSetName->Items->Delete( idx );

    ColoursSetName->Text = "";

    delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursSaveClick(TObject *Sender)
{
	TIniFile   *ini = new TIniFile( GetCurrentDir() + "\\Data\\Colors.ini" );

	TmpPrefs->GetColorSet()->Save( ini, ColoursSetName->Text );

    if( ColoursSetName->Items->IndexOf( ColoursSetName->Text ) < 0 )
		ColoursSetName->Items->Add( ColoursSetName->Text );

    delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursLoadClick(TObject *Sender)
{
	TIniFile   *ini = new TIniFile( GetCurrentDir() + "\\Data\\Colors.ini" );

	TmpPrefs->GetColorSet()->Load( ini, ColoursSetName->Text );

    ColoursDisplay->Repaint();

    delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersDeletion(TObject *Sender,
      TListItem *Item)
{
	delete reinterpret_cast<TFilter *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersClick(TObject *Sender)
{
	bool enable = Filters->Selected != NULL;

    FiltersMod->Enabled  = enable;
    FiltersDel->Enabled  = enable;
    FiltersUp->Enabled   = enable;
    FiltersDown->Enabled = enable;

    if( Filters->Selected ) {
		TFilter	   *filter = reinterpret_cast<TFilter *>( Filters->Selected->Data );

        filter->Describe( FiltersShow );

    } else
    	FiltersShow->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersDelClick(TObject *Sender)
{
	Filters->Selected->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersModClick(TObject *Sender)
{
	TListItem  *item = Filters->Selected;

    if( item ) {
		TFilter	   *filter = reinterpret_cast<TFilter *>( item->Data );
		TFilterWin *wnd = new TFilterWin( NULL, filter );

        if( wnd->ShowModal() == mrOk ) {

        	item->Caption = filter->GetName();

            FiltersClick( Filters );
        }

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersAddClick(TObject *Sender)
{
    TFilter	   *filter = new TFilter();
    TFilterWin *wnd = new TFilterWin( NULL, filter );

    if( wnd->ShowModal() == mrOk ) {

        Filters->Selected = AddFilter( filter );

        FiltersClick( Filters );

    } else
    	delete filter;

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersUpClick(TObject *Sender)
{
	TListItem  *item = Filters->Selected;

    if( item && ( item->Index > 0 )) {
    	TListItem  *item2 = Filters->Items->Insert( item->Index - 1 );

        item2->Data    = item->Data;
        item2->Caption = item->Caption;
        item2->Checked = item->Checked;

        item->Data = NULL;

        item->Delete();

        Filters->Selected = item2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FiltersDownClick(TObject *Sender)
{
	TListItem  *item = Filters->Selected;

    if( item && ( item->Index + 1 < Filters->Items->Count )) {
    	TListItem  *item2 = Filters->Items->Insert( item->Index + 2 );

        item2->Data    = item->Data;
        item2->Caption = item->Caption;
        item2->Checked = item->Checked;

        item->Data = NULL;

        item->Delete();

        Filters->Selected = item2;
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ListClick(TObject *Sender)
{
	if( List->ItemIndex >= 0 )
    	Pages->ActivePage = Pages->Pages[ List->ItemIndex ];
    else
    	List->ItemIndex = Pages->ActivePage->TabIndex;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendFiltersClick(TObject *Sender)
{
	bool enable = DCCSendFilters->Selected != NULL;

	DCCSendEdit->Enabled = enable;
	DCCSendDel->Enabled  = enable;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendDelClick(TObject *Sender)
{
	TListItem *item = DCCSendFilters->Selected;

    if( item )
    	item->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendFiltersDeletion(TObject *Sender,
      TListItem *Item)
{
	delete reinterpret_cast<TFileFilter *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendAcceptClick(TObject *Sender)
{
	bool enable = DCCSendAccept->Checked;

	DCCSendAsk2->Enabled      = enable;
	DCCSendResume->Enabled    = enable;
	DCCSendOverwrite->Enabled = enable;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendAddClick(TObject *Sender)
{
	TFileFilter    *filter = new TFileFilter();
    TFileFilterWin *wnd = new TFileFilterWin( NULL, filter );

    if( wnd->ShowModal() == mrOk )
    	AddDCCSendFilter( filter );
    else
    	delete filter;

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AddDCCSendFilter( TFileFilter *filter )
{
	TListItem  *item = DCCSendFilters->Items->Add();

    item->Data    = filter;
    item->Caption = filter->GetName();
    item->Checked = filter->IsEnabled();

    item->SubItems->Add(( filter->GetAction() == FILE_IGNORE ) ? "Ignore" : "Save" );
}
//---------------------------------------------------------------------------
TListItem * __fastcall TOptionsWin::AddFilter( TFilter *filter )
{
    TListItem  *item = Filters->Items->Add();

    item->Data    = filter;
    item->Caption = filter->GetName();
    item->Checked = filter->IsEnabled();

    return( item );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::URLNewBrowserClick(TObject *Sender)
{
	URLBrowser->Enabled = URLNewBrowser->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::URLCatcherEnableClick(TObject *Sender)
{
	URLSave->Enabled = URLCatcherEnable->Checked;

    if( URLSave->Enabled )
    	URLSaveClick( URLSave );
    else
    	URLFile->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::URLSaveClick(TObject *Sender)
{
	URLFile->Enabled = URLSave->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::WhoOnJoinClick(TObject *Sender)
{
	WhoOnJoinMaxUsers->Enabled = WhoOnJoin->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::PagerNeverClick(TObject *Sender)
{
	PagerSound->Enabled     = false;
	PagerSoundTest->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::PagerAlwaysClick(TObject *Sender)
{
	PagerSound->Enabled     = true;
	PagerSoundTest->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::PagerSoundTestClick(TObject *Sender)
{
	PlaySound( PagerSound->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCSendEditClick(TObject *Sender)
{
	TListItem  *item = DCCSendFilters->Selected;

    if( item ) {
		TFileFilter	   *filter = reinterpret_cast<TFileFilter *>( item->Data );
		TFileFilterWin *wnd = new TFileFilterWin( NULL, filter );

        if( wnd->ShowModal() == mrOk ) {

		    item->Caption = filter->GetName();
		    item->Checked = filter->IsEnabled();

		    item->SubItems->Strings[0] = ( filter->GetAction() == FILE_IGNORE ) ? "Ignore" : "Save";

            DCCSendFiltersClick( Filters );
        }

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DCCBindRangeClick(TObject *Sender)
{
	DCCBindFrom->Enabled = DCCBindRange->Checked;
	DCCBindTo->Enabled   = DCCBindRange->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ScriptsRemoveClick(TObject *Sender)
{
	Scripts->Items->BeginUpdate();

    for( int i = Scripts->Items->Count - 1; i >= 0; i-- ) {
    	TListItem  *item = Scripts->Items->Item[ i ];

        if( item->Selected )
        	item->Delete();
    }

    Scripts->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ScriptsAddClick(TObject *Sender)
{
	OpenDlg->Title      = "Select a script...";
    OpenDlg->DefaultExt = "ophp";
    OpenDlg->Filter     = "Orion scripts|*.ophp";

    if( OpenDlg->Execute() ) {
    	TScript *script = new TScript();

        script->SetFile( OpenDlg->FileName );

        AddScript( script );
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AddScript( TScript *script )
{
    TListItem  *item = Scripts->Items->Add();

    item->Caption = script->GetFile();
    item->Checked = script->IsEnabled();
    item->Data    = script;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ScriptsDeletion(TObject *Sender, TListItem *Item)
{
	delete reinterpret_cast<TScript *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ScriptsClick(TObject *Sender)
{
	ScriptsRemove->Enabled = Scripts->SelCount > 0;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AliasesClick(TObject *Sender)
{
	AliasesRemove->Enabled = Aliases->SelCount > 0;
	AliasesEdit->Enabled   = AliasesRemove->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AliasesDeletion(TObject *Sender, TListItem *Item)
{
	delete reinterpret_cast<TAlias *>( Item->Data );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AliasesRemoveClick(TObject *Sender)
{
	if( Aliases->Selected )
		Aliases->Selected->Delete();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AddAlias( TAlias *alias )
{
    TListItem  *item = Aliases->Items->Add();

    item->Caption = alias->GetName();
    item->Data    = alias;

    item->SubItems->Add( alias->GetCommand() );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AliasesAddClick(TObject *Sender)
{
	TEditAliasWin  *wnd = new TEditAliasWin( NULL, NULL );

    if( wnd->ShowModal() == mrOk )
    	AddAlias( wnd->GetItem() );

    delete wnd;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::AliasesEditClick(TObject *Sender)
{
	TListItem *item = Aliases->Selected;

	if( item ) {
    	TAlias 		   *alias = reinterpret_cast<TAlias *>( item->Data );
		TEditAliasWin  *wnd = new TEditAliasWin( NULL, alias );

		if( wnd->ShowModal() == mrOk ) {

        	item->Caption                = alias->GetName();
            item->SubItems->Strings[ 0 ] = alias->GetCommand();

            item->Update();
        }

        delete wnd;
    }
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsInviteClick(TObject *Sender)
{
	SoundsInviteFile->Enabled = SoundsInvite->Checked;
	SoundsInvitePlay->Enabled = SoundsInvite->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsNoticeClick(TObject *Sender)
{
	SoundsNoticeFile->Enabled = SoundsNotice->Checked;
	SoundsNoticePlay->Enabled = SoundsNotice->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsKickClick(TObject *Sender)
{
	SoundsKickFile->Enabled = SoundsKick->Checked;
	SoundsKickPlay->Enabled = SoundsKick->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDisconnectClick(TObject *Sender)
{
	SoundsDisconnectFile->Enabled = SoundsDisconnect->Checked;
	SoundsDisconnectPlay->Enabled = SoundsDisconnect->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsQueryClick(TObject *Sender)
{
	SoundsQueryFile->Enabled = SoundsQuery->Checked;
	SoundsQueryPlay->Enabled = SoundsQuery->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCOfferClick(TObject *Sender)
{
	SoundsDCCOfferFile->Enabled = SoundsDCCOffer->Checked;
	SoundsDCCOfferPlay->Enabled = SoundsDCCOffer->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCFailClick(TObject *Sender)
{
	SoundsDCCFailFile->Enabled = SoundsDCCFail->Checked;
	SoundsDCCFailPlay->Enabled = SoundsDCCFail->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCSendCompleteClick(TObject *Sender)
{
	SoundsDCCSendCompleteFile->Enabled = SoundsDCCSendComplete->Checked;
	SoundsDCCSendCompletePlay->Enabled = SoundsDCCSendComplete->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCRecvCompleteClick(TObject *Sender)
{
	SoundsDCCRecvCompleteFile->Enabled = SoundsDCCRecvComplete->Checked;
	SoundsDCCRecvCompletePlay->Enabled = SoundsDCCRecvComplete->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsNotifyClick(TObject *Sender)
{
	SoundsNotifyFile->Enabled = SoundsNotify->Checked;
	SoundsNotifyPlay->Enabled = SoundsNotify->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsInvitePlayClick(TObject *Sender)
{
	PlaySound( SoundsInviteFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsNoticePlayClick(TObject *Sender)
{
	PlaySound( SoundsNoticeFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsKickPlayClick(TObject *Sender)
{
	PlaySound( SoundsKickFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDisconnectPlayClick(TObject *Sender)
{
	PlaySound( SoundsDisconnectFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsQueryPlayClick(TObject *Sender)
{
	PlaySound( SoundsQueryFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCOfferPlayClick(TObject *Sender)
{
	PlaySound( SoundsDCCOfferFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCFailPlayClick(TObject *Sender)
{
	PlaySound( SoundsDCCFailFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCSendCompletePlayClick(TObject *Sender)
{
	PlaySound( SoundsDCCSendCompleteFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsDCCRecvCompletePlayClick(TObject *Sender)
{
	PlaySound( SoundsDCCRecvCompleteFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::SoundsNotifyPlayClick(TObject *Sender)
{
	PlaySound( SoundsNotifyFile->FileName.c_str(), NULL, SND_ASYNC | SND_FILENAME );
}
//---------------------------------------------------------------------------
void __fastcall	TOptionsWin::FillSounds( void )
{
	SoundsMgr  *mgr = Prefs.GetSounds();

	SoundsInviteFile->FileName          = mgr->GetSound( SoundsMgr::INVITE            );
	SoundsNoticeFile->FileName          = mgr->GetSound( SoundsMgr::NOTICE            );
	SoundsKickFile->FileName            = mgr->GetSound( SoundsMgr::KICK              );
	SoundsDisconnectFile->FileName      = mgr->GetSound( SoundsMgr::DISCONNECT        );
	SoundsQueryFile->FileName           = mgr->GetSound( SoundsMgr::QUERY             );
	SoundsDCCOfferFile->FileName        = mgr->GetSound( SoundsMgr::DCC_OFFER         );
	SoundsDCCFailFile->FileName         = mgr->GetSound( SoundsMgr::DCC_FAIL          );
	SoundsDCCSendCompleteFile->FileName = mgr->GetSound( SoundsMgr::DCC_SEND_COMPLETE );
	SoundsDCCRecvCompleteFile->FileName = mgr->GetSound( SoundsMgr::DCC_RECV_COMPLETE );
	SoundsNotifyFile->FileName          = mgr->GetSound( SoundsMgr::NOTIFY            );

	SoundsInvite->Checked          = mgr->IsSoundEnabled( SoundsMgr::INVITE            );
	SoundsNotice->Checked          = mgr->IsSoundEnabled( SoundsMgr::NOTICE            );
	SoundsKick->Checked            = mgr->IsSoundEnabled( SoundsMgr::KICK              );
	SoundsDisconnect->Checked      = mgr->IsSoundEnabled( SoundsMgr::DISCONNECT        );
	SoundsQuery->Checked           = mgr->IsSoundEnabled( SoundsMgr::QUERY             );
	SoundsDCCOffer->Checked        = mgr->IsSoundEnabled( SoundsMgr::DCC_OFFER         );
	SoundsDCCFail->Checked         = mgr->IsSoundEnabled( SoundsMgr::DCC_FAIL          );
	SoundsDCCSendComplete->Checked = mgr->IsSoundEnabled( SoundsMgr::DCC_SEND_COMPLETE );
	SoundsDCCRecvComplete->Checked = mgr->IsSoundEnabled( SoundsMgr::DCC_RECV_COMPLETE );
	SoundsNotify->Checked          = mgr->IsSoundEnabled( SoundsMgr::NOTIFY            );

	SoundsInviteClick( SoundsInvite );
	SoundsNoticeClick( SoundsNotice );
	SoundsKickClick( SoundsKick );
	SoundsDisconnectClick( SoundsDisconnect );
	SoundsQueryClick( SoundsQuery );
	SoundsDCCOfferClick( SoundsDCCOffer );
	SoundsDCCFailClick( SoundsDCCFail );
	SoundsDCCSendCompleteClick( SoundsDCCSendComplete );
	SoundsDCCRecvCompleteClick( SoundsDCCRecvComplete );
	SoundsNotifyClick( SoundsNotify );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::StoreSounds( void )
{
	SoundsMgr  *mgr = Prefs.GetSounds();

	mgr->SetSound( SoundsMgr::INVITE,            SoundsInviteFile->FileName,          SoundsInvite->Checked          );
	mgr->SetSound( SoundsMgr::NOTICE,            SoundsNoticeFile->FileName,          SoundsNotice->Checked          );
	mgr->SetSound( SoundsMgr::KICK,              SoundsKickFile->FileName,            SoundsKick->Checked            );
	mgr->SetSound( SoundsMgr::DISCONNECT,        SoundsDisconnectFile->FileName,      SoundsDisconnect->Checked      );
	mgr->SetSound( SoundsMgr::QUERY,             SoundsQueryFile->FileName,           SoundsQuery->Checked           );
	mgr->SetSound( SoundsMgr::DCC_OFFER,         SoundsDCCOfferFile->FileName,        SoundsDCCOffer->Checked        );
	mgr->SetSound( SoundsMgr::DCC_FAIL,          SoundsDCCFailFile->FileName,         SoundsDCCFail->Checked         );
	mgr->SetSound( SoundsMgr::DCC_SEND_COMPLETE, SoundsDCCSendCompleteFile->FileName, SoundsDCCSendComplete->Checked );
	mgr->SetSound( SoundsMgr::DCC_RECV_COMPLETE, SoundsDCCRecvCompleteFile->FileName, SoundsDCCRecvComplete->Checked );
	mgr->SetSound( SoundsMgr::NOTIFY,            SoundsNotifyFile->FileName,          SoundsNotify->Checked          );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursBoldClick(TObject *Sender)
{
   	TmpPrefs->GetColorSet()->SetBold( (TIRCEvent)ColoursList->ItemIndex, ColoursBold->Checked );
	ColoursDisplay->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::ColoursItalicClick(TObject *Sender)
{
   	TmpPrefs->GetColorSet()->SetItalic( (TIRCEvent)ColoursList->ItemIndex, ColoursItalic->Checked );
	ColoursDisplay->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DblClickNickCallCmdClick(TObject *Sender)
{
	DblClickNickCmd->Enabled = DblClickNickCallCmd->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DblClickChannelCallCmdClick(TObject *Sender)
{
	DblClickChannelCmd->Enabled = DblClickChannelCallCmd->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DblClickQueryCallCmdClick(TObject *Sender)
{
	DblClickQueryCmd->Enabled = DblClickQueryCallCmd->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::DblClickServerCallCmdClick(TObject *Sender)
{
	DblClickServerCmd->Enabled = DblClickServerCallCmd->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::FillDblClickActions( void )
{
	switch( Prefs.GetDblClickAction( DBL_CLICK_NICK ).GetAction() ) {

    	case DblClickAction::WHOIS:
        	DblClickNickWhois->Checked = true;
        	break;

    	case DblClickAction::OPEN_QUERY:
        	DblClickNickQuery->Checked = true;
        	break;

    	case DblClickAction::CALL_CMD:
        	DblClickNickCallCmd->Checked = true;
        	break;
    }

	switch( Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).GetAction() ) {

    	case DblClickAction::CHANNEL_PROPS:
	        DblClickChannelEditProps->Checked = true;
        	break;

    	case DblClickAction::CALL_CMD:
        	DblClickChannelCallCmd->Checked = true;
        	break;
    }

	switch( Prefs.GetDblClickAction( DBL_CLICK_QUERY ).GetAction() ) {

    	case DblClickAction::WHOIS:
        	DblClickQueryWhois->Checked = true;
        	break;

    	case DblClickAction::CALL_CMD:
        	DblClickQueryCallCmd->Checked = true;
        	break;
    }

	switch( Prefs.GetDblClickAction( DBL_CLICK_SERVER ).GetAction() ) {

    	case DblClickAction::SERVER_STATS:
        	DblClickServerStats->Checked = true;
        	break;

    	case DblClickAction::CALL_CMD:
        	DblClickServerCallCmd->Checked = true;
        	break;
    }

	DblClickNickCmd->Text    = Prefs.GetDblClickAction( DBL_CLICK_NICK    ).GetCommand();
	DblClickChannelCmd->Text = Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).GetCommand();
	DblClickQueryCmd->Text   = Prefs.GetDblClickAction( DBL_CLICK_QUERY   ).GetCommand();
	DblClickServerCmd->Text  = Prefs.GetDblClickAction( DBL_CLICK_SERVER  ).GetCommand();

	DblClickNickCallCmdClick( DblClickNickCallCmd );
    DblClickChannelCallCmdClick( DblClickChannelCallCmd );
    DblClickQueryCallCmdClick( DblClickQueryCallCmd );
    DblClickServerCallCmdClick( DblClickServerCallCmd );
}
//---------------------------------------------------------------------------
void __fastcall TOptionsWin::StoreDblClickActions( void )
{
	if( DblClickNickWhois->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_NICK ).SetAction( DblClickAction::WHOIS );
    else if( DblClickNickQuery->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_NICK ).SetAction( DblClickAction::OPEN_QUERY );
    else if( DblClickNickCallCmd->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_NICK ).SetAction( DblClickAction::CALL_CMD );

	if( DblClickChannelEditProps->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).SetAction( DblClickAction::CHANNEL_PROPS );
    else if( DblClickChannelCallCmd->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).SetAction( DblClickAction::CALL_CMD );

	if( DblClickQueryWhois->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_QUERY ).SetAction( DblClickAction::WHOIS );
    else if( DblClickQueryCallCmd->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_QUERY ).SetAction( DblClickAction::CALL_CMD );

	if( DblClickServerStats->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_SERVER ).SetAction( DblClickAction::SERVER_STATS );
    else if( DblClickServerCallCmd->Checked )
		Prefs.GetDblClickAction( DBL_CLICK_SERVER ).SetAction( DblClickAction::CALL_CMD );

	Prefs.GetDblClickAction( DBL_CLICK_NICK    ).SetCommand( DblClickNickCmd->Text    );
	Prefs.GetDblClickAction( DBL_CLICK_CHANNEL ).SetCommand( DblClickChannelCmd->Text );
	Prefs.GetDblClickAction( DBL_CLICK_QUERY   ).SetCommand( DblClickQueryCmd->Text   );
	Prefs.GetDblClickAction( DBL_CLICK_SERVER  ).SetCommand( DblClickServerCmd->Text  );
}
//---------------------------------------------------------------------------

