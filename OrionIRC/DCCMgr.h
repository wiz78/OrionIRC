/* $Id: DCCMgr.h,v 1.1 2003/03/31 22:18:13 wiz Exp $ */
//---------------------------------------------------------------------------
#ifndef DCCMgrH
#define DCCMgrH
//---------------------------------------------------------------------------

#include "ExtListView.hpp"

class TDCCTransfer;

class TDCCMgr
{
public:
						TDCCMgr();
					   ~TDCCMgr();

	void				Add( TDCCTransfer *dcc );
	void				Remove( TDCCTransfer *dcc );

    void				SetListView( TdfsExtListView *list );

private:
	TList		   	   *DCC;
    TdfsExtListView    *Listview;

	void				AddToListview( TDCCTransfer *dcc );
};

extern TDCCMgr			DCCMgr;

#endif
