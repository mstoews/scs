/*** General Ledger Specification ***/
#include "scs.h"

/*** Database Include Files ***/

#include "db_glspc.h"
#include "db_resrc.h"

_CLASSDEF(TGLResList)
class TGLResList : public TSCResList
{
 public:
  TGLResList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTResrc t):
  TSCResList(AParent,AName,AQuery,t)
  {
  strcpy(szQuery,AQuery);
  Resrc = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
};

void TGLResList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(Resrc->seek(szItem) == 0)
  {
    SendMessage(GetDlgItem(Parent->HWindow,ID_RESCODE)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fCode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RESNAME)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fRate->str());
    SendMessage(Parent->HWindow,WM_COMMAND,IDM_UPDATE,NULL);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 SetFocus(Parent->HWindow);
}


/*** Maintenance: General Ledger Accounts ***/
_CLASSDEF(TGLSpecification)
class TGLSpecification : public TDialog
{
 public:
  BOOL bRemake;
  BOOL bAcctList;
  PTSpread List;
  // pointers to the database files.
  PTGLSpec  GLSpec;
  PTResrc   Resrc;
  PTglaccts GLAccount;

  // pointers to the database lists windows.
  PTSCGLSpecList SPECS;
  PTGLResList    RESOURCE;
  PTGLAcctList   GLACCOUNT;


  TGLSpecification (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle)
  {
   List = new TSpread(this,ID_DISTLIST);
  };
  virtual void Add              (RTMessage Msg ) = [ID_FIRST + ID_ADD];
  virtual void Modify           (RTMessage Msg ) = [ID_FIRST + ID_MODIFY];
  virtual void Search           (RTMessage Msg ) = [ID_FIRST + ID_SEARCH];
  virtual void UpdateSearch     (RTMessage Msg ) = [CM_FIRST + ID_ACCOUNTSEARCH];
  virtual void Next             (RTMessage Msg ) = [ID_FIRST + ID_FORWARD];
  virtual void Previous         (RTMessage Msg ) = [ID_FIRST + ID_BACK];
  virtual void Remove           (RTMessage Msg ) = [ID_FIRST + ID_DELETE];
  virtual void SearchResource   (RTMessage Msg ) = [ID_FIRST + ID_RESSEARCH];
  virtual void Cancel           (RTMessage Msg ) = [ID_FIRST + IDCANCEL];
  virtual void UpdateScreen     (RTMessage Msg ) = [CM_FIRST + IDM_UPDATE];
  virtual void AccountList      (RTMessage Msg ) = [ID_FIRST + ID_ACCOUNT];
  virtual void HandleAccounts   (RTMessage Msg ) = [WM_FIRST + SSM_DBLCLK];
  void SetupGLListWindow(void);
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
  virtual void SetupWindow();
};
void TGlWindow::SCSGLSpecification      (RTMessage)
 {GetApplication()->ExecDialog(new TGLSpecification(this,"DGLACCOUNTS"));}

void TGLSpecification::AccountList(RTMessage)
{
 if(bAcctList)
 {
  GLACCOUNT = new TGLAcctList(this,"PICKLIST",GLAccount,"TYPE = 'I'");
  GLACCOUNT->EnableAutoCreate();
  GetApplication()->MakeWindow(GLACCOUNT);
  bAcctList = FALSE;
 }
 if(!IsWindowVisible(GLACCOUNT->HWindow))
   GLACCOUNT->Show(SW_SHOW);
  else
   GLACCOUNT->Show(SW_HIDE);


}
void TGLSpecification::SetupWindow()
{
 HourGlassOn();
 TDialog::SetupWindow();
 Resrc      = new TResrc(&cb);
 GLAccount  = new Tglaccts(&cb);
 GLSpec     = new TGLSpec(&cb);



 SetupGLListWindow();

 SPECS = new TSCGLSpecList(this,"SPECLIST","FPCODE <> ' '",GLSpec);
 SPECS->EnableAutoCreate();
 GetApplication()->MakeWindow(SPECS);

 RESOURCE = new TGLResList(this,
  "PICKLIST","FCOSTTYPE = 'ACT' .AND. FTYPE <> 'R ' .AND. FTYPE <> 'RI'.AND. FTYPE<>'WI' .AND. FTYPE <>'W '",Resrc);
 RESOURCE->EnableAutoCreate();
 GetApplication()->MakeWindow(RESOURCE);

 bAcctList = TRUE;
 ClearScreen();
 HourGlassOff();
}

void TGLSpecification::Remove      (RTMessage Msg)
{
 if(*GetField(ID_RESCODE) == 0)
 {
  BWCCMessageBox(HWindow,"Resource code must be specified.","Resource Code",MB_ICONINFORMATION);
  return;
 }

 char szSeek[23];
 strcpy(szSeek,GetField(ID_RESCODE));
 strcat(szSeek,GetField(ID_ACCOUNTS));
 strcat(szSeek,GetField(ID_DEPART));
 if(BWCCMessageBox(HWindow,"Are you sure you want to remove this specification?","Remove Record",MB_ICONQUESTION | MB_YESNO)==IDYES)
  {
    if(GLSpec->seek(szSeek) == 0)
     {
      GLSpec->deleteRec();
      GLSpec->pack();
      ClearScreen();
      UpdateScreen(Msg);
      bRemake = TRUE;
     }
    else
     {
      BWCCMessageBox(HWindow,"This specification not found in master!","Remove Record",MB_ICONINFORMATION);
      return;
     }
  }
}

void TGLSpecification::Next        (RTMessage Msg)
{
 if(*GetField(ID_RESCODE) == 0)
 {
  GLSpec->bottom();
  SetField(ID_RESCODE, GLSpec->fRescode);
  SetField(ID_ACCOUNTS,GLSpec->fAccount);
  SetField(ID_DEPART,  GLSpec->fDepart);
  SetField(ID_DESCRIPT,GLSpec->fName);
 }
 else
 {
 GLSpec->SetIndex("unique");
 if(GLSpec->seek(GetField(ID_RESCODE))== 0)
  { 
   GLSpec->skip();
   GLSpec->SetIndex("fpcode");
   SetField(ID_RESCODE, GLSpec->fRescode);
   SetField(ID_ACCOUNTS,GLSpec->fAccount);
   SetField(ID_DEPART,  GLSpec->fDepart);
   SetField(ID_DESCRIPT,GLSpec->fName);
  }
  else
  {
  GLSpec->bottom();
  GLSpec->SetIndex("fpcode");  // I can write anything I want here and it won't affect the program.
  }
 }
 UpdateScreen(Msg);
}

void TGLSpecification::Previous        (RTMessage Msg)
{
 if(*GetField(ID_RESCODE) == 0)
 {
  GLSpec->top();
  SetField(ID_RESCODE, GLSpec->fRescode);
  SetField(ID_ACCOUNTS,GLSpec->fAccount);
  SetField(ID_DEPART,  GLSpec->fDepart);
  SetField(ID_DESCRIPT,GLSpec->fName);
 }
 else
 {
 GLSpec->SetIndex("unique");
 if(GLSpec->seek(GetField(ID_RESCODE))== 0)
  { 
   GLSpec->skip(-1);
   GLSpec->SetIndex("fpcode");
   SetField(ID_RESCODE, GLSpec->fRescode);
   SetField(ID_ACCOUNTS,GLSpec->fAccount);
   SetField(ID_DEPART,  GLSpec->fDepart);
   SetField(ID_DESCRIPT,GLSpec->fName);
  }
  else
  {
  GLSpec->bottom();
  GLSpec->SetIndex("fpcode");
  }
 }
 UpdateScreen(Msg);
}



BOOL TGLSpecification::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TGLSpecification::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TGLSpecification::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TGLSpecification::UpdateWindow()
{
 char szSeek[12];
 SetField(ID_ACCOUNTS,  	GLSpec->fAccount);
 SetField(ID_DEPART, 		GLSpec->fDepart);
 strcpy(szSeek,GLSpec->fAccount->str());
 strcat(szSeek,GLSpec->fDepart->str());
 GLAccount->seek(szSeek);
 SetField(ID_DESCRIPT, GLAccount->descript->str());
 SetFocus(GetDlgItem(HWindow,ID_ACCOUNTS));
}

void TGLSpecification::UpdateScreen(RTMessage)
{

 char szSeek[12];
 HWND hwnd = GetDlgItem(HWindow,ID_DISTLIST);
 if(GLSpec->seek(GetField(ID_RESCODE))== 0)
 strcpy(szSeek,GLSpec->fRescode->str());
 else
 {
  BWCCMessageBox(HWindow,"Resource not found. No accounts specified","Update Accounts List",MB_ICONINFORMATION);
  SSClearData(hwnd,SS_ALLCOLS,SS_ALLROWS);
  return;
 }
 int i = 1;
 SSClearData(hwnd,SS_ALLCOLS,SS_ALLROWS);
 SSSetBool(hwnd,SSB_REDRAW,FALSE);
 while(!GLSpec->eof() && strcmp(szSeek,GLSpec->fRescode->str()) == 0)
 { 
  SSSetData(hwnd,1,i,GLSpec->fAccount->str());
  SSSetData(hwnd,2,i,GLSpec->fDepart->str());
  SSSetData(hwnd,3,i,GLSpec->fName->str());  
  GLSpec->skip();
  i++;
 }
 SSSetBool(hwnd,SSB_REDRAW,TRUE);
 if(i>6)
 SSSetMaxRows(hwnd,i);
 else
 SSSetMaxRows(hwnd,6);
 SetField(ID_ACCOUNTS,"");
 SetField(ID_DEPART,"");
 SetField(ID_DESCRIPT,"");
 SetFocus(GetDlgItem(HWindow,ID_ACCOUNTS));
}


void TGLSpecification::ClearScreen()
{
 SetField(ID_ACCOUNTS, "");
 SetField(ID_DEPART,   "");
 SetField(ID_DESCRIPT, "");
 SetField(ID_RESCODE,"");
 SetField(ID_RESNAME,"");
 FloatSetDlgItemValue(HWindow,ID_CURRBAL,0);
 FloatSetDlgItemValue(HWindow,ID_RATE,0);
 SetFocus(GetDlgItem(HWindow,ID_ACCOUNTS));
}

void TGLSpecification::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}

void TGLSpecification::EnterError(int ID)
{
 BWCCMessageBox(HWindow,"All fields must be completed."
                       ,GetString(GetApplication()->hInstance,ID)
                       ,MB_ICONEXCLAMATION);
 return;
} 

void TGLSpecification::Add    (RTMessage Msg)
{
 char szSeek[22];
 char szAccount[12];
 if(*GetField(ID_RESCODE) == 0)
 {
  BWCCMessageBox(HWindow,"Resource code must be specified.","Resource Code",MB_ICONINFORMATION);
  return;
 }
 if(*GetField(ID_ACCOUNTS) == 0)
 {
  BWCCMessageBox(HWindow,"Account code must be specified.","Account Code",MB_ICONINFORMATION);
  return;
 } 
 strcpy(szAccount,GetField(ID_ACCOUNTS));
 strcat(szAccount,GetField(ID_DEPART));
 if(GLAccount->seek(szAccount) != 0)
 {
   BWCCMessageBox(HWindow,"Account code not found.","Account Code",MB_ICONINFORMATION);
  return;
 }

  GLSpec->SetIndex("Accounts");
  if(GLSpec->seek(szAccount) == 0)
   {
   if(strcmp(GLSpec->fRescode->str(),GetField(ID_RESCODE)) != 0)
   {
    char szMsg[55];
    strcpy(szMsg,"Account code is in use by resource : ");
    strcat(szMsg,GLSpec->fRescode->str());
    BWCCMessageBox(HWindow,szMsg,"Account Code",MB_ICONINFORMATION);
    GLSpec->SetIndex("fpcode");
    return;
   }
 }
  GLSpec->SetIndex("fpcode");
  strcpy(szSeek,GetField(ID_RESCODE));
  strcat(szSeek,szAccount);
  if(GLSpec->seek(szSeek) == 0)
  {
   Modify(Msg);
  }
  else
  {
   GLSpec->appendStart(0);
   GLSpec->fRescode->assign(GetField(ID_RESCODE));
   GLSpec->fAccount->assign(GetField(ID_ACCOUNTS) );
   GLSpec->fDepart->assign (GetField(ID_DEPART) );
   GLSpec->fName->assign(GLAccount->descript->str());
   GLSpec->fCosttype->assign("ACT");
   GLSpec->append();
   UpdateScreen(Msg);
   SendMessage(HWindow,WM_COMMAND,ID_ACCOUNTSEARCH,NULL);
  }
}

void TGLSpecification::Modify (RTMessage Msg)
{
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES) {
   GLSpec->fAccount->assign(GetField(ID_ACCOUNTS) );
   GLSpec->fDepart->assign (GetField(ID_DEPART) );
   GLSpec->fRescode->assign(GetField(ID_RESCODE));
   GLSpec->fName->assign(GLAccount->descript->str());
   GLSpec->flush();   
   UpdateScreen(Msg);
   SendMessage(HWindow,WM_COMMAND,ID_ACCOUNTSEARCH,NULL);   
  }
}

void TGLSpecification::UpdateSearch     (RTMessage )
{
 SPECS->List->ClearData(SS_ALLCOLS,SS_ALLROWS);
 long i = 1;
 GLSpec->top();
 while(!GLSpec->eof())
  {
   SPECS->List->SetData(1,i,GLSpec->fRescode->str());
   SPECS->List->SetData(2,i,GLSpec->fAccount->str());
   SPECS->List->SetData(3,i,GLSpec->fDepart->str());
   SPECS->List->SetData(4,i,GLSpec->fName->str());
   i++;
   GLSpec->skip(1);
  }
}

void TGLSpecification::Search           (RTMessage )
{ 
 if(bRemake) {    
  SPECS = new TSCGLSpecList(this,"SPECLIST","ACCOUNT <> ' '",GLSpec);
  SPECS->EnableAutoCreate();
  GetApplication()->MakeWindow(SPECS);
  bRemake = FALSE;
 }

 if(!IsWindowVisible(SPECS->HWindow))
   SPECS->Show(SW_SHOW);
  else
   SPECS->Show(SW_HIDE);
}


void TGLSpecification::SearchResource   (RTMessage )
{
 if(!IsWindowVisible(RESOURCE->HWindow))
   RESOURCE->Show(SW_SHOW);
  else
   RESOURCE->Show(SW_HIDE);
}


void TSCGLSpecList::HandleListBox(RTMessage Msg)
{
 char szSeek[20];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szSeek);
 if(GLSpec->seek(szSeek) == 0)
  {
   //SendMessage(Parent->HWindow,WM_COMMAND,IDM_UPDATE,NULL);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 //Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

void TSCGLSpecList::SetupWindow()
{
 TDialog::SetupWindow();
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);

 GetClientRect(HWindow,&rc);
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));
 MoveWindow(List->HWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
 short d1Width,d2Width,d3Width;

 List->SetColWidth(1,10);
 List->SetColWidth(2,7);
 List->SetColWidth(3,4);
 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 List->GetColWidthInPixels(3,(LPSHORT)&d3Width);

 short ColWidth = (rc.right - rc.left) - d1Width - d2Width - d3Width;
 List->SetColWidthInPixels(4,ColWidth);
 List->SetMaxCols(5);
 if(GLSpec->recCount()>9)
  List->SetMaxRows(GLSpec->recCount());
 else
  List->SetMaxRows(9);

 List->SetData(1,SS_HEADER, "Resource");
 List->SetData(2,SS_HEADER, "Account");
 List->SetData(3,SS_HEADER, "Dept");
 List->SetData(4,SS_HEADER, "Specification Desciption");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 long i = 1; 
 GLSpec->top();
 while(!GLSpec->eof())
  {
   List->SetData(1,i,GLSpec->fRescode->str());
   List->SetData(2,i,GLSpec->fAccount->str());
   List->SetData(3,i,GLSpec->fDepart->str());
   List->SetData(4,i,GLSpec->fName->str());   
   i++;
   GLSpec->skip(1);
  }
 SetCaption("Standard Cost General Ledger Specification");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;

}

void TGLSpecification::HandleAccounts(RTMessage Msg)
{
 char szAccount[8];
 char szDepart[5];
 char szSeek[22];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szAccount);
 List->GetData(2,Cell->Row,szDepart);
 if(*GetField(ID_RESCODE) ==0)
 return;
 strcpy(szSeek,GetField(ID_RESCODE));
 strcat(szSeek,szAccount);
 strcat(szSeek,szDepart);
 if(GLSpec->seek(szSeek) == 0)
  {
   SetField(ID_ACCOUNTS, GLSpec->fAccount);
   SetField(ID_DEPART,   GLSpec->fDepart);
   SetField(ID_DESCRIPT, GLSpec->fName);
  }
  else
  BWCCMessageBox(HWindow,"Resource specification could not be found.","E R R O R",MB_ICONEXCLAMATION);
 SetFocus(Parent->HWindow);
}


void TGLSpecification::SetupGLListWindow(void)
{ 
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);
 HWND hwnd = GetDlgItem(HWindow,ID_DISTLIST);

 GetClientRect(hwnd,&rc); 
 short d1Width,d2Width;
 SSSetMaxCols(hwnd,3);

 SSSetColWidth(hwnd,1,7);
 SSSetColWidth(hwnd,2,4);
 SSGetColWidthInPixels(hwnd,1,(LPSHORT)&d1Width);
 SSGetColWidthInPixels(hwnd,2,(LPSHORT)&d2Width);
 short ColWidth = (rc.right - rc.left) - d1Width - d2Width - GetSystemMetrics(SM_CXHSCROLL) + 2 ;
 SSSetColWidthInPixels(hwnd,3,ColWidth);
 
 SSSetData(hwnd,1,SS_HEADER, "Account");
 SSSetData(hwnd,2,SS_HEADER, "Dept");
 SSSetData(hwnd,3,SS_HEADER, "Description");

 SSSetBool(hwnd,SSB_MOVEACTIVEONFOCUS, FALSE);
 SSSetTypeStaticText(hwnd,&CellType,SS_TEXT_LEFT);
 SSSetCellType(hwnd,SS_ALLCOLS, SS_ALLROWS, &CellType);
 SSSetMaxRows(hwnd,10);
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}