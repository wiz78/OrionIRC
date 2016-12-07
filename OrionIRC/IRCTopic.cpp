/* $Id: IRCTopic.cpp,v 1.3 2005/01/25 21:16:16 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "IRCTopic.h"
#include "IRCEdit.h"
#include "IRCListRow.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TIRCTopic::TIRCTopic(TComponent* Owner)
	: TCustomControl(Owner)
{
    FBackground  = clWhite;
    InputCtrl    = NULL;
    ControlStyle = ControlStyle << csOpaque;
    BackBuffer   = new Graphics::TBitmap();
}
//---------------------------------------------------------------------------
__fastcall TIRCTopic::~TIRCTopic()
{
	delete BackBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TIRCTopic::SetBackground( TColor bg )
{
    FBackground = bg;
    Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TIRCTopic::MouseUp( TMouseButton Button, Classes::TShiftState Shift, int X, int Y )
{
    if( InputCtrl )
    	InputCtrl->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TIRCTopic::SetInputCtrl( TIRCEdit *ctrl )
{
	InputCtrl = ctrl;
}
//---------------------------------------------------------------------------
void __fastcall	TIRCTopic::SetTopic( AnsiString topic )
{
	Topic = topic;
    Repaint();
}
//---------------------------------------------------------------------------
AnsiString __fastcall TIRCTopic::GetTopic( void ) const
{
	return( Topic );
}
//---------------------------------------------------------------------------
void __fastcall TIRCTopic::Paint( void )
{
    Windows::TRect  rect = Rect( 0, 0, Width, Height );
    TCanvas	   	   *canvas = BackBuffer->Canvas;
    IRCListRow 	   	row( NULL, NULL, Topic, IRC_NORMAL );

    BackBuffer->Width    = Width;
    BackBuffer->Height   = Height;
    canvas->Font         = Font;
    canvas->Brush->Style = bsSolid;
    canvas->Brush->Color = FBackground;

    canvas->FillRect( rect );

    row.PaintText( canvas, rect, 0 );

    canvas->Pen->Color = clBtnFace;
    canvas->Pen->Style = psSolid;

    canvas->MoveTo( 0, Height - 1 );
    canvas->LineTo( Width - 1, Height - 1 );

    Canvas->Draw( 0, 0, BackBuffer );
}
//---------------------------------------------------------------------------

