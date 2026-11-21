
/*
   General Ledger  - (C) Copyright 1992 by Tegara Corporation
   Modify, add and delete accounts functions.
*/

#include "ap.h"
#include "gl_dlg.h"
#include "gl_acct.h"
extern Code4 cb;


TGLAcctList::~TGLAcctList()
{
 
}


void TGLAcctList::HandleListBox (RTMessage Msg)
{
 int nIndx;
 int nRecord;
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 char *szBuffer = new char [gl->accounts->len() + gl->dept->len() + 1];
 char *szDept   = new char [gl->dept->len() + 1];
 List->GetData(1,Cell->Row,szBuffer);
 List->GetData(2,Cell->Row,szDept);

 long lRecord=atol(szBuffer);
 gl->SetIndex("accounts");
 if(gl->seek(strcat(szBuffer,szDept))==0)
 {
  SendMessage(GetDlgItem(Parent->HWindow,ID_ACCOUNTS),WM_SETTEXT,NULL,(LPARAM)(LPSTR)gl->accounts->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_DEPART  ),WM_SETTEXT,NULL,(LPARAM)(LPSTR)gl->dept->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_JDESCRIPT),WM_SETTEXT,NULL,(LONG)(LPSTR)gl->descript->str());
  
 }
  SendMessage(Parent->HWindow,WM_COMMAND,ID_ACCTLIST,NULL);
  SetFocus(GetDlgItem(Parent->HWindow,ID_REFERENCE));
}


void TGLAcctList::SetupWindow ()
{
 TDialog::SetupWindow();
 RECT rc;
 SS_CELLTYPE 	CellType;

 HourGlassOn();
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 short d1Width,d2Width;
 SSSetMaxCols(SHWindow, 3);
 SSSetColWidth(SHWindow,1,7);
 SSSetColWidth(SHWindow,2,5);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d1Width);
 SSGetColWidthInPixels(SHWindow,2,(LPSHORT)&d2Width);
 GetClientRect(HWindow,&rc);
 short ColWidth = (rc.right - rc.left) - d1Width - d2Width - GetSystemMetrics(SM_CXHSCROLL);
 SSSetColWidthInPixels(SHWindow,3,ColWidth);
 SSSetData(SHWindow,1,SS_HEADER, "Account");
 SSSetData(SHWindow,2,SS_HEADER, "Dept");
 SSSetData(SHWindow,3,SS_HEADER, "Description");
 SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);
 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
 SSSetCellType(SHWindow,SS_ALLCOLS, SS_ALLROWS, &CellType);
 
 gl->SetIndex("accounts");
 SSSetMaxRows(SHWindow,gl->recCount());
 Relate4set *rQuery = new Relate4set(gl);
 long i=1;
  rQuery->querySet("ACCOUNT <> ' '"); 
  for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   SSSetData(SHWindow,1,i,gl->accounts->str());
   SSSetData(SHWindow,2,i,gl->dept->str());
   SSSetData(SHWindow,3,i,gl->descript->str());
   i++;
  }
 rQuery->unlock();
 rQuery->free();
 SetListSize(8);
 HourGlassOff();
}
