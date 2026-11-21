// General Ledger  - (C) Copyright 1992 by Tegara Corporation
                                                                          

#include <bwcc.h>
#include <tbowl.h>
#include <dir.h>
#include <commdlg.h>
#include <owl.h>
#include <mdi.h>
#ifndef D4ALL_INC 
#include "d4all.h"
#endif

#ifndef D4DATA_H
#include "d4data.hpp"
#endif

#ifndef   __GLCLASS_H
#include "glclass.h"
#endif

#include "gl.h"
#include "toolbox.h"
#include "crpe.h"
#include "db_dept.h"
#include "db_profi.h"
#include "db_accts.h"
#include "db_btch.h"
#include "db_btchd.h"
#include "db_dist.h"
#include "db_srce.h"

#include "gl_dlg.h"
#include "gl_main.h"
#include "gl_strg.h"
#include "gl_clsm.h"
#include "gl_mains.h"
#include "gl_btche.h"
#include "gl_budgt.h"
#include "gl_srcc.h"
#include "gl_repts.h"
#include "gl_postb.h"
#include "gl_btcan.h"
#include "gl_profi.h"
#include "gl_rept.h"
#include "gl_alloc.h"


#define IDD_STATUSBAR_MENUHELP 2
#define STATUSBAR_HEIGHT       26
#define GWW_STATUSBAR          0
#define  BUTTON_WIDTH       45
#define ID_LOADING_AP       16007
#define  MESSAGE    BWCCMessageBox(GetFocus(),"Unable to locate TEGARA DLL's","E R R O R",MB_OK);
#define ID_ITEMSTART 1105
#define ID_ITEMEND   1106
#define ID_TITLE     1107

// Define global variables for Codebase.
Code4 cb;
LPSTR  path;
char   CurDir[MAXPATH];
char   CurrentPeriod[45];
char   *szCompanyName;
extern PTWindowsObject GetObjectPtr;
LPSTR  dbf;
int  PeriodNo;
int X;
int Y;

#define TOOLBAR_WIDTH          32
#define TOOLBAR_HEIGHT         32

#define GWW_STATUSBAR          0
#define GWW_TOOLBOX            2
#define GWW_TOOLBAR            4

#define   TOOLBOX_STARTID          4043
#define   TOOLBOX_ITEMS            8
#define   IDS_GL               16001


HINSTANCE hDLL,hTegara1,hTegara2;
HMENU HGLMenu,HGLExMenu, HARMenu, HAPMenu;
HWND  prev_wnd;
FARPROC find_prev_wndi;
BOOL CALLBACK _export find_prev_wnd(HWND,LPARAM);

BOOL CALLBACK _export find_prev_wnd(HWND wnd, LPARAM lParam)
{
 /* Look for a top level window having the same instance. */
 if (!GetParent(wnd) && GetWindowWord(wnd, GWW_HINSTANCE) == (HINSTANCE) lParam)
 {
   prev_wnd = wnd;
   return FALSE;
 }
 return TRUE;
}

LPSTR GetString(HINSTANCE hInstance, int ID)
{
 GLOBALHANDLE hString = GlobalAlloc(GMEM_MOVEABLE,85);
 LPSTR lpString = GlobalLock(hString);
 LoadString(hInstance,ID,lpString,85);
 GlobalUnlock(hString);
 return lpString;
}



void TGlWindow::SendStatusMsg (int ID)
{
  SendMessage(HStatusBar, SBRM_SETITEMTEXT, IDD_STATUSBAR_MENUHELP,
   (LPARAM)GetString(GetApplication()->hInstance,ID));
}
// Define a class derived from TApplication
class TAcctModule :public TApplication
{
public:
  TAcctModule(LPSTR AName, HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
    : TApplication(AName, hInstance, hPrevInstance, lpCmdLine, nCmdShow){};
  virtual void InitMainWindow();
  ~TAcctModule()
  {
  FreeLibrary(hTegara1);
  FreeLibrary(hTegara2);
  DestroyMenu(HGLMenu);
  DestroyMenu(HGLExMenu);
  PECloseEngine();
  };
};


void TGlWindow::CMHelp (RTMessage)
{
  WinExec("winhelp winhelp.hlp",SW_SHOW);
}

void TGlWindow::GLBackup(RTMessage)
{
 if(BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,1073),"Backup Databases",MB_ICONQUESTION | MB_YESNO) == IDYES)
  {
   cb.closeAll();
   WinExec("savegl.pif",SW_SHOW);
  }
}

void TGlWindow::GLRestore(RTMessage)
{
 cb.closeAll();
 if(BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,1074),"Restore Databases",MB_ICONQUESTION | MB_YESNO) == IDYES)
  {
   cb.closeAll();
   WinExec("restore.pif",SW_SHOW);
  }
}


void TGlWindow::CMAbout (RTMessage)
{
  GetApplication()->ExecDialog(new TDialog(this,"ABOUT"));
}

void TGlWindow::GLYearEnd (RTMessage)
{
  GetApplication()->ExecDialog(new TDialog(this,"GL_YEARE"));
}


void TGlWindow::GetWindowClass(WNDCLASS& wc)
{
 TWindow::GetWindowClass(wc);
 wc.hIcon=LoadIcon(wc.hInstance, "GL");
}


void TPrintDlg::HandlePrintBtn( RTMessage)
{
 SendDlgItemMsg(IDPRINTCHECK,BM_SETCHECK,1,0L);
 SendDlgItemMsg(2012,BM_SETCHECK,0,0L);
 PrintToScreen = 1;
}

void TPrintDlg::HandleScreenBtn( RTMessage)
{
 SendDlgItemMsg(IDPRINTCHECK,BM_SETCHECK,0,0L);
 SendDlgItemMsg(2012,BM_SETCHECK,1,0L);
 PrintToScreen = 0;
}


void TPrintDlg::WMInitDialog (RTMessage)
{
 SendDlgItemMsg(IDREPORTNAME,WM_SETTEXT,NULL,(LONG)(LPSTR)szRepName);
 SendDlgItemMsg(IDPRINTCHECK,BM_SETCHECK,1,0L);
 PrintToScreen = 1;
}

void TPrintDlg::Ok (RTMessage)
{
 HCURSOR hCursor ;
 hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);
 ShowCursor (FALSE);
 SetCursor (hCursor);

}

void TPrintDlg::Cancel (RTMessage)
{
// PECloseEngine();
 ShutDownWindow(HWindow);
}


// DIALOGS #5-7: Common printer, setup, and options dialogs
void TGlWindow::CMDialogPrint(RTMessage)
{
 DlgPrinterSetup(HWindow);
}


void TGlWindow::SetupWindow()
 {
  TMDIFrame::SetupWindow();
  ToolBar = (TToolBar *) new TToolBar(this,"TOOLBAR");
  ToolBar->EnableAutoCreate();
  GetApplication()->MakeWindow(ToolBar);
  ToolBar->Show(SW_SHOW); 
  HStatusBar = GLCreateStatusBar();
  SendMessage(HStatusBar, SBRM_SETITEMTEXT,3,(LPARAM)GetCurrentPeriod());
  cb.exclusive = 0;
  cb.readOnly = 0;
  cb.readLock = 0;
  cb.lockAttempts = -1;
}



char  *TGlWindow::GetCurrentPeriod ()
{
  static char szCurrPeriod[20];
  cb.autoOpen = FALSE;
  PTprofile profile = new Tprofile(&cb);
  profile->top();
  strcpy(szCurrPeriod,profile->curr_pd->str());
  profile->close();
  cb.autoOpen = TRUE;
  return szCurrPeriod;
}

double GetCurrentPeriodn()
{
  double dCurr_pd;
  cb.autoOpen = FALSE;
  PTprofile profile = new Tprofile(&cb);
  profile->top();
  dCurr_pd =  double (Str4ptr(profile->curr_pdn->str()));
  profile->close();
  cb.autoOpen = TRUE;
  return dCurr_pd;
}

 void TGlWindow::GLMainDept     (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"GL_MAINSPEC"));
 }

void TGlWindow::GLMainGroup    (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"GL_MAINGROUP"));
 }



TGlWindow::TGlWindow(LPSTR ATitle,LPSTR MenuName) : TMDIFrame(ATitle,MenuName)
 {
   ChildMenuPos = 0;
   Attr.X=0;
   Attr.Y=0;
   Attr.W=CW_USEDEFAULT;
   Attr.H=0;
   getcwd(CurDir,MAXPATH);
   StrCat(CurDir,"\\..\\..\\DBF");
   chdir(CurDir);
 }

void TGlWindow::WMSize(RTMessage)
{
 RECT rect;
 GetClientRect(HWindow, &rect);
 MoveWindow(GetClient()->HWindow,0,38,rect.right,rect.bottom-64,1);
 MoveWindow(ToolBar->HWindow,-7,0,rect.right-rect.left+7,38,1);
 MoveWindow(HStatusBar, 0, rect.bottom-STATUSBAR_HEIGHT, rect.right, STATUSBAR_HEIGHT, TRUE);
}


void TToolBar::WMPaint(TMessage&)
{
  PAINTSTRUCT PS;
  HDC PaintDC;
  PaintDC = BeginPaint(HWindow, &PS);
  HDC memDC;
  RECT clientRect;
  HANDLE oldBmp;
  HBITMAP bmpButtonLine;
  GetClientRect(HWindow, &clientRect);
  memDC = CreateCompatibleDC(PaintDC);
  bmpButtonLine = LoadBitmap(GetApplication()->hInstance, "TOOLBAR");
  oldBmp = SelectObject(memDC, bmpButtonLine);
  StretchBlt(PaintDC,0,0, clientRect.right - clientRect.left,38,memDC,0,0,10,38, SRCCOPY);
  SelectObject(memDC, oldBmp);
  DeleteDC(memDC);
  EndPaint(HWindow, &PS);
}



void TToolBar::GL       (RTMessage){
   RECT rc;
   GetClientRect(HWindow,&rc);
   GLBar(&rc);
   SendMessage(Parent->HWindow,WM_COMMAND,ID_GL,NULL);
 }

void TToolBar::GLAdd     (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_ACCTADD,NULL);}
void TToolBar::GLBatch   (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_MODBATCH,NULL);}
void TToolBar::GLPost    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_GLPOSTBATCH,NULL);}
void TToolBar::GLInquiry (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_GLCHARTINQ,NULL);}
void TToolBar::GLQuit    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,24340,NULL);}
void TToolBar::GLPrint   (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,110,NULL);}
void TToolBar::GLTrash   (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,24338,NULL);}
void TToolBar::GLOpen    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,112,NULL);}
void TToolBar::GLSave    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,113,NULL);}
void TToolBar::GLNew     (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,111,NULL);}


void TToolBar::GLBar(LPRECT rect){

  ShowWindow(GetDlgItem(HWindow,ID_GLBATCH),TRUE);
  ShowWindow(GetDlgItem(HWindow,ID_GLADD),TRUE);
  ShowWindow(GetDlgItem(HWindow,ID_GLINQ),TRUE);
  ShowWindow(GetDlgItem(HWindow,ID_GLPOST),TRUE);
 
  MoveWindow(GetDlgItem(HWindow,ID_GLBATCH)     ,rect->left+10 ,rect->top+4,40,30,0);
  MoveWindow(GetDlgItem(HWindow,ID_GLADD)       ,rect->left+10+(BUTTON_WIDTH) ,rect->top+4,40,30,0);
  MoveWindow(GetDlgItem(HWindow,ID_GLPOST)      ,rect->left+10+(2*BUTTON_WIDTH) ,rect->top+4,40,30,0);
  MoveWindow(GetDlgItem(HWindow,ID_GLINQ)       ,rect->left+10+(3*BUTTON_WIDTH) ,rect->top+4,40,30,0);

}




void TToolBar::WMInitDialog (RTMessage)
{
 RECT rect;
 GetClientRect(HWindow,&rect);
 SendDlgItemMsg(ID_GLBATCH,   SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"PAYMENT");
 SendDlgItemMsg(ID_GLINQ,     SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"VIEW");
 SendDlgItemMsg(ID_GLPOST,    SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"POST");
 SendDlgItemMsg(ID_GLADD,     SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ACCTMODIFY");
 SendDlgItemMsg(ID_CANCEL,    SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"CANCEL");

 MoveWindow(GetDlgItem(HWindow,ID_GLBATCH)     ,rect.left+10 ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_GLADD)       ,rect.left+10+(BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_GLPOST)      ,rect.left+10+(2*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_GLINQ)       ,rect.left+10+(3*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_CANCEL)      ,rect.left+10+(4*BUTTON_WIDTH) ,rect.top+4,40,30,0);
}



void TGLClient::WMRButtonDown (RTMessage Msg)
{ 
 POINT point = MAKEPOINT(Msg.LParam);
 ClientToScreen(HWindow,&point);
 X=point.x;
 Y=point.y;
 SendMessage(GetApplication()->MainWindow->HWindow,WM_POPUPMENU,(WPARAM) Msg.LParam,NULL);
}

void TGlWindow::WMMenuSelect(RTMessage Msg)
{
  char Str[85];
  int nFlags = Msg.LP.Lo;
  int hSysMenu = Msg.LP.Hi;
  if (HStatusBar != 0)
  {
   if((unsigned)nFlags == 0xFFFF && hSysMenu == NULL)
    {
     LoadString(GetApplication()->hInstance, IDS_READY, Str, sizeof(Str));
    }
   else if(nFlags & MF_POPUP )
    {
     LoadString(GetApplication()->hInstance, IDS_MENUITEM, Str, sizeof(Str));
    }
   else if(nFlags & MF_SYSMENU )
    {
     LoadString(GetApplication()->hInstance, IDS_SYSTEM, Str, sizeof(Str));
    }
   else 
    LoadString(GetApplication()->hInstance, Msg.WParam, Str, sizeof(Str));
    SendMessage(HStatusBar, SBRM_SETITEMTEXT, IDD_STATUSBAR_MENUHELP, (LPARAM)Str);
  }
}

// "Takeda General Ledger Accounting Module"
void TGlWindow::PopUpMenu (RTMessage) 
{
   HMENU hMenu;
   char szTitleBar[85];
   char Str[60];
   GetWindowText(HWindow,szTitleBar,sizeof(szTitleBar));
   if(lstrcmp(szTitleBar,GetString(GetApplication()->hInstance,ID_TITLE))==0)
    {
     hMenu = LoadMenu(GetApplication()->hInstance,"GLACCT");
     if (HStatusBar != 0)
      {
       LoadString(GetApplication()->hInstance,IDS_GL, Str, sizeof(Str));
       SendMessage(HStatusBar, SBRM_SETITEMTEXT, IDD_STATUSBAR_MENUHELP, (LPARAM)Str);
      }
    }
   else if(lstrcmp(szTitleBar,"General Ledger")==0){hMenu = LoadMenu(GetApplication()->hInstance,"GLSUB");}
   hMenu = GetSubMenu(hMenu,0);
   TrackPopupMenu(hMenu,TPM_LEFTBUTTON,X,Y,0,GetApplication()->MainWindow->HWindow,NULL);
   return;
}


void TGlWindow::GLMenu(RTMessage)
{ 
 char Str[60];
 HMENU HPopUpMenu = GetSubMenu(HGLMenu,0); // Get the child menu position.
 SendMessage(ClientWnd->HWindow,WM_MDISETMENU,0,MAKELONG(HGLMenu,HPopUpMenu));
 DrawMenuBar(HWindow);
 SetWindowText(HWindow,"General Ledger");
 if (HStatusBar != 0)
   {
   LoadString(GetApplication()->hInstance,IDS_GL, Str, sizeof(Str));
   SendMessage(HStatusBar, SBRM_SETITEMTEXT, IDD_STATUSBAR_MENUHELP, (LPARAM)Str);
   }

}




void TAcctModule::InitMainWindow()
{
  MainWindow = new TGlWindow("General Ledger","gl");
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow)
{
  TAcctModule AcctModule ("AcctApp", hInstance, hPrevInstance,lpCmdLine, nCmdShow);  
  if(hPrevInstance)
  {
   find_prev_wndi = MakeProcInstance((FARPROC)find_prev_wnd,hInstance);
   EnumWindows(find_prev_wndi,hPrevInstance);
   FreeProcInstance(find_prev_wndi);
   if (!prev_wnd)
     return 0;
   prev_wnd = GetLastActivePopup(prev_wnd);
   ShowWindow(prev_wnd, SW_SHOWNORMAL);
   SetActiveWindow(prev_wnd);
   return 0;
  }
  hTegara1 = LoadLibrary("tegara1.dll");
  if ((int)hTegara1 < 32)
    exit(1);

  hTegara2 = LoadLibrary("tegara2.dll");
  if ((int)hTegara2 < 32)
    exit(1);


  hDLL = LoadLibrary("BWCC.DLL");
  if ((int)hDLL < 32)
    exit(1);
  
  HGLMenu = LoadMenu(hInstance,"gl");
  HGLExMenu = LoadMenu(hInstance,"gle");
  PEOpenEngine();
  szCompanyName = new char[40];
  AcctModule.Run();
  return AcctModule.Status;
}

HWND TGlWindow::GLCreateStatusBar()
{
  HWND hWndSBR;
  RECT RectClient;
  LOGFONT lFont;
  cb.autoOpen = FALSE;
  PTprofile profile  = new Tprofile(&cb);
  cb.autoOpen = TRUE;
  char *szCompany = new char[40];
  strcpy(szCompany,profile->company->str());
  strcpy(szCompanyName,"'");
  strcat(szCompanyName,szCompany);
  strcat(szCompanyName,"'");

  static STATUSBARITEM Items[] =
     {
     {1, 6, 200,szCompany,SBRS_TEXTLEFT},
     {2, 6, SBR_AUTOSIZE, ""             ,SBRS_TEXTLEFT},
     {3, 6, 140,  "",                        SBRS_TEXTCENTER}, 
     };
  delete szCompany;
  GetClientRect(HWindow, &RectClient);

  hWndSBR = tbCreateStatusBar(HWindow, WS_CHILD | WS_VISIBLE | SBRS_DRAWTOPLINE |SBRS_PIXELS,  0,
                  RectClient.bottom - STATUSBAR_HEIGHT,
                  RectClient.right, STATUSBAR_HEIGHT,
		  Items, sizeof(Items) / sizeof(STATUSBARITEM));
 HFONT  hfontDlg = (HFONT) NULL;
    if ((hfontDlg == (HFONT) SendMessage(hWndSBR, WM_GETFONT, 0, 0L))) {
        if (GetObject(hfontDlg, sizeof(LOGFONT), (LPSTR) &lFont)) {
	    lFont.lfHeight = 30;
            lFont.lfWeight = FW_LIGHT;
            if (hfontDlg == CreateFontIndirect((LPLOGFONT) &lFont)) {
		SendMessage(hWndSBR,WM_SETFONT,(WPARAM)hfontDlg,(LPARAM)FALSE);
	    }
	}
    }
 return (hWndSBR);

}

void TGlWindow::GLCloseMonth (RTMessage )
 {
   GetApplication()->ExecDialog(new TGLCloseMonth(this,"GL_CLOSEM"));
   SendMessage(HStatusBar, SBRM_SETITEMTEXT,3,(LPARAM)GetCurrentPeriod());
 }


void TGlWindow::GLMainSpec     (RTMessage)
 {
  GetApplication()->ExecDialog(new TGLMainSpecs(this,"GL_MAINSPEC"));
 }



void TGlWindow::GLModifyBatch(RTMessage)
 {
  GetApplication()->ExecDialog(new TGLModBatch(this,"GL_MODIFYBATCH"));
 }

void TGlWindow::GLBatchEnt(RTMessage)
 {
  GetApplication()->ExecDialog(new TGLBatchEntry(this,"GL_BATCHENTRY"));
 }


void TGlWindow::ModifyBatch   (RTMessage){SendMessage(GetActiveWindow(),WM_MODIFYBATCH,NULL,NULL);}
void TGlWindow::NewBatch      (RTMessage){SendMessage(GetActiveWindow(),WM_NEWBATCH   ,NULL,NULL);}
void TGlWindow::SearchGL      (RTMessage){SendMessage(GetActiveWindow(),WM_SEARCHGL   ,NULL,NULL);}
void TGlWindow::CancelBatch   (RTMessage){SendMessage(GetActiveWindow(),WM_CANCELBATCH,NULL,NULL);}
void TGlWindow::GLAcctBudget  (RTMessage){GetApplication()->ExecDialog(new TGLAcctBudget(this,"GL_ACCTBUDGET"));}
void TGlWindow::GLCostAlloc   (RTMessage){GetApplication()->ExecDialog(new TGLCostAlloc(this,"GL_ALLOCATE"));}

void TGlWindow::GLPrtDepart  (RTMessage)
{
  char szRepName[35];
  char szFileName[11];
  StrCpy(szRepName,"Department Code Listing");
  StrCpy(szFileName,"gldept.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);
}



void TGlWindow::GLPrtPrePost     (RTMessage)
{
  char szRepName[35];
  char szFileName[10];
  StrCpy(szRepName,"Preposting Distribution Listing");
  StrCpy(szFileName,"glpre.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

}


void TGlWindow::GLPrtSlipSummary (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Slip Summary Report");
  StrCpy(szFileName,"btchsum.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);
 }

 void TGlWindow::GLPrtSourcej   (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Slip Sheet Report");
  StrCpy(szFileName,"slip.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);
 }

void TGlWindow::GLPrtSlip (RTMessage)
 {
  char szRepName[35];
  char szFileName[11];
  StrCpy(szRepName,"Slip Detail Report");
  StrCpy(szFileName,"glbtchd.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);
 }

void TGlWindow::GLTrialBalance (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Trial Balance");
  StrCpy(szFileName,"gltb.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
   BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLAccounts (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Chart of Accounts");
  StrCpy(szFileName,"glaccts.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLTBbyDept (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Trial Balance by Dept.");
  StrCpy(szFileName,"gldepts.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
   BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLSelling (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Selling Expenses");
  StrCpy(szFileName,"selling.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLGeneral (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"General Administration Expenses");
  StrCpy(szFileName,"admin.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLManufacture (RTMessage)
 {
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"Manufacturing Expenses");
  StrCpy(szFileName,"Manufact.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

 }

void TGlWindow::GLByPeriod1 (RTMessage)
{
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"General Ledger - (April - July)");
  StrCpy(szFileName,"glbyprd1.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

}

void TGlWindow::GLByPeriod2 (RTMessage)
{
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"General Ledger - (August - November)");
  StrCpy(szFileName,"glbyprd2.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

}

void TGlWindow::GLByPeriod3 (RTMessage)
{
  char szRepName[35];
  char szFileName[12];
  StrCpy(szRepName,"General Ledger - (December - March)");
  StrCpy(szFileName,"glbyprd3.rpt");
  if(!PrintCR(GetApplication()->MainWindow->HWindow,szRepName,szFileName))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),WARNING,MB_ICONEXCLAMATION);

}


BOOL PrintCR(HWND HWindow,char *szReportTitle, char *szReportName){
  short JobNumber = PEOpenPrintJob(szReportName);
  PEOutputToWindow(JobNumber,szReportTitle,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,HWindow);
  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  if(JobNumber == 0)
   return FALSE;
  else
   return TRUE;
}

 void TGlWindow::GLMainSource   (RTMessage)
 {
  GetApplication()->ExecDialog(new TGLSourceCodes(this,"GL_MAINSOURCE"));
 }

void TGlWindow::GLPostBatch   (RTMessage)
 {  
  GetApplication()->ExecDialog(new TGLPostBatch(this,"GL_BATCHPOST"));
 }


void TGlWindow::GLDeleteBatch (RTMessage)
{
  GetApplication()->ExecDialog(new TGLCancelBatch(this,"GL_CANCELBATCH"));
}
void TGlWindow::GLPrtCompany   (RTMessage)
{
  GetApplication()->ExecDialog(new TGLProfile(this,"COMPANYPROFILE"));
}

void TGlWindow::GLCustomReport (RTMessage)
{
  GetApplication()->ExecDialog(new TGLCustomReport(this,"GL_CUSTREPORT"));
}

_CLASSDEF(TRptDialog)
class TRptDialog : public TDialog
{
 public:
 BOOL bDetail; 
 char szFileName[20];
 char szReportTitle[60];
 PTGLAcctList GLACCTS;
 PTglaccts glaccts;
 TRptDialog(PTWindowsObject AParent, LPSTR AName, LPSTR AReport,LPSTR ATitle):TDialog(AParent,AName)
 {
  strcpy(szReportTitle,ATitle);
  strcpy(szFileName,AReport);
 };
 virtual void SetupWindow(){
 TDialog::SetupWindow();
 HourGlassOn();
 SetCaption(szReportTitle);
 glaccts = new Tglaccts(&cb);
 glaccts->SetIndex("account");
 glaccts->top();
 SendDlgItemMsg(ID_ITEMSTART,WM_SETTEXT,NULL,(LPARAM)(LPSTR)glaccts->accounts->str());
 glaccts->bottom();
  SendDlgItemMsg(ID_ITEMEND,WM_SETTEXT,NULL,(LPARAM)(LPSTR)glaccts->accounts->str());
 GLACCTS = new TGLAcctList(this,"GL_ACCTLIST",glaccts);
 GLACCTS->EnableAutoCreate(); 
 GetApplication()->MakeWindow(GLACCTS);
 HourGlassOff();
 };
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL]{ShutDownWindow();cb.closeAll();};
 virtual void Search         (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
 void PrintReport(char *szFileName,char *szReportTitle,char *szPeriod);
};

void TRptDialog::Search(RTMessage)
{
 if(IsWindowVisible(GLACCTS->HWindow))
  GLACCTS->Show(SW_HIDE);
  else
  GLACCTS->Show(SW_SHOW);
}

void TGlWindow::GLPrtDistList  (RTMessage)
{
  GetApplication()->ExecDialog(new TRptDialog(this,"GL_DISTRPT","gldist.rpt","Slip Distribution Report"));
}

void TRptDialog::Ok(RTMessage)
{
  char szPeriod[3];
  long lCurrentPeriod = GetCurrentPeriodn();
  ltoa(lCurrentPeriod,szPeriod,10);
  PrintReport(szFileName,szReportTitle,szPeriod);
}

#pragma argsused
void TRptDialog::PrintReport (char *szFilename,char *szReportTitle,char *szPeriod)
{
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  char StartItem[10];
  char EndItem[10];
  char *Start = "'1010000'";
  char *End   = "'1010000'";
  char *Period = "'2'";

  GetDlgItemText(HWindow,ID_ITEMSTART,StartItem,sizeof(StartItem));
  if(*StartItem == 0){
   BWCCMessageBox(HWindow,"Enter a starting account code.","Account Code",MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return;
  }   
  GetDlgItemText(HWindow,ID_ITEMEND,EndItem,sizeof(EndItem));
  if(*EndItem == 0){
   BWCCMessageBox(HWindow,"Enter a ending account code.","Account Code",MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return;
  }   

  strcpy(Start,"'"); strcat(Start,StartItem); strcat(Start,"'");
  strcpy(End,"'");  strcat(End,EndItem);  strcat(End,"'");
  strcpy(Period,"'"); strcat(Period,szPeriod); strcat(Period,"'");

  short JobNumber = PEOpenPrintJob(szFileName);

  if (JobNumber == 0){
   BWCCMessageBox(HWindow,"Print job failed","",MB_OK);
   EnableWindow(HWindow,TRUE);
   return; 
   }


 
  if(!PESetFormula (JobNumber,"Starting Account",Start))
   {
    BWCCMessageBox(HWindow,"Error setting first item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

   if(!PESetFormula (JobNumber,"Current Period",Period))
   {
    BWCCMessageBox(HWindow,"Period is not set.","PERIOD",MB_OK);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }


  if(!PESetFormula (JobNumber,"Ending Account",End))
   {
    BWCCMessageBox(HWindow,"Error setting last item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  PEOutputToWindow(JobNumber,szReportTitle,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL
  ,GetApplication()->MainWindow->HWindow);

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
  EnableWindow(HWindow,TRUE);
  ShutDownWindow();

}
