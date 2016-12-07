/* $Id: Notify.h,v 1.3 2004/12/21 21:42:28 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef NotifyH
#define NotifyH
//---------------------------------------------------------------------------

class TNotify
{
public:
                    TNotify();
					TNotify( AnsiString nick );
					TNotify( TNotify *copy );

	TNotify&		operator =( const TNotify& rhs );

    AnsiString		GetNick( void ) const;
	bool			IsOn( void ) const;

    void			SetNick( AnsiString str );
    void			SetIsOn( bool set );

	void			Load( TRegistry *reg, AnsiString key );
	void			Save( TRegistry *reg, AnsiString key );

private:
	AnsiString		Nick;
    bool			On;
};

#endif
 