// Elements maintainance
#include "scs.h"
#include "db_elem.h"
#include "db_resrc.h"
extern Code4 cb;

_CLASSDEF(TDElement)
class TDElement : public TDialog
{
 public:
  Date4 UserDate;
  char szType[2];
  PTSCElementList  ELEMENT;
  PTElement   element;
  PTResrc     Resrc;
  TDElement (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
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
  virtual void Material         (RTMessage Msg ) = [ID_FIRST + ID_MATERIAL];
  virtual void Labour           (RTMessage Msg ) = [ID_FIRST + ID_LABOUR];
  virtual void IndOverhead      (RTMessage Msg ) = [ID_FIRST + ID_INDIRECT];
  virtual void DirOverhead      (RTMessage Msg ) = [ID_FIRST + ID_DIRECT];
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  char *GetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
};


void TGlWindow::SCSDataElements   (RTMessage)
 {GetApplication()->ExecDialog(new TDElement(this,"DELEMENTS"));}


void TDElement::EnterError(int ID)
{
 BWCCMessageBox(HWindow,"All fields must be completed.",GetString(GetApplication()->hInstance,ID),MB_ICONEXCLAMATION);
 return;
}



BOOL TDElement::SetField(int ID, Field4 *field)
{
  if(field->type() == r4date)
  {
   Date4 *Date = new Date4(field->str(),"CCYYMMDD");
   char *szTemp = new char[8 + 1];
   Date->format(szTemp,"MM/DD/YY");
   SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)szTemp);
   delete Date;
   delete szTemp;

  }
  else
   SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDElement::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TDElement::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TDElement::UpdateWindow()
{
 Date4 *Today;
 Today = (Date4 *) new Date4();
 Today->today();
 char szToday[9];
 Today->format(szToday,"MM/DD/YY");

 SetField(ID_CODE,     		element->fCode);
 SetField(ID_DESCRIPT, 		element->fName);
 SetField(ID_NAME,      	element->fShortName);
 SetField(ID_USER,              element->fUser);
 SetField(ID_USERUPDATE,  	szUser);
 SetField(ID_DATEUPDATED,       szToday);
 SetField(ID_DATE,              element->fDate);

 if(strcmp(element->fTypeFlag->str(),"I")==0){
     SendMessage(GetDlgItem(HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_INDIRECT),BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);
    }
 else if (strcmp(element->fTypeFlag->str(),"D")==0){
     SendMessage(GetDlgItem(HWindow,ID_DIRECT)  ,BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);
    }
 else if (strcmp(element->fTypeFlag->str(),"M")==0){
     SendMessage(GetDlgItem(HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_MATERIAL),BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);

    }
 else if (strcmp(element->fTypeFlag->str(),"L")==0){
     SendMessage(GetDlgItem(HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(HWindow,ID_LABOUR)  ,BM_SETCHECK,TRUE,NULL);
    }

 SetFocus(GetDlgItem(HWindow,ID_CODE));
}

void TDElement::ClearScreen()
{
 SetField(ID_CODE,     		"");
 SetField(ID_NAME,    		"");
 SetField(ID_DESCRIPT,     	"");
 SetField(ID_USER,      	"");
 SendMessage(GetDlgItem(HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
 SendMessage(GetDlgItem(HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
 SendMessage(GetDlgItem(HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
 SendMessage(GetDlgItem(HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);
 SetFocus(GetDlgItem(HWindow,ID_RESCODE));
}


void TDElement::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}


void TDElement::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 cb.optimize = -1;
 element = new TElement(&cb);
 Resrc   = new TResrc (&cb);

 ELEMENT = new TSCElementList(this,"PICKLIST","FCODE <> ' '",element);
 ELEMENT->EnableAutoCreate();
 GetApplication()->MakeWindow(ELEMENT);
 SetField(ID_USERUPDATE,szUser);

 HourGlassOff();
}

void TDElement::Search(RTMessage)
{
if(!IsWindowVisible(ELEMENT->HWindow))
   ELEMENT->Show(SW_SHOW);
  else
   ELEMENT->Show(SW_HIDE);
}

void TDElement::Top(RTMessage)
{
 element->top();
 UpdateWindow();
}

void TDElement::Forward(RTMessage)
{
 element->skip(1);
 if(element->eof())
 element->bottom();
 UpdateWindow();
}

void TDElement::Back(RTMessage)
{
 element->skip(-1);
 if(element->bof())
 element->top();
 UpdateWindow();
}

void TDElement::Material         (RTMessage )
  {
    strcpy(szType,"M");   
    SendMessage(GetDlgItem(HWindow,ID_DIRECT)   ,BM_SETCHECK,FALSE,NULL);
    SendMessage(GetDlgItem(HWindow,ID_INDIRECT) ,BM_SETCHECK,FALSE,NULL);
    SendMessage(GetDlgItem(HWindow,ID_MATERIAL) ,BM_SETCHECK,TRUE,NULL);
    SendMessage(GetDlgItem(HWindow,ID_LABOUR)   ,BM_SETCHECK,FALSE,NULL);
  }

void TDElement::Labour           (RTMessage )
 {
   strcpy(szType,"L");
   SendMessage(GetDlgItem(HWindow,ID_DIRECT)   ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_INDIRECT) ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_MATERIAL) ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_LABOUR)   ,BM_SETCHECK,TRUE,NULL);
 }

void TDElement::IndOverhead      (RTMessage )
 {
   strcpy(szType,"I");
   SendMessage(GetDlgItem(HWindow,ID_DIRECT)   ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_INDIRECT) ,BM_SETCHECK,TRUE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_MATERIAL) ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_LABOUR)   ,BM_SETCHECK,FALSE,NULL);
 }

void TDElement::DirOverhead      (RTMessage )
 {
   strcpy(szType,"D");
   SendMessage(GetDlgItem(HWindow,ID_DIRECT)   ,BM_SETCHECK,TRUE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_INDIRECT) ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_MATERIAL) ,BM_SETCHECK,FALSE,NULL);
   SendMessage(GetDlgItem(HWindow,ID_LABOUR)   ,BM_SETCHECK,FALSE,NULL);
 }



void TDElement::Bottom(RTMessage)
{
 element->bottom();
 UpdateWindow();
}

void TDElement::Delete(RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to delete this record?","Delete Record",MB_ICONQUESTION | MB_YESNO) == IDYES)
 {  
  Resrc->SetIndex("felmncd");
  if(Resrc->seek(GetField(ID_CODE)) == 0)
   {
    char szMsg[100];
    strcpy(szMsg,"Element is in use by Resource : ");
    strcat(szMsg,Resrc->fCode->str());
    strcat(szMsg,". Can not be deleted.");
    BWCCMessageBox(HWindow,szMsg,"Delete Record",MB_ICONINFORMATION);
    return;
   }
   
  element->deleteRec();
  element->pack();
  UpdateWindow();
  delete ELEMENT;
  ELEMENT = new TSCElementList(this,"PICKLIST","FCODE <> ' '",element);
  ELEMENT->EnableAutoCreate();
  GetApplication()->MakeWindow(ELEMENT);
 }
}

void TDElement::Add (RTMessage Msg)
{
 BOOL bChecked = FALSE;
 if(*GetField(ID_CODE) == 0){
  SetFocus(GetDlgItem(HWindow,ID_CODE));
  BWCCMessageBox(HWindow,"Ensure all fields are completed.","Adding a new Element",MB_ICONEXCLAMATION);
  return;
 }
 if(element->seek(GetField(ID_CODE)) !=0)
 {
   if(*GetField(ID_CODE)  == 0 ){ EnterError(ID_CODE); return; }
   if(*GetField(ID_NAME)  == 0 ){ EnterError(ID_NAME); return; }
   if(*GetField(ID_DESCRIPT) == 0 ){ EnterError(ID_DESCRIPT); return; }

   element->appendStart(0);
   element->fCode->assign    	(GetField(ID_CODE) );
   element->fName->assign    	(GetField(ID_DESCRIPT) );
   element->fShortName->assign  (GetField(ID_NAME));
   element->fUser->assign    	(GetField(ID_USERUPDATE) );
   UserDate.assign(GetField(ID_DATE));
   UserDate.format("MM/DD/YY");
   element->fDate->assign    	(UserDate.str());
   DWORD dResult = SendDlgItemMsg(ID_DIRECT, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"D");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_INDIRECT, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"I");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_MATERIAL, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"R");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_LABOUR, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"L");
     bChecked = TRUE;
    }
   if(!bChecked)
   {
    BWCCMessageBox(HWindow,"Check the element type box.","Element Types",MB_ICONEXCLAMATION);
    return;
   }

   element->fTypeFlag->assign   (szType);
   element->append();
   ClearScreen();
   delete ELEMENT;
   ELEMENT = new TSCElementList(this,"PICKLIST","FCODE <> ' '",element);
   ELEMENT->EnableAutoCreate();
   GetApplication()->MakeWindow(ELEMENT);
 }
 else
 Modify(Msg);
}
void TDElement::Modify (RTMessage)
{
  BOOL bChecked = FALSE;
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this record?","Modify Record",MB_ICONQUESTION | MB_YESNO)==IDYES) {
   element->fCode->assign      (GetField(ID_CODE) );
   element->fName->assign      (GetField(ID_DESCRIPT) );
   element->fShortName->assign (GetField(ID_NAME));
   element->fUser->assign      (GetField(ID_USERUPDATE) );
   UserDate.assign(GetField(ID_DATE));
   UserDate.format("MM/DD/YY");
   element->fDate->assign    	(UserDate.str());
   DWORD dResult = SendDlgItemMsg(ID_DIRECT, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"D");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_INDIRECT, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"I");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_MATERIAL, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"M");
     bChecked = TRUE;
     }
   dResult = SendDlgItemMsg(ID_LABOUR, BM_GETCHECK,NULL,NULL);
   if(dResult){
     strcpy(szType,"L");
     bChecked = TRUE;
    }
   if(!bChecked)
   {
    BWCCMessageBox(HWindow,"Check the element type box.","Element Types",MB_ICONEXCLAMATION);
    return;
   }

   element->fTypeFlag->assign   (szType);
   element->flush();
   ClearScreen();
   delete ELEMENT;
   ELEMENT = new TSCElementList(this,"PICKLIST","FCODE <> ' '",element);
   ELEMENT->EnableAutoCreate();
   GetApplication()->MakeWindow(ELEMENT);

  }
}

void TSCElementList::SetupWindow ()
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
 List->SetMaxRows(element->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 element->SetIndex("fcode");
 element->top();
 long i = 1;
 rQuery = new Relate4set(element);
 rQuery->querySet(szQuery);
 while(!element->eof())
  {
	List->SetData(1,i,element->fCode->str());
	List->SetData(2,i,element->fShortName->str());
	List->SetData(3,i,element->fName->str());
	i++;
	element->skip(1);
  }

 rQuery->unlock();
 rQuery->free(0);
 List->SetMaxRows(i);
 SetCaption("Standard Cost Element List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCElementList::HandleListBox (RTMessage Msg)
{
 Date4 *Today;
 Today = (Date4 *) new Date4();
 Today->today();
 char szToday[9];
 Today->format(szToday,"MM/DD/YY");

 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(element->seek(szItem) == 0)
  {
    SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fCode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_NAME)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fShortName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DESCRIPT)    ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fName->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_USER)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fUser->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATEUPDATE)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)element->fDate->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_DATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)szToday);

    if(strcmp(element->fTypeFlag->str(),"I")==0){
     SendMessage(GetDlgItem(Parent->HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_INDIRECT),BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);

    }
    else if (strcmp(element->fTypeFlag->str(),"D")==0){
     SendMessage(GetDlgItem(Parent->HWindow,ID_DIRECT)  ,BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);
    }
    else if (strcmp(element->fTypeFlag->str(),"M")==0){
     SendMessage(GetDlgItem(Parent->HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_MATERIAL),BM_SETCHECK,TRUE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_LABOUR)  ,BM_SETCHECK,FALSE,NULL);

    }
    else if (strcmp(element->fTypeFlag->str(),"L")==0){
     SendMessage(GetDlgItem(Parent->HWindow,ID_DIRECT)  ,BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_INDIRECT),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_MATERIAL),BM_SETCHECK,FALSE,NULL);
     SendMessage(GetDlgItem(Parent->HWindow,ID_LABOUR)  ,BM_SETCHECK,TRUE,NULL);
    }

  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

