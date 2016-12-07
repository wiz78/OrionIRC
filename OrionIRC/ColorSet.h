/* $Id: ColorSet.h,v 1.3 2005/06/04 17:40:54 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ColorSetH
#define ColorSetH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "irc.h"

class ColorSet
{
private:
	enum { S_BOLD   = (1 << 0),
    	   S_ITALIC = (1 << 1) };

public:
				ColorSet();

	TColor		GetColor( TIRCEvent type );
    void		SetColor( TIRCEvent type, TColor col );

    bool		IsBold( TIRCEvent type ) const;
    void		SetBold( TIRCEvent type, bool set );

    bool		IsItalic( TIRCEvent type ) const;
    void		SetItalic( TIRCEvent type, bool set );

    void		Save( TRegistry *reg, AnsiString key );
    void		Load( TRegistry *reg, AnsiString key );

    void		Save( TIniFile *reg, AnsiString section );
    void		Load( TIniFile *reg, AnsiString section );

private:
	TColor		Colors[ IRC_NUM_EVENTS ];
    char		Styles[ IRC_NUM_EVENTS ];
};

#endif
