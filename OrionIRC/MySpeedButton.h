/* $Id: MySpeedButton.h,v 1.2 2005/09/09 22:52:50 wiz Exp $*/
//---------------------------------------------------------------------------
#ifndef MySpeedButtonH
#define MySpeedButtonH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class PACKAGE MySpeedButton : public TSpeedButton
{
private:
protected:
	int				AltNum;

public:
	__fastcall 		MySpeedButton(TComponent* Owner);

    void __fastcall	SetAltNum( int num );
    int __fastcall	GetAltNum( void ) const;

__published:
    __property      PopupMenu;
};
//---------------------------------------------------------------------------
#endif
