/* $Id: URLWnd.h,v 1.1 2003/05/13 22:16:36 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef URLWndH
#define URLWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TURL;
class TURLWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TButton *Ok;
	TButton *Cancel;
	TEdit *Address;
	TEdit *Description;
	TEdit *User;
	TFormStorage *FormStorage1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TURL	   *URL;
public:		// User declarations
	__fastcall 	TURLWin( TComponent* Owner, TURL *url );
};
//---------------------------------------------------------------------------
extern PACKAGE TURLWin *URLWin;
//---------------------------------------------------------------------------
#endif
