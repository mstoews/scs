/*
   General Ledger  - (C) Copyright 1992 by Tegara Corporation
   Income Statement - Report
*/
#include "scs.h"
#include "gl_wks.h"

extern Code4 cb;
extern HMENU HGLMenu,HGLExMenu, HARMenu, HAPMenu;
//BOOL FAR PASCAL GetAccountValue(LPSS_VALUE Result, LPSS_VALUE Values, short ValueCnt);
BOOL FAR PASCAL dGetAcctValue(LPSS_VALUE Result, LPSS_VALUE Values, short ValueCnt);

void TSetBorder::WMInitDialog (RTMessage)
{
 SendDlgItemMsg(4002,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,1,0L);
 BORDERTYPE = SS_BORDERTYPE_BOTTOM;
 BORDERSTYLE = SS_BORDERSTYLE_SOLID;
}

void TSetBorder::Ok  (RTMessage){
 if(ecol == 0)
 {
  Spread->SetBorder(col,row,BORDERTYPE,BORDERSTYLE,NULL);
 }
 else
 Spread->SetBorderRange(col,row,ecol,erow,BORDERTYPE,BORDERSTYLE,NULL);
 InvalidateRect(Spread->HWindow, NULL, TRUE );
 Spread->SetBool(SSB_CHANGEMADE,TRUE);
 ShutDownWindow();
}

void TSetBorder::NoBorderBtn      (RTMessage){
 SendDlgItemMsg(4001,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4002,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4003,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4004,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4005,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERTYPE_NONE;
}
void TSetBorder::TopBtn           (RTMessage) {
 SendDlgItemMsg(4001,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4002,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4003,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4004,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4005,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERTYPE_TOP;
}
void TSetBorder::BottomBtn        (RTMessage) {
 SendDlgItemMsg(4001,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4002,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4003,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4004,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4005,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERTYPE_BOTTOM;
}
void TSetBorder::RightBtn         (RTMessage) {
 SendDlgItemMsg(4001,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4002,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4003,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4004,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4005,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERTYPE_RIGHT;
}
void TSetBorder::LeftBtn          (RTMessage) {
 SendDlgItemMsg(4001,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4002,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4003,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4004,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4005,BM_SETCHECK,1,0L);
 BORDERSTYLE = SS_BORDERTYPE_LEFT;
}
void TSetBorder::NoneBtn          (RTMessage) {
 SendDlgItemMsg(4006,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4008,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4009,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4010,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERSTYLE_NONE;
}
void TSetBorder::SolidBtn         (RTMessage) {
 SendDlgItemMsg(4006,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4008,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4009,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4010,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERSTYLE_SOLID;
}
void TSetBorder::DashBtn          (RTMessage) {
 SendDlgItemMsg(4006,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4008,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4009,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4010,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERSTYLE_DASHDOT;
}
void TSetBorder::DotBtn           (RTMessage) {
 SendDlgItemMsg(4006,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4008,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4009,BM_SETCHECK,1,0L);
 SendDlgItemMsg(4010,BM_SETCHECK,0,0L);
 BORDERSTYLE = SS_BORDERSTYLE_DOT;
}
void TSetBorder::DashDotBtn       (RTMessage) {
 SendDlgItemMsg(4006,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4007,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4008,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4009,BM_SETCHECK,0,0L);
 SendDlgItemMsg(4010,BM_SETCHECK,1,0L);
 BORDERSTYLE = SS_BORDERSTYLE_DASHDOT;
}


void TSetValue::WMInitDialog(RTMessage)
{
   IntSetValue(GetDlgItem(HWindow,ID_ROW),row);
   IntSetValue(GetDlgItem(HWindow,ID_COL),col);
   glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
   GLAcctList = new TGLAcctList((TDialog *)this,"GL_ACCTSRCH",glaccts);
   GLAcctList->EnableAutoCreate();
   GetApplication()->MakeWindow(GLAcctList);

}

void TSetValue::Ok (RTMessage)
{
  char szSAccount[8];
  char szSDept[5];
  char szPeriod[2];

  SendDlgItemMsg(ID_ACCOUNTS,WM_GETTEXT,sizeof(szSAccount),(LONG)(LPSTR)szSAccount);
  SendDlgItemMsg(ID_DEPART,WM_GETTEXT,sizeof(szSDept),(LONG)(LPSTR)szSDept);
  SSSetTypeFloat(GetDlgItem(Parent->HWindow,ID_SPREAD),&CellType,FS_SEPARATOR,9,2,0,9999999999999.99);
  SSSetCellType(GetDlgItem(Parent->HWindow,ID_SPREAD),col,row, &CellType);
  char szFormula[85];
  StrCpy(szFormula,"PD(");
  StrCat(szFormula,szSAccount);
  StrCat(szFormula,",");
  if(*szSDept != 0)
  StrCat(szFormula,szSDept);
  else
  StrCat(szFormula,"0");
  StrCat(szFormula,",");
  itoa((int)GetCurrentPeriodn(),szPeriod,10);
  StrCat(szFormula,szPeriod);
  StrCat(szFormula,")");
  MessageBox(HWindow,szFormula,"Formula",MB_OK);
  Spread->SetFormula(col,row,szFormula,TRUE);
  ShutDownWindow();
}

void TSetValue::HandleSearchBtn (RTMessage)
{
 if(!GLAcctList->HWindow)
 {
  BWCCMessageBox(HWindow,"Search window can not be found!","OK",MB_ICONEXCLAMATION);
  return;
 }
 GLAcctList->Show(SW_SHOW);
}



void TGlWindow::SCSWorkSheet (RTMessage)
{
 PTWorksheet Income = new TWorksheet(this,"\\dbf\\untitled.wk1");
 GetApplication()->MakeWindow(Income);
}

/*
void TGlWindow::GLManAccounts    (RTMessage)
 {

 PTWorksheet Manufacturing =  new TWorksheet(this,"\\MANUFACT.WK1");
 GetApplication()->MakeWindow(Manufacturing);
 }

void TGlWindow::GLFinRE    (RTMessage)
 {

 PTWorksheet Retained = new TWorksheet(this,"\\RETAINED.WK1");
 GetApplication()->MakeWindow(Retained);
 }

void TGlWindow::GLFinFA    (RTMessage)
 {
 PTWorksheet Worksheet = new TWorksheet(this,"\\FIXED.WK1");
 GetApplication()->MakeWindow(Worksheet);
 }
void TGlWindow::GLFinDep    (RTMessage)
 {
 PTWorksheet Worksheet = new TWorksheet(this,"\\DEPREC.WK1");
 GetApplication()->MakeWindow(Worksheet);
 }

void TGlWindow::New (RTMessage)
{
 PTWorksheet Untitled = new TWorksheet(this,"\\Untitled.wk1");
 GetApplication()->MakeWindow(Untitled);
}

*/