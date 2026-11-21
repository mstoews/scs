
// Centre maintainance
#include "scs.h"
// Databases
#include "db_centr.h"
#include "db_koute.h"
#include "db_resrc.h"
#include "gl_dlg.h"
extern Code4 cb;

char szKoutei[11];

_CLASSDEF(TDProcess)
class TDProcess : public TDialog
{
 public:
  BOOL bKoutei;
  BOOL bUpdateCentres;
  PTSpread List;
  PTSCCentreList  CENTRE;
  PTKouteiList    PROCESS;
  PTProc     Proc;
  PTCentre   centre;
  PTKoutei   Koutei;
  PTResrc    Resrc;
  TDProcess (PTWindowsObject AParent, LPSTR ATitle,PTCentre c,PTKoutei k,PTProc p):TDialog(AParent,ATitle)
  {
  centre = c;
  Koutei = k;
  Proc   = p;
  List = new TSpread(this,ID_DISTLIST);
  }
  virtual void SetupWindow();
  virtual void Add        (RTMessage Msg) = [ID_FIRST + ID_ADD];
  virtual void Search     (RTMessage Msg) = [ID_FIRST + ID_ITEMSEARCH];
  virtual void Top        (RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom     (RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Forward    (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
  virtual void Back       (RTMessage Msg) = [ID_FIRST + ID_BACK];
  virtual void Delete     (RTMessage Msg) = [ID_FIRST + ID_DELETE];
  virtual void Cancel     (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Modify     (RTMessage Msg) = [ID_FIRST + ID_MODIFY];
  virtual void Process    (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
  virtual void GetProcess (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  virtual void SetProcess (RTMessage Msg) = [CM_FIRST + CM_SETRESOURCE];
  virtual void SetListTable(void);
  virtual void SetupCosts(void);
  virtual void UpdateWindow(void);
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  double dGetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
};

_CLASSDEF(TDCentre)
class TDCentre : public TMainDialog
{
 public:
 BOOL bKoutei;
 BOOL bUpdateCentres;
 PTSCCentreList  CENTRE;
 PTProc     Proc;
 PTCentre   centre;
 PTKoutei   Koutei;
 PTResrc    Resrc;
 TDCentre (PTWindowsObject AParent, LPSTR ATitle):TMainDialog(AParent,ATitle){};

 virtual void Add         (RTMessage Msg) = [ID_FIRST + ID_ADD];
 virtual void Modify      (RTMessage Msg) = [ID_FIRST + ID_MODIFY]; 
 virtual void Forward     (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
 virtual void Back        (RTMessage Msg) = [ID_FIRST + ID_BACK];
 virtual void Delete      (RTMessage Msg) = [ID_FIRST + ID_DELETE];
 virtual void AddProcess  (RTMessage Msg) = [ID_FIRST + ID_ADDPROCESS];
 virtual void Search      (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
 virtual void UpdateWindow();
 virtual void SetWindow(RTMessage Msg) = [CM_FIRST + CM_SETRESOURCE]{UpdateWindow();};
 virtual void ClearScreen(void);
 virtual void SetupWindow();
};

void TDCentre::Search (RTMessage)
{
 if(bUpdateCentres)
 {
   centre->SetIndex("fcode1");
   CENTRE = new TSCCentreList(this,"PICKLIST","FCODE <> ' '",centre);
   CENTRE->EnableAutoCreate();
   GetApplication()->MakeWindow(CENTRE);
   bUpdateCentres = FALSE;
 }
 if(!IsWindowVisible(CENTRE->HWindow))
   CENTRE->Show(SW_SHOW);
 else
   CENTRE->Show(SW_HIDE);

}

void TDCentre::Add         (RTMessage Msg)
{
 if(*GetField(ID_CODE) == 0)
  {
   BWCCMessageBox(HWindow,"Enter a Cost Centre Code","Cost Centre",MB_ICONINFORMATION);
   return;
  }

 if(centre->seek(GetField(ID_CODE)) !=0)
 { 
   centre->appendStart(0);
   centre->fCode->assign       (GetField(ID_CODE) );
   centre->fName->assign       (GetField(ID_DESCRIPT));
   centre->fShortName->assign  (GetField(ID_NAME));
   centre->fUser->assign       (GetField(ID_USERUPDATE) );
   centre->fDate->assign       (GetField(ID_DATEUPDATED) );
   centre->append();
   ClearScreen();
   delete CENTRE;
   bUpdateCentres = TRUE;
 }
 else
 Modify(Msg);

};

void TDCentre::AddProcess   (RTMessage )
{
 if(*GetField(ID_CODE) == 0)
 return;
 if(centre->seek(GetField(ID_CODE)) != 0)
 {
  char szMsg[150];
  strcpy(szMsg,"Unable to locate the cost centre code : ");
  strcat(szMsg,GetField(ID_CODE));
  BWCCMessageBox(HWindow,szMsg,GetString(ID_WARNING),MB_ICONINFORMATION);
  return;
 }

 CENTRE->Show(SW_HIDE);
 GetApplication()->ExecDialog(new TDProcess(this,"DPROCESSADD",centre,Koutei,Proc)); 
 CENTRE->Show(SW_SHOW);

}

void TDCentre::Modify       (RTMessage )
{
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES)
  {
   centre->fCode->assign       (GetField(ID_CODE) );
   centre->fName->assign       (GetField(ID_DESCRIPT));
   centre->fShortName->assign  (GetField(ID_NAME));
   centre->fUser->assign       (GetField(ID_USERUPDATE) );
   centre->fDate->assign       (GetField(ID_DATEUPDATED) );
   centre->flush();
   ClearScreen();
   delete CENTRE;
   bUpdateCentres = TRUE;
  }

};

void TDCentre::Forward     (RTMessage )
{
 if(centre->seek(GetField(ID_CODE)) == 0)
 {
  if(!centre->eof())
   centre->skip();
 }
 UpdateWindow();
};

void TDCentre::Back        (RTMessage )
{
if(centre->seek(GetField(ID_CODE)) == 0)
 {
  if(!centre->bof())
   centre->skip(-1);
 }
 UpdateWindow();
};

void TDCentre::Delete      (RTMessage )
{
 if(DlgMsg(ID_DELETE,ID_WARNING,MB_ICONQUESTION | MB_YESNO) == IDYES)
 {
   centre->SetIndex("fcode");
   char szCode[10];
   strcpy(szCode,GetField(ID_CODE));
   if(centre->seek(szCode) == 0)
   {
     centre->deleteRec();
     centre->pack();
     delete CENTRE;
     bUpdateCentres = TRUE;
   }
   char szCompare[10];
   Proc->SetIndex("Fcode");
   if(Proc->seek(szCode) == 0)
   {
    strcpy(szCompare,szCode);
    while(strcmp(szCompare,szCode) == 0)
    {
     Proc->deleteRec();
     Proc->skip();
     strcpy(szCompare,Proc->fCode->str());
    }
    Proc->pack();
  }
    ClearScreen();
 }
};


void TDCentre::UpdateWindow()
{
 SetField(ID_CODE,     		centre->fCode);
 SetField(ID_DESCRIPT, 		centre->fName);
 SetField(ID_NAME,      	centre->fShortName);
 SetField(ID_USERUPDATE,  	szUser);
 SetField(ID_DATEUPDATED,       centre->fDate);
 SetFocus(GetDlgItem(HWindow,ID_CODE));
};


void TDCentre::ClearScreen(void)
{
 SetField(ID_CODE,     		"");
 SetField(ID_NAME,    		"");
 SetField(ID_DESCRIPT,     	"");
 SetFocus(GetDlgItem(HWindow,ID_CODE));
};

void TDCentre::SetupWindow()
{
 TDialog::SetupWindow();
 bKoutei = TRUE;
 bUpdateCentres = FALSE;

 HourGlassOn();
 cb.optimize = -1;
 centre = new TCentre(&cb);
 Koutei = new TKoutei(&cb);
 Proc   = new TProc(&cb);

 SendDlgItemMsg(ID_USERUPDATE,WM_SETTEXT,NULL,(LPARAM)(LPSTR)szUser);

 centre->SetIndex("fcode1");
 CENTRE = new TSCCentreList(this,"PICKLIST","FCODE <> ' '",centre);
 CENTRE->EnableAutoCreate();
 GetApplication()->MakeWindow(CENTRE);
 HourGlassOff();
};



void TDProcess::SetupCosts(void)
{
 Relate4set *rQuery;
 rQuery = new Relate4set(Resrc);
 rQuery->querySet("FCOSTTYPE = 'STD'");
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
 {
  if(strcmp(Resrc->fType->str(),"L ")==0 || strcmp(Resrc->fType->str(),"L")==0 )
   {
    SendDlgItemMsg(ID_LABOUR_U,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
    SendDlgItemMsg(ID_LABOUR_U,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
   }
  if(strcmp(Resrc->fType->str(),"O ")==0 || strcmp(Resrc->fType->str(),"D ")==0 )
   {
    SendDlgItemMsg(ID_OVERHEAD_U,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
    SendDlgItemMsg(ID_OVERHEAD_U,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
   }
  if(strcmp(Resrc->fType->str(),"I ")==0 || strcmp(Resrc->fType->str(),"I")==0 )
   {
    SendDlgItemMsg(ID_INDIRECT_U,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
    SendDlgItemMsg(ID_INDIRECT_U,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
   }
 }
 rQuery->unlock();
 rQuery->free();
}



void TDProcess::GetProcess (RTMessage Msg)
{
 char szKoutei[10];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szKoutei);
 if(Koutei->seek(szKoutei) == 0)
  {
   SetField(ID_KOUTEI,Koutei->fKoutei);
   SetField(ID_KNAME, Koutei->fFullnm);
  }

 return;
} 


void TDProcess::SetProcess(RTMessage)
{ 
 int i = 1;
 char szCompare[11];
 char szCode[11];
 Proc->SetIndex("Fcode");
 List->ClearData(SS_ALLCOLS,SS_ALLROWS);
 
 strcpy(szCode,GetField(ID_CODE));
 strcpy(szCompare,szCode);
 StrTrim(szCompare);
 StrTrim(szCode);
 if(Proc->seek(szCode) == 0)
 {
  while(strcmp(szCompare,szCode) == 0)
  {
    
   List->SetData(1,i,Proc->fKoutei->str());
   if(Koutei->seek(Proc->fKoutei->str()) == 0)
   {
    List->SetData(2,i,Koutei->fFullnm->str());
   }
   List->SetData(3,i,Proc->fLabour->str());
   List->SetData(4,i,Proc->fOh->str());
   List->SetData(5,i,Proc->fIDoh->str());
   i++;
   Proc->skip();
   strcpy(szCompare,Proc->fCode->str());
   StrTrim(szCompare);
  }
 }
}


void TGlWindow::SCSDataCentres    (RTMessage)
 {GetApplication()->ExecDialog(new TDCentre(this,"DCENTRE"));}

void TDProcess::SetupWindow()
{
 TDialog::SetupWindow();
 bKoutei = TRUE;
 bUpdateCentres = FALSE;
 HourGlassOn();
 Resrc  = new TResrc(&cb);
 UpdateWindow();
 SetupCosts();
 SetListTable();
 SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
 HourGlassOff();
}


#pragma argsused
void TDProcess::EnterError(int ID)
{
 char *szMsg = new char[(int)150];
 char *szField = new char[(int)20]; 
 HINSTANCE hInst = GetApplication()->hInstance;
 strcpy(szField,GetString(hInst,ID));
 wsprintf(szMsg,GetString(hInst,ID_BLANKFIELD),szField);
 BWCCMessageBox(HWindow,szMsg,GetString(hInst,ID_ERROR),MB_ICONEXCLAMATION);
 delete szMsg,szField;
 return;
}



BOOL TDProcess::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDProcess::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      

double TDProcess::dGetField(int ID)
{
 double dValue;
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}


char *TDProcess::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TDProcess::UpdateWindow(void)
{
 SetField(ID_CODE,     		centre->fCode);
 SetField(ID_JDESCRIPT, 	centre->fName);
 SetField(ID_NAME,      	centre->fShortName);
 SetField(ID_USERUPDATE,  	centre->fUser);
 SetField(ID_DATEUPDATED,       centre->fDate);
 SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
 SetFocus(GetDlgItem(HWindow,ID_CODE));
}

void TDProcess::ClearScreen()
{
 SetField(ID_KOUTEI,"");
 SetField(ID_KNAME,"");
 SetFocus(GetDlgItem(HWindow,ID_CODE));
}


void TDProcess::Cancel (RTMessage)
{
 ShutDownWindow();
 Resrc->close();
}



void TDProcess::Process (RTMessage)
{

if(bKoutei)
 {
  PROCESS = new TKouteiList(this,"PICKLIST","FKOUTEI <> ' '",Koutei);
  PROCESS->EnableAutoCreate();
  GetApplication()->MakeWindow(PROCESS);
  bKoutei = FALSE;
 }

if(!IsWindowVisible(PROCESS->HWindow))
   PROCESS->Show(SW_SHOW);
  else
   PROCESS->Show(SW_HIDE);
}


void TDProcess::Search(RTMessage)
{
if(bUpdateCentres)
{
   centre->SetIndex("fcode1");
   CENTRE = new TSCCentreList(this,"PICKLIST","FCODE <> ' '",centre);
   CENTRE->EnableAutoCreate();
   GetApplication()->MakeWindow(CENTRE);
   centre->SetIndex("fcode");
}
if(!IsWindowVisible(CENTRE->HWindow))
   CENTRE->Show(SW_SHOW);
  else
   CENTRE->Show(SW_HIDE);
}

void TDProcess::Top(RTMessage)
{
 centre->top();
 UpdateWindow();
}

void TDProcess::Forward(RTMessage)
{
 centre->skip(1);
 if(centre->eof())
 centre->bottom();
 UpdateWindow();
}

void TDProcess::Back(RTMessage)
{
 centre->skip(-1);
 if(centre->bof())
 centre->top();
 UpdateWindow();
}


void TDProcess::Bottom(RTMessage)
{
 centre->bottom();
 UpdateWindow();
}

void TDProcess::Delete(RTMessage)
{
 if(*GetField(ID_KOUTEI) == 0)
 return;
 char szSeek[20];
 strcpy(szSeek,GetField(ID_CODE));
 strcat(szSeek,GetField(ID_KOUTEI));
 if(BWCCMessageBox(HWindow,"Are you sure you want to delete this record?","Delete Record",MB_ICONQUESTION | MB_YESNO) == IDYES)
 {
  if(Proc->seek(szSeek) == 0)
   {
    Proc->deleteRec();
    Proc->pack();
    UpdateWindow();
   }
 }
}


void TDProcess::Add (RTMessage Msg)
{
 
 if(*GetField(ID_CODE) == 0)
 return;
 if(*GetField(ID_KOUTEI) == 0)
 return;
 Proc->SetIndex("fkoutei");
 if(Proc->seek(GetField(ID_KOUTEI)) == 0)
 {
  if(strcmp(GetField(ID_CODE),Proc->fCode->str()) != 0)
   {
   char szMsg[100];
   strcpy(szMsg,"The process :");
   strcat(szMsg,GetField(ID_KOUTEI));
   strcat(szMsg," has be used already in cost centre : ");
   strcat(szMsg,Proc->fCode->str());
   BWCCMessageBox(HWindow,szMsg,"Add Process",MB_ICONINFORMATION);
   return;
   }
 }
 char szSeek[20];
 strcpy(szSeek,GetField(ID_CODE));
 strcat(szSeek,GetField(ID_KOUTEI));
 Proc->SetIndex("fcode1");
 if(Proc->seek(szSeek)!=0)
 {   
   Proc->appendStart(0);
   Proc->fCode->assign       (GetField(ID_CODE) );
   Proc->fKoutei->assign     (GetField(ID_KOUTEI));
   Proc->fKname->assign      (GetField(ID_KNAME));
   Proc->fLabour->assign     (GetField(ID_LABOUR_U));
   Proc->fOh->assign         (GetField(ID_OVERHEAD_U));
   Proc->fIDoh->assign       (GetField(ID_INDIRECT_U));
   Proc->append();   
   ClearScreen();
   SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
 }
 else
 Modify(Msg);
}


void TDProcess::Modify (RTMessage)
{
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES)
  {
   Proc->fCode->assign       (GetField(ID_CODE) );
   Proc->fKoutei->assign     (GetField(ID_KOUTEI));
   Proc->fKname->assign      (GetField(ID_KNAME));
   Proc->fLabour->assign     (GetField(ID_LABOUR_U));
   Proc->fOh->assign         (GetField(ID_OVERHEAD_U));
   Proc->fIDoh->assign       (GetField(ID_INDIRECT_U));
   Proc->flush();
   ClearScreen();
   SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
  }
}

void TSCCentreList::SetupWindow ()
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
 List->SetMaxRows(centre->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 centre->top();
 long i = 1;
 rQuery = new Relate4set(centre);
 rQuery->querySet(szQuery); 
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   List->SetData(1,i,centre->fCode->str());
   List->SetData(2,i,centre->fShortName->str());
   List->SetData(3,i,centre->fName->str());
   i++;
  }

 rQuery->unlock();
 rQuery->free(0);
 if(i < 20)
 i = 20;
 List->SetMaxRows(i);
 SetCaption("Standard Cost Centre List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCCentreList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(centre->seek(szItem) == 0)
  {
   SendMessage(Parent->HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 //Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

void TKouteiList::SetupWindow ()
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
 List->SetMaxRows(Koutei->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 
 Koutei->SetIndex("FKOUTEI");

 long i = 1;
 rQuery = new Relate4set(Koutei);
 rQuery->querySet(szQuery);

 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   List->SetData(1,i,Koutei->fKoutei->str());
   List->SetData(2,i,Koutei->fKounm->str());
   List->SetData(3,i,Koutei->fFullnm->str());
   i++;
  }

 rQuery->unlock();
 rQuery->free(0);
 List->SetMaxRows(i);
 SetCaption("TOPICS Process List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TKouteiList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 char szIdcode[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 SendMessage(GetDlgItem(Parent->HWindow,ID_CODE),WM_GETTEXT,sizeof(szIdcode),(LPARAM)(LPSTR)szIdcode);
 if(szIdcode == 0)
 return;
 if(Koutei->seek(szItem) == 0)
  {   
   strcpy(szKoutei,szItem);
   SetField(ID_KOUTEI, Koutei->fKoutei);
   SetField(ID_KNAME,  Koutei->fFullnm);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 //Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

void TKouteiList::SetField (int ID, Field4 *field)
{
 SendMessage(GetDlgItem(Parent->HWindow,ID),WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
}


void TDProcess::SetListTable ()
{
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);

 short  d1Width,d3Width,d4Width,d5Width;
 List->SetMaxCols(5);
 List->GetClientRect(&rc);
 List->SetColWidth(1,5);
 List->SetColWidth(3,10);
 List->SetColWidth(4,10);
 List->SetColWidth(5,10);

 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(3,(LPSHORT)&d3Width);
 List->GetColWidthInPixels(4,(LPSHORT)&d4Width);
 List->GetColWidthInPixels(5,(LPSHORT)&d5Width);

 short ColWidth = (rc.right - rc.left) - d1Width
						 - d3Width
						 - d4Width
						 - d5Width;

 List->SetColWidthInPixels(2,ColWidth);
 List->SetData(1,SS_HEADER, "Proc");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Labour");
 List->SetData(4,SS_HEADER, "Overhead");
 List->SetData(5,SS_HEADER, "Indirect");

 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
 }