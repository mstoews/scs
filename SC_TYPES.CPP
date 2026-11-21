// Typess maintainance
#include "scs.h"
#include "db_types.h"
#define  ID_SETACTUAL  1001

extern Code4 cb;

_CLASSDEF(TDTypes)
class TDTypes : public TDialog
{
 public:
  PTSCTypesList  ELEMENT;
  PTTypes   typem;
  TDTypes (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
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
  virtual void Clear      (RTMessage Msg) = [ID_FIRST + ID_NEW];
  virtual BOOL CheckActual ();
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
};

void TDTypes::Clear(RTMessage)
{
 SetField(ID_CODE,     		"");
 SetField(ID_DESCRIPT, 		"");
 SetField(ID_NAME,      	"");
 SetFocus(GetDlgItem(HWindow,ID_CODE));
}



BOOL TDTypes::CheckActual()
{
 for(typem->top();!typem->eof();typem->skip())
  {
    if(strcmp(typem->fActual->str(),"T") == 0)
    return TRUE;
  }
 return FALSE;
}



void TGlWindow::SCSDataTypes   (RTMessage)
 {GetApplication()->ExecDialog(new TDTypes(this,"DTYPE"));}

#pragma argsused
void TDTypes::EnterError(int ID)
{

 BWCCMessageBox(HWindow,"All fields must be completed.","E R R O R",MB_ICONEXCLAMATION);
 return;
}



BOOL TDTypes::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDTypes::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TDTypes::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TDTypes::UpdateWindow()
{
 SetField(ID_CODE,     		typem->fCode);
 SetField(ID_DESCRIPT, 		typem->fName);
 SetField(ID_NAME,      	typem->fShortName);
 SetField(ID_USERUPDATE,  	typem->fUser);
 SetField(ID_DATEUPDATED,       typem->fDate);
 if(strcpy(typem->fActual->str(),"T") == 0)
    SendDlgItemMsg(ID_ACTUAL,BM_SETCHECK,TRUE,NULL);
 else
    SendDlgItemMsg(ID_ACTUAL,BM_SETCHECK,FALSE,NULL);

 SetFocus(GetDlgItem(HWindow,ID_CODE));
}

void TDTypes::ClearScreen()
{
 SetField(ID_CODE,     		"");
 SetField(ID_NAME,    		"");
 SetField(ID_DESCRIPT,     	"");
 SetField(ID_USERUPDATE,  	"");
 SendDlgItemMsg(ID_ACTUAL,BM_SETCHECK,FALSE,NULL);
 SetFocus(GetDlgItem(HWindow,ID_RESCODE));
}


void TDTypes::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}


void TDTypes::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 cb.optimize = -1;
 typem = new TTypes(&cb);

 ELEMENT = new TSCTypesList(this,"PICKLIST","FCODE <> ' '",typem);
 ELEMENT->EnableAutoCreate();
 GetApplication()->MakeWindow(ELEMENT);
 SetField(ID_USER,szUser);
 HourGlassOff();
}

void TDTypes::Search(RTMessage)
{
if(!IsWindowVisible(ELEMENT->HWindow))
   ELEMENT->Show(SW_SHOW);
  else
   ELEMENT->Show(SW_HIDE);
}

void TDTypes::Top(RTMessage)
{
 typem->top();
 UpdateWindow();
}

void TDTypes::Forward(RTMessage)
{
 typem->skip(1);
 if(typem->eof())
 typem->bottom();
 UpdateWindow();
}

void TDTypes::Back(RTMessage)
{
 typem->skip(-1);
 if(typem->bof())
 typem->top();
 UpdateWindow();
}


void TDTypes::Bottom(RTMessage)
{
 typem->bottom();
 UpdateWindow();
}

void TDTypes::Delete(RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to delete this record?","Delete Record",MB_ICONQUESTION | MB_YESNO) == IDYES) {
  if(strcmp(GetField(ID_CODE),"STD") == 0 || strcmp(GetField(ID_CODE),"ACT") == 0)
  {
   BWCCMessageBox(HWindow,"Standard or Actual cost types may not be deleted.","Delete Type",MB_ICONINFORMATION);
   return;
  }
  typem->deleteRec();
  typem->pack();
  UpdateWindow();
 }
}

void TDTypes::Add (RTMessage Msg)
{
 char szActual[2];
 if(*GetField(ID_CODE) == 0){
  SetFocus(GetDlgItem(HWindow,ID_CODE));
  return;
 }
 if(typem->seek(GetField(ID_CODE)) !=0)
 {   
   if(BWCCMessageBox(HWindow,"Creating a new cost type based upon the standard cost composition master.","New Cost Type",MB_YESNO | MB_ICONQUESTION) == IDYES)
   {
    if(*GetField(ID_CODE)  == 0 ){ EnterError(ID_CODE); return; }
    if(*GetField(ID_NAME)  == 0 ){ EnterError(ID_NAME); return; }
    if(*GetField(ID_DESCRIPT) == 0 ){ EnterError(ID_DESCRIPT); return; }
    typem->appendStart(0);
    typem->fCode->assign    	(GetField(ID_CODE) );
    typem->fName->assign    	(GetField(ID_DESCRIPT) );
    typem->fShortName->assign  (GetField(ID_NAME));
    typem->fUser->assign    	(GetField(ID_USER) );
    if(SendDlgItemMsg(ID_ACTUAL,BM_GETCHECK,NULL,NULL))
     { 
      if(!CheckActual())
       strcpy(szActual,"T");
     }      
    else
    strcpy(szActual,"S");
    typem->fActual->assign(szActual);
    typem->append();
    ClearScreen();
    delete ELEMENT;
    ELEMENT = new TSCTypesList(this,"PICKLIST","FCODE <> ' '",typem);
    ELEMENT->EnableAutoCreate();
    GetApplication()->MakeWindow(ELEMENT);
   }
   else
   return;
 }
 else
 Modify(Msg);
}
void TDTypes::Modify (RTMessage)
{
  char szActual[2];

  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES)
  {
   typem->fCode->assign      (GetField(ID_CODE) );
   typem->fName->assign      (GetField(ID_DESCRIPT) );
   typem->fShortName->assign (GetField(ID_NAME));
   typem->fUser->assign      (GetField(ID_USER) );
   if(SendDlgItemMsg(ID_ACTUAL,BM_GETCHECK,NULL,NULL))
   { 
    if(!CheckActual())
       strcpy(szActual,"T");
    else
    strcpy(szActual,"S");
   }      
   MessageBox(HWindow,szActual,"",MB_OK);
   typem->fActual->assign(szActual);
   typem->flush();
   ClearScreen();
  }
}
       
void TSCTypesList::SetupWindow ()
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
 List->SetMaxRows(typem->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 typem->SetIndex("fcode");
 long i = 1;
 rQuery = new Relate4set(typem);
 rQuery->querySet(szQuery);
 while(!typem->eof())
  {
   List->SetData(1,i,typem->fCode->str());
   List->SetData(2,i,typem->fShortName->str());
   List->SetData(3,i,typem->fName->str());
   i++;
   typem->skip(1);
  }

 rQuery->unlock();
 rQuery->free(0);
 if(i>9)
 List->SetMaxRows(i-1);
 else
 List->SetMaxRows(9);
 SetCaption("Standard Cost Types List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCTypesList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(typem->seek(szItem) == 0)
  {
    SendMessage(GetDlgItem(Parent->HWindow,ID_CODE) ,WM_SETTEXT,NULL,(LONG)(LPSTR)typem->fCode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_NAME)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)typem->fShortName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DESCRIPT)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)typem->fName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_USER)   ,WM_SETTEXT,NULL,(LONG)(LPSTR)typem->fUser->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATE)   ,WM_SETTEXT,NULL,(LONG)(LPSTR)typem->fDate->str());
  }
 else
 BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

