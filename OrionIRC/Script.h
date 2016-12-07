/* $Id: Script.h,v 1.1 2005/01/23 02:14:26 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ScriptH
#define ScriptH
//---------------------------------------------------------------------------

class TScript
{
public:
				  	TScript();
					TScript( const TScript *copy );

	TScript&		operator =( const TScript& rhs );

    AnsiString		GetFile( void ) const			{ return( File ); }
    bool			IsEnabled( void ) const			{ return( Enabled ); }

    void			SetFile( AnsiString str )		{ File = str; }
    void			SetEnabled( bool set )	 		{ Enabled = set; }

	void			Load( TRegistry *reg, AnsiString key );
	void			Save( TRegistry *reg, AnsiString key );

private:
	AnsiString	  	File;
    bool		  	Enabled;
};

#endif
 