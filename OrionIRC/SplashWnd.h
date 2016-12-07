/* $Id: SplashWnd.h,v 1.2 2003/04/15 20:56:21 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef SplashWndH
#define SplashWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSplashWin : public TForm
{
__published:	// IDE-managed Components
    TTimer *Timer;
	TLabel *Copyright;
	TLabel *Ver;
	TImage *Image1;
	TLabel *Homepage;
	TLabel *Build;
    void __fastcall TimerTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	
private:	// User declarations
public:		// User declarations
    __fastcall TSplashWin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashWin *SplashWin;
//---------------------------------------------------------------------------
#endif
