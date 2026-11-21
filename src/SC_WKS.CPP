
/*
   Standard Cost System  - (C) Copyright 1993 by Tegara Corporation
   Spreadsheet Configuration file.
*/

#include <math.h>
#include <dir.h>
#include "scs.h"
#include "gl_wks.h"


extern Code4 cb;
extern HMENU HSCSMenuA, HGLExMenu;

static OPENFILENAME ofn;
static char szFileDialogText[] = "Worksheet (*.wk1)\0*.wk1\0All Files(*.*)\0*.*";
static char PathName[128] = "\0";
static char FileName[128] = "\0";
static CHOOSEFONT cfTemp;
HFONT hfFont;
BOOL tfFontLoaded;
COLORREF crColor;
SS_CELLCOORD UL;
SS_CELLCOORD LR;
static LOGFONT lfTemp;

HINSTANCE hInst;

#pragma argsused
BOOL FAR PASCAL dGetAcctValue(LPSS_VALUE Result, LPSS_VALUE Values, short ValueCnt)
{
 LPSS_VALUE GLAccount;
 LPSS_VALUE GLDept;
 double dAccount;
 double dDept;
 double dAmount = 0;
 char szAccount[8];
 char szDept[5];

 GLAccount = &Values[0];
 GLDept    = &Values[1];

 if(GLAccount->Status != SS_VALUE_STATUS_OK)
    return FALSE;

 if (GLAccount->Status == SS_VALUE_STATUS_OK) {
    Result->Type = GLAccount->Type;

    if (GLAccount->Type == SS_VALUE_TYPE_DOUBLE)
       dAccount = GLAccount->Val.ValDouble;
    else
       dAccount = (double)GLAccount->Val.ValLong;

    if (GLDept->Status == SS_VALUE_STATUS_OK)
       {
       if (GLDept->Type == SS_VALUE_TYPE_DOUBLE)
	  {
	  Result->Type = GLDept->Type;
	  dDept = GLDept->Val.ValDouble;
	  }
       else
	  dDept = (double)GLDept->Val.ValLong;

    }
  gcvt(dAccount,8,szAccount);
  gcvt(dDept,5,szDept);
  if(dDept == 0)
   lstrcpy(szDept,"U");
  dAmount = GetGLAmount((LPSTR)szAccount,(LPSTR)szDept);
  Result->Type = SS_VALUE_TYPE_DOUBLE;
  Result->Status = SS_VALUE_STATUS_OK;
  Result->Val.ValDouble = dAmount;
 }
 return TRUE;
}

#pragma argsused
BOOL FAR PASCAL dGetPeriodValue(LPSS_VALUE Result, LPSS_VALUE Values, short ValueCnt)
{
 LPSS_VALUE GLAccount;
 LPSS_VALUE GLDept;
 LPSS_VALUE GLPeriod;
 double dAccount;
 double dDept;
 long   iPeriod;
 double dAmount = 0;
 char szAccount[8];
 char szDept[5];

 GLAccount = &Values[0];
 GLDept    = &Values[1];
 GLPeriod  = &Values[2];

 if(GLPeriod->Status !=SS_VALUE_STATUS_OK)
    return FALSE;
   else if (GLPeriod->Type == SS_VALUE_TYPE_LONG)
    iPeriod = GLPeriod->Val.ValLong;

 if(GLAccount->Status != SS_VALUE_STATUS_OK)
    return FALSE;

 if (GLAccount->Status == SS_VALUE_STATUS_OK) {
    Result->Type = GLAccount->Type;

    if (GLAccount->Type == SS_VALUE_TYPE_DOUBLE)
       dAccount = GLAccount->Val.ValDouble;
    else
       dAccount = (double)GLAccount->Val.ValLong;

    if (GLDept->Status == SS_VALUE_STATUS_OK)
       {
       if (GLDept->Type == SS_VALUE_TYPE_DOUBLE)
	  {
	  Result->Type = GLDept->Type;
	  dDept = GLDept->Val.ValDouble;
	  }
       else
	  dDept = (double)GLDept->Val.ValLong;

    }
  gcvt(dAccount,8,szAccount);
  gcvt(dDept,5,szDept);
  if(dDept == 0)
   lstrcpy(szDept,"U");
  dAmount = GetPeriod((LPSTR)szAccount,(LPSTR)szDept,(int)iPeriod);
  Result->Type = SS_VALUE_TYPE_DOUBLE;
  Result->Status = SS_VALUE_STATUS_OK;
  Result->Val.ValDouble = dAmount;
 }
 return TRUE;
}

#pragma argsused
BOOL FAR PASCAL dGetOpenBalance(LPSS_VALUE Result, LPSS_VALUE Values, short ValueCnt)
{
 LPSS_VALUE GLAccount;
 LPSS_VALUE GLDept;

 double dAccount;
 double dDept;

 double dAmount = 0;
 char szAccount[8];
 char szDept[5];

 GLAccount = &Values[0];
 GLDept    = &Values[1];


 if(GLAccount->Status != SS_VALUE_STATUS_OK)
    return FALSE;

 if (GLAccount->Status == SS_VALUE_STATUS_OK) {
    Result->Type = GLAccount->Type;

    if (GLAccount->Type == SS_VALUE_TYPE_DOUBLE)
       dAccount = GLAccount->Val.ValDouble;
    else
       dAccount = (double)GLAccount->Val.ValLong;

    if (GLDept->Status == SS_VALUE_STATUS_OK)
       {
       if (GLDept->Type == SS_VALUE_TYPE_DOUBLE)
	  {
	  Result->Type = GLDept->Type;
	  dDept = GLDept->Val.ValDouble;
	  }
       else
	  dDept = (double)GLDept->Val.ValLong;

    }
  gcvt(dAccount,8,szAccount);
  gcvt(dDept,5,szDept);
  if(dDept == 0)
   lstrcpy(szDept,"U");
  dAmount = GetOpenBalance((LPSTR)szAccount,(LPSTR)szDept);
  Result->Type = SS_VALUE_TYPE_DOUBLE;
  Result->Status = SS_VALUE_STATUS_OK;
  Result->Val.ValDouble = dAmount;
 }
 return TRUE;
}

double GetOpenBalance(LPSTR lpAccountNo,LPSTR lpDept)
{
 PTglaccts glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
 char szQuery[85]; 
 double dValue = 0;
 if(lstrcmp(lpDept,"U")==0)  
  StrCpy(szQuery,lpAccountNo);
 else{
  StrCpy(szQuery,lpAccountNo);
  StrCat(szQuery,lpDept);
 }
 if( glaccts->seek(szQuery) == 0){
   dValue = double(Str4ptr(glaccts->previous12->str()));
 }
 else
 BWCCMessageBox(GetFocus(),"Can't find the account number.",
 		GetString(hInst,ID_WARNING),MB_OK);
 glaccts->close();
 return fabs(dValue);
}


double GetGLAmount(LPSTR lpAccountNo,LPSTR lpDept)
{
 PTglaccts glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
 char szQuery[85]; 
 double dValue = 0;
 int iPeriod;
 if(lstrcmp(lpDept,"U")==0)  
  StrCpy(szQuery,lpAccountNo);
 else{
  StrCpy(szQuery,lpAccountNo);
  StrCat(szQuery,lpDept);
 }
 iPeriod = (int)GetCurrentPeriodn();
 if( glaccts->seek(szQuery) == 0){
   switch (iPeriod){
    case 1: {dValue = glaccts->dPeriod1();break;}
    case 2: {dValue = glaccts->dPeriod2();break;}
    case 3: {dValue = glaccts->dPeriod3();break;}
    case 4: {dValue = glaccts->dPeriod4();break;}
    case 5: {dValue = glaccts->dPeriod5();break;}
    case 6: {dValue = glaccts->dPeriod6();break;}
    case 7: {dValue = glaccts->dPeriod7();break;}
    case 8: {dValue = glaccts->dPeriod8();break;}
    case 9: {dValue = glaccts->dPeriod9();break;}
    case 10: {dValue = glaccts->dPeriod10();break;}
    case 11: {dValue = glaccts->dPeriod11();break;}
    case 12: {dValue = glaccts->dPeriod12();break;}
    default: dValue = glaccts->Curr_bal();
   }
 }
 else
 BWCCMessageBox(GetFocus(),"Can't find the account number.",
			GetString(hInst,ID_WARNING)
 			,MB_OK);
 glaccts->close();
 return fabs(dValue);
}

double GetPeriod(LPSTR lpAccountNo,LPSTR lpDept,int dPeriod)
{
 PTglaccts glaccts = (Tglaccts *) new Tglaccts(&cb,"glaccts");
 char szQuery[85];
 double dValue = 0;
 if(lstrcmp(lpDept,"U")==0)
  StrCpy(szQuery,lpAccountNo);
 else{
  StrCpy(szQuery,lpAccountNo);
  StrCat(szQuery,lpDept);
 }
 switch(dPeriod){
    case 1 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod1();
	       else dValue = 0;
	       break;
	     }
    case 2 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod2();
	       else dValue = 0;
	       break;}
    case 3 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod3();
	       else dValue = 0;
	       break;}
    case 4 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod4();
	       else dValue = 0;
	       break;}
    case 5 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod5();
	       else dValue = 0;
	       break;}
    case 6 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod6();
	       else dValue = 0;
	       break;}
    case 7 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod7();
	       else dValue = 0;
	       break;}
    case 8 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod8();
	       else dValue = 0;
	       break;}
    case 9 : { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod9();
	       else dValue = 0;
	       break;}
    case 10: { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod10();
	       else dValue = 0;
	       break;}
    case 11: { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod11();
	       else dValue = 0;
	       break;}
    case 12: { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->dPeriod12();
	       else dValue = 0;
	       break;}
    default: { if( glaccts->seek(szQuery) == 0)
		dValue = glaccts->Curr_bal();
	       else dValue = 0;
	       break;}
   }
 glaccts->close();
 return fabs(dValue);
}


void TWorksheet::CMSave(RTMessage){Save();}
BOOL TWorksheet::CMSaveAs(RTMessage){if(SaveAs()) return TRUE; return FALSE;}
void TWorksheet::CMOpen(RTMessage){
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
   Open(FileName);
  }
}

void TWorksheet::SetupWindow()
{
 TWindow::SetupWindow(); 
 HourGlassOn();
 Spread->Attr.Style |= SSS_ALLOWCELLOVERFLOW;
 Spread->SetBool(SSB_ALLOWUSERSELBLOCK,TRUE);
 Spread->SetBool(SSB_ALLOWUSERFORMULAS,TRUE);
 Spread->SetBool(SSB_ALLOWDRAGDROP,TRUE);
 Spread->SetBool(SSB_CHANGEMADE,FALSE);
 hInst = GetApplication()->hInstance;
 getcwd(szFileName,MAXPATH);
 StrCat(szFileName,szTitle);
 Spread->SetMaxRows(100);
 Spread->SetMaxCols(20);
 lpfnGetOpen = MakeProcInstance((FARPROC)dGetOpenBalance, GetApplication()->hInstance);
 Spread->AddCustomFunction("OPEN",2,lpfnGetOpen);

 lpfnGetPeriod = MakeProcInstance((FARPROC)dGetPeriodValue, GetApplication()->hInstance);
 Spread->AddCustomFunction("PD",3,lpfnGetPeriod);

 lpfnGetCurrPeriod = MakeProcInstance((FARPROC)dGetAcctValue, GetApplication()->hInstance);
 Spread->AddCustomFunction("ACCT",2,lpfnGetCurrPeriod);


 if(StrCmp(szTitle,"Untitled.wk1") != 0)
 Open(szFileName);
 HourGlassOff();

}

BOOL TWorksheet::Save ()
{
 char FileName[MAXPATH];
 GetWindowText(HWindow,FileName,sizeof(FileName));
 if(strcmp(FileName,"Untitled")==0)
  if(SaveAs()){
     Spread->SetBool(SSB_CHANGEMADE,FALSE);
     return TRUE;
  }
 else
 Spread->SaveToFile(FileName,FALSE);
 Spread->SetBool(SSB_CHANGEMADE,FALSE);
 return TRUE;
}

BOOL TWorksheet::SaveAs ()
{
  ofn.lStructSize     = sizeof(OPENFILENAME);
  ofn.lpstrFile         = FileName ;          // Set in Open and Close functions
  ofn.nMaxFile          = _MAX_PATH ;
  ofn.lpstrFileTitle    = NULL ;          // Set in Open and Close functions
  ofn.hwndOwner       = HWindow;
  ofn.lpstrFilter     = szFileDialogText;
  ofn.lpstrInitialDir = PathName;
  ofn.Flags           = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  ofn.lpfnHook        = NULL;

  if(GetSaveFileName(&ofn)!=0)
  {
   Spread->SaveToFile(FileName,FALSE);
   SetWindowText(HWindow,FileName);
   return TRUE;
  }
   return FALSE;
}

void TWorksheet::Open (char *szFileName)
{
   Spread->LoadFromFile(szFileName);
   SetWindowText(HWindow,szFileName);
}

void TWorksheet::Color(RTMessage)
{
 HourGlassOn();
 
 for(int i=6;i<100;i++){
  for(int j=2;j<6;j++){   
  COLORREF lpBack, lpFore;
  Spread->GetColor(j,i,&lpBack,&lpFore);
  Spread->SetColor(j,i, RGBCOLOR_RED, RGBCOLOR_DARKBLUE);
  GLOBALHANDLE hFormula = GlobalAlloc(GHND,Spread->GetFormulaLen(2,i));
  LPSTR lpFormula = (LPSTR)GlobalLock(hFormula);
  Spread->GetFormula(j,i,lpFormula);
  Spread->SetFormula(j,i,lpFormula,TRUE);
  GlobalUnlock(hFormula);
  Spread->SetColor(j,i,lpBack,lpFore);
  }
 }
 Spread->ReCalc();
 HourGlassOff();
 return;
}


void TWorksheet::Character (RTMessage)
{ 
 if(Spread->GetSelectBlock(&UL,&LR))
 {
  Spread->SetTypeEdit (&CellType,ES_LEFT,50,SS_CHRSET_CHR,SS_CASE_NOCASE);
  Spread->SetCellTypeRange(UL.Col,UL.Row,LR.Col,LR.Row,&CellType);
  Spread->SetEditMode(TRUE);
 }
 else
 {
  SS_COORD Col,Row;
  Spread->SetTypeEdit (&CellType,ES_LEFT,50,SS_CHRSET_CHR,SS_CASE_NOCASE);
  Spread->GetActiveCell(&Col,&Row);
  Spread->SetCellType(Col,Row,&CellType);
 }
}

void TWorksheet::AccountNo (RTMessage)
{
 /*
  SS_COORD Col,Row;
  Spread->GetActiveCell(&Col,&Row);
  GetApplication()->ExecDialog(new TSetValue(this,"GL_ACCOUNTNO",(SS_COORD)Row,(SS_COORD)Col,Spread));
 */
}

void TWorksheet::WMMDIActivate(TMessage& Msg)
{
  TWindow::WMMDIActivate(Msg);
  DefWndProc(Msg);
  if ( Parent )
  {
    ((PTGlWindow)Parent)->Worksheet= (Msg.WParam ? this : NULL);
  }
  if(Msg.WParam == TRUE)
  {
    HMENU HPopUpMenu = GetSubMenu(HGLExMenu,0); // Get the child menu position.
    SendMessage(((PTGlWindow)Parent)->ClientWnd->HWindow,WM_MDISETMENU,0,MAKELONG(HGLExMenu,HPopUpMenu));
    DrawMenuBar(GetApplication()->MainWindow->HWindow);
  }
  else if (Msg.WParam == NULL)
  {
    HMENU HPopUpMenu = GetSubMenu(HSCSMenuA,0); // Get the child menu position.
    SendMessage(((PTGlWindow)Parent)->ClientWnd->HWindow,WM_MDISETMENU,0,MAKELONG(HSCSMenuA,HPopUpMenu));
    DrawMenuBar(GetApplication()->MainWindow->HWindow);

  }
  if ( !IsFlagSet(WB_ALIAS) )
  {
    ActivationResponse(Msg.WParam, IsIconic(HWindow));
  }

}



void TWorksheet::Number(RTMessage)
{ 
 if(Spread->GetSelectBlock(&UL,&LR))
 { 
  Spread->SetTypeFloat(&CellType,FS_SEPARATOR,9,2,0,9999999999999.99);
  Spread->SetCellTypeRange(UL.Col,UL.Row,LR.Col,LR.Row,&CellType);
  Spread->SetEditMode(TRUE);
  Spread->SetBool(SSB_ALLOWUSERFORMULAS,TRUE);
 }
 else
 {
  SS_COORD Col,Row;
  Spread->GetActiveCell(&Col,&Row);
  Spread->SetTypeFloat(&CellType,FS_SEPARATOR,9,2,0,9999999999999.99);
  Spread->SetCellType(Col,Row,&CellType);
  Spread->SetEditMode(FALSE);
  Spread->SetBool(SSB_ALLOWUSERFORMULAS,TRUE);
 }

}
void TWorksheet::WMRButtonDown (RTMessage Msg)
{
 RECT rc;
 GetWindowRect(Spread->HWindow,&rc);
 LPSS_RBUTTON RButton = (LPSS_RBUTTON)Msg.LParam;
 HMENU hMenu = LoadMenu(GetApplication()->hInstance,"SPREAD");
 hMenu = GetSubMenu(hMenu,0);
 int x = rc.left + RButton->xPos;
 int y = rc.top+ RButton->yPos;
 TrackPopupMenu(hMenu,TPM_LEFTALIGN,x,y,0,HWindow,NULL);
 return;
}

void TWorksheet::PrintSheet (RTMessage)
{
 	    SS_PRINTFORMAT PrintFormat;
	    _fmemset(&PrintFormat,'\0',sizeof(SS_PRINTFORMAT));
	    PrintFormat.dPrintType = SS_PRINTTYPE_CELLRANGE;
            PrintFormat.x1CellRange = 1;
            PrintFormat.y1CellRange = 1;
            PrintFormat.x2CellRange = 4;
	    PrintFormat.y2CellRange = 92;
            PrintFormat.nPageStart = 1;
	    PrintFormat.nPageEnd = 1;
            PrintFormat.fMarginLeft = (float).5;
            PrintFormat.fMarginTop = (float).5;
            PrintFormat.fMarginRight = (float).5;
	    PrintFormat.fMarginBottom = (float).5;
	    Spread->Print("Income Statement", &PrintFormat, NULL, NULL, NULL);
}

void TWorksheet::Copy (RTMessage){
 Spread->ClipboardCopy();
 Spread->SetEditMode(TRUE);
}

void TWorksheet::Paste (RTMessage){ Spread->ClipboardPaste();}

void TWorksheet::Cut (RTMessage){ Spread->ClipboardCut();}

void TWorksheet::Font (RTMessage){
 if( tfFontLoaded == TRUE )	// cfTemp contains previous selections
   {
	cfTemp.Flags |= CF_INITTOLOGFONTSTRUCT;
	cfTemp.rgbColors = crColor;
   }
   else
   {
	cfTemp.lStructSize = sizeof( CHOOSEFONT );
	cfTemp.hwndOwner = HWindow;
	cfTemp.hDC = 0;
	cfTemp.lpLogFont = &lfTemp;	// Store the result here
	cfTemp.Flags = CF_EFFECTS | CF_FORCEFONTEXIST | CF_SCREENFONTS ;
	cfTemp.rgbColors = crColor;	// Color and font dialogs use the same color
	cfTemp.lCustData = 0L;
	cfTemp.lpfnHook = NULL;
	cfTemp.lpTemplateName = NULL;
	cfTemp.hInstance = 0;
	cfTemp.lpszStyle = NULL;
	cfTemp.nFontType = SCREEN_FONTTYPE;
	cfTemp.nSizeMin = 0;
	cfTemp.nSizeMax = 0;
  }
 if( ChooseFont( &cfTemp ) == TRUE )
  {
   if( tfFontLoaded == TRUE )
	DeleteObject( hfFont );
	crColor = cfTemp.rgbColors;
	hfFont = CreateFontIndirect( &lfTemp );
	tfFontLoaded = TRUE;
       if(Spread->GetSelectBlock(&UL,&LR))
         {
	  Spread->SetFontRange(UL.Col,UL.Row,LR.Col,LR.Row,hfFont,FALSE);
	  Spread->SetBool(SSB_CHANGEMADE,TRUE);
         }
	 else
         {
          SS_COORD Col,Row;
          Spread->GetActiveCell(&Col,&Row);
          Spread->SetFont(Col,Row,hfFont,FALSE);
          Spread->SetBool(SSB_CHANGEMADE,TRUE);
         }
	  InvalidateRect(Spread->HWindow, NULL, TRUE );
  }

}



void TWorksheet::Border (RTMessage)
{
  if(Spread->GetSelectBlock(&UL,&LR)){
     GetApplication()->ExecDialog(new TSetBorder(this,"SS_BORDER",(unsigned) UL.Row,
     (unsigned)UL.Col,(unsigned)LR.Row,(unsigned)LR.Col,Spread));
  }
  else{
     SS_COORD Col,Row;
     Spread->GetActiveCell(&Col,&Row);  
     GetApplication()->ExecDialog(new TSetBorder(this,"SS_BORDER",(unsigned)Row,(unsigned)Col,Spread));
  }
}
BOOL TWorksheet::CanClose ()
{
 int rc;
 if (Spread->GetBool(SSB_CHANGEMADE)){
   rc = BWCCMessageBox(HWindow,"Do you want to save changes?","WARNING",MB_YESNOCANCEL | MB_ICONQUESTION);
   if(rc == IDYES){
     char FileName[MAXPATH];
     GetWindowText(HWindow,FileName,sizeof(FileName));
     Spread->SaveToFile(FileName,FALSE);
     return TRUE;
   }
   if (rc == IDNO){
    return TRUE;
   }
   if(rc == IDCANCEL){
    return FALSE;
   }
 }
 return TRUE;
}

void TWorksheet::GetWindowClass(WNDCLASS& wc)
{
 TWindow::GetWindowClass(wc);
 wc.hIcon=LoadIcon(wc.hInstance, "TABLE");
}

void TWorksheet::WMSize ( RTMessage  Msg)
{
  TWindow::WMSize(Msg);
  SetWindowPos(Spread->HWindow, 0, -1, -1, LOWORD(Msg.LParam)+2,
  HIWORD(Msg.LParam)+2, SWP_NOZORDER);
}


void TWorksheet::SetPageBreak(RTMessage)
{
 SS_COORD Col,Row;
 Spread->GetActiveCell(&Col,&Row);
 if(Spread->SetRowPageBreak(47,TRUE))
 return;
}