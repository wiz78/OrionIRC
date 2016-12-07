/***************************************************************************
                                  bitfield.h
                             -------------------
	revision             : $Id: bitfield.h,v 1.4 2005/01/09 17:37:53 wiz Exp $
    copyright            : (C) 2002 by Simone Tellini
    email                : tellini@users.sourceforge.net

	description          : utility class to manipulate flags
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, version 2                               *
 *                                                                         *
 ***************************************************************************/

#ifndef PROM_BITFIELD_H
#define PROM_BITFIELD_H

class BitField
{
public:

					BitField() : Flags(0) {};

	BitField&		operator =( const int f )
    				{
                    	Flags = f;
                        
                        return( *this );
    				}

	void			Set( unsigned int f )
					{
						Flags |= f;
					}

	void			Set( unsigned int f, bool set )
					{
						if( set )
							Set( f );
						else
							Clear( f );
					}

	void			Clear( unsigned int f )
					{
						Flags &= ~f;
					}

	void			Clear( void )
					{
						Flags = 0;
					}

	bool			IsSet( unsigned int f ) const
					{
						return(( Flags & f ) ? true : false );
					}

    bool			AllSet( unsigned int f ) const
    				{
                    	return(( Flags & f ) == f );
                    }

    unsigned int	Get( void ) const
    				{
                    	return( Flags );
                    }

private:
	unsigned int	Flags;
};

#endif
