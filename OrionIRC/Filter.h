/* $Id: Filter.h,v 1.5 2005/06/06 18:34:31 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FilterH
#define FilterH
//---------------------------------------------------------------------------

#include <registry.hpp>

#include "bitfield.h"

typedef enum {
	FILTER_ALL, FILTER_QUERY, FILTER_CHANNEL,
    FILTER_ALL_MATCHING, FILTER_ALL_EXCLUDING
} TFilterApply;

#define FILTER_ENABLED				(1 << 0)
#define FILTER_HIGHLIGHT			(1 << 1)
#define FILTER_HIDE					(1 << 2)
#define FILTER_STOP					(1 << 3)
#define FILTER_CHANGE_COLOR			(1 << 4)
#define FILTER_PRINT_ACTIVE_WIN		(1 << 5)
#define FILTER_PLAY_SOUND			(1 << 6)
#define FILTER_SOUND_IS_BEEP		(1 << 7)
#define FILTER_FLASH_STATUS_BAR		(1 << 8)

#define FILTER_NOP_MASK				( FILTER_HIGHLIGHT | FILTER_HIDE | \
									  FILTER_STOP | FILTER_CHANGE_COLOR | \
                                      FILTER_PRINT_ACTIVE_WIN | FILTER_PLAY_SOUND | \
                                      FILTER_FLASH_STATUS_BAR )

class TFilter
{
public:
					TFilter();
					TFilter( const TFilter *copy );

	bool			DoNothing( void )  			{ return(( Actions.Get() & FILTER_NOP_MASK ) == 0 ); }
    bool			DoHighlight( void )			{ return( Actions.IsSet( FILTER_HIGHLIGHT )); }
    bool			DoHide( void )				{ return( Actions.IsSet( FILTER_HIDE )); }
    bool			DoChangeColor( void )  		{ return( Actions.IsSet( FILTER_CHANGE_COLOR )); }
    bool			DoStop( void )				{ return( Actions.IsSet( FILTER_STOP )); }
    bool			DoPrintOnActiveWin( void )	{ return( Actions.IsSet( FILTER_PRINT_ACTIVE_WIN )); }
    bool			DoPlaySound( void )			{ return( Actions.IsSet( FILTER_PLAY_SOUND )); }
    bool			DoPlayBeep( void )			{ return( Actions.IsSet( FILTER_SOUND_IS_BEEP )); }
    bool			DoFlashStatusBar( void ) 	{ return( Actions.IsSet( FILTER_FLASH_STATUS_BAR )); }

    bool			IsEnabled( void )			{ return( Actions.IsSet( FILTER_ENABLED )); }
    void			SetEnabled( bool set )	 	{ Actions.Set( FILTER_ENABLED, set ); }

    AnsiString		GetName( void )             { return( Name ); }
    AnsiString		GetFilter( void )			{ return( Filter ); }
    AnsiString		GetWinMatching( void )		{ return( WinMatching ); }
    AnsiString		GetWinExcluding( void )		{ return( WinExcluding ); }
    AnsiString		GetSound( void )			{ return( Sound ); }
    TFilterApply	GetApply( void )			{ return( Apply ); }
	TColor			GetColor( void ) 			{ return( ColorToSet ); }

	void			SetNothing( bool set ) 			{ if( set ) Actions.Clear( FILTER_NOP_MASK ); }
    void			SetHighlight( bool set )	 	{ Actions.Set( FILTER_HIGHLIGHT,        set ); }
    void			SetHide( bool set )				{ Actions.Set( FILTER_HIDE,             set ); }
    void			SetChangeColor( bool set )   	{ Actions.Set( FILTER_CHANGE_COLOR,     set ); }
    void			SetStop( bool set )				{ Actions.Set( FILTER_STOP,             set ); }
    void			SetPrintOnActiveWin( bool set )	{ Actions.Set( FILTER_PRINT_ACTIVE_WIN, set ); }
    void			SetPlaySound( bool set )		{ Actions.Set( FILTER_PLAY_SOUND,       set ); }
    void			SetPlayBeep( bool set )			{ Actions.Set( FILTER_SOUND_IS_BEEP,    set ); }
    void			SetFlashStatusBar( bool set ) 	{ Actions.Set( FILTER_FLASH_STATUS_BAR, set ); }

    void			SetName( AnsiString str )       	{ Name = str; }
    void			SetFilter( AnsiString str )			{ Filter = str; }
    void			SetWinMatching( AnsiString str )	{ WinMatching = str; }
    void			SetWinExcluding( AnsiString str )	{ WinExcluding = str; }
    void			SetSound( AnsiString str )			{ Sound = str; }
    void			SetApply( TFilterApply apply )		{ Apply = apply; }
	void			SetColor( TColor col ) 				{ ColorToSet = col; }

    TFilter&		operator=( const TFilter& rhs );

    void			Load( TRegistry *reg, AnsiString key );
    void			Save( TRegistry *reg, AnsiString key );

    bool			Matches( AnsiString line, TForm *wnd );

    void			PlaySnd( void );

    void			Describe( TRichEdit *edit );

private:
	BitField		Actions;
    TColor			ColorToSet;
    AnsiString		Name;
    AnsiString		Filter;
    TFilterApply    Apply;
    AnsiString		WinMatching;
    AnsiString		WinExcluding;
    AnsiString		Sound;

	bool 			Match( AnsiString exp, AnsiString str );
};

#endif
