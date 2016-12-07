/* $Id: FileProps.cpp,v 1.1 2005/04/11 17:05:09 wiz Exp $ */
//---------------------------------------------------------------------------
#include <windows.h>
#include <shlobj.h>
#include <Objbase.h>

#include "FileProps.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void FILEPROPSCC FP_WriteProperties( wchar_t *file, const char *product, const char *comment )
{
	HRESULT	( WINAPI *myStgOpenStorageEx )( WCHAR *, DWORD, DWORD, DWORD, void *, void *, REFIID, void ** );
	HMODULE	lib;

	CoInitialize( NULL );

	lib                = LoadLibrary( "Ole32.dll" );
    myStgOpenStorageEx = NULL;

	if( lib )
    	myStgOpenStorageEx = reinterpret_cast<HRESULT ( WINAPI * )( WCHAR *, DWORD, DWORD, DWORD, void *, void *, REFIID, void ** )>( GetProcAddress( lib, "StgOpenStorageEx" ));

    if( myStgOpenStorageEx ) {
        IPropertySetStorage	   *pPropSetStg = NULL;
        IPropertyStorage 	   *pPropStg = NULL;

        try {
            PROPSPEC 		rgpspec[ 2 ];
	        PROPVARIANT		rgvar[ 2 ];
            HRESULT			hr;

            if( FAILED( myStgOpenStorageEx( file,
                                          	STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                          	STGFMT_ANY, 0, NULL, NULL,
                                          	IID_IPropertySetStorage,
                                          	reinterpret_cast<void **>( &pPropSetStg ))))
                throw "Failed StgOpenStorageEx";

            hr = pPropSetStg->Open( FMTID_SummaryInformation,
                                    STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                    &pPropStg );

            if( hr != S_OK )
                hr = pPropSetStg->Create( FMTID_SummaryInformation, &FMTID_SummaryInformation,
                                          PROPSETFLAG_ANSI,
                                          STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
                                          &pPropStg );

            if( FAILED( hr ))
                throw "Failed IPropertySetStorage::Open and Create";

			for( int i = 0; i < 2; i++ ) {
				PropVariantInit( &rgvar[ i ] );
				rgvar[ i ].vt       = VT_LPSTR;
				rgpspec[ i ].ulKind = PRSPEC_PROPID;
			}

			rgpspec[ 0 ].propid = PIDSI_COMMENTS;
			rgpspec[ 1 ].propid = PIDSI_APPNAME;

            rgvar[ 0 ].pszVal = const_cast<char *>( comment );
            rgvar[ 1 ].pszVal = const_cast<char *>( product );

            if( FAILED( pPropStg->WriteMultiple( 2, rgpspec, rgvar, PID_FIRST_USABLE )))
                throw "Failed IPropertyStorage::WriteMultiple";
        }
        catch( const char *str ) {
			OutputDebugString( str );
        }

        if( pPropStg )
            pPropStg->Release();

        if( pPropSetStg )
            pPropSetStg->Release();
    }

	CoUninitialize();
    FreeLibrary( lib );
}
//---------------------------------------------------------------------------
