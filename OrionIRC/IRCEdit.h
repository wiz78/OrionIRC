/* $Id: IRCEdit.h,v 1.10 2005/05/26 15:06:08 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCEditH
#define IRCEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------

typedef void __fastcall ( __closure *TIRCInputEvent )( System::TObject *Sender, AnsiString text );

class PACKAGE TIRCEdit : public TCustomEdit
{
private:
	TStringList		   		   *History;
    int							HistIdx;
	TIRCInputEvent				FOnCommand;
	TIRCInputEvent				FOnMessage;
    int							NickIdx;
    TListBox		   		   *NickList;
    AnsiString					NickToComplete;
    TWinControl				   *WheelTarget;
    AnsiString					ChannelName;

	void __fastcall 			HistoryPrev( void );
	void __fastcall 			HistoryNext( void );
	void __fastcall 			Submit( Classes::TShiftState Shift );
    void __fastcall				AddToHistory( AnsiString line );
	bool __fastcall 			InsertCtrlKey( char ch, Classes::TShiftState Shift );
    void __fastcall				PageScroll( int dir );
    void __fastcall				Clear( void );
    void __fastcall				DeleteBeforeCursor( void );
    void __fastcall				DeleteAfterCursor( void );

	void __fastcall 			TabCompletion( void );
	void __fastcall 			Complete( AnsiString str, int start, int end );

    void __fastcall				WMGetDlgCode( TMessage &Message );
    void __fastcall				WMPaste( TMessage &Message );
    void __fastcall				WMMouseWheel( TMessage &Message );
	void __fastcall 			PasteFromClipboard( void );

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_GETDLGCODE, TMessage, WMGetDlgCode )
    MESSAGE_HANDLER( WM_PASTE, TMessage, WMPaste )
    MESSAGE_HANDLER( WM_MOUSEWHEEL, TMessage, WMMouseWheel )
END_MESSAGE_MAP( TCustomEdit )

protected:
	void __fastcall 			KeyDown( Word &Key, Classes::TShiftState Shift );
	void __fastcall 			KeyPress( char &Key );

public:
	__fastcall 					TIRCEdit( TComponent* Owner );
    __fastcall		   		   ~TIRCEdit();

    void __fastcall				SetNickList( TListBox *list );
    void __fastcall				SetWheelTarget( TWinControl *target );
    void __fastcall				SetChannelName( AnsiString channel );

__published:
	__property					Text;
	__property					Font;
	__property					Color;
    __property TIRCInputEvent  	OnCommand = { read = FOnCommand, write = FOnCommand };
    __property TIRCInputEvent  	OnMessage = { read = FOnMessage, write = FOnMessage };
};
//---------------------------------------------------------------------------
#endif
