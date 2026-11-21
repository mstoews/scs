// General Ledger  - (C) Copyright 1992 by Tegara Corporation
#include "ap.h"
#define IDD_STATUSBAR_MENUHELP 2
#define STATUSBAR_HEIGHT       26
#define GWW_STATUSBAR          0
#define  BUTTON_WIDTH   45


Code4 cb;
LPSTR  path;
char   CurDir[MAXPATH];
extern PTWindowsObject GetObjectPtr;
LPSTR  dbf;
int  PeriodNo;
int X;
int Y;
HINSTANCE hInst;

#define TOOLBAR_WIDTH          32
#define TOOLBAR_HEIGHT         32
#define GWW_STATUSBAR          0
#define GWW_TOOLBOX            2
#define GWW_TOOLBAR            4



HINSTANCE hDLL;
HMENU  HAPMenu;
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

// Used to create the toolbar at the top of the application.
_CLASSDEF (TToolBar)
class TToolBar : public TDialog
{

  public:
  TToolBar(PTWindowsObject AParent,LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow();
  virtual void WMPaint       (RTMessage Msg) = [WM_FIRST + WM_PAINT];
  virtual void APAdd         (RTMessage Msg) = [ID_FIRST + ID_APADD];
  virtual void APCash        (RTMessage Msg) = [ID_FIRST + ID_CASH];
  virtual void APBatch       (RTMessage Msg) = [ID_FIRST + ID_APBATCH];
  virtual void APAdjust      (RTMessage Msg) = [ID_FIRST + ID_ADJBATCH];
  virtual void APPost        (RTMessage Msg) = [ID_FIRST + ID_APPOST];
  virtual void APInquiry     (RTMessage Msg) = [ID_FIRST + ID_APINQ];
  virtual void APCancelBatch (RTMessage Msg) = [ID_FIRST + ID_CHANGE];
  virtual void APQuit        (RTMessage Msg) = [ID_FIRST + ID_ADJCANCEL];

};


void TToolBar::APAdd         (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_APADDVENDORS,NULL);}
void TToolBar::APCash        (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_APCASH,NULL);}
void TToolBar::APBatch       (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_APBATCH,NULL);}
void TToolBar::APAdjust      (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_ADJBATCH,NULL);}
void TToolBar::APPost        (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_APPOSTBATCH,NULL);}
void TToolBar::APInquiry     (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_OPENACCTS,NULL);}
void TToolBar::APCancelBatch (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,ID_BATCHCANCEL,NULL);}
void TToolBar::APQuit        (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,24340,NULL);}

void TToolBar::SetupWindow()
{
 RECT rect;
 GetClientRect(HWindow,&rect);

 SendDlgItemMsg(ID_APBATCH,      SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"CALC");
 SendDlgItemMsg(ID_ADJBATCH,     SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ADJ");
 SendDlgItemMsg(ID_CASH,         SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"CASH");
 SendDlgItemMsg(ID_APINQ,        SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"VIEW");
 SendDlgItemMsg(ID_APPOST,       SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"POST");
 SendDlgItemMsg(ID_APADD,        SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ADD");
 SendDlgItemMsg(ID_CHANGE,       SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"CANCEL");
 SendDlgItemMsg(ID_ADJCANCEL,    SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"QUIT");

 MoveWindow(GetDlgItem(HWindow,ID_APBATCH)     ,rect.left+10                  ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_ADJBATCH)    ,rect.left+10+(BUTTON_WIDTH)   ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_CASH)        ,rect.left+10+(2*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_APADD)       ,rect.left+10+(3*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_APPOST)      ,rect.left+10+(4*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_CHANGE)      ,rect.left+10+(5*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_APINQ)       ,rect.left+10+(6*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_ADJCANCEL)   ,rect.left+10+(7*BUTTON_WIDTH) ,rect.top+4,40,30,0);
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
  DestroyMenu(HAPMenu);
  PECloseEngine();
  };
};


void TGlWindow::CMHelp (RTMessage)
{
  WinExec("winhelp winhelp.hlp",SW_SHOW);
}

void TGlWindow::CMAbout (RTMessage)
{
  GetApplication()->ExecDialog(new TDialog(this,"ABOUT"));
}


void TGlWindow::GetWindowClass(WNDCLASS& wc)
{
 TWindow::GetWindowClass(wc);
 wc.hIcon=LoadIcon(wc.hInstance, "GL");
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
  SendMessage(HStatusBar, SBRM_SETITEMTEXT,1,(LPARAM)GetCompanyName());
  cb.exclusive = 0;
  cb.readOnly = 0;
  cb.readLock = 0;
  cb.lockAttempts =5;
}

char  *TGlWindow::GetCompanyName ()
{
  static char szCompanyName[20];
  cb.autoOpen = FALSE;
  profile = new Tprofile(&cb);
  cb.autoOpen = TRUE;
  profile->top();
  strcpy(szCompanyName,profile->company->str());
  profile->close();
  return szCompanyName;
}


char  *TGlWindow::GetCurrentPeriod ()
{
  static char szCurrPeriod[20];
  cb.autoOpen = FALSE;
  profile = new Tprofile(&cb);
  cb.autoOpen = TRUE;
  profile->top();
  strcpy(szCurrPeriod,profile->curr_pd->str());
  profile->close();
  return szCurrPeriod;
}

double GetCurrentPeriodn()
{
  double dCurr_pd;
  cb.autoOpen = FALSE;
  PTprofile profile = new Tprofile(&cb);
  cb.autoOpen = TRUE;

  profile->top();
  dCurr_pd =  double (Str4ptr(profile->curr_pdn->str()));
  profile->close();
  return dCurr_pd;
}


TGlWindow::TGlWindow(LPSTR ATitle,LPSTR MenuName) : TMDIFrame(ATitle,MenuName)
 {
   ChildMenuPos = 0;
   Attr.X=0;
   Attr.Y=0;
   Attr.W=CW_USEDEFAULT;
   Attr.H=0;
   getcwd(CurDir,MAXPATH);
   strcat(CurDir,"\\..\\..\\DBF");
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







void TGLClient::WMRButtonDown (RTMessage Msg)
{ 
 POINT point = MAKEPOINT(Msg.LParam);
 ClientToScreen(HWindow,&point);
 X=point.x;
 Y=point.y;
 SendMessage(GetApplication()->MainWindow->HWindow,WM_POPUPMENU,(unsigned)Msg.LParam,NULL);
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


void TGlWindow::PopUpMenu (RTMessage) 
{
   HMENU hMenu = LoadMenu(GetApplication()->hInstance,"APSUB");
   hMenu = GetSubMenu(hMenu,0);
   TrackPopupMenu(hMenu,TPM_LEFTBUTTON,X,Y,0,GetApplication()->MainWindow->HWindow,NULL);
   return;
}




void TGlWindow::APMenu(RTMessage)
{                      
 HMENU HPopUpMenu = GetSubMenu(HAPMenu,0); // Get the child menu position.
 SendMessage(ClientWnd->HWindow,WM_MDISETMENU,0,MAKELONG(HAPMenu,HPopUpMenu));
 DrawMenuBar(HWindow);
 SetWindowText(HWindow,"Accounts Payable Module");
}


void TAcctModule::InitMainWindow()
{
  MainWindow = new TGlWindow("Accounts Payable Module","ap");
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
  hInst = hInstance;
  HWND hTegara1 = LoadLibrary("tegara1.dll");
  if ((int)hTegara1 < 32)
      exit(1);
  HWND hTegara2 = LoadLibrary("tegara2.dll");
  if ((int)hTegara2 < 32)
      exit(1);

  hDLL = LoadLibrary("BWCC.DLL");
  if ((int)hDLL < 32)
    exit(1);
  HAPMenu = LoadMenu(hInstance,"ap");
  PEOpenEngine();
  AcctModule.Run();
  return AcctModule.Status;
}

HWND TGlWindow::GLCreateStatusBar()
{
  HWND hWndSBR;
  RECT RectClient;
  LOGFONT lFont;

  static STATUSBARITEM Items[] =
     {
     {1, 6, 240, "Company Name",SBRS_TEXTLEFT},
     {2, 6, SBR_AUTOSIZE, ""             ,SBRS_TEXTLEFT},
     {3, 6, 140,  "",                        SBRS_TEXTCENTER}, 
     };

  GetClientRect(HWindow, &RectClient);

  hWndSBR = tbCreateStatusBar(HWindow, WS_CHILD | WS_VISIBLE | SBRS_DRAWTOPLINE |SBRS_PIXELS,  0,
                  RectClient.bottom - STATUSBAR_HEIGHT,
                  RectClient.right, STATUSBAR_HEIGHT,
		  Items, sizeof(Items) / sizeof(STATUSBARITEM));
 HFONT  hfontDlg = (HFONT) NULL;
    if ( (hfontDlg =  (HFONT) SendMessage(hWndSBR, WM_GETFONT, 0, 0L))!=0 ) {
        if (GetObject(hfontDlg, sizeof(LOGFONT), (LPSTR) &lFont)) {
	    lFont.lfHeight = -30;
            lFont.lfWeight = FW_LIGHT;
	    if (hfontDlg == CreateFontIndirect((LPLOGFONT) &lFont)) {
		SendMessage(hWndSBR,WM_SETFONT,(WPARAM)hfontDlg,(LPARAM)FALSE);
	    }
	}
    }
 return (hWndSBR);
}



//Adjustment journal entry
void TGlWindow::APAdjPosting  (RTMessage)
 {
  SendMessage(HWindow,WM_COMMAND,ID_APPOSTBATCH,NULL);
 // GetApplication()->ExecDialog(new TDialog(this,"AP_ADJPOSTING"));
 }

void TGlWindow::APVenPayment  (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"AP_VENPAYMENT"));
 }

void TGlWindow::APExternalBat (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"AP_EXTBATCH"));
 }



void TGlWindow::APYearEnd     (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"AP_YEAREND"));
 }



void TGlWindow::ModifyBatch   (RTMessage){SendMessage(GetActiveWindow(),WM_MODIFYBATCH,NULL,NULL);}
void TGlWindow::NewBatch      (RTMessage){SendMessage(GetActiveWindow(),WM_NEWBATCH   ,NULL,NULL);}
void TGlWindow::SearchGL      (RTMessage){SendMessage(GetActiveWindow(),WM_SEARCHGL   ,NULL,NULL);}
void TGlWindow::CancelBatch   (RTMessage){SendMessage(GetActiveWindow(),WM_CANCELBATCH,NULL,NULL);}



void TGlWindow::APInqVendors  (RTMessage)
 {
  GetApplication()->ExecDialog(new TDialog(this,"AP_INQVENDORS"));
 }

void TGlWindow::APCustomReport (RTMessage)
{
  GetApplication()->ExecDialog(new TGLCustomReport(this,"GL_CUSTREPORT"));
}

