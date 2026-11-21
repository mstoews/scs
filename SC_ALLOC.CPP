#include "scs.h"
// Typess maintainance
#include "db_alloc.h"
#include "db_centr.h"

extern Code4 cb;
_CLASSDEF(TDAlloc)
class TDAlloc : public TDialog
{
 public:
  PTSCAllocList  ALLOCATE;
  PTGLAcctList   GLACCOUNT;
  PTSCCentreList CENTRE;
  PTglaccts      glaccts;
  PTCentre       centre;  
  PTAlloc   	 alloc;
  BOOL           bRemake;
  TDAlloc (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow();
  virtual void Add         (RTMessage Msg) = [ID_FIRST + ID_ADD];
  virtual void Allocation  (RTMessage Msg) = [ID_FIRST + ID_ALLOCSEARCH];
  virtual void Centre      (RTMessage Msg) = [ID_FIRST + ID_CENTRESEARCH];
  virtual void GLAccount   (RTMessage Msg) = [ID_FIRST + ID_GLSEARCH];
  virtual void Top         (RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom      (RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Forward     (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
  virtual void Back        (RTMessage Msg) = [ID_FIRST + ID_BACK];
  virtual void Delete      (RTMessage Msg) = [ID_FIRST + ID_DELETE];
  virtual void Cancel      (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Modify      (RTMessage Msg) = [ID_FIRST + ID_MODIFY];
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
};

void TGlWindow::SCSDataAllocation (RTMessage)
 {GetApplication()->ExecDialog(new TDAlloc(this,"DALLOC"));}

void TDAlloc::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 cb.optimize = -1;
 alloc = new TAlloc(&cb);
 glaccts = new Tglaccts(&cb);
 centre  = new TCentre(&cb);
 centre->SetIndex("fcode1");

 ALLOCATE = new TSCAllocList(this,"PICKLIST","FCODE <> ' '",alloc);
 ALLOCATE->EnableAutoCreate();
 GetApplication()->MakeWindow(ALLOCATE);
 bRemake = FALSE;

 GLACCOUNT = new TGLAcctList(this,"PICKLIST",glaccts,"ACCOUNT <> ' '");
 GLACCOUNT->EnableAutoCreate();
 GetApplication()->MakeWindow(GLACCOUNT);

 CENTRE = new TSCCentreList(this,"PICKLIST","FCODE <> ' '",centre);
 CENTRE->EnableAutoCreate();
 GetApplication()->MakeWindow(CENTRE);

 HourGlassOff();
}

#pragma argsused
void TDAlloc::EnterError(int ID)
{
 BWCCMessageBox(HWindow,"All fields must be completed.","E R R O R",MB_ICONEXCLAMATION);
 return;
}



BOOL TDAlloc::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDAlloc::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TDAlloc::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TDAlloc::UpdateWindow()
{
 SetField(ID_FCODE,     		alloc->fCode);
 SetField(ID_FULLN, 		alloc->fName);
 SetField(ID_NAME,      	alloc->fShortName);
 SetField(ID_USERUPDATE,  	alloc->fUser);
 SetField(ID_DATEUPDATED,       alloc->fDate);
 SetField(ID_RATE,		alloc->fRate);
 SetField(ID_CENTRE,		alloc->fCentre);
 SetField(ID_ACCOUNTS,		alloc->fGlacct);
 SetField(ID_DEPART,		alloc->fGldepart);
 SetFocus(GetDlgItem(HWindow,ID_FCODE));
}

void TDAlloc::ClearScreen()
{
 SetField(ID_FCODE,    		"");
 SetField(ID_NAME,    		"");
 SetField(ID_FULLN,     	"");
 SetField(ID_USERUPDATE,  	"");
 SetField(ID_RATE,		"");
 SetField(ID_CODE,		"");
 SetField(ID_ACCOUNTS,		"");
 SetField(ID_DEPART,		"");
 SetFocus(GetDlgItem(HWindow,ID_FCODE));
}


void TDAlloc::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}

void TDAlloc::Allocation(RTMessage)
{
if(bRemake)
{
 delete ALLOCATE;
 ALLOCATE = new TSCAllocList(this,"PICKLIST","FCODE <> ' '",alloc);
 ALLOCATE->EnableAutoCreate();
 GetApplication()->MakeWindow(ALLOCATE);
 bRemake = FALSE;
}
if(!IsWindowVisible(ALLOCATE->HWindow))
   ALLOCATE->Show(SW_SHOW);
  else
   ALLOCATE->Show(SW_HIDE);
}

void TDAlloc::Centre(RTMessage)
{
if(!IsWindowVisible(CENTRE->HWindow))
   CENTRE->Show(SW_SHOW);
  else
   CENTRE->Show(SW_HIDE);
}

void TDAlloc::GLAccount(RTMessage)
{
if(!IsWindowVisible(GLACCOUNT->HWindow))
   GLACCOUNT->Show(SW_SHOW);
  else
   GLACCOUNT->Show(SW_HIDE);
}


void TDAlloc::Top(RTMessage)
{
 alloc->top();
 UpdateWindow();
}

void TDAlloc::Forward(RTMessage)
{
 alloc->skip(1);
 if(alloc->eof())
 alloc->bottom();
 UpdateWindow();
}

void TDAlloc::Back(RTMessage)
{
 alloc->skip(-1);
 if(alloc->bof())
 alloc->top();
 UpdateWindow();
}


void TDAlloc::Bottom(RTMessage)
{
 alloc->bottom();
 UpdateWindow();
}

void TDAlloc::Delete(RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to delete this record?","Delete Record",MB_ICONQUESTION | MB_YESNO) == IDYES) {
  alloc->deleteRec();
  alloc->pack();
  UpdateWindow();
 }
}

void TDAlloc::Add (RTMessage Msg)
{
 if(*GetField(ID_FCODE) == 0){
  EnterError(ID_FCODE);
  SetFocus(GetDlgItem(HWindow,ID_FCODE));
  return;
 }
 if(alloc->seek(GetField(ID_FCODE)) !=0)
 {
   if(*GetField(ID_FCODE)  == 0 ){ EnterError(ID_FCODE); return; }
   if(*GetField(ID_NAME)  == 0 ){ EnterError(ID_NAME); return; }
   if(*GetField(ID_FULLN) == 0 ){ EnterError(ID_FULLN); return; }

   alloc->appendStart(0);
   alloc->fCode->assign    	(GetField(ID_FCODE) );
   alloc->fName->assign    	(GetField(ID_FULLN) );
   alloc->fShortName->assign    (GetField(ID_NAME));
   alloc->fUser->assign    	(GetField(ID_USER) );
   alloc->fDate->assign    	(GetField(ID_DATE) );
   alloc->fRate->assign    	(GetField(ID_RATE) );
   alloc->fCentre->assign    	(GetField(ID_CENTRE) );
   alloc->fGlacct->assign    	(GetField(ID_ACCOUNTS) );
   alloc->fGldepart->assign    	(GetField(ID_DEPART) );
   alloc->append();
   ClearScreen();
   bRemake = TRUE;
 }
 else
 Modify(Msg);
}
void TDAlloc::Modify (RTMessage)
{
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES) {
   alloc->fCode->assign     	(GetField(ID_FCODE) );
   alloc->fName->assign      	(GetField(ID_FULLN) );
   alloc->fShortName->assign 	(GetField(ID_NAME));
   alloc->fUser->assign      	(GetField(ID_USER) );
   alloc->fDate->assign      	(GetField(ID_DATE) );
   alloc->fRate->assign    	(GetField(ID_RATE) );
   alloc->fCentre->assign    	(GetField(ID_CENTRE) );
   alloc->fGlacct->assign    	(GetField(ID_ACCOUNTS) );
   alloc->fGldepart->assign    	(GetField(ID_DEPART) );
   alloc->flush();
   ClearScreen();
   bRemake = TRUE;
  }
}

void TSCAllocList::SetupWindow ()
{
 TDialog::SetupWindow();
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);

 GetClientRect(HWindow,&rc);
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));
 MoveWindow(List->HWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
 short d1Width,d2Width;

 List->SetColWidth(1,7);
 List->SetColWidth(2,10);
 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 short ColWidth = (rc.right - rc.left) - d1Width - d2Width;// - GetSystemMetrics(SM_CXHSCROLL);
 List->SetColWidthInPixels(3,ColWidth);

 List->SetMaxCols( 3);
 List->SetMaxRows(alloc->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 alloc->SetIndex("fcode");
 alloc->top();
 long i = 1;
 rQuery =  new Relate4set(alloc);
 rQuery->querySet(szQuery);
 while(!alloc->eof())
  {
   List->SetData(1,i,alloc->fCode->str());
   List->SetData(2,i,alloc->fShortName->str());
   List->SetData(3,i,alloc->fName->str());
   i++;
   alloc->skip(1);
  }

 rQuery->unlock();
 rQuery->free(0);
 List->SetMaxRows(i);
 SetCaption("Standard Cost Allocation List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCAllocList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(alloc->seek(szItem) == 0)
  {
    SendMessage(GetDlgItem(Parent->HWindow,ID_FCODE)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fCode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_NAME)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fShortName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_FULLN)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_USER)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fUser->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATE)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fDate->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RATE)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fRate->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_CENTRE)    ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fCentre->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_ACCOUNTS)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fGlacct->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DEPART)    ,WM_SETTEXT,NULL,(LONG)(LPSTR)alloc->fGldepart->str());
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

