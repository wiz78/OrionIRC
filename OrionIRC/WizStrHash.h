/* $Id: WizStrHash.h,v 1.2 2004/11/21 00:16:08 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef WizStrHashH
#define WizStrHashH
//---------------------------------------------------------------------------

class WizStrHashNode
{
public:
    AnsiString             	String;
    void                   *Data;
};

class WizStrHash
{
private:
    TList         		  **HashTable;
    DWORD                   Size;

    DWORD                   ComputeHash( char *str );
    WizStrHashNode  	   *Find( char *str );

public:
                            WizStrHash( DWORD size );
                           ~WizStrHash();
    void                    Clear( void );
    void                    Add( char *str, void *data = NULL );
    void                    Remove( char *str );
    bool                    Contains( char *str ) { return( Find( str ) != NULL ); };
    void                   *FindData( char *str );
};

#endif
