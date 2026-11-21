#include "scs.h"

#include "db_resrc.h"
#include "db_elem.h"
#include "db_types.h"


/*** Resource Update ***/
_CLASSDEF(TDResource)
class TDResource : public TDialog
{
 public:
  BOOL bUpdateRes;
  char szCurrentType[4];
  PTSCResList     SEARCH;
  PTscitemm       Items;
  PTResrc         Resrc;
  PTElement       Element;
  PTTypes         Types;
  TDResource (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow();
  virtual void Add        		(RTMessage Msg) = [ID_FIRST + ID_ADD];
  virtual void Browse     		(RTMessage Msg) = [ID_FIRST + ID_RESSEARCH];
  virtual void Search                   (RTMessage Msg) = [ID_FIRST + ID_SEARCH]; 
  virtual void Top        		(RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom    		(RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Next      		(RTMessage Msg) = [ID_FIRST + ID_NEXT];
  virtual void Previous   		(RTMessage Msg) = [ID_FIRST + ID_PREVIOUS];
  virtual void Delete    		(RTMessage Msg) = [ID_FIRST + ID_DELETE];
  virtual void Modify     		(RTMessage Msg) = [ID_FIRST + ID_MODIFY];
  virtual void Cancel     		(RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void ElementName              (RTMessage Msg) = [ID_FIRST + ID_RESELEMENT];
  virtual void CostType                 (RTMessage Msg) = [ID_FIRST + ID_COSTTYPE];
  virtual void UpdateRawMaterialRes	(RTMessage Msg) = [ID_FIRST + ID_UPDATEMATERIAL];
  virtual void SetDetail                (RTMessage Msg) = [CM_FIRST + CM_SETRESOURCE]{Search(Msg);};
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  double dGetField(int ID);
  void  LoadUnits(PTscitemm Items);
  void  LoadTypes(PTTypes   Types);
  BOOL  CheckUnits(PTscitemm Items,char *unit);
  void  LoadElements(PTElement Element);
  BOOL  CheckElements(PTElement Element, char *elements);
  void  ClearScreen(void);
  void  EnterError(int ID);
  void  CheckError(int ID);
};

void TGlWindow::SCSDataResource   (RTMessage)
 {GetApplication()->ExecDialog(new TDResource(this,"DRESOURCE"));}

void TDResource::Search(RTMessage)
{
 char szSeek[15];
 strcpy(szSeek,GetField(ID_RESCODE));
 strcat(szSeek,GetField(ID_COSTTYPE));
 Resrc->seek(szSeek);
 UpdateWindow();
}

double TDResource::dGetField(int ID)
{
 double dValue; 
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}


void TDResource::ElementName(RTMessage Msg)
{
 DWORD Indx;
 char szElement[20];
 if(Msg.LP.Hi == CBN_SELCHANGE)
 {
  Indx = SendDlgItemMsg(ID_RESELEMENT,CB_GETCURSEL,0,0);
  if(Indx != CB_ERR)
    SendDlgItemMsg(ID_RESELEMENT,CB_GETLBTEXT, (WPARAM)Indx, (LPARAM) ((LPSTR) szElement));
  Element->seek(szElement);
  SetField(ID_ELEMENTNAME,Element->fName);
  SetField(ID_TYPE,Element->fTypeFlag);
 }
}

void TDResource::CostType(RTMessage Msg)
{
 DWORD Indx;
 char szType[4];
 if(Msg.LP.Hi == CBN_SELCHANGE)
 {
  Indx = SendDlgItemMsg(ID_COSTTYPE,CB_GETCURSEL,0,0);
  if(Indx != CB_ERR)
    SendDlgItemMsg(ID_COSTTYPE,CB_GETLBTEXT, (WPARAM)Indx, (LPARAM) ((LPSTR) szType));
  Types->seek(szType);
  SetField(ID_COSTNAME,Types->fName);
  if(strcmp(szCurrentType,szType) != 0)
   {
    strcpy(szCurrentType,szType);
    if(bUpdateRes == FALSE)
    {
     bUpdateRes = TRUE;
     if(IsWindowVisible(SEARCH->HWindow))
       SEARCH->Show(SW_HIDE);
    }     
   }
 }
}


void TDResource::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 cb.optimize = -1;
 Items    = new Tscitemm (&cb);
 Resrc    = new TResrc   (&cb);
 Element  = new TElement (&cb);
 Types    = new TTypes   (&cb);

 LoadUnits(Items);
 LoadElements(Element);
 LoadTypes(Types);
 SetField(ID_USER,szUser);
 bUpdateRes = TRUE;
 HourGlassOff();

}

void TDResource::LoadTypes(PTTypes)
{
 DWORD Indx;
 Types->top();
 while(!Types->eof())
 {
  Indx = SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Types->fCode->str());
  Types->skip(1);
 }
 SendDlgItemMsg(ID_COSTTYPE,CB_SETCURSEL,Indx,NULL); 
 if(Types->seek(GetField(ID_COSTTYPE)) ==0)
  SetField(ID_COSTNAME,Types->fName);
 strcpy(szCurrentType,Types->fCode->str());
 return;
}


void TDResource::LoadElements(PTElement Element)
{
 Element->top();
 while(!Element->eof())
 {
  SendDlgItemMsg(ID_RESELEMENT,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Element->fCode->str());
  Element->skip(1);
 }
 return; 
}

BOOL  TDResource::CheckElements(PTElement Element, char *elements)
{
  Element->SetIndex("FCODE");  
  if(Element->seek(elements) == 0){
   return TRUE;
  }
  else
  return FALSE;
}

BOOL TDResource::CheckUnits(PTscitemm Items,char *unit)
{
  Items->SetIndex("units");
  if (Items->seek(unit) == 0){
  Items->SetIndex("fpcode");
  return TRUE;
  }
  else{
  Items->SetIndex("fpcode");
  return FALSE;
  }  
}

void TDResource::LoadUnits(PTscitemm Items)
{
  Items->SetIndex("units");
  Items->top();
  while(!Items->eof())
  {
   SendDlgItemMsg(ID_UNIT,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Items->fUnit->str());
   Items->skip();
  }
 Items->SetIndex("fpcode");
 Items->top();
}

void TDResource::EnterError(int ID)
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

void TDResource::CheckError(int ID)
{ 
 char *szMsg = new char[(int)150];
 HINSTANCE hInst = GetApplication()->hInstance;
 strcpy(szMsg,GetString(hInst,ID_INPUTERROR));
 strcat(szMsg,GetString(hInst,ID));
 BWCCMessageBox(HWindow,szMsg,GetString(hInst,ID_ERROR),MB_ICONEXCLAMATION);
 delete szMsg;
 return;
}

void TDResource::Add (RTMessage Msg)
{
 char szSeek[14];
 Date4 Today,Update;
 char szUpdateDate[9],szDate[9],szCode[11];
 memset(szUpdateDate,NULL,sizeof(szUpdateDate));
 memset(szDate,NULL,sizeof(szDate));
 memset(szCode,NULL,sizeof(szCode));
 if(*GetField(ID_RESCODE)   == 0){EnterError(ID_RESCODE)   ; return;}
 if(*GetField(ID_NAME)      == 0){EnterError(ID_NAME)      ; return;}
 if(*GetField(ID_RESNAME)   == 0){EnterError(ID_RESNAME)   ; return;}
 //if(*GetField(ID_RESELEMENT)== 0){EnterError(ID_RESELEMENT); return;}
 //if(*GetField(ID_TYPE)      == 0){EnterError(ID_TYPE)      ; return;}

 strcpy(szSeek,GetField(ID_RESCODE));
 strcat(szSeek,GetField(ID_COSTTYPE));
 if(Resrc->seek(szSeek) !=0)
 {
   //if(CheckElements(Element,GetField(ID_RESELEMENT)) == FALSE){CheckError(ID_RESELEMENT);return;}
   strcpy(szDate,GetField(ID_DATE));
   strcpy(szUpdateDate,GetField(ID_DATEUPDATED));
   Today.today();
 
   Resrc->appendStart(0);
   Resrc->fCode->assign       (GetField(ID_RESCODE)    );
   Resrc->fShortName->assign  (GetField(ID_NAME)    );
   Resrc->fUnit->assign       (GetField(ID_UNIT)       );
   Resrc->fElement->assign    (GetField(ID_RESELEMENT) );
   Resrc->fName->assign       (GetField(ID_RESNAME)   );
   Resrc->fUpdateuser->assign (GetField(ID_USERUPDATE) );
   Resrc->fUser->assign       (GetField(ID_USER)       );
   if(Element->seek(GetField(ID_RESELEMENT))==0)
   Resrc->fType->assign        (Element->fTypeFlag->str());
   Resrc->fRate->assignDouble  (dGetField(ID_RATE)   );
   Resrc->fCostType->assign    (GetField(ID_COSTTYPE));
   Resrc->append();   
   char szQuery[45];
   char szSetCaption[30];
   strcpy(szQuery,"FCOSTTYPE = '");
   strcat(szQuery,GetField(ID_COSTTYPE));
   strcat(szQuery,"'");

   if(bUpdateRes)
   {
    SEARCH = new TSCResList(this,"RESLIST",szQuery,Resrc);
    SEARCH->EnableAutoCreate();
    GetApplication()->MakeWindow(SEARCH);
   }

   SEARCH->List->ClearData(SS_ALLCOLS,SS_ALLROWS);
   long i = 1;
   Relate4set *rQuery = new Relate4set(Resrc);
   rQuery->querySet(szQuery);
   for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
    {
    SEARCH->List->SetData(1,i,Resrc->fCode->str());
    SEARCH->List->SetData(2,i,Resrc->fShortName->str());
    SEARCH->List->SetData(3,i,Resrc->fName->str());
    i++;
    }
    rQuery->unlock();
    rQuery->free();
    if(i>15)
    SEARCH->List->SetMaxRows(i);
    else
    SEARCH->List->SetMaxRows(15);
    strcpy(szSetCaption,"Standard Cost Resource - ");
    strcat(szSetCaption,GetField(ID_COSTTYPE));
    SEARCH->SetCaption(szSetCaption);
    ClearScreen();
 }
 else
 Modify(Msg);
}

void TDResource::UpdateRawMaterialRes(RTMessage)
{
 Date4 *Today;
 Today = (Date4 *) new Date4();
 Today->today();
 char szToday[9];
 memset(szToday,NULL,sizeof(szToday));

 Items->top();
 Items->SetIndex("raw");
 while(!Items->eof())
  {
   char szResource[15];
   strcpy(szResource,"STD");
   strcat(szResource,Items->fPcode->str());
   if(Resrc->seek(szResource) != 0)
    {
       Resrc->appendStart(0);
       Resrc->fCode->assign        (Items->fPcode->str());
       Resrc->fCostType->assign    ("STD");
       Resrc->fShortName->assign   (Items->fPname->str());
       Resrc->fUnit->assign        (Items->fUnit->str());
       Resrc->fElement->assign     ("RAW");
       Resrc->fName->assign        (Items->fFulln->str());
       Resrc->fDate->assign        (Today->str());
       Resrc->fUpdateuser->assign  (szUser);
       Resrc->fUpdatedate->assign  (Today->str());
       Resrc->fType->assign        (Items->fPtype->str());
       Resrc->fRate->assignDouble (0.00);
       Resrc->fCostType->assign    ("STD");
       Resrc->append();
    }
   else
    {
       Resrc->fCode->assign        (Items->fPcode->str());
       Resrc->fShortName->assign   (Items->fPname->str());
       Resrc->fUnit->assign        (Items->fUnit->str());
       Resrc->fElement->assign     ("RAW");
       Resrc->fName->assign        (Items->fFulln->str());
       Resrc->fDate->assign        (Today->str());
       Resrc->fUpdateuser->assign  (szUser);
       Resrc->fUpdatedate->assign  (Today->str());
       Resrc->fType->assign        (Items->fPtype->str());
       Resrc->fCostType->assign    ("STD");
       Resrc->fRate->assignDouble  (0.00);
       Resrc->flush();
    }
   SendDlgItemMsg(ID_CODE,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Items->fPcode->str());
   Items->skip(1);
  }
 bUpdateRes = TRUE;
 return;
}

void TDResource::Modify (RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO) == IDYES)
 {

  if(*GetField(ID_RESCODE)   == 0){EnterError(ID_RESCODE)   ; return;}
  if(*GetField(ID_NAME)      == 0){EnterError(ID_NAME)      ; return;}
  if(*GetField(ID_UNIT)      == 0){EnterError(ID_UNIT)      ; return;}
  if(*GetField(ID_RESNAME)   == 0){EnterError(ID_RESNAME)   ; return;}
  // if(*GetField(ID_RESELEMENT)== 0){EnterError(ID_RESELEMENT); return;}
  //if(dGetField(ID_RATE)      == 0){EnterError(ID_RATE)      ; return;}
  //if(*GetField(ID_TYPE)      == 0){EnterError(ID_TYPE)      ; return;}

  //if(CheckElements(Element,GetField(ID_RESELEMENT)) == FALSE){CheckError(ID_RESELEMENT);return;}

   Resrc->fCode->assign       (GetField(ID_RESCODE)    );
   Resrc->fShortName->assign  (GetField(ID_NAME)    );
   Resrc->fUnit->assign       (GetField(ID_UNIT)       );
   Resrc->fElement->assign    (GetField(ID_RESELEMENT) );
   Resrc->fName->assign       (GetField(ID_RESNAME)   );
   Resrc->fUpdateuser->assign (GetField(ID_USERUPDATE) );
   Element->SetIndex("FCODE");

   Resrc->fUser->assign       (GetField(ID_USER));
   if(Element->seek(GetField(ID_RESELEMENT))==0)
   Resrc->fType->assign       (Element->fTypeFlag->str());
   Resrc->fRate->assignDouble (dGetField(ID_RATE));
   Resrc->fCostType->assign    (GetField(ID_COSTTYPE));
   Resrc->flush();
   char szQuery[45];
   char szSetCaption[30];
   strcpy(szQuery,"FCOSTTYPE = '");
   strcat(szQuery,GetField(ID_COSTTYPE));
   strcat(szQuery,"'");
   if(bUpdateRes)
   {
    SEARCH = new TSCResList(this,"RESLIST",szQuery,Resrc);
    SEARCH->EnableAutoCreate();
    GetApplication()->MakeWindow(SEARCH);
   }

   UpdateWindow();
   bUpdateRes = TRUE;
 }
 return;
}

void TSCResList::SetField(int ID, Field4 *field)
{ 
  SendMessage(GetDlgItem(Parent->HWindow,ID),WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
}      


BOOL TDResource::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDResource::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TDResource::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TDResource::UpdateWindow()
{
 SetField(ID_RESCODE,     Resrc->fCode);
 SetField(ID_RESNAME,     Resrc->fName);
 SetField(ID_NAME,        Resrc->fShortName);
 SetField(ID_COSTTYPE,    Resrc->fCostType);
 if(Types->seek(Resrc->fCostType->str())==0)
    SetField(ID_COSTNAME, Types->fName);
 SetField(ID_RESELEMENT,  Resrc->fElement);
 if(Element->seek(Resrc->fElement->str()) == 0)
  SetField(ID_ELEMENTNAME,Element->fName);

 SetField(ID_UNIT,        Resrc->fUnit);
 SetField(ID_DATE,        Resrc->fDate);
 SetField(ID_USER,        szUser);
 SetField(ID_USERUPDATE,  Resrc->fUpdateuser);
 SetField(ID_DATEUPDATED, Resrc->fUpdatedate);
 SetField(ID_TYPE ,       Resrc->fType);
 SetField(ID_RATE,        Resrc->fRate);
 SetFocus(GetDlgItem(HWindow,ID_RESCODE));
}

void TDResource::ClearScreen()
{
 SetField(ID_RESCODE,     "");
 SetField(ID_NAME,        "");
 SetField(ID_UNIT,        "");
 SetField(ID_RESELEMENT,  "");
 SetField(ID_TYPE,        "");
 SetField(ID_DATE,        "");
 SetField(ID_USER,        szUser);
 SetField(ID_USERUPDATE,  "");
 SetField(ID_DATEUPDATED, "");
 SetField(ID_ITEMNO,      "");
 SetField(ID_RESNAME,     "");
 SetField(ID_RATE,        "");
 SetField(ID_ACCOUNTS,    "");
 SetField(ID_DEPART,      "");
 SetField(ID_QUANTITY,    "");
 SetFocus(GetDlgItem(HWindow,ID_RESCODE));
}


void TDResource::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}




void TDResource::Top(RTMessage)
{
 Resrc->top();
 UpdateWindow();
}

void TDResource::Next(RTMessage)
{
 Resrc->skip(1);
 if(Resrc->eof())
 Resrc->bottom();
 UpdateWindow();

}

void TDResource::Previous(RTMessage)
{
 Resrc->skip(-1);
 if(Resrc->bof())
 Resrc->top();
 UpdateWindow();
}


void TDResource::Bottom(RTMessage)
{
 Resrc->bottom();
 UpdateWindow();
}

void TDResource::Delete(RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to delete this record?","Delete Record",MB_ICONQUESTION | MB_YESNO) == IDYES)
 {
  Resrc->deleteRec();
  Resrc->pack();
  UpdateWindow();
 }
 return;
}

void TDResource::Browse(RTMessage)
{
 if(bUpdateRes)
 {
  char szQuery[45];
  char szSetCaption[30];
  strcpy(szQuery,"FCOSTTYPE = '");
  strcat(szQuery,szCurrentType);
  strcat(szQuery,"'");
  SEARCH = new TSCResList(this,"RESLIST",szQuery,Resrc);
  SEARCH->EnableAutoCreate();
  GetApplication()->MakeWindow(SEARCH);
  strcpy(szSetCaption,"Resource Browse Window - ");
  strcat(szSetCaption,szCurrentType);
  strcat(szSetCaption,"\0");
  SEARCH->SetCaption((LPSTR)szSetCaption);
  bUpdateRes = FALSE;
 }
 if(!IsWindowVisible(SEARCH->HWindow))
   SEARCH->Show(SW_SHOW);
 else
   SEARCH->Show(SW_HIDE);
}

void TSCResList::SetupWindow ()
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
 List->SetMaxRows(Resrc->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 Resrc->top();
 long i = 1;
 rQuery = new Relate4set(Resrc);
 rQuery->querySet(szQuery);
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   List->SetData(1,i,Resrc->fCode->str());
   List->SetData(2,i,Resrc->fShortName->str());
   List->SetData(3,i,Resrc->fName->str());
   i++;
  }
 rQuery->unlock();
 rQuery->free(0);
 if(i>15)
  List->SetMaxRows(i);
 else
  List->SetMaxRows(15);

 SetCaption("Standard Cost Resource List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCResList::HandleListBox (RTMessage Msg)
{
 char szItem[15];
 char szCostType[4];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 PTElement element = new TElement(&cb);
 SendMessage(GetDlgItem(Parent->HWindow,ID_COSTTYPE),WM_GETTEXT,sizeof(szCostType),(LPARAM)(LPSTR)szCostType);
 if(*szCostType == 0)
 strcpy(szCostType,"STD");
 strcat(szItem,szCostType);
 if(Resrc->seek(szItem) == 0)
  {
    SetField(ID_RESCODE,Resrc->fCode);
    SendMessage(GetDlgItem(Parent->HWindow,ID_RESCODE)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fCode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_NAME)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fShortName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RESNAME)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_USER)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)szUser);
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fDate->str());    
    SendMessage(GetDlgItem(Parent->HWindow,ID_USERUPDATE)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUpdateuser->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATEUPDATED) ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUpdatedate->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_UNIT)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUnit->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RESELEMENT)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fElement->str());
    if(element->seek(Resrc->fElement->str()) == 0)
    SendMessage(GetDlgItem(Parent->HWindow,ID_ELEMENTNAME) ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fName->str());

    SendMessage(GetDlgItem(Parent->HWindow,ID_TYPE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fType->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_RATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fRate->str());
    SendMessage(Parent->HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 delete element;
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

