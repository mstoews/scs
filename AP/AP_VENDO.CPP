/*
   General Ledger  - (C) Copyright 1992 by Tegara Corporation
   Modify, add and delete vendor AP accounts functions.
*/
#include "ap.h"
#include <math.h>
#include "db_vend.h"
#include "gl_dlg.h"
#include "ap_vendl.h"

extern Code4 cb;
static int iVendorList;


void TAPVendorList::SetParentField(int ID, Field4 *field)
{
 SendDlgItemMessage(Parent->HWindow,ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str()); 
}

void TAPVendorList::SetList()
{

    HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
    MessageBox(HWindow,"","2",MB_OK);

    SSSetBool(SHWindow,SSB_REDRAW,FALSE);
    SSClearData(SHWindow,SS_ALLCOLS,SS_ALLROWS);

    Relate4set *rQuery = new Relate4set(apvendor);
    rQuery->querySet("VENDORNO <> ' '");
    int i = 1;
    for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
    {
     SSSetData(SHWindow,1,i,apvendor->vendorno->str());
     SSSetData(SHWindow,2,i,apvendor->vdname->str());
     i++;
    } 
    rQuery->unlock();
    rQuery->free();
    SSSetBool(SHWindow,SSB_REDRAW,TRUE);
    iVendorList = (int)i-1;
}


_CLASSDEF (TAPVendorAdd)
class TAPVendorAdd : public TMainDialog
{
 public:
 BOOL bVendorList;
 PTAPVendorList VendorList;
 PTapvendor apvendor;
 PTglaccts  glaccts;
 int iVat; 
 TAPVendorAdd(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL): TMainDialog (AParent,AName)
 {
 };
 virtual void SetupWindow();
 virtual void Search         (RTMessage Msg) = [ID_FIRST + ID_SEARCHCUST];
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
 virtual void HandleVat      (RTMessage Msg) = [ID_FIRST + ID_VAT];
 virtual void Next	     (RTMessage Msg) = [ID_FIRST + ID_NEXT];
 virtual void Previous	     (RTMessage Msg) = [ID_FIRST + ID_PREVIOUS];
 virtual void SetData	     (RTMessage Msg) = [ID_FIRST + ID_SETDATA];
 virtual void SetDetail      (RTMessage Msg) = [CM_FIRST + CM_SETVENDOR]{SetData(Msg);};
 virtual void New            (RTMessage Msg) = [ID_FIRST + CM_NEW];
 virtual void Clear          (RTMessage Msg) = [CM_FIRST + CM_NEW]{ New(Msg); };
};

void TAPVendorAdd::SetData(RTMessage)
{
 if(apvendor->seek(GetField(ID_VENDORNO) == 0))
 {
  SetField(ID_VENDORNO       , apvendor->vendorno );
  SetField(ID_VENDORSHORT    , apvendor->vdshort );
  SetField(ID_VENDORNAME     , apvendor->vdname );
  SetField(ID_VENDORADDRESS1 , apvendor->address1 );
  SetField(ID_VENDORADDRESS2 , apvendor->address2 );
  SetField(ID_VENDORADDRESS3 , apvendor->address3 );
  SetField(ID_VENDORPOST     , apvendor->postal );
  SetField(ID_VENDORCONTACT  , apvendor->contact );
  SetField(ID_VENDORREF      , apvendor->ref );
  SetField(ID_VENDORSTATUS   , apvendor->status );
  SetField(ID_TERMS          , apvendor->terms );
  SetField(ID_VATPERCENTAGE  , apvendor->percentage );
  SetField(ID_REBATE         , apvendor->rebate );
  SetField(ID_PHONE	     , apvendor->phone );
  SetField(ID_FAX	     , apvendor->fax );
  SetField(ID_APCONTROL	     , apvendor->account );
  SetField(ID_APDEPART       , apvendor->depart );
  if(strcmp(apvendor->type->str(),"C") == 0 )
  SendDlgItemMsg(ID_COMPANY,BM_SETCHECK,TRUE,NULL);
  else
  SendDlgItemMsg(ID_INDIVIDUAL,BM_SETCHECK,TRUE,NULL);
 }
else
BWCCMessageBox(HWindow,"Vendor Number not found.","Vendor Update",MB_OK);

}

void TAPVendorAdd::Previous(RTMessage)
{
 if(*GetField(ID_VENDORNO) == 0)
 {
  apvendor->top();
  SendMessage(HWindow,WM_COMMAND,CM_SETVENDOR,NULL);
  return;
 }

 if(apvendor->seek(GetField(ID_VENDORNO)) == 0 && !apvendor->bof())
 {
  apvendor->skip(-1);
  SetField(ID_VENDORNO, apvendor->vendorno);
  SendMessage(HWindow,WM_COMMAND,CM_SETVENDOR,NULL);
 }
 
}

void TAPVendorAdd::Next(RTMessage)
{
 if(*GetField(ID_VENDORNO) == 0)
 {
  apvendor->top();
  SetField(ID_VENDORNO, apvendor->vendorno);
  SendMessage(HWindow,WM_COMMAND,CM_SETVENDOR,NULL);
  return;
 }
 if(apvendor->seek(GetField(ID_VENDORNO)) == 0)
 {
  apvendor->skip();
  SetField(ID_VENDORNO, apvendor->vendorno);
  SendMessage(HWindow,WM_COMMAND,CM_SETVENDOR,NULL);
 } 
}


_CLASSDEF (TAPVendorMod)
class TAPVendorMod : public TAPVendorAdd
{
 public:
 TAPVendorMod(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL):TAPVendorAdd(AParent,AName){};
 virtual void SetupWindow();
};

_CLASSDEF (TAPVendorDel)
class TAPVendorDel : public TAPVendorAdd
{
 public:
 TAPVendorDel(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL):TAPVendorAdd(AParent,AName){};
 virtual void SetupWindow();
 virtual void Ok            (RTMessage Msg)  = [ID_FIRST + IDOK];
};








TAPVendorList::~TAPVendorList()
{
 delete ListArray;
}


void TAPVendorList::HandleListBox (RTMessage Msg)
{
  char szSeek[12];
  LPSS_CELLCOORD Cell = (LPSS_CELLCOORD) Msg.LParam;
  SSGetData(GetDlgItem(HWindow,ID_DISTLIST),1,Cell->Row,szSeek);
  
  if(apvendor->seek(szSeek) == 0)
  {  
   SetField(ID_VENDORNO       , apvendor->vendorno );
   SetField(ID_VENDORSHORT    , apvendor->vdshort );
   SetField(ID_VENDORNAME     , apvendor->vdname );
   SetField(ID_VENDORADDRESS1 , apvendor->address1 );
   SetField(ID_VENDORADDRESS2 , apvendor->address2 );
   SetField(ID_VENDORADDRESS3 , apvendor->address3 );
   SetField(ID_VENDORPOST     , apvendor->postal );
   SetField(ID_VENDORCONTACT  , apvendor->contact );
   SetField(ID_VENDORREF      , apvendor->ref );
   SetField(ID_VENDORSTATUS   , apvendor->status );
   SetField(ID_TERMS          , apvendor->terms );
   SetField(ID_VATPERCENTAGE  , apvendor->percentage );
   SetField(ID_REBATE         , apvendor->rebate );
   SetField(ID_PHONE	      , apvendor->phone );
   SetField(ID_FAX	      , apvendor->fax );
   SetField(ID_APCONTROL      , apvendor->account );
   SetField(ID_APDEPART       , apvendor->depart );
   SetFocus(Parent->HWindow);
   if(strcmp(apvendor->type->str(),"C") == 0 )
     {
      SendDlgItemMessage(Parent->HWindow,ID_COMPANY,BM_SETCHECK,TRUE,NULL);
      SendDlgItemMessage(Parent->HWindow,ID_INDIVIDUAL,BM_SETCHECK,FALSE,NULL);
     }
   else
     {
      SendDlgItemMessage(Parent->HWindow,ID_INDIVIDUAL,BM_SETCHECK,TRUE,NULL);
      SendDlgItemMessage(Parent->HWindow,ID_COMPANY,BM_SETCHECK,FALSE,NULL);
     }
   SendMessage(Parent->HWindow,WM_COMMAND,CM_SETVENDOR,NULL);
   }
  SetFocus(GetDlgItem(Parent->HWindow,ID_INVOICENO));
  
}


void TAPVendorList::SetupWindow()
{
TDialog::SetupWindow();
SetListSize(8);
RECT rc;
SS_CELLTYPE 	CellType;
HourGlassOn();
HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
GetClientRect(HWindow, &rc);

short dWidth;
SSSetColWidth(SHWindow,1,9);
SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&dWidth);
short ColWidth = rc.right - rc.left - dWidth - GetSystemMetrics(SM_CXHSCROLL);
SSSetColWidthInPixels(SHWindow,2,ColWidth);
 long RecTotal = apvendor->recCount();
 if(RecTotal < 8)
  RecTotal = 8;
 
SSSetMaxCols(SHWindow, 2);
SSSetMaxRows(SHWindow,RecTotal);

SSSetData(SHWindow,1,SS_HEADER, "Vendor No.");
SSSetData(SHWindow,2,SS_HEADER, "Vendor Name");
SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);
SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
SSSetCellType(SHWindow,SS_ALLCOLS, SS_ALLROWS, &CellType);

apvendor->SetIndex("vendorno");
HourGlassOff();
} 


void TAPVendorAdd::HandleVat (RTMessage)
{
 if(SendDlgItemMsg(ID_VAT,BM_GETCHECK,NULL,NULL))
  {
   iVat = 1;
  }
 else
  {
   iVat = 0;
  }
}


void TAPVendorAdd::Search  (RTMessage)
{
 if(bVendorList)
  {
    HWND SHWindow = GetDlgItem(VendorList->HWindow,ID_DISTLIST);
    SSSetBool(SHWindow,SSB_REDRAW,FALSE);
    SSClearData(SHWindow,SS_ALLCOLS,SS_ALLROWS);

    Relate4set *rQuery = new Relate4set(apvendor);
    rQuery->querySet("VENDORNO <> ' '");
    int i = 1;
    for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
    {
     SSSetData(SHWindow,1,i,apvendor->vendorno->str());
     SSSetData(SHWindow,2,i,apvendor->vdname->str());
     i++;
    } 
    rQuery->unlock();
    rQuery->free();
    SSSetBool(SHWindow,SSB_REDRAW,TRUE);
    iVendorList = (int)i-1;
  }
 if(!IsWindowVisible(VendorList->HWindow))
   {
    VendorList->Show(SW_SHOW);
    SetField(ID_SEARCHCUST,"&Hide Search");
   }
 else
   {
    VendorList->Show(SW_HIDE);
    SetField(ID_SEARCHCUST,"&Search");
   }

}

void TAPVendorAdd::SetupWindow()
{
 TDialog::SetupWindow();
 apvendor = new Tapvendor(&cb,"ap_vend");
 glaccts  = new Tglaccts(&cb,"glaccts");
 apvendor->SetIndex("vendorno");
 VendorList = new TAPVendorList(this,"AP_VENDORLIST",apvendor);
 VendorList->EnableAutoCreate();
 GetApplication()->MakeWindow(VendorList);
 iVat = 0;
 bVendorList = TRUE;
}

void TAPVendorMod::SetupWindow()
{
 TDialog::SetupWindow();
 apvendor = (Tapvendor *) new Tapvendor(&cb,"ap_vend");
 VendorList = new TAPVendorList(this,"AP_VENDORLIST",apvendor);
 VendorList->EnableAutoCreate();
 GetApplication()->MakeWindow(VendorList);
 SetWindowText(HWindow,"Accounts Payable Vendor Maintenance - Modify");
 SetField(IDOK,"&Modify");
}

void TAPVendorDel::SetupWindow()
{
 TDialog::SetupWindow();
 apvendor = (Tapvendor *) new Tapvendor(&cb,"ap_vend");
 VendorList = new TAPVendorList(this,"AP_VENDORLIST",apvendor);
 VendorList->EnableAutoCreate();
 GetApplication()->MakeWindow(VendorList);
 SetWindowText(HWindow,"Accounts Payable Vendor Maintenance - Delete");
 SetField(IDOK,"&Delete");
}

void TAPVendorDel::Ok (RTMessage)
{
 if(apvendor->eof())
 return;
 if(BWCCMessageBox(HWindow,"Do you want to delete this vendor?",GetString(ID_WARNING),MB_YESNO|MB_ICONQUESTION)==IDYES)
  {
  apvendor->deleteRec();
  apvendor->pack();

 
  HWND hwnd = GetDlgItem(VendorList->HWindow,ID_DISTLIST);  
  SSClearData(hwnd,SS_ALLCOLS,SS_ALLROWS);
  apvendor->top();
  if(apvendor->eof())
   return;
  long i;
  i=1;
  SSSetBool(hwnd,SSB_REDRAW,FALSE);
  while (!apvendor->eof())
   {
   SSSetData(hwnd,1,i,apvendor->vendorno->str());
   SSSetData(hwnd,2,i,apvendor->vdname->str());
   i=i+1;
   apvendor->skip(1);
   }
   iVendorList = (int) i-1;
  SSSetBool(hwnd,SSB_REDRAW,TRUE);
  }
 SendMessage(HWindow,WM_COMMAND,CM_NEW,NULL);
}


void TAPVendorAdd::Ok             (RTMessage)
{

 if(*GetField(ID_VENDORNO) == 0)
 {
 BWCCMessageBox(HWindow,"Enter an vendor number.",GetString(ID_WARNING),MB_OK|MB_ICONEXCLAMATION);
 SetFocus(GetDlgItem(HWindow,ID_VENDORNO));
 return;
 }

 
 if(*GetField(ID_VENDORSHORT) == 0)
 {
 BWCCMessageBox(HWindow,"Enter a vendor short name.",GetString(ID_WARNING),MB_OK|MB_ICONEXCLAMATION);
 SetFocus(GetDlgItem(HWindow,ID_VENDORSHORT));
 return;
 }


 if(*GetField(ID_VENDORNAME) == 0)
 {
 BWCCMessageBox(HWindow,"Enter a vendor name.",GetString(ID_WARNING),MB_OK|MB_ICONEXCLAMATION);
 SetFocus(GetDlgItem(HWindow,ID_VENDORNAME));
 return;
 }

 char szType[2];
 if(SendDlgItemMsg(ID_INDIVIDUAL,BM_GETSTATE,NULL,NULL))
    strcpy(szType,"I");
 else
 if(SendDlgItemMsg(ID_COMPANY,BM_GETSTATE,NULL,NULL))
     strcpy(szType,"C");

 if(apvendor->seek(GetField(ID_VENDORNO))==0)
 {
   if(BWCCMessageBox(HWindow,"Vendor account has already been entered.\nDo you want to update it?",GetString(ID_WARNING),MB_YESNO|MB_ICONQUESTION)==IDYES)
   {
     apvendor->vdshort->assign(GetField(ID_VENDORSHORT));
     apvendor->vdname->assign(GetField(ID_VENDORNAME));
     apvendor->address1->assign(GetField(ID_VENDORADDRESS1));
     apvendor->address2->assign(GetField(ID_VENDORADDRESS2));
     apvendor->address3->assign(GetField(ID_VENDORADDRESS3));
     apvendor->postal->assign(GetField(ID_VENDORPOST));
     apvendor->contact->assign(GetField(ID_VENDORCONTACT));
     apvendor->phone->assign(GetField(ID_PHONE));
     apvendor->fax->assign(GetField(ID_FAX));
     apvendor->ref->assign(GetField(ID_REFERENCE));
     apvendor->type->assign(szType);
     apvendor->terms->assignDouble(dGetField(ID_TERMS));
     apvendor->account->assign(GetField(ID_APCONTROL));
     apvendor->depart->assign(GetField(ID_APDEPART));
     apvendor->percentage->assignDouble(dGetField(ID_VATPERCENTAGE));
     apvendor->rebate->assignDouble(dGetField(ID_REBATE));
     apvendor->flush();
     bVendorList = TRUE;
   }
   return;
 }


     apvendor->appendStart();
     apvendor->vendorno->assign(GetField(ID_VENDORNO));
     apvendor->vdshort->assign(GetField(ID_VENDORSHORT));
     apvendor->vdname->assign(GetField(ID_VENDORNAME));
     apvendor->address1->assign(GetField(ID_VENDORADDRESS1));
     apvendor->address2->assign(GetField(ID_VENDORADDRESS2));
     apvendor->address3->assign(GetField(ID_VENDORADDRESS3));
     apvendor->postal->assign(GetField(ID_VENDORPOST));
     apvendor->contact->assign(GetField(ID_VENDORCONTACT));
     apvendor->phone->assign(GetField(ID_PHONE));
     apvendor->fax->assign(GetField(ID_FAX));
     apvendor->ref->assign(GetField(ID_REFERENCE));
     apvendor->type->assign(szType);
     apvendor->terms->assignDouble(dGetField(ID_TERMS));
     apvendor->account->assign(GetField(ID_APCONTROL));
     apvendor->depart->assign(GetField(ID_APDEPART));
     apvendor->percentage->assignDouble(dGetField(ID_VATPERCENTAGE));
     apvendor->rebate->assignDouble(dGetField(ID_REBATE));     
     apvendor->append();

     HWND hwnd = GetDlgItem(VendorList->HWindow,ID_DISTLIST);
    iVendorList++;
    if(iVendorList > 6)
    SSInsRow(hwnd,iVendorList);
    SSSetData(hwnd,1,iVendorList,apvendor->vendorno->str());
    SSSetData(hwnd,2,iVendorList,apvendor->vdname->str());
    SendMessage(HWindow,WM_COMMAND,CM_NEW,NULL); 

}

void TAPVendorAdd::New (RTMessage)
{
 SetField(ID_VENDORNO       , "" );
 SetField(ID_VENDORSHORT    , "" );
 SetField(ID_VENDORNAME     , "" );
 SetField(ID_VENDORADDRESS1 , "" );
 SetField(ID_VENDORADDRESS2 , "" );
 SetField(ID_VENDORADDRESS3 , "" );
 SetField(ID_VENDORPOST     , "" );
 SetField(ID_VENDORTEL      , "" );
 SetField(ID_VENDORCONTACT  , "" );
 SetField(ID_VENDORREF      , "" );
 SetField(ID_VENDORSTATUS   , "" );
 SetField(ID_VATPERCENTAGE  , "" );
 SetField(ID_TERMS          , "" );
 SetField(ID_REBATE         , "" );
 SetField(ID_PHONE          , "" );
 SetField(ID_FAX            , "" );
 SetField(ID_APCONTROL      , "" );
 SetField(ID_APDEPART       , "" );
}

     
void TAPVendorAdd::Cancel         (RTMessage)
{
 cb.closeAll();
 ShutDownWindow();
}




     
// Maintain Vendor Accounts
void TGlWindow::APAddVendors  (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPVendorAdd(this,"AP_ADDVENDORS"));
 }
void TGlWindow::APModVendors  (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPVendorMod(this,"AP_ADDVENDORS"));
 }

void TGlWindow::APDelVendors  (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPVendorDel(this,"AP_ADDVENDORS"));
 }
