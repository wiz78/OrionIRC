/* $Id: Orion.cpp,v 1.54 2006/11/25 23:52:40 wiz Exp $ */
//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Version.h"
//---------------------------------------------------------------------
USEFILE("Changes.txt");
USELIB("..\WizUpdate\Release_Build\WizUpdate.lib");
USELIB("..\Libraries\pcre-5.0\Release\pcre_Borland.lib");
USELIB("OrionScript\Release\OrionScript_Borland.lib");
USELIB("FileProps\Release\FileProps_Borland.lib");
USEUNIT("Version.cpp");
USEFILE("irc.h");
USEFORM("Main.cpp", MainWin);
USERES("Orion.res");
USEFORM("About.cpp", AboutWin);
USEFORM("RawWnd.cpp", RawWin);
USEFORM("ServerWnd.cpp", ServerWin);
USEFORM("ChanWnd.cpp", ChanWin);
USEFORM("QueryWnd.cpp", QueryWin);
USEUNIT("tcpsocket.cpp");
USEUNIT("buffer.cpp");
USEUNIT("iodispatcher.cpp");
USEUNIT("socket.cpp");
USEUNIT("IRCUtils.cpp");
USEUNIT("IRCList.cpp");
USEUNIT("IRCListRow.cpp");
USEUNIT("IRCParser.cpp");
USEUNIT("IRCEdit.cpp");
USEUNIT("IRCTopic.cpp");
USEUNIT("Commands.cpp");
USEFORM("OptionsWnd.cpp", OptionsWin);
USEFORM("ServersWnd.cpp", ServersWin);
USEUNIT("Prefs.cpp");
USEUNIT("Server.cpp");
USEUNIT("Hostmask.cpp");
USEFORM("DCCChatOfferWnd.cpp", DCCChatOfferWin);
USEFORM("DCCMgrWnd.cpp", DCCMgrWin);
USEUNIT("DCCTransfer.cpp");
USEFORM("DCCSendOfferWnd.cpp", DCCSendOfferWin);
USEUNIT("DCCMgr.cpp");
USEFORM("AskRemoveWnd.cpp", AskRemoveWin);
USEUNIT("Hotspot.cpp");
USEUNIT("Identd.cpp");
USEFORM("ChanPropWnd.cpp", ChanPropWin);
USEUNIT("DNSReq.cpp");
USEUNIT("ColorSet.cpp");
USEFORM("FilterWnd.cpp", FilterWin);
USEUNIT("Filter.cpp");
USEUNIT("FilterMgr.cpp");
USEUNIT("MySpeedButton.cpp");
USEFORM("FavouritesWnd.cpp", FavouritesWin);
USEUNIT("Favourite.cpp");
USEFORM("FavWnd.cpp", FavWin);
USEUNIT("FavouritesMgr.cpp");
USEUNIT("Logger.cpp");
USEUNIT("FileFilter.cpp");
USEFORM("FileFilterWnd.cpp", FileFilterWin);
USEUNIT("FileFilterMgr.cpp");
USEUNIT("URLCatcher.cpp");
USEUNIT("URL.cpp");
USEFORM("URLCatcherWnd.cpp", URLCatcherWin);
USEFORM("URLWnd.cpp", URLWin);
USEUNIT("WizStrHash.cpp");
USEFORM("KickBanWnd.cpp", KickBanWin);
USEUNIT("User.cpp");
USEUNIT("IRCChanData.cpp");
USEUNIT("Notify.cpp");
USEUNIT("NotifyMgr.cpp");
USEUNIT("NotifyHandler.cpp");
USEFORM("NotifyWnd.cpp", NotifyWin);
USEFORM("EditNotifyWnd.cpp", EditNotifyWin);
USEUNIT("MatchStr.cpp");
USEUNIT("IgnoreMgr.cpp");
USEUNIT("Ignore.cpp");
USEFORM("IgnoreWnd.cpp", IgnoreWin);
USEUNIT("MyIniFile.cpp");
USEUNIT("Eyelin.cpp");
USEUNIT("Alias.cpp");
USEUNIT("AliasesMgr.cpp");
USEFORM("EditAliasWnd.cpp", EditAliasWin);
USEUNIT("OrionScriptInterface.cpp");
USEFORM("ScriptingWnd.cpp", ScriptingWin);
USEUNIT("ScriptsMgr.cpp");
USEUNIT("Script.cpp");
USEUNIT("ScriptClasses.cpp");
USEUNIT("ScriptBasic.cpp");
USEUNIT("ScriptEvent.cpp");
USEUNIT("ScriptConstants.cpp");
USEFORM("BufferSearchWnd.cpp", BufferSearchWin);
USEFORM("SearchResultsWnd.cpp", SearchResultsWin);
USEUNIT("SoundsMgr.cpp");
USEUNIT("DblClickAction.cpp");
USEFORM("ChanListWnd.cpp", ChanListWin);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
		Application->Initialize();
		Application->Title = "Orion";

		Application->CreateForm(__classid(TMainWin), &MainWin);
		Application->CreateForm(__classid(TAboutWin), &AboutWin);
		Application->CreateForm(__classid(TRawWin), &RawWin);
		Application->CreateForm(__classid(TServersWin), &ServersWin);
		Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }

	return 0;
}
//---------------------------------------------------------------------
