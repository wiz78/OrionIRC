/* $Id: IRCEdit.cpp,v 1.16 2005/05/26 15:06:08 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <clipbrd.hpp>

#include "IRCEdit.h"
#include "irc.h"

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TIRCEdit::TIRCEdit(TComponent* Owner)
	: TCustomEdit(Owner)
{
	Align        = alBottom;
    Color        = clWindow;
    BorderStyle  = bsNone;
    History      = new TStringList();
    HistIdx      = -1;
    NickIdx      = 0;
    NickList     = NULL;
    FOnCommand   = NULL;
    FOnMessage   = NULL;
    WheelTarget  = NULL;
}
//---------------------------------------------------------------------------
__fastcall TIRCEdit::~TIRCEdit()
{
	delete History;
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::WMGetDlgCode( TMessage &Message )
{
	Message.Result |= DLGC_WANTTAB | DLGC_WANTARROWS;
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::WMPaste( TMessage &Message )
{
	PasteFromClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::WMMouseWheel( TMessage &Message )
{
	if( WheelTarget )
    	PostMessage( WheelTarget->Handle, WM_MOUSEWHEEL, Message.WParam, Message.LParam );
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::KeyDown( Word &Key, Classes::TShiftState Shift )
{
    switch( Key ) {

    	case VK_UP:
        	Key = 0;
            HistoryPrev();
        	break;

        case VK_DOWN:
        	Key = 0;
            HistoryNext();
        	break;

    	case VK_RETURN:
        	Key = 0;
            Submit( Shift );
        	break;

        case VK_LEFT:
        case VK_RIGHT:
        case VK_HOME:
        case VK_END:
        case VK_SHIFT:
        case VK_CONTROL:
    	case VK_MENU:
            NickIdx        = 0;
            NickToComplete = "";
        	break;

        case VK_TAB:
        	if( NickList )
	        	TabCompletion();

            Key = 0;
        	break;

        case VK_NEXT:
        	PageScroll( -1 );
            Key = 0;
        	break;

        case VK_PRIOR:
        	PageScroll( 1 );
            Key = 0;
        	break;

        case VK_ESCAPE:
            Clear();
            Key = 0;
            break;

        case VK_BACK:
        	if( Shift.Contains( ssCtrl )) {
            	DeleteBeforeCursor();
            	Key = 0;
            }
        	break;

        case VK_DELETE:
        	if( Shift.Contains( ssCtrl )) {
            	DeleteAfterCursor();
            	Key = 0;
            }
        	break;

        case 'G': // bell
        	if( InsertCtrlKey( TEXT_BEEP, Shift ))
                Key = 0;
        	break;

        case 'I':
        	if( InsertCtrlKey( TEXT_ITALIC, Shift ))
                Key = 0;
        	break;

        case 'O': // plain
        	if( InsertCtrlKey( TEXT_RESET, Shift ))
                Key = 0;
        	break;

        case 'R': // reverse
        	if( InsertCtrlKey( TEXT_REVERSE, Shift ))
                Key = 0;
        	break;

        default:
        	HistIdx        = -1;
            NickIdx        = 0;
            NickToComplete = "";
        	break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::KeyPress( char &Key )
{
	switch( Key ) {

    	case '\r': // suppress the fscking beep!
        case '\n':
		case '\t':
        case 0x1B: // ESC
        case 0x7F: // CTRL Backspace
	    	Key = '\0';
            break;

        case 'B' - 'A' + 1:
            SelText = AnsiString::StringOfChar( TEXT_BOLD, 1 );
        	Key     = '\0';
            break;

        case 'K' - 'A' + 1:
            SelText = AnsiString::StringOfChar( TEXT_COLOR, 1 );
        	Key     = '\0';
        	break;

        case 'U' - 'A' + 1:
            SelText = AnsiString::StringOfChar( TEXT_UNDERLINE, 1 );
        	Key     = '\0';
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::HistoryPrev( void )
{
    if( HistIdx == -1 )
        HistIdx = History->Count;

    if( HistIdx > 0 ) {
        Text     = History->Strings[ --HistIdx ];
        SelStart = Text.Length();
    }

    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::HistoryNext( void )
{
    if(( HistIdx >= 0 ) && ( HistIdx + 1 < History->Count )) {
        Text     = History->Strings[ ++HistIdx ];
        SelStart = Text.Length();
    }

    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
bool __fastcall TIRCEdit::InsertCtrlKey( char ch, Classes::TShiftState Shift )
{
	bool ret = false;

    if( Shift.Contains( ssCtrl )) {
        SelText = AnsiString::StringOfChar( ch, 1 );
        ret     = true;
    }

    HistIdx        = -1;
    NickIdx        = 0;
    NickToComplete = "";

    return( ret );
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::PageScroll( int dir )
{
	if( WheelTarget )
    	PostMessage( WheelTarget->Handle, WM_MOUSEWHEEL, MAKELONG( MK_CONTROL, WHEEL_DELTA * dir ), 0 );
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::Clear( void )
{
    Text           = "";
    HistIdx        = -1;
    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::DeleteBeforeCursor( void )
{
	AnsiString str = Text.SubString( SelStart + 1, Text.Length() );

    Text           = str;
    HistIdx        = -1;
    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::DeleteAfterCursor( void )
{
	AnsiString str = Text.SubString( 1, SelStart );

    Text           = str;
    SelStart       = Text.Length();
    HistIdx        = -1;
    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::Submit( Classes::TShiftState Shift )
{
    if( !Text.IsEmpty() ) {

    	AddToHistory( Text );

        if(( Text[ 1 ] == '/' ) && !Shift.Contains( ssCtrl )) {

            if( FOnCommand )
                FOnCommand( this, Text.SubString( 2, Text.Length() - 1 ));

        } else if( FOnMessage )
            FOnMessage( this, Text );

        Text = "";
    }

    HistIdx        = -1;
    NickIdx        = 0;
    NickToComplete = "";
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::AddToHistory( AnsiString line )
{
    if( !History->Count || History->Strings[ History->Count - 1 ].AnsiCompareIC( line ))
        History->Add( line );
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::SetNickList( TListBox *list )
{
	NickList = list;
}
//---------------------------------------------------------------------------
void __fastcall	TIRCEdit::SetWheelTarget( TWinControl *target )
{
	WheelTarget = target;
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::SetChannelName( AnsiString channel )
{
	ChannelName = channel;
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::TabCompletion( void )
{
	int			start = 0, end;
    AnsiString  nick;
    bool		found = false;

    end   = SelStart; // + 1;
    nick  = Text.SubString( 1, end );

    for( start = end - 1; ( start > 0 ) && ( nick[ start ] != ' ' ); start-- );

    if( start )
    	nick.Delete( 1, start );

    if( NickToComplete.IsEmpty() )
    	NickToComplete = nick;

    for( int wrap = 0; !found && ( wrap < 3 ); wrap++ ) {

		for( ; !found && ( NickIdx < NickList->Items->Count ); NickIdx++ ) {
        	AnsiString	tmp = NickList->Items->Strings[ NickIdx ];

            if( tmp.Length() >= NickToComplete.Length() ) {

            	found = !tmp.SubString( 1, NickToComplete.Length() ).AnsiCompareIC( NickToComplete );

                if( found )
                	nick = tmp;
            }
	    }

        if( !found ) {

        	NickIdx = 0;

            if( wrap == 1 )
            	NickToComplete = nick;
        }
    }

    if( found ) {

		Complete( nick, start, end );

    } else {

    	NickIdx = 0;

        if( !ChannelName.IsEmpty() && !ChannelName.SubString( 1, 1 ).AnsiCompare( NickToComplete ))
			Complete( ChannelName, start, end );
    }
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::Complete( AnsiString str, int start, int end )
{
    AnsiString tmp = Text;

    start++;

    tmp.Delete( start, end - start + 1 );
    tmp.Insert( str, start );

    if( start == 0 )
        start = 1;

    Text     = tmp;
    SelStart = start + str.Length() - 1;
}
//---------------------------------------------------------------------------
void __fastcall TIRCEdit::PasteFromClipboard( void )
{
	TStringList	*list = new TStringList();
    char 		 buf[ 8 * 1024 ];

    Clipboard()->GetTextBuf( buf, sizeof( buf ));

	list->Text = AnsiString( buf );

    if( list->Count > 1 ) {

    	SelText = list->Strings[ 0 ];

    	list->Strings[ 0 ] = Text;

    	for( int i = 0; i < list->Count; i++ ) {

        	AddToHistory( list->Strings[ i ]);

	        if( FOnMessage )
    	   		FOnMessage( this, list->Strings[ i ]);
        }

        Text = "";

    } else if( list->Count )
    	SelText = list->Strings[ 0 ];

    delete list;
}
//---------------------------------------------------------------------------
