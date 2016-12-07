/* $Id: Version.h,v 1.3 2003/03/16 15:48:11 wiz Exp $ */
#ifndef VERSION_H
#define VERSION_H

#define PROD_EMAIL		"<orion@tellini.info>"
#define PROD_URL		"http://tellini.info/software/orion/"

class VersionInfo
{
public:
				VersionInfo();
               ~VersionInfo();

	int			GetVersion( void )  	{ return( Version     ); }
	int			GetRevision( void ) 	{ return( Revision    ); }
	int			GetRelease( void )  	{ return( Release     ); }
	int			GetBuild( void )    	{ return( Build       ); }
	AnsiString	GetCopyright( void )    { return( Copyright   ); }
	AnsiString	GetProductName( void )  { return( ProductName ); }
	AnsiString	GetVersionStr( void )   { return( Format( "%d.%d", ARRAYOFCONST(( Version, Revision )))); }
	AnsiString	GetDate( void )         { return( Date ); }

private:
	int			Version;
    int			Revision;
    int			Release;
    int			Build;
    AnsiString	Copyright;
    AnsiString	ProductName;
    AnsiString	Date;
    TStringList *Translations;

	const char *GetString( void *buf, AnsiString which );
};

extern VersionInfo	Version;

#endif /* VERSION_H */