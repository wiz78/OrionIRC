/* $Id: URL.cpp,v 1.3 2004/11/21 00:16:08 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "URL.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TURL::TURL()
{
    Date = TDateTime::CurrentDateTime();
}
//---------------------------------------------------------------------------
TURL::TURL( AnsiString user, AnsiString url )
{
	Description = url;
	User        = user;
    URL         = url;
    Date        = TDateTime::CurrentDateTime();
}
//---------------------------------------------------------------------------
void TURL::Load( MyIniFile *ini, AnsiString section )
{
	Description = ini->ReadString( section, "Description", "" );
	User        = ini->ReadString( section, "User", "-" );
	URL         = ini->ReadString( section, "URL", "" );

    Date = ini->ReadDateTime( section, "Date", Date );
}
//---------------------------------------------------------------------------
void TURL::Save( MyIniFile *ini, AnsiString section )
{
	ini->WriteString( section, "Description", Description );
	ini->WriteString( section, "User",        User        );
	ini->WriteString( section, "URL",         URL         );

    ini->WriteDateTime( section, "Date", Date );
}
//---------------------------------------------------------------------------

