// Standard Cost System  - (C) Copyright 1992 by Tegara Corporation
#include "scs.h"
#include "db_resrc.h"
#include "db_trstr.h"
#include "db_elem.h"
#include "db_types.h"
#include "db_topic.h"
#include "sc_csdef.h"

void TResourceUpdate::DetailReSet(RTMessage)
{
    PTElement Element = new TElement(&cb);
    SendMessage(GetDlgItem(HWindow,ID_RESCODE)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fCode->str());
    SendMessage(GetDlgItem(HWindow,ID_NAME)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fShortName->str());
    SendMessage(GetDlgItem(HWindow,ID_RESNAME)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fName->str());
    SendMessage(GetDlgItem(HWindow,ID_USER)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)szUser);
    SendMessage(GetDlgItem(HWindow,ID_DATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fDate->str());
    SendMessage(GetDlgItem(HWindow,ID_USERUPDATE)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUpdateuser->str());
    SendMessage(GetDlgItem(HWindow,ID_DATEUPDATED) ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUpdatedate->str());
    SendMessage(GetDlgItem(HWindow,ID_UNIT)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fUnit->str());
    SendMessage(GetDlgItem(HWindow,ID_RESELEMENT)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fElement->str());
    if(Element->seek(Resrc->fElement->str()) == 0)
    SendMessage(GetDlgItem(HWindow,ID_ELEMENTNAME) ,WM_SETTEXT,NULL,(LONG)(LPSTR)Element->fName->str());
    SendMessage(GetDlgItem(HWindow,ID_TYPE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fType->str());
    SendMessage(GetDlgItem(HWindow,ID_RATE)        ,WM_SETTEXT,NULL,(LONG)(LPSTR)Resrc->fRate->str());
    Element->close();
}

void TGlWindow::SCSResourceUpdate (RTMessage)
{
   SCSDef = new TSCSDefinition(this,"SCS_DEFINITION",ResUpdate);
   SCSDef->EnableAutoCreate();
   GetApplication()->MakeWindow(SCSDef);
   PTResourceUpdate ResUpdate;
   ResUpdate = new TResourceUpdate(this,"RRESOURCE_UPDATE",SCSDef);
   ResUpdate->EnableAutoCreate();
   GetApplication()->MakeWindow(ResUpdate);
   ResUpdate->Show(SW_SHOW);

   //GetApplication()->ExecDialog(new TResourceUpdate(this,"RRESOURCE_UPDATE",SCSDef));
}


void TResourceUpdate::MultiLevel (RTMessage)
{
 if(bMultiLevel)
 bMultiLevel = FALSE;
 else
 bMultiLevel = TRUE;
}


BOOL TSCSDefinition::SetField(int ID, Field4 *field)
{
  SendMessage(GetDlgItem(ResUpdate->HWindow,ID),WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TSCSDefinition::SetField(int ID, char *field)
{
  SendMessage(GetDlgItem(ResUpdate->HWindow,ID),WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      


void TResourceUpdate::SetupWindow ()
 {
   TDialog::SetupWindow();
   HourGlassOn();
   cb.optimize = -1 ;
   Items    = new Tscitemm  (&cb);
   Resrc    = new TResrc    (&cb);
   CostStr  = new Ttrstr    (&cb);  // Cost Structure Master
   Types    = new TTypes    (&cb);

   bSearch = TRUE;
   bResource = TRUE;
   bMultiLevel = FALSE;
   Items->SetIndex("FPCODE");

   Types->top();
   while (!Types->eof())
   {
    SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)((LPSTR)Types->fCode->str()));
    Types->skip(1);
   }
   SendDlgItemMsg(ID_COSTTYPE,WM_SETTEXT,NULL,(LPARAM)((LPSTR)"STD")); 
   SCSDef->ResUpdate = this;
   SCSDef->Resrc = Resrc;
   
   szMainWindowCaption = GetLP(100);
   szCompositionCaption = GetLP(100);
   szCostType = GetLP(4);
   lstrcpy(szMainWindowCaption,"Cost Item Update - ");
   lstrcat(szMainWindowCaption,"Standard");
   SetCaption(szMainWindowCaption);
   lstrcpy(szCompositionCaption,"Composition Definition - ");
   lstrcat(szCompositionCaption,"Standard");
   SCSDef->SetCaption(szCompositionCaption);
   lstrcpy(szCostType,GetField(ID_COSTTYPE));
   SendDlgItemMsg(ID_LEVEL,BM_SETCHECK,FALSE,NULL);
   SCSDef->Show(SW_SHOW);
   HourGlassOff();
 }

void TResourceUpdate::CostType (RTMessage Msg)
{
 DWORD Indx;
 char szType[4];
 if(Msg.LP.Hi == CBN_SELCHANGE)
 {
  Indx = SendDlgItemMsg(ID_COSTTYPE,CB_GETCURSEL,0,0);
  if(Indx != CB_ERR)
    SendDlgItemMsg(ID_COSTTYPE,CB_GETLBTEXT, (WPARAM)Indx, (LPARAM) ((LPSTR) szType));
  Types->seek(szType);
  lstrcpy(szMainWindowCaption,"Cost Item Update - ");
  lstrcat(szMainWindowCaption,Types->fShortName->str());
  SetCaption(szMainWindowCaption);
  lstrcpy(szCompositionCaption,"Composition Definition - ");
  lstrcat(szCompositionCaption,Types->fShortName->str());
  SCSDef->SetCaption(szCompositionCaption);
  //SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
 }
}



void TResourceUpdate::Delete (RTMessage)
{
 if(BWCCMessageBox(HWindow,"Are you sure you want to DELETE record?","Delete a Resource",MB_ICONQUESTION | MB_YESNO) == IDNO)
 {
  return;
 }
 Str4ten Type;
 char  szCode[11];
 char  szRescode[11];
 char  szSeqno[4];

 SendDlgItemMsg(ID_CODE,WM_GETTEXT,sizeof(szCode),(LPARAM)(LPSTR)szCode);
 if(*szCode == 0){
  BWCCMessageBox(HWindow,"Enter an Item Number.","Delete a Resource",MB_ICONEXCLAMATION);
  return;
 }

 SendDlgItemMsg(ID_RESCODE,WM_GETTEXT,sizeof(szCode),(LPARAM)(LPSTR)szRescode);
 if(*szRescode == 0 )
 {
  BWCCMessageBox(HWindow,"Enter an Resource Number.","Delete a Resource",MB_ICONEXCLAMATION);
  return;
 }

 if(*GetField(ID_COSTTYPE) ==0)
  {
   BWCCMessageBox(HWindow,"Enter a cost type.","Delete a Resource",MB_ICONEXCLAMATION);
   return;
  }
 else
 {  
  Type.assign(GetField(ID_COSTTYPE));
  Type.trim();
 }

 if(*GetField(ID_SEQNO) == 0)
  {
   BWCCMessageBox(HWindow,"Sequence number must not be blank.","Delete a Resource",MB_ICONINFORMATION);
   return;
  }
 else
 strcpy(szSeqno,GetField(ID_SEQNO));


 char szSeek[26];
 strcpy(szSeek,Type.str());
 strcat(szSeek,szCode); 
 strcat(szSeek,szRescode);
 strcat(szSeek,szSeqno);

 CostStr->SetIndex("fopcode2");
 if(CostStr->seek(szSeek) == 0)
 {
       CostStr->deleteRec();
       CostStr->pack();
       SetResource();       
 }
 else
 BWCCMessageBox(HWindow,"Unable to locate resource - Delete Failed!","Delete a Resource",MB_ICONINFORMATION);
 CostStr->SetIndex("fopcode");
 return;
}

void TResourceUpdate::Add(RTMessage)
{
 Str4ten Type;
 LPSTR szCode = GetLP(11);
 LPSTR szFpcode = GetLP(11);
 LPSTR szKpcode = GetLP(11);
 LPSTR szElement = GetLP(11);
 LPSTR szSeqno = GetLP(5);
 LPSTR szRescode = GetLP(11);
 LPSTR szCostType = GetLP(4);


 if(*GetField(ID_CODE) == 0){
  BWCCMessageBox(HWindow,"Enter an Item Number.","Add New Resource",MB_ICONEXCLAMATION);
  return;
 }
 else
 lstrcpy(szCode,GetField(ID_CODE));

 if(*GetField(ID_COSTTYPE) == 0)
 {
  BWCCMessageBox(HWindow,"Enter an select a cost type.","Add New Resource",MB_ICONEXCLAMATION);
  return;
 }
 else
 {
  Type.assign(GetField(ID_COSTTYPE));
  Type.trim();
  if(Types->seek(Type.str()) != 0)
  {
   BWCCMessageBox(HWindow,"Invalid cost type.","Add New Resource",MB_ICONINFORMATION);
   return;
  }
 }
  
 if(*GetField(ID_RESCODE) == 0 )
 {
  BWCCMessageBox(HWindow,"Enter a Resource Number.","Add New Resource",MB_ICONEXCLAMATION);
  return;
 }
 else
 lstrcpy(szRescode,GetField(ID_RESCODE));

if(*GetField(ID_SEQNO) == 0)
 {
  BWCCMessageBox(HWindow,"Enter a Sequence Number.","Add New Resource",MB_ICONEXCLAMATION);
  return;
 }
 else
 lstrcpy(szSeqno,GetField(ID_SEQNO));
 
 char szItem[45]; 
 lstrcpy(szItem,Type.str());
 lstrcat(szItem,szCode); 
 lstrcat(szItem,szRescode);
 lstrcat(szItem,szSeqno);
 

 CostStr->SetIndex("fopcode2");
 if(CostStr->seek(szItem)==0)
  {   
     CostStr->fKousei->assignDouble (dGetField(ID_QUANTITY));
     CostStr->fSeqno->assign(GetField(ID_SEQNO));
     CostStr->flush();
     ClearResource();
     SetResource();
     return;
  }
 else
  {
   CostStr->appendStart(0);
   CostStr->fOpcode->assign(szCode);
   CostStr->fKpcode->assign(szRescode);
   CostStr->fType->assign(Type.str());
   CostStr->fSeqno->assign(szSeqno);
   Items->seek(GetField(ID_CODE));
   CostStr->fStqty->assignDouble (double(Str4ptr(Items->fLotsize->str())));
   CostStr->fKousei->assignDouble(dGetField(ID_QUANTITY));
   CostStr->append();
   ClearResource();
   SetResource();   
  }
 CostStr->SetIndex("fopcode");
}


void TResourceUpdate::CalQuantity (RTMessage)
{
 double dQuantity = 0;
 char szItem[11],szRescode[11],szSeek[25],szSeqno[4];
 Str4ten Type;

 strcpy(szSeqno,GetField(ID_SEQNO));
 strcpy(szItem,GetField(ID_CODE));
 strcpy(szRescode,GetField(ID_RESCODE));
 Type.assign(GetField(ID_COSTTYPE));
 Type.trim();

 strcpy(szSeek,Type.str());
 strcat(szSeek,szItem); 
 strcat(szSeek,szRescode);
 strcat(szSeek,szSeqno);

 CostStr->SetIndex("fopcode2");
 if(CostStr->seek(szSeek) == 0)
 {
   dQuantity = CostStr->dKousei();
   FloatSetValue(GetDlgItem(HWindow,ID_QUANTITY),dQuantity);
   SetField(ID_SEQNO,CostStr->fSeqno);	
 }
 CostStr->SetIndex("fopcode");
}

void TResourceUpdate::Next (RTMessage)
{ 
 Items->SetIndex("MAN");
 Items->seek(GetField(ID_CODE));
 Items->skip(1);
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_DETAIL,NULL);
 return;
}

void TResourceUpdate::SetDetail(RTMessage)
{
 Items->SetIndex("MAN");
 Items->seek(GetField(ID_CODE));
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_DETAIL,NULL);
 return;
}

void TResourceUpdate::Top (RTMessage)
{
 Items->SetIndex("MAN");
 Items->top();
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_DETAIL,NULL);
 return;
}

void TResourceUpdate::Bottom (RTMessage)
{ 
 Items->SetIndex("MAN");
 Items->bottom();
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_DETAIL,NULL);
 return;
}


void TResourceUpdate::Previous (RTMessage)
{
 Items->SetIndex("MAN");
 Items->seek(GetField(ID_CODE));
 Items->skip(-1); 
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_DETAIL,NULL);
 return;
}


void TResourceUpdate::UpdateWindow()
{
 SetField(ID_CODE,    Items->fPcode); 
 SetField(ID_SNAME,   Items->fPname);
 SetField(ID_FULLN,   Items->fFulln);
 SetField(ID_PTYPE,   Items->fPtype);
 SetField(ID_SHOP,    Items->fShop1);
 
 SetField(ID_UNIT,    Items->fUnit);
 SetField(ID_AVPRICE, Items->fAvprice);
 SetField(ID_SEMICODE,Items->fSkbn);
 double dItemCost = 0;

 if(strcmp(GetField(ID_COSTTYPE),"ACT") == 0)
  {
   CostStr->SetIndex("fopcode");
   char szSeek[14];
   strcpy(szSeek,"ACT");
   strcat(szSeek,Items->fPcode->str());
   if(CostStr->seek(szSeek) == 0)
    {
     SetField(ID_ACTUAL,"Prod.");
     SetField(ID_LOTSIZE,CostStr->fStqty->str());
     dItemCost = (dStandard * double (Str4ptr(CostStr->fStqty->str())));
    }
  }
 else
 {
  SetField(ID_ACTUAL,"Lot Size");
  SetField(ID_LOTSIZE, Items->fLotsize);
  dItemCost = (dStandard * double (Str4ptr(Items->fLotsize->str())));
 }
 FloatSetValue(GetDlgItem(HWindow,ID_STANDARD),dItemCost);
 FloatSetValue(GetDlgItem(HWindow,ID_AVPRICE),dStandard);
}

int TResourceUpdate::GetProduction(Field4 *fPcode)
{
 char szCompare[11];
 int iLots = 0;
 PTTopicsTmast Tmast = new TTopicsTmast(&cb);
 Tmast->SetIndex("FPCODE");
 if (Tmast->seek(fPcode->str()) == 0)
  {
   strcpy(szCompare,Tmast->fPcode->str());
   while (strcmp(szCompare,Tmast->fPcode->str()) == 0)
    {
     iLots++;
     Tmast->skip();
    }
  } 
 if(iLots == 0)
 iLots = 1;
 return iLots;
}

BOOL TResourceUpdate::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      

BOOL TResourceUpdate::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

char *TSCSDefinition::GetField(int ID)
{
 DWORD len =  SendMessage(GetDlgItem(Parent->HWindow,ID),WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendMessage(GetDlgItem(Parent->HWindow,ID),WM_GETTEXT,(int)len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

char *TResourceUpdate::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

double TResourceUpdate::dGetField(int ID)
{
 double dValue; 
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}

/*** Recursive Get Resource Function ***/
/***
    This is a recursive routine that moves down the levels
    of the structure master when it encounters a manufactured
    item. It is recursive because it calls itself as many times as
    there are levels in the the tree.
*/


int TResourceUpdate::GetResource(char *szRescode,char *szCostType, int iLevel, int i,double dRequirement)
{
  char szOpcode[15];
  char szKpcode[15];
  char szKpcodeSeek[15];
  char szKpItemCode[15];

  if(CostStr->seek(szRescode) == 0)
  {   
   strcpy(szOpcode,szCostType);              // create a comparison key for the cost structure file.
   strcat(szOpcode,CostStr->fOpcode->str());   
   while (strcmp(szRescode,szOpcode) == 0)     // loop through the database.
     {      
      strcpy(szKpcode,CostStr->fKpcode->str());      
      strcpy(szKpcodeSeek,CostStr->fKpcode->str());
      strcat(szKpcodeSeek,szCostType);
       if(Resrc->seek(szKpcodeSeek) == 0)
	 {
	  SetResourceValues(iLevel,i,Resrc,dRequirement);
	  i++;
	 }
       else                                      // Set an upper level manfactured items costs.
       {	 
	long lRecno = CostStr->recNo();
	double dUpperQuantity = CostStr->dQuantity();
	double dUpperKousei = CostStr->dKousei();	
	strcpy(szKpItemCode,szCostType);
        strcat(szKpItemCode,szKpcode);
	if(CostStr->seek(szKpItemCode) == 0)
	 {  
	  int iCurrentLevel = iLevel;
	  double CurrentRequirement  = dRequirement;
	  SetManufacturedValues(iLevel,i,szKpcode,dUpperKousei,dUpperQuantity);  
	  iLevel++;
	  i++;
	  if(dUpperKousei != dUpperQuantity)
	    dRequirement = dRequirement * dUpperKousei/dUpperQuantity;
	  i = GetResource(szKpItemCode,szCostType,iLevel,i,dRequirement);
          dRequirement = CurrentRequirement;
	  iLevel = iCurrentLevel;       

         }
	CostStr->go(lRecno);
       }
       CostStr->skip();
       strcpy(szOpcode,szCostType);
       strcat(szOpcode,CostStr->fOpcode->str());
    }
  }    
 return i;
}

#pragma argsused
void TResourceUpdate::SetResourceValues(int iLevel,int i,PTResrc Resrc,double dRequirement)
       {
	  char szLevel[3];
	  LPSTR szQuantity = GetLP(14);
	  LPSTR szKousei = GetLP(14);
	  LPSTR szSum = GetLP(14);
	  HWND hList = GetDlgItem(SCSDef->HWindow,ID_DISTLIST);
	  itoa(iLevel,szLevel,10);
	  double dSum = 0;
          //FloatToFString(szQuantity,CostStr->dQuantity(),4,NULL,NULL);
	  gcvt(CostStr->dQuantity(),10,szQuantity); // Parent Lot size - if no parent then current	  
	  gcvt(CostStr->dKousei(),10,szKousei); // Current kousei amount
	  double dRatio = (dRequirement/CostStr->dQuantity());
	  dSum = (Resrc->dRate() * dRatio * CostStr->dKousei());
	  double dTotal = Resrc->dRate() * CostStr->dKousei();
	  gcvt(dTotal,10,szSum);
	  if(iLevel == 1)
          {
	   SSSetData(hList,1,iLine,szLevel);
	   SSSetData(hList,2,iLine,Resrc->fCode->str());
	   SSSetData(hList,3,iLine,Resrc->fElement->str());
	   SSSetData(hList,4,iLine,Resrc->fUnit->str());
	   SSSetData(hList,5,iLine,Resrc->fType->str());
	   SSSetData(hList,6,iLine,CostStr->fSeqno->str());
	   SSSetData(hList,7,iLine,szQuantity);
	   SSSetData(hList,8,iLine,szKousei);
	   SSSetData(hList,9,iLine,Resrc->fRate->str());
	   SSSetData(hList,10,iLine,szSum);
           iLine++;
	  }
	  else
	  if(bMultiLevel)
	  {
	   SSSetData(hList,1,iLine,szLevel);
	   SSSetData(hList,2,iLine,Resrc->fCode->str());
	   SSSetData(hList,3,iLine,Resrc->fElement->str());
	   SSSetData(hList,4,iLine,Resrc->fUnit->str());
	   SSSetData(hList,5,iLine,Resrc->fType->str());
	   SSSetData(hList,6,iLine,CostStr->fSeqno->str());
	   SSSetData(hList,7,iLine,szQuantity);
	   SSSetData(hList,8,iLine,szKousei);
	   SSSetData(hList,9,iLine,Resrc->fRate->str());
	   SSSetData(hList,10,iLine,szSum);
           iLine++;
	  }
	  dStandard = dStandard + dSum;
       }

void TResourceUpdate::SetResource()
{

 HWND hList = GetDlgItem(SCSDef->HWindow,ID_DISTLIST);
 char szCostItem[15];
 char szRescode[11];
 char szCostType[4];
 Str4ten CostType;
 CostType.assign(GetField(ID_COSTTYPE));
 CostType.trim();
 strcpy(szCostType,CostType.str());
 strcpy(szRescode,GetField(ID_CODE));
 strcpy(szCostItem,szCostType);
 strcat(szCostItem,szRescode);
 CostStr->SetIndex("fopcode");
 if(*szRescode == 0)
 {  
  BWCCMessageBox(HWindow,"Enter a item code.","Resource Code",MB_OK);
  return;
 }
 ClearResource();

 if(CostStr->seek(szCostItem) == 0)
 {  
  SSSetBool(hList,SSB_REDRAW,FALSE);
  dStandard = 0.0000;
  iLine = 1;
  Items->seek(szRescode);
  int i = GetResource(szCostItem,szCostType,1,1,1);
  if (i > 10)
  SSSetMaxRows(hList,i);
  else
  SSSetMaxRows(hList,10);
  Items->seek(szRescode);
  UpdateWindow();
  SSSetBool(hList,SSB_REDRAW,FALSE);
 }
 return;
}
#pragma argsused
void TResourceUpdate::SetManufacturedValues(int iLevel,int i,char *szRescode,double UpperKousei,double UpperLotsize)
{
   HWND hList = GetDlgItem(SCSDef->HWindow,ID_DISTLIST);
   char szLevel[4];
   LPSTR szUpperLot = GetLP(14);
   LPSTR szUpperKousei = GetLP(14);
   Items->seek(szRescode);
   itoa(iLevel,szLevel,10);
   if(iLevel == 1)
   {
    SSSetData(hList,1,iLine,szLevel);
    SSSetData(hList,2,iLine,szRescode);
    SSSetData(hList,3,iLine,"Man. Item");
    SSSetData(hList,5,iLine,Items->fPtype->str());
    SSSetData(hList,6,iLine,CostStr->fSeqno->str());    
     StringToFloat(szUpperLot,&UpperLotsize);
    //SSSetData(hList,7,iLine,szUpperLot);
     StringToFloat(szUpperKousei,&UpperKousei);
    //SSSetData(hList,8,iLine,szUpperKousei);
    iLine++;
   }
   else
   if(bMultiLevel)
   {
    SSSetData(hList,1,iLine,szLevel);
    SSSetData(hList,2,iLine,szRescode);
    SSSetData(hList,3,iLine,"Man. Item");
    SSSetData(hList,5,iLine,Items->fPtype->str());
    SSSetData(hList,6,iLine,CostStr->fSeqno->str());
     StringToFloat(szUpperLot,&UpperLotsize);
    //SSSetData(hList,7,iLine,szUpperLot);
     StringToFloat(szUpperKousei,&UpperKousei);
    // SSSetData(hList,8,iLine,szUpperKousei);
    iLine++;
   }
  }


void TResourceUpdate::SearchItemNo (RTMessage)
{
 ClearResource();


 if(bSearch)
 { 
   ITEM = new TSCItemList(this,"PICKLIST","FPTYPE = 'M ' .OR. FPTYPE= 'B ' .OR. FPTYPE= 'A '",Items);
   ITEM->EnableAutoCreate();
   GetApplication()->MakeWindow(ITEM);
   bSearch = FALSE;
 }
 if(!IsWindowVisible(ITEM->HWindow))
   ITEM->Show(SW_SHOW);
 else
   ITEM->Show(SW_HIDE);
}      

void TResourceUpdate::SearchResource (RTMessage)
{
 if(bResource)
  {
   char szQuery[45];
   strcpy(szQuery,"FCOSTTYPE = '");
   strcat(szQuery,GetField(ID_COSTTYPE));
   strcat(szQuery,"'");
   SEARCH = new TSCResList(this,"RESLIST",szQuery,Resrc);
   SEARCH->EnableAutoCreate();
   GetApplication()->MakeWindow(SEARCH);
   bResource = FALSE;
  }
 if(!IsWindowVisible(SEARCH->HWindow))
   SEARCH->Show(SW_SHOW);
 else
   SEARCH->Show(SW_HIDE);
}      


void TSCtrstrList::SetupWindow ()
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
 List->SetMaxRows(trstr->recCount());
 List->SetData(1,SS_HEADER, "Code");
 List->SetData(2,SS_HEADER, "Name");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 long i = 1;
 rQuery = new Relate4set(trstr);
 rQuery->querySet(szQuery);
 for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
  {
   List->SetData(1,i,trstr->fOpcode->str());
   List->SetData(2,i,trstr->fSeqno->str());
   List->SetData(3,i,trstr->fKpcode->str());
   i++;
   trstr->skip(1);
  }

 rQuery->unlock();
 rQuery->free(0);
 delete rQuery;
 List->SetMaxRows(i);
 SetCaption("Standard Cost Master List");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TResourceUpdate::ClearResource ()
{
    SendDlgItemMsg(ID_RESCODE     ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_NAME        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_RESNAME     ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_USER        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_DATE        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_USERUPDATE  ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_DATEUPDATED ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_UNIT        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_RESELEMENT  ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_TYPE        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_RATE        ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_QUANTITY    ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SendDlgItemMsg(ID_SEQNO,      WM_SETTEXT,NULL,(LONG)(LPSTR)"");
    SSClearData(GetDlgItem(SCSDef->HWindow,ID_DISTLIST),SS_ALLCOLS,SS_ALLROWS);
}

void TSCtrstrList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(trstr->seek(szItem) == 0)
  {
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fOpcode->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_SEQNO)      ,WM_SETTEXT,NULL,(LONG)trstr->fSeqno->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fKpcode->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fStqty->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fKousei->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fLt1->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fLt2->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fComm->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fCorectm->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fSpcrtp->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)       ,WM_SETTEXT,NULL,(LONG)trstr->fChk_flg->str());
  }
  else
 BWCCMessageBox(HWindow,"Only resource not manufactured items may be modified.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(GetDlgItem(Parent->HWindow,ID_CODE));
}

void TSCSDefinition::SetupWindow()
{
 
 TDialog::SetupWindow();
 //TWindow::SetupWindow();
 HourGlassOn();
 RECT rc;
 SS_CELLTYPE 	CellType;

 GetClientRect(HWindow,&rc);  
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER)-2,GetSystemMetrics(SM_CYBORDER));
 MoveWindow(List->HWindow,rc.left, rc.top, rc.right - rc.left-1, rc.bottom - rc.top, TRUE);
 short d0Width,d1Width,d2Width,d3Width,d4Width,d5Width,d6Width,d7Width,d8Width,d9Width;

 List->SetBool(SSB_ALLOWUSERSELBLOCK,TRUE);
 List->SetColWidth(1,3);
 List->SetColWidth(2,7);
 List->SetColWidth(3,7);
 List->SetColWidth(4,5);
 List->SetColWidth(5,5);
 List->SetColWidth(6,6);
 List->SetColWidth(7,12);
 List->SetColWidth(8,12);
 List->SetColWidth(9,12);


 List->SetMaxCols(10);
 List->GetColWidthInPixels(0,(LPSHORT)&d0Width);
 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 List->GetColWidthInPixels(3,(LPSHORT)&d3Width);
 List->GetColWidthInPixels(4,(LPSHORT)&d4Width);
 List->GetColWidthInPixels(5,(LPSHORT)&d5Width);
 List->GetColWidthInPixels(6,(LPSHORT)&d6Width);
 List->GetColWidthInPixels(7,(LPSHORT)&d7Width);
 List->GetColWidthInPixels(8,(LPSHORT)&d8Width);
 List->GetColWidthInPixels(9,(LPSHORT)&d9Width);
 short ColWidth = (rc.right - rc.left)
		- d0Width
		- d1Width
		- d2Width
		- d3Width
		- d4Width
		- d5Width
		- d6Width
		- d7Width
		- d8Width
		- d9Width
		- GetSystemMetrics(SM_CXHSCROLL);
 List->SetColWidthInPixels(10,ColWidth);
 int l = 1;
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_LEVEL));
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_RESOURCECODE));
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_ELEMENTCODE));
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_UNIT));
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_TYPE));
 List->SetData(l++,SS_HEADER, "Seq No.");
 List->SetData(l++,SS_HEADER, "Lot Size");
 List->SetData(l++,SS_HEADER, "Quantity"); 
 List->SetData(l++,SS_HEADER, GetString(GetApplication()->hInstance,ID_RATE));
 List->SetData(10,SS_HEADER, "Total");
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 List->SetTypeFloat(&CellType,FS_SEPARATOR,10,3,0,999999999999.999);
 List->SetCellType(7, SS_ALLROWS, &CellType);
 List->SetCellType(8, SS_ALLROWS, &CellType);
 List->SetCellType(9, SS_ALLROWS, &CellType);
 List->SetCellType(10, SS_ALLROWS, &CellType);

 HourGlassOff();
}


void TSCSDefinition::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 char szCostType[4];
 char szSeek[15];
 char szSeqno[4];
 char szResType[11];
 char szLevel[4];
 
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam; 
 List->GetData(1,Cell->Row,szLevel);
 List->GetData(2,Cell->Row,szItem);
 List->GetData(3,Cell->Row,szResType);
 List->GetData(6,Cell->Row,szSeqno);
 
 SendMessage(GetDlgItem(ResUpdate->HWindow,ID_COSTTYPE),WM_GETTEXT,sizeof(szCostType),(LPARAM)(LPSTR)szCostType);
 strcpy(szSeek,szItem);
 strcat(szSeek,szCostType);
 //strcat(szSeek,GetField(ID_COSTTYPE));
 int iLevel = atoi(szLevel);

 if (iLevel > 1 )
 {
  BWCCMessageBox(HWindow,"Only level one resources may be modified.","Manufactured Items",MB_ICONEXCLAMATION);
  return;
 } 

 if(Resrc->seek(szSeek) == 0)
  {
    SetField(ID_RESCODE,     Resrc->fCode);
    SetField(ID_NAME,        Resrc->fShortName);
    SetField(ID_RESNAME,     Resrc->fName);
    SetField(ID_USER,        Resrc->fUser);
    SetField(ID_DATE,        Resrc->fDate);
    SetField(ID_USERUPDATE,  Resrc->fUpdateuser);
    SetField(ID_DATEUPDATED, Resrc->fUpdatedate);
    SetField(ID_UNIT,        Resrc->fUnit);
    SetField(ID_RESELEMENT,  Resrc->fElement);
    SetField(ID_TYPE,        Resrc->fType);
    SetField(ID_SEQNO,       szSeqno);
    SetField(ID_RATE,        Resrc->fRate);
    PTElement Element = new TElement(&cb);
    Element->seek(Resrc->fElement->str());
    SetField(ID_JDESCRIPT,Element->fName);
    Element->close();
  }
 else
  {

   Resrc->top();
   return;
  } 
 ResUpdate->CalQuantity(Msg);
 SetFocus(ResUpdate->HWindow);
}


