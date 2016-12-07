/* $Id: IRCUtils.cpp,v 1.6 2005/04/15 21:44:21 wiz Exp $ */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <string.h>

#include "IRCUtils.h"
#include "irc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
bool IRCUtils::IsPrintable( char ch )
{
	static const char Codes[] = { TEXT_BOLD, TEXT_COLOR, TEXT_TRUE_COLOR,
								  TEXT_BEEP, TEXT_ITALIC, TEXT_RESET,
								  TEXT_REVERSE, TEXT_ESCAPE, TEXT_UNDERLINE,
                                  '\0' };

	return( !strchr( Codes, ch ) && ((unsigned char)ch >= ' ' ) );
}
//---------------------------------------------------------------------------
bool IRCUtils::IsDigit( char ch )
{
	return(( ch >= '0' ) && ( ch <= '9' ));
}
//---------------------------------------------------------------------------
AnsiString IRCUtils::StripCodes( AnsiString str )
{
	char	txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;
    int		n = 1, len = str.Length();

	while( n <= len ) {
        int		partial;

	    if( !IsPrintable( str[ n ] ))
    		n = SkipStyle( str, n );

        while(( n <= len ) && IsPrintable( str[ n ]))
        	*to++ = str[ n++ ];
    }

    *to = '\0';

    return( AnsiString( txt ));
}
//---------------------------------------------------------------------------
AnsiString IRCUtils::StripColors( AnsiString str )
{
	char	txt[ IRC_MAX_MSG_LENGTH + 1 ], *to = txt;
    int		n = 1, len = str.Length();

	while( n <= len ) {
        int		partial;

	    if( !IsPrintable( str[ n ] )) {

        	if(( str[ n ] != TEXT_COLOR ) && ( str[ n ] != TEXT_TRUE_COLOR ))
            	*to++ = str[ n++ ];
    		else
	    		n = SkipStyle( str, n );
        }

        while(( n <= len ) && IsPrintable( str[ n ]))
        	*to++ = str[ n++ ];
    }

    *to = '\0';

    return( AnsiString( txt ));
}
//---------------------------------------------------------------------------
int IRCUtils::SkipStyle( AnsiString& line, int n )
{
	int len = line.Length();

	switch( line[ n++ ] ) {

    	case TEXT_COLOR:
        	if(( n <= len ) && IsDigit( line[ n ])) {

                if(( ++n <= len ) && IsDigit( line[ n ]))
                	n++;

                if(( n + 1 <= len ) && ( line[ n ] == ',' ) && IsDigit( line[ n + 1 ])) {

                	n += 2;

	                if(( n <= len ) && IsDigit( line[ n ]))
    	            	n++;
                }
            }
            break;

        case TEXT_TRUE_COLOR:
        	n += 6;
            break;
    }

    return( n );
}
//---------------------------------------------------------------------------
AnsiString IRCUtils::GetNick( const char *nick, TIRCEvent Event )
{
	AnsiString Nick;

    if( nick ) {

    	Nick = nick;

    	switch( Event ) {

        	case IRC_OWN_MSG:
            case IRC_MSG:
            case IRC_NORMAL:
            	Nick = "<" + Nick + ">";
                break;

            case IRC_HIGHLIGHT:
            	switch( Nick[1] ) {

                	case '[':
                    case '<':
                    case '«':
                    case '-':
                    case '*':
                    	break;

                    default:
                    	Nick = "<" + Nick + ">";
                        break;
                }
            	break;

            case IRC_DCC_CHAT:
            	Nick = "[" + Nick + "]";
                break;

            case IRC_NOTICE:
            	Nick = "-" + Nick + "-";
                break;

            case IRC_CTCP:
            	Nick = ">" + Nick + "<";
                break;
        }

    } else {
    	static const struct {
        	TIRCEvent	event;
            const char  *label;
        } Labels[] = {
			{ IRC_JOIN, "JOIN" },
            { IRC_PART, "PART" },
            { IRC_QUIT, "QUIT" },
            { IRC_CTCP, "CTCP" },
    		{ IRC_DCC, "DCC" },
    		{ IRC_ERROR, "ERROR" },
            { IRC_NICK, "NICK" },
            { IRC_MOTD, "MOTD" },
            { IRC_WALLOP, "WALLOP" },
            { IRC_TOPIC, "TOPIC" },
            { IRC_KICK, "KICK" },
            { IRC_MODE, "MODE" },
            { IRC_NOTIFY, "NOTIFY" },
    		{ IRC_INVITE, "INVITE" },
            { IRC_WHOIS, "WHOIS" },
    		{ IRC_LOCAL, "LOCAL" },
            { IRC_SCRIPT, "SCRIPT" },
        };
#define NUM_LABELS	( sizeof( Labels ) / sizeof( Labels[ 0 ] ))

        for( int i = 0; i < NUM_LABELS; i++ )
        	if( Event == Labels[ i ].event ) {
	        	Nick = "«" + AnsiString( Labels[ i ].label ) + "»";
                break;
            }

    	if( Nick.IsEmpty() )
    		Nick = "*";
    }

    return( Nick );
}
//---------------------------------------------------------------------------

