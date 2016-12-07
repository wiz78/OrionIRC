/* $Id: ScriptEvent.h,v 1.4 2005/01/30 21:31:30 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ScriptEventH
#define ScriptEventH
//---------------------------------------------------------------------------

#include "OrionScriptInterface.h"

class TScriptEvent
{
public:
				TScriptEvent( zval *callback, ScriptEvent event );
               ~TScriptEvent();

	int			Fire( va_list va );

private:
	zval	   *Callback;
    ScriptEvent Event;

	zval 	  **GetArgs( va_list va, int *num_args );
	zval 	  **GetWin( va_list va, int *num_args );
	zval 	  **GetWinHostmask( va_list va, int *num_args );
	zval 	  **GetWinHostmaskString( va_list va, int *num_args );
	zval 	  **GetWinHostmaskStringString( va_list va, int *num_args );
	zval 	  **GetWinString( va_list va, int *num_args );
	zval 	  **GetWinStringString( va_list va, int *num_args );
	zval 	  **GetDCCTransfer( va_list va, int *num_args );

	zval 	   *GetWin( TForm *win );
};

#endif
 