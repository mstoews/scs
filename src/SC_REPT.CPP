/*
   General Ledger  - (C) Copyright 1992 by Tegara Corporation
   Custom Report Dialog Box.
*/
#include "scs.h"
#include "crpe.h"
#include "db_rept.h"
static OPENFILENAME ofn;
static char szFileDialogText[] = "Worksheet (*.rpt)\0*.rpt\0All Files(*.*)\0*.*";
static char PathName[128] = "\0";
static char FileName[128] = "\0";
extern Code4 cb;
char WARNING[] = "WARNING";

_CLASSDEF(TGLCustomReport)
class TGLCustomReport : public TDialog
{
 public:
 PTListBox ReportList;
 PTStatic  ReportName;
 PTglcrept glrept;
 TGLCustomReport(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL): TDialog(AParent,AName)
 {
  ReportList = new TListBox(this,ID_ACCTLIST);
  ReportName = new TStatic(this,ID_DESCRIPT,40);
 };
 virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void HandlePrint    (RTMessage Msg) = [ID_FIRST + IDM_PRINT];
 virtual void HandleListBox  (RTMessage Msg) = [ID_FIRST + ID_ACCTLIST];
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
 virtual void Add            (RTMessage Msg) = [ID_FIRST + ID_ADD];
 virtual void Delete         (RTMessage Msg) = [ID_FIRST + ID_DELETE];
 virtual void Modify	     (RTMessage Msg) = [ID_FIRST + ID_MODIFY];
 virtual void ResetListBox();
};

void TGlWindow::GLCustomReport(RTMessage)
 {
   GetApplication()->ExecDialog(new TGLCustomReport(this,"GL_CUSTREPORT"));
 }

_CLASSDEF(TGLAddReport)
class TGLAddReport : public TDialog
{
 public:
  PTglcrept glrept;
  char szFileName[128];
  TGLAddReport(PTWindowsObject AParent, LPSTR AName,PTglcrept repts,PTModule AModule = NULL): TDialog(AParent,AName)
  {
   glrept = repts;
  };
  virtual void WMInitDialog (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
  virtual void Ok           (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel       (RTMessage Msg) = [ID_FIRST + IDCANCEL]{
   ShutDownWindow();
  }
};


_CLASSDEF(TGLModifyReport)
class TGLModifyReport : public TDialog
{
 public:
  PTglcrept glrept;
  TGLModifyReport(PTWindowsObject AParent, LPSTR AName,PTglcrept repts,PTModule AModule = NULL): TDialog(AParent,AName)
  {
   glrept = repts;
  };
  virtual void WMInitDialog (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG]
   {
    SetWindowText(HWindow,"Modify Report Definition");
    SetWindowText(GetDlgItem(HWindow,IDOK),"&Modify");
    SetDlgItemText(HWindow,ID_NAME,(LPCSTR)glrept->cname->str());
    SetDlgItemText(HWindow,ID_DESCRIPT,(LPCSTR)glrept->descript->str());
    SetDlgItemText(HWindow,ID_FILENAME,(LPCSTR)glrept->fname->str());
    };
  virtual void Ok           (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel       (RTMessage Msg) = [ID_FIRST + IDCANCEL]{ShutDownWindow();}
};

_CLASSDEF(TGLDeleteReport)
class TGLDeleteReport : public TDialog
{
 public:
  PTglcrept glrept;
  TGLDeleteReport(PTWindowsObject AParent, LPSTR AName,PTglcrept repts,PTModule AModule = NULL): TDialog(AParent,AName)
  {
   glrept = repts;
  };
  virtual void WMInitDialog (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG]{
   SetWindowText(HWindow,"Delete Report Definition");
   SetWindowText(GetDlgItem(HWindow,IDOK),"&Delete");
   SetDlgItemText(HWindow,ID_NAME,(LPCSTR)glrept->cname->str());
   SetDlgItemText(HWindow,ID_DESCRIPT,(LPCSTR)glrept->descript->str());
   SetDlgItemText(HWindow,ID_FILENAME,(LPCSTR)glrept->fname->str());
  };
  virtual void Ok           (RTMessage Msg) = [ID_FIRST + IDOK]{
   glrept->deleteRec();
   glrept->pack();
   Cancel(Msg);
  };
  virtual void Cancel       (RTMessage Msg) = [ID_FIRST + IDCANCEL]{
   ShutDownWindow();}
};

void TGLModifyReport::Ok           (RTMessage)
  {
   char szName[20];
   char szDescription[40];
   char szFName[128];
   GetDlgItemText(HWindow,ID_NAME,szName,sizeof(szName));
   GetDlgItemText(HWindow,ID_DESCRIPT,szDescription,sizeof(szDescription));
   GetDlgItemText(HWindow,ID_FILENAME,szFName,sizeof(szFName));
   if(*szName == 0 || *szFName == 0 || *szDescription == 0){
   BWCCMessageBox(HWindow,"All fields must be entered.","ERROR",MB_ICONEXCLAMATION);
   return;
   }  
   glrept->cname->assign(szName);
   glrept->fname->assign(szFName);
   glrept->descript->assign(szDescription);
   glrept->flush();
   ShutDownWindow();
   return;
  }


void TGLAddReport::WMInitDialog (RTMessage)
{

  ofn.lStructSize     = sizeof(OPENFILENAME);
  ofn.lpstrFile       = FileName ;          // Set in Open and Close functions
  ofn.nMaxFile        = _MAX_PATH ;
  ofn.lpstrFileTitle  = NULL ;          // Set in Open and Close functions
  ofn.hwndOwner       = HWindow;
  ofn.lpstrFilter     = szFileDialogText;
  ofn.lpstrInitialDir = PathName;
  ofn.Flags           = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  ofn.lpfnHook        = NULL;

  if(GetOpenFileName(&ofn)!=0)
  {
   strcpy(szFileName,FileName);
   SetDlgItemText(HWindow,ID_FILENAME,(LPCSTR)szFileName);
  }

}

void TGLAddReport::Ok (RTMessage)
{
 char szName[20];
 char szDescription[40];
 char szFName[128];
 GetDlgItemText(HWindow,ID_NAME,szName,sizeof(szName));
 GetDlgItemText(HWindow,ID_DESCRIPT,szDescription,sizeof(szDescription));
 GetDlgItemText(HWindow,ID_FILENAME,szFName,sizeof(szFName));
 if(*szName == 0 || *szFName == 0 || *szDescription == 0){
  BWCCMessageBox(HWindow,"All fields must be entered.","ERROR",MB_ICONEXCLAMATION);
 }
 else{
 glrept->appendStart(1);
 glrept->cname->assign(szName);
 glrept->fname->assign(szFName);
 glrept->descript->assign(szDescription);
 glrept->append();
 ShutDownWindow();
 } 
 return;
}




void TGLCustomReport::WMInitDialog   (RTMessage){
  glrept = new Tglcrept(&cb);
  glrept->top();
  while(!glrept->eof()) {
   SendDlgItemMsg(ID_ACCTLIST,LB_ADDSTRING,NULL,(LPARAM)(LPCSTR)glrept->descript->str());
   glrept->skip(1);
  }
   
  SetFocus(GetDlgItem(HWindow,ID_DESCRIPT));
}

void TGLCustomReport::HandleListBox (RTMessage Msg) {
 switch (Msg.LP.Hi)
  { 
   case LBN_DBLCLK:
     char szBuf[85];
     DWORD dwIndex = SendDlgItemMsg(ID_ACCTLIST, LB_GETCURSEL,NULL, NULL);
     SendDlgItemMsg(ID_ACCTLIST,LB_SETCURSEL,dwIndex,0);
     SendDlgItemMsg(ID_ACCTLIST,LB_GETTEXT,dwIndex,(LONG)(LPSTR)szBuf);
 
     if(glrept->seek(szBuf)==0)
     {      
      SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LONG)(LPSTR)glrept->cname->str());
     }
     else     
     SetFocus(GetDlgItem(HWindow,ID_DESCRIPT));
   break;
 }
}


void TGLCustomReport::HandlePrint   (RTMessage){  
  char temp[20]; 
  char *szFileName; 
  char *szRepName;  
  szFileName = (char *)malloc(glrept->cname->len()+1);
  szRepName  = (char *)malloc(glrept->fname->len()+1);
  GetDlgItemText(HWindow,ID_DESCRIPT,temp,sizeof(temp));
  if(*temp != 0)
  {
   strcpy(szRepName,glrept->cname->str());
   strcpy(szFileName,glrept->fname->str());
   PrintCR(HWindow,szRepName,szFileName);
  }
  else
  BWCCMessageBox(HWindow,"Choose a report.",WARNING,MB_ICONINFORMATION);
  free(szFileName);
  free(szRepName);
  return;
}

void TGLCustomReport::Add (RTMessage)
{ 
 GetApplication()->ExecDialog(new TGLAddReport(this,"GL_ADDREPORT",glrept));
 ResetListBox();
 return;
}

void TGLCustomReport::Modify (RTMessage)
{
 char *temp = new char[20];
 GetDlgItemText(HWindow,ID_DESCRIPT,temp,sizeof(temp));
 if(*temp != 0){
 GetApplication()->ExecDialog(new TGLModifyReport(this,"GL_ADDREPORT",glrept));
 ResetListBox();
 }
 else
 BWCCMessageBox(HWindow,"Choose a report.",WARNING,MB_ICONINFORMATION);
 delete temp;
}


void TGLCustomReport::Delete  (RTMessage)
{
 GetApplication()->ExecDialog(new TGLDeleteReport(this,"GL_ADDREPORT",glrept));
 ResetListBox();
}

void TGLCustomReport::Cancel  (RTMessage){
 glrept->close();
 ShutDownWindow();
}

void TGLCustomReport::ResetListBox  ()
{
 SendDlgItemMsg(ID_ACCTLIST,LB_RESETCONTENT,NULL,NULL);
 glrept->top();
 while(!glrept->eof()) {
   SendDlgItemMsg(ID_ACCTLIST,LB_ADDSTRING,NULL,(LPARAM)(LPCSTR)glrept->descript->str());
   glrept->skip(1);
  }   
  SetFocus(GetDlgItem(HWindow,ID_DESCRIPT));
}

BOOL PrintCR(HWND HWindow,char *szReportTitle, char *szFileName){
  MessageBox(HWindow,szFileName,szReportTitle,MB_OK);
  short JobNumber = PEOpenPrintJob(szFileName);
  if(JobNumber != 0)
  {
  PEOutputToWindow(JobNumber,szReportTitle,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,HWindow);
  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  return TRUE;
  }
  else
  {
  HANDLE textHandle;
  char   *textBuffer;
  short   textLength;
  PEGetErrorText(JobNumber,&textHandle,&textLength);
  textBuffer = (char *)malloc(textLength);
  PEGetHandleString(textHandle,textBuffer,textLength);
  BWCCMessageBox(HWindow,textBuffer,"Report Error",MB_OK);
  return FALSE;
  }
}
