/* $Id: MyIniFile.h,v 1.2 2006/04/16 19:38:29 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef MyIniFileH
#define MyIniFileH
//---------------------------------------------------------------------------

class MyIniFile
{
public:
					MyIniFile( AnsiString file, bool load = true );
                   ~MyIniFile();

	void			Clear( void );

	int 			ReadInteger( AnsiString section, AnsiString key, int def );
	AnsiString		ReadString( AnsiString section, AnsiString key, AnsiString def );
	TDateTime 		ReadDateTime( AnsiString section, AnsiString key, TDateTime def );

	void 			WriteInteger( AnsiString section, AnsiString key, int value );
	void 			WriteString( AnsiString section, AnsiString key, AnsiString value );
	void 			WriteDateTime( AnsiString section, AnsiString key, TDateTime value );

private:
	AnsiString		File;
	TStringList	   *Keys;
	TStringList	   *Data;
    bool			Update;

    void			Load( void );
};

#endif
