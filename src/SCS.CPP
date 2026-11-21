// Standard Cost System  - (C) Copyright 1992 by Tegara Corporation

#include "scs.h"
#include "crpe.h"
#include "db_access.h"
#include "gl_strg.h"
#include "db_topic.h"
#include "db_mrslt.h"
#include "gl_dlg.h"
#include <direct.h>
#include <lzexpand.h>

#define IDD_STATUSBAR_MENUHELP 2
#define STATUSBAR_HEIGHT       26
#define GWW_STATUSBAR          0
#define  BUTTON_WIDTH          45



// Define global variables for Codebase.
Code4 cb;
HINSTANCE hInst;
LPSTR  path;
HWND hStatus;
char   *szCompanyName;
char   CurDir[MAXPATH];
char   CurrentPeriod[45];
char szAccessLevel[5],szUser[11];
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


HINSTANCE hDLL;
HMENU HSCSMenuA,HSCSMenuB,HSCSMenuC, HSCSMenuD,HGLExMenu;
HWND  prev_wnd;
FARPROC find_prev_wndi;
BOOL CALLBACK _export find_prev_wnd(HWND,LPARAM);

char *GetString(HINSTANCE hInstance, int ID)
{
 static char szString[85];
 LoadString(hInstance,ID,szString,sizeof(szString));
 return szString;
}



BOOL CALLBACK _export find_prev_wnd(HWND wnd, LPARAM lParam)
{
 /* Look for a top level window having the same instance. */
 if (!GetParent(wnd) && (HINSTANCE)GetWindowWord(wnd, GWW_HINSTANCE) == (HINSTANCE) lParam)
 {
   prev_wnd = wnd;
   return FALSE;
 }
 return TRUE;
}

void TGlWindow::SCSOpenWriter (RTMessage)
  {
   int rc = WinExec("c:\\crw\\crw.exe",SW_SHOW);
   if (rc < 32)
    {
      BWCCMessageBox(HWindow,"Unable to find Report Writer.","Crystal Reports",MB_ICONEXCLAMATION);
    }
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
  DestroyMenu(HSCSMenuA);
  DestroyMenu(HSCSMenuB);
  DestroyMenu(HSCSMenuC);
  DestroyMenu(HSCSMenuD);
  PECloseEngine();
  };
};


void TGlWindow::CMHelp (RTMessage)
{
  WinExec("winhelp winhelp.hlp",SW_SHOW);
}

void TGlWindow::SCSTransferGL (RTMessage)
{
 if(chdir("a:\\"))
 {
  chdir(CurDir);
  BWCCMessageBox(HWindow,GetString(hInst,1363),GetString(hInst,1364),MB_ICONSTOP);
  return;
 }
 else
 {
  if(BWCCMessageBox(HWindow,GetString(hInst,1365),GetString(hInst,1364),MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
  {
  chdir(CurDir);
  WinExec("transgl.pif",SW_SHOW);
  }
 }
}


void TGlWindow::SCSWorkSheet (RTMessage)
{
}

void TGlWindow::SCSMaterialUsage (RTMessage)
{
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
  //GetApplication()->ExecDialog(new TGLAccess(this,"password"));
  strcpy(szUser,"User MT");
  ToolBar = (TToolBar *) new TToolBar(this,"TOOLBAR");
  ToolBar->EnableAutoCreate();
  GetApplication()->MakeWindow(ToolBar);
  ToolBar->Show(SW_SHOW); 
  HStatusBar = GLCreateStatusBar();
  hStatus = HStatusBar;
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
  profile =  new Tprofile(&cb);
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
  PTprofile profile = (Tprofile *) new Tprofile(&cb,"profile");
  cb.autoOpen = TRUE;
  profile->top();
  dCurr_pd =  double (Str4ptr(profile->curr_pdn->str()));
  profile->close();
  return dCurr_pd;
}


TGlWindow::TGlWindow(LPSTR ATitle,LPSTR MenuName) : TMDIFrame(ATitle,MenuName)
 {
   ChildMenuPos = 5;
   Attr.X=0;
   Attr.Y=0;
   Attr.W=CW_USEDEFAULT;
   Attr.H=0;
   getcwd(CurDir,MAXPATH);
   strcat(CurDir,"\\..\\..\\DBF");
   chdir(CurDir);
   getcwd(CurDir,MAXPATH);
 }

void TGlWindow::WMSize(RTMessage)
{
 RECT rect;
 GetClientRect(HWindow, &rect);
 MoveWindow(GetClient()->HWindow,0,38,rect.right,rect.bottom-64,1);
 MoveWindow(ToolBar->HWindow,-7,0,rect.right-rect.left+7,38,1);
 MoveWindow(HStatusBar, 0, rect.bottom-STATUSBAR_HEIGHT, rect.right, STATUSBAR_HEIGHT, TRUE);
}


void TGLClient::WMRButtonDown (RTMessage Msg)
{ 
 POINT point = MAKEPOINT(Msg.LParam);
 ClientToScreen(HWindow,&point);
 X=point.x;
 Y=point.y;
 SendMessage(GetApplication()->MainWindow->HWindow,WM_POPUPMENU,(WPARAM)Msg.LParam,NULL);
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
   HMENU hMenu;
   char szTitleBar[85];
   GetWindowText(HWindow,szTitleBar,sizeof(szTitleBar));   
   hMenu = LoadMenu(GetApplication()->hInstance,"SCSSUB");
   hMenu = GetSubMenu(hMenu,0);
   TrackPopupMenu(hMenu,TPM_LEFTBUTTON,X,Y,0,GetApplication()->MainWindow->HWindow,NULL);
   return;
}


void TGlWindow::GLMenu(RTMessage)
{ 
 char Str[60];
 HMENU HPopUpMenu = GetSubMenu(HSCSMenuA,0); // Get the child menu position.
 SendMessage(ClientWnd->HWindow,WM_MDISETMENU,0,MAKELONG(HSCSMenuA,HPopUpMenu));
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
  MainWindow = new TGlWindow("Standard Cost System","scsA");
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdShow)
{
  TAcctModule AcctModule ("AcctApp", hInstance, hPrevInstance,lpCmdLine, nCmdShow);
  if(hPrevInstance)
  {
   find_prev_wndi = MakeProcInstance((FARPROC)find_prev_wnd,hInstance);
	EnumWindows((WNDENUMPROC)find_prev_wndi,(LPARAM)hPrevInstance);
	FreeProcInstance(find_prev_wndi);
   if (!prev_wnd)
     return 0;
   prev_wnd = GetLastActivePopup(prev_wnd);
   ShowWindow(prev_wnd, SW_SHOWNORMAL);
   SetActiveWindow(prev_wnd);
   return 0;
  }
  /*
  if(hPrevInstance)
  { 
   HWND hWnd,hWnd1;
   if(hWnd = FindWindow("Standard Cost System",NULL))
    {
    if(hWnd1 = GetLastActivePopup(hWnd)) hWnd = hWnd1;
     BringWindowToTop(hWnd);
    if(IsIconic(hWnd))
     ShowWindow(hWnd,SW_RESTORE);
   }
   return FALSE;
  }
  */
  hInst = hInstance;
  HINSTANCE hTegara1 = LoadLibrary("tegara1.dll");
  if ((int)hTegara1 < 32)
		exit(1);
  HINSTANCE hTegara2 = LoadLibrary("tegara2.dll");
  if ((int)hTegara2 < 32)
      exit(1);

  hDLL = LoadLibrary("BWCC.DLL");
  if ((int)hDLL < 32)
    exit(1);
  if(PEOpenEngine() == 0)
   BWCCMessageBox(GetFocus(),"Unable to open print engine.","WARNING",MB_ICONEXCLAMATION);

  HSCSMenuA   = LoadMenu(hInstance,"scsA");

  HGLExMenu   = LoadMenu(hInstance,"gle");
  szCompanyName = new char[40];
  AcctModule.Run();
  FreeLibrary(hTegara1);
  FreeLibrary(hTegara2);
  FreeLibrary(hDLL);
  PECloseEngine();
  DestroyMenu(HSCSMenuA);
  DestroyMenu(HSCSMenuB);
  DestroyMenu(HSCSMenuC);
  DestroyMenu(HSCSMenuD);
  delete szCompanyName; 
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





