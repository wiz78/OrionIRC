/* $Id: DCCSendOfferWnd.h,v 1.4 2003/04/24 12:13:14 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DCCSendOfferWndH
#define DCCSendOfferWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ToolEdit.hpp"
#include <ExtCtrls.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
#include "tcpsocket.h"
#include "Hostmask.h"

class TServerWin;

class TDCCSendOfferWin : public TForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TLabel *Label;
	TButton *Ok;
	TButton *Cancel;
	TButton *Ignore;
	TFilenameEdit *File;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *FileOffered;
	TLabel *SizeLabel;
	TCheckBox *Resume;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall OkClick(TObject *Sender);
	void __fastcall FileChange(TObject *Sender);
private:	// User declarations
	TServerWin *Server;
	Prom_Addr	Addr;
    int			Port;
    AnsiString	Nick;
    int			Size;

    AnsiString __fastcall	GetFile( void );

public:		// User declarations
	__fastcall TDCCSendOfferWin( TComponent* Owner, Prom_Addr *addr, int port, HostMask& mask, AnsiString file, int size );
};
//---------------------------------------------------------------------------
extern PACKAGE TDCCSendOfferWin *DCCSendOfferWin;
//---------------------------------------------------------------------------
#endif
