/* $Id: ChanListWnd.h,v 1.1 2005/08/19 14:53:03 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef ChanListWndH
#define ChanListWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EnhListView.hpp"
#include "ExtListView.hpp"
#include "Placemnt.hpp"
#include <ComCtrls.hpp>
#include "RXSpin.hpp"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TServerWin;

class TChanListWin : public TForm
{
__published:	// IDE-managed Components
	TFormStorage *FormStorage1;
	TdfsExtListView *List;
	TPanel *PanelButtons;
	TButton *Refresh;
	TLabel *Label1;
	TEdit *ChanName;
	TLabel *Label2;
	TRxSpinEdit *MinUsers;
	TLabel *Label3;
	TRxSpinEdit *MaxUsers;
	TButton *Filter;
	TLabel *Label4;
	TEdit *Topic;
	void __fastcall ListDblClick(TObject *Sender);
	void __fastcall RefreshClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FilterClick(TObject *Sender);
	void __fastcall ListVMGetItemInfo(TObject *Sender, int Item, int SubItem,
          TLVVMMaskItems &Mask, int &Image, int &Param, int &State,
          int &StateMask, int &Indent, AnsiString &Text);
	void __fastcall ListColumnClick(TObject *Sender, TListColumn *Column);
	
private:	// User declarations
	TServerWin		   *Server;
    TCursor             SaveCursor;
    TList			   *Channels;
    TList			   *VisibleChannels;
    bool				SortAsc;
    int					SortCol;

	void __fastcall 	SortChannels( void );

public:		// User declarations
	__fastcall 			TChanListWin( TServerWin *Owner );

    void __fastcall		Clear( void );

    void __fastcall		BeginList( void );
    void __fastcall		AddItem( AnsiString channel, int users, AnsiString topic );
    void __fastcall		EndList( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TChanListWin *ChanListWin;
//---------------------------------------------------------------------------
#endif
 