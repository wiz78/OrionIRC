/* $Id: IRCTopic.h,v 1.2 2005/01/25 21:16:16 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef IRCTopicH
#define IRCTopicH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TIRCEdit;
class PACKAGE TIRCTopic : public TCustomControl
{
private:
	TIRCEdit			   *InputCtrl;
    AnsiString				Topic;
    TColor					FBackground;
    Graphics::TBitmap	   *BackBuffer;

	void __fastcall 		SetBackground( TColor bg );

protected:
    void __fastcall         WMEraseBkgnd(TMessage &Message) { Message.Result = 0; };

	void __fastcall 		MouseUp( TMouseButton Button, Classes::TShiftState Shift, int X, int Y );
	void __fastcall 		Paint( void );

BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER( WM_ERASEBKGND, TMessage, WMEraseBkgnd )
END_MESSAGE_MAP( TCustomControl )

public:
	__fastcall 				TIRCTopic( TComponent* Owner );
	virtual __fastcall 	   ~TIRCTopic();

    void __fastcall         SetInputCtrl( TIRCEdit *ctrl );
    void __fastcall			SetTopic( AnsiString topic );

    AnsiString __fastcall	GetTopic( void ) const;

__published:
    __property TColor       Background = { read = FBackground, write = SetBackground, default = clWhite, stored = true };
    __property              Align;
    __property              Hint;
    __property              ShowHint;
    __property              ParentShowHint;
    __property              Visible;
    __property              Font;
    __property              PopupMenu;
    __property              OnClick;
    __property              OnDblClick;
    __property              OnMouseDown;
    __property              OnMouseMove;
    __property              OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
 