//---------------------------------------------------------------------------
#ifndef AskRemoveWndH
#define AskRemoveWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TAskRemoveWin : public TForm
{
__published:	// IDE-managed Components
    TLabel *Msg;
    TButton *Cancel;
    TButton *Yes;
    TButton *YesAll;
    TButton *No;
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall RemoveClick(TObject *Sender);
    void __fastcall NoClick(TObject *Sender);
    void __fastcall YesAllClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
    int                 ReturnCode;
public:		// User declarations
    __fastcall          TAskRemoveWin(TComponent* Owner);
    int __fastcall      Execute( void );
};

enum { ARR_YES_ALL = 0, ARR_YES, ARR_CANCEL, ARR_NO };

//---------------------------------------------------------------------------
extern PACKAGE TAskRemoveWin *AskRemoveWin;
//---------------------------------------------------------------------------
#endif
