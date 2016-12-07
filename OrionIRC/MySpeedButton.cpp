/* $Id: MySpeedButton.cpp,v 1.2 2005/09/09 22:52:50 wiz Exp $*/
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MySpeedButton.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall MySpeedButton::MySpeedButton(TComponent* Owner)
	: TSpeedButton(Owner)
{
	AltNum = -1;
}
//---------------------------------------------------------------------------
void __fastcall MySpeedButton::SetAltNum( int num )
{
	AltNum = num;

    if( num >= 0 )
	    Caption = "&" + IntToStr( num ) + " " + Caption;
    else
    	Caption = Caption.SubString( 4, Caption.Length() );
}
//---------------------------------------------------------------------------
int __fastcall MySpeedButton::GetAltNum( void ) const
{
	return( AltNum );
}
//---------------------------------------------------------------------------
