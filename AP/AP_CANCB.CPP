
/*
  Accounts Payable  - (C) Copyright 1992 by Tegara Corporation
   AP Cancel Batch.
   ap_cancb.cpp
*/
#include "ap.h"
//#include "db_dist.h"
#include "db_apbtc.h"
#include "db_apdst.h"
#include "db_cash.h"
#include "ap_cancb.h"


extern Code4 cb;

void TGlWindow::APCancelBatch (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPCancelBatch(this,"AP_CANCELBATCH"));
 }
void TGlWindow::APCancelCash (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPCancelCash(this,"AP_CANCELBATCH"));
 }
void TGlWindow::APCancelAdj (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPCancelAdj(this,"AP_CANCELBATCH"));
 }




void TAPCancelBatch::SetupWindow()
{
 HourGlassOn();
 HWND SHWindow;
 RECT rc;
 SS_CELLTYPE 	CellType;
 LOGFONT 	lfFont;
 HFONT   	hFont;
 cb.closeAll();
 SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetClientRect(SHWindow,&rc);
 short d0Width,d1Width,d3Width,d4Width;

 SSSetColWidth(SHWindow,1,6);
 SSSetColWidth(SHWindow,3,10);
 SSSetColWidth(SHWindow,4,10);

 SSGetColWidthInPixels(SHWindow,0,(LPSHORT)&d0Width);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d1Width);
 SSGetColWidthInPixels(SHWindow,3,(LPSHORT)&d3Width);
 SSGetColWidthInPixels(SHWindow,4,(LPSHORT)&d4Width);


 short ColWidth = (rc.right - rc.left)
		  - d0Width
		  - d1Width
		  - d3Width
		  - d4Width;

 SSSetColWidthInPixels(SHWindow,2,ColWidth);


 lfFont.lfHeight          = 8;
 lfFont.lfWidth           = 0;
 lfFont.lfEscapement      = 0;
 lfFont.lfOrientation     = 0;
 lfFont.lfWeight          = FW_NORMAL;
 lfFont.lfItalic          = 0;
 lfFont.lfUnderline       = 0;
 lfFont.lfStrikeOut       = 0;
 lfFont.lfCharSet         = 1; //default character set.
 lfFont.lfOutPrecision    = 0;
 lfFont.lfClipPrecision   = 0;
 lfFont.lfQuality         = 0;
 lfFont.lfPitchAndFamily  = FF_SWISS;
 StrCpy (lfFont.lfFaceName, "HELV");
 hFont = CreateFontIndirect((LPLOGFONT) &lfFont);

SSSetBool(SHWindow,SSB_ALLOWUSERSELBLOCK,FALSE);
SSSetBool(SHWindow,SSB_PROTECT,TRUE);
SSSetBool(SHWindow,SSB_SCROLLBAREXTMODE,TRUE);
SSSetBool(SHWindow,SSB_NOBEEP,TRUE);
SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);

apbtch = (Tapbtch *) new Tapbtch(&cb,"gl_btch");
apbtch->SetIndex("batchno");

apbtchd = (Tapbtchd *) new Tapbtchd(&cb,"gl_btchd");
apbtchd->SetIndex("batchno");

glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
glaccts->SetIndex("account");

apdist  = (Tapdist *) new Tapdist(&cb,"ap_dist");
apdist->SetIndex("account");

long RecTotal=0;

 RecTotal = apbtch->recCount();
 if(RecTotal < 10 )
 RecTotal = 10;

 SSSetMaxCols(SHWindow, 4);
 SSSetMaxRows(SHWindow, RecTotal);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_ALLROWS,hFont, TRUE);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_HEADER,GetStockObject(hFont), TRUE);


 SSSetData(SHWindow,1,SS_HEADER, "Btch");
 SSSetData(SHWindow,2,SS_HEADER, "Description");
 SSSetData(SHWindow,3,SS_HEADER, "Invoice No.");
 SSSetData(SHWindow,4,SS_HEADER, "Payable Amount");


 SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);
 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
 SSSetCellType(SHWindow,1, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,2, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,3, SS_ALLROWS, &CellType);
 SSSetTypeFloat(SHWindow,&CellType,FS_SEPARATOR,14,2,0.00,99999999999.99);
 SSSetCellType(SHWindow,4, SS_ALLROWS, &CellType);
 long i=1;
 Relate4set *rQuery = new Relate4set(apbtch);
 rQuery->querySet("MODE = '1'");
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
      SSSetData(SHWindow,1,i,apbtch->batchno->str());
      SSSetData(SHWindow,2,i,apbtch->descript->str());
      SSSetData(SHWindow,3,i,apbtch->invoiceno->str());
      SSSetData(SHWindow,4,i,apbtch->payable->str());
     i++;
  }
 rQuery->unlock();
 rQuery->free();
 HourGlassOff();
}


void TAPCancelCash::SetupWindow()
{
 SetCaption("Cancel Accounts Payable Cash Journal");
 HourGlassOn();
 HWND SHWindow;
 RECT rc;
 SS_CELLTYPE 	CellType;
 LOGFONT 	lfFont;
 HFONT   	hFont;
 //SendDlgItemMsg(ID_PRINT,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"GLPRINT");
 cb.closeAll();
 SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetClientRect(SHWindow,&rc);
 short d0Width,d1Width,d3Width,d4Width;

 SSSetColWidth(SHWindow,1,6);
 SSSetColWidth(SHWindow,3,10);
 SSSetColWidth(SHWindow,4,10);

 SSGetColWidthInPixels(SHWindow,0,(LPSHORT)&d0Width);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d1Width);
 SSGetColWidthInPixels(SHWindow,3,(LPSHORT)&d3Width);
 SSGetColWidthInPixels(SHWindow,4,(LPSHORT)&d4Width);


 short ColWidth = (rc.right - rc.left)
		  - d0Width
		  - d1Width
		  - d3Width
		  - d4Width;

 SSSetColWidthInPixels(SHWindow,2,ColWidth);


 lfFont.lfHeight          = 8;
 lfFont.lfWidth           = 0;
 lfFont.lfEscapement      = 0;
 lfFont.lfOrientation     = 0;
 lfFont.lfWeight          = FW_NORMAL;
 lfFont.lfItalic          = 0;
 lfFont.lfUnderline       = 0;
 lfFont.lfStrikeOut       = 0;
 lfFont.lfCharSet         = 1; //default character set.
 lfFont.lfOutPrecision    = 0;
 lfFont.lfClipPrecision   = 0;
 lfFont.lfQuality         = 0;
 lfFont.lfPitchAndFamily  = FF_SWISS;
 StrCpy (lfFont.lfFaceName, "HELV");
 hFont = CreateFontIndirect((LPLOGFONT) &lfFont);

SSSetBool(SHWindow,SSB_ALLOWUSERSELBLOCK,FALSE);
SSSetBool(SHWindow,SSB_PROTECT,TRUE);
SSSetBool(SHWindow,SSB_SCROLLBAREXTMODE,TRUE);
SSSetBool(SHWindow,SSB_NOBEEP,TRUE);
SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);

apcash = (Tapcash *) new Tapcash(&cb,"gl_btch");
apcash->SetIndex("batchno");

apcashd = (Tapcashd *) new Tapcashd(&cb,"gl_btchd");
apcashd->SetIndex("batchno");

glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
glaccts->SetIndex("account");

apdist  = (Tapdist *) new Tapdist(&cb,"ap_dist");
apdist->SetIndex("account");

long RecTotal=0;

 RecTotal = apcash->recCount();
 if(RecTotal < 10 )
 RecTotal = 10;

 SSSetMaxCols(SHWindow, 4);
 SSSetMaxRows(SHWindow, RecTotal);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_ALLROWS,hFont, TRUE);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_HEADER,GetStockObject(hFont), TRUE);


 SSSetData(SHWindow,1,SS_HEADER, "Btch");
 SSSetData(SHWindow,2,SS_HEADER, "Description");
 SSSetData(SHWindow,3,SS_HEADER, "Invoice No.");
 SSSetData(SHWindow,4,SS_HEADER, "Payable Amount");


 SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);
 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
 SSSetCellType(SHWindow,1, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,2, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,3, SS_ALLROWS, &CellType);
 SSSetTypeFloat(SHWindow,&CellType,FS_SEPARATOR,14,2,0.00,99999999999.99);
 SSSetCellType(SHWindow,4, SS_ALLROWS, &CellType);

 long i=1;
 Relate4set *rQuery = new Relate4set(apcash);
 rQuery->querySet("MODE = '1'");
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
      SSSetData(SHWindow,1,i,apcash->batchno->str());
      SSSetData(SHWindow,2,i,apcash->descript->str());
      SSSetData(SHWindow,3,i,apcash->invoiceno->str());
      SSSetData(SHWindow,4,i,apcash->payable->str());
     i++;
  }
 rQuery->unlock();
 rQuery->free();
HourGlassOff();
}


void TAPCancelAdj::SetupWindow()
{
}
void TAPCancelCash::HandleListBox( RTMessage Msg)
{
 char szBuffer[10];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetData(SHWindow,1,Cell->Row,szBuffer);
 long lRecord=atol(szBuffer);
 apcash->SetIndex("batchno");
 apcashd->SetIndex("batchno");
 if(apcash->seek(lRecord)==0)
 {
  if( BWCCMessageBox(HWindow,"Do you want to delete this slip?","W A R N I N G",MB_ICONQUESTION | MB_YESNO) == IDYES)
   {
    SSDelRow(SHWindow,Cell->Row);
    apcash->deleteRec();
    if(apcashd->seek(lRecord) == 0)
      {
       while(long (Str4ptr(apcashd->batchno->str())) == lRecord)
       {
	apcashd->deleteRec();
	apcashd->skip();
       }
     } 
    apcash->pack();
    apcashd->pack();
    apcashd->unlock();
    apcash->unlock();
   }
 }
 return;
}


void TAPCancelBatch::HandleListBox( RTMessage Msg)
{
 char szBuffer[10];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetData(SHWindow,1,Cell->Row,szBuffer);
 long lRecord=atol(szBuffer);
 apbtch->SetIndex("batchno");
 apbtchd->SetIndex("batchno");
 if(apbtch->seek(lRecord)==0)
 {
  if( BWCCMessageBox(HWindow,"Do you want to delete this slip?","W A R N I N G",MB_ICONQUESTION | MB_YESNO) == IDYES)
   {
    SSDelRow(SHWindow,Cell->Row);
    apbtch->deleteRec();
    if(apbtchd->seek(lRecord) == 0)
      {
       while(long (Str4ptr(apbtchd->batchno->str())) == lRecord)
       {
	apbtchd->deleteRec();
	apbtchd->skip();
       }
     } 
    apbtch->pack();
    apbtchd->pack();
    apbtchd->unlock();
    apbtch->unlock();
   }
 }
 return;
}
