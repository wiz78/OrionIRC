/* $Id: FileFilterWnd.h,v 1.1 2003/05/11 17:30:47 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef FileFilterWndH
#define FileFilterWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Placemnt.hpp"
#include "ToolEdit.hpp"
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TFileFilter;
class TFileFilterWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TButton *Ok;
	TButton *Cancel;
	TFormStorage *FormStorage1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label;
	TEdit *Name;
	TEdit *Extensions;
	TDirectoryEdit *Directory;
	TLabel *Label3;
	TRadioButton *Ignore;
	TRadioButton *Save;
	void __fastcall SaveClick(TObject *Sender);
	
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
private:	// User declarations
	TFileFilter	   *Filter;
public:		// User declarations
	__fastcall 		TFileFilterWin( TComponent* Owner, TFileFilter *filter );
};
//---------------------------------------------------------------------------
extern PACKAGE TFileFilterWin *FileFilterWin;
//---------------------------------------------------------------------------
#endif
