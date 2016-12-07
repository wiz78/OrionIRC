/* $Id: FileProps.h,v 1.1 2005/04/11 17:05:09 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FILEPROPS_H
#define FILEPROPS_H
//---------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#define FILEPROPSCC		__cdecl

#ifdef FILEPROPS_EXPORTS
#define FILEPROPSAPI(x)	__declspec(dllexport) x FILEPROPSCC
#else
#define FILEPROPSAPI(x)	__declspec(dllimport) x FILEPROPSCC
#endif


FILEPROPSAPI(void) __cdecl	FP_WriteProperties( wchar_t *file, const char *product, const char *comment );


#ifdef __cplusplus
};
#endif

//---------------------------------------------------------------------------
#endif /* FILEPROPS_H */
//---------------------------------------------------------------------------
