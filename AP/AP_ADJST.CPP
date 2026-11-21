#include "ap.h"
#include "db_dept.h"
#include "db_apbtc.h"
#include "db_cash.h"
#include "ap_class.h"
#include "math.h"

_CLASSDEF(TAPAdjEntry)
class TAPAdjEntry : public TAPInvoiceEntry
{
 public:
  BOOL bRedraw;
  BOOL bDeleted;
  BOOL bRecovered;
  PTAPInvoiceList InvoiceList;
  PTgldist gldist;
  PTglaccts glaccts;
  PTapdist  apdist;
  PTapsetup  apsetup;
  TAPAdjEntry(PTWindowsObject AParent,LPSTR ATitle):TAPInvoiceEntry(AParent,ATitle){};
  virtual LPSTR GetClassName(){ return "APAdjEntry"; }
  virtual void SetupWindow();
  virtual void SList (RTMessage Msg) = [ID_FIRST + ID_SLIPLIST];
  virtual void CurBatch        (RTMessage Msg) = [CM_FIRST + CM_BATCH];
  virtual void Ok              (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void CreateAdjustment(RTMessage Msg) = [ID_FIRST + ID_CREATEADJ];
  virtual void RebateCalc      (RTMessage Msg) = [ID_FIRST + ID_ADJUSTMENT];
  void RecoverPostedInvoice();
  void SetPeriodAmount(double period,char *szAccountNo,double dBatchNo);
  void CreateRebateAdjustment();
  void UpdateBatchDetail(long iBatchno, char *szDescript, char *szAccount,char *szDepart,double dEntry);
  void UpdateBatchHeader(long iBatchno, double AMOUNT, double VAT, double REBATE);
};





static COLORREF lpBack, lpFore;

void TAPAdjEntry::CreateAdjustment (RTMessage Msg)
{
  // Check to see if the invoice has already been entered.
 if(lGetField(ID_BATCHNO) == 0) 
 {
  apbtch->SetIndex("invoiceno");
  char *szInvoiceno = strcpy(new char[apbtch->invoiceno->len()+ apbtch->vendorno->len() + 1],GetField(ID_VENDORNO));
  strcat(szInvoiceno,GetField(ID_INVOICENO));                  
  if(apbtch->seek(szInvoiceno) == 0)
  {
    //DlgMsg(ID_INVOICECREATED,ID_ADJUSTMENT,MB_ICONEXCLAMATION);
    SetField(IDD_STATUSBOX,GetString(ID_INVOICECREATED));
    { 
     CurBatch(Msg);
     return;
    }
  }
 }

 if(lGetField(ID_BATCHNO) > 0 && strcmp(GetField(IDOK),GetString(ID_MODIFY)) != 0)
 {
  apbtch->SetIndex("invoiceno");
  char *szInvoiceno = strcpy(new char[apbtch->invoiceno->len()+ apbtch->vendorno->len() + 1],GetField(ID_VENDORNO));
  strcat(szInvoiceno,GetField(ID_INVOICENO));                  
  if(apbtch->seek(szInvoiceno) == 0)
  {
    if(DlgMsg(ID_INVOICECREATED,ID_ADJUSTMENT,MB_YESNO | MB_ICONQUESTION) == IDYES)
    { 
     CurBatch(Msg);
     return;
    }
  }
 }


  
 apbtch->SetIndex("batchno");

 szAdj = strcpy(new char[apbtch->adj->len() + 1],"A");
 StrTrim(szAdj);

 SendDlgItemMsg(ID_DISTLIST, WM_SETREDRAW, FALSE, 0L);
 iPeriodNo = (int)lGetField(ID_PERIOD);
 szVendorNo = strcpy(new char[apbtch->vendorno->len() + 1],GetField(ID_VENDORNO));
 if(*szVendorNo==0)
   {
    CheckError(ID_VENDORNO);
    return;
   }

 if(apvendor->seek(szVendorNo)!=0)
  {
   MessageBeep(MB_ICONEXCLAMATION);
   if(IsWindowVisible(VendorList->HWindow))
    VendorList->Show(SW_HIDE);
   else
    VendorList->Show(SW_SHOW);
   SetFocus(VendorList->HWindow);
   return;
  }

 szInvoiceNo = strcpy(new char[apbtch->invoiceno->len() + 1], GetField(ID_INVOICENO));
 if(*szInvoiceNo==0)
  {
   CheckError(ID_INVOICENO);
   return;
  }

 szInvoiceDate = strcpy(new char[apbtch->date->len() + 1],GetField(ID_INVDATE));
 if(*szInvoiceDate==0)
  {
   CheckError(ID_INVDATE);
   return;
  }
 InvoiceDate.assign(szInvoiceDate,"MM/DD/YY");


 szDueDate = strcpy(new char[apbtch->date->len() + 1],GetField(ID_DUEDATE));
 if(*szDueDate==0)
   {
    CheckError(ID_DUEDATE);
    return;
   }
 DueDate.assign(szDueDate,"MM/DD/YY");

 dTerms = dGetField(ID_TERMS);
 long lDueDate = long (InvoiceDate) + dTerms;
 DueDate.assign(lDueDate);

 szOrderNo = strcpy(new char[apbtch->orderno->len() + 1],GetField(ID_CURRBAL));

 szReference = strcpy(new char[apbtch->reference->len() + 1],GetField(ID_REFERENCE));
 if(*szReference==0) {
   CheckError(ID_REFERENCE);
   return;
 }

 szDescript = strcpy(new char[apbtch->descript->len() + 1],GetField(ID_JDESCRIPT));
 if(*szDescript == 0)
 {
  CheckError(ID_JDESCRIPT);
  return;
 }

 szDepart = strcpy(new char[apbtch->depart->len() + 1],GetField(ID_DEPART));
 szAccounts = strcpy(new char[apbtch->account->len() + 1],GetField(ID_ACCOUNTS));
 char *szSeek = new char[apbtch->account->len() + apbtch->depart->len() + 1];
 strcpy(szSeek,szAccounts);
 strcat(szSeek,szDepart);
 if(*szAccounts==0){CheckError(ID_ACCOUNTS);return;}
 if(!CheckGLAccount(szSeek)){SearchGL(Msg);SetFocus(GetDlgItem(HWindow,ID_ACCOUNTS));return;}


 iLineno = lGetField(ID_LINENO);

 lInvoiceAmount = dGetField(ID_INVOICE);
 if(lInvoiceAmount == 0 ){CheckError(ID_INVOICE);return;}


 dVatPercentage = dGetField(ID_VATPERCENTAGE);
 dRebatePercentage = dGetField(ID_REBATE);
 iBatchno = (int)lGetField(ID_BATCHNO);

  if(iBatchno != 0)
   {
     if(apbtch->lockTestFile() > 0)
       SetField(IDD_STATUSBOX,GetString(ID_BATCHSTATUS));
     if(apbtch->lockAll() != 0)
      {       
       SetField(IDD_STATUSBOX,GetString(ID_TRYAGAIN));
       return;
      }     
     if(apbtchd->lockAll() !=0 )
      {
      SetField(IDD_STATUSBOX,GetString(ID_TRYAGAIN));
       return;
      }

     if(dGetField(ID_ADJUSTMENT) != 0)
      lInvoiceAmount = dGetField(ID_ADJUSTMENT);

    if(dVatPercentage > 0 && dRebatePercentage == 0)
     CreateVatSlip(apbtch,apbtchd,D);
    else if(dVatPercentage > 0 && dRebatePercentage > 0)
     CreateVat_RebateSlip(apbtch,apbtchd,D);
    else if (dVatPercentage == 0 && dRebatePercentage > 0)
     CreateRebateSlip(apbtch,apbtchd,D);
    else
     CreateNewApSlip(apbtch,apbtchd,D);   
     SetField(IDD_STATUSBOX,"");
     return;
   }
   else
   {
     if((dGetField(ID_DEBIT) == 0) && (dGetField(ID_CREDIT) == 0))
      {
	BWCCMessageBox(HWindow,"A debit or credit must be greater than 0.",GetString(ID_WARNING),MB_OK);
        return;
      }
     CreateLineItem(apbtch,apbtchd);
   } 

  lCredit = dGetField(ID_CREDIT);
  lDebit  = dGetField(ID_DEBIT);

  if((lCredit == 0) && (lDebit == 0))
   {
     BWCCMessageBox(HWindow,"A debit or credit must be greater than 0.",GetString(ID_WARNING),MB_OK);
     return;
   }
  
  if((lCredit > 0) && (lDebit > 0))
   {
     BWCCMessageBox(HWindow,"Both debit and credit cannot be  greater than 0.",GetString(ID_WARNING),MB_OK);
     return;
   }

  ModifyCurrentSlip(apbtch,apbtchd,D);
  return;
}


void TAPInvoiceList::Redraw(RTMessage)
{
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSClearData(SHWindow,SS_ALLCOLS,SS_ALLROWS);
 apbtch->SetIndex("batchno");
 SSSetBool(SHWindow,SSB_REDRAW,FALSE);
 Relate4set *rQuery = new Relate4set(apbtch);
 rQuery->querySet("MODE = '0'.OR. MODE = '1'");
 int i = 1;
 while (!apbtch->eof())
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
 {
  SSSetData(SHWindow,1,i,StrTrim(apbtch->batchno->str()));
  SSSetData(SHWindow,2,i,apbtch->invoiceno->str());
  SSSetData(SHWindow,3,i,apbtch->reference->str());
  if(strcmp(apbtch->mode->str(),"0") == 0)
   SSSetColor(SHWindow,SS_ALLCOLS,i,RGB(255,255,130), RGBCOLOR_BLUE);
  i++;
 }
 rQuery->unlock();
 rQuery->free();
 SSSetBool(SHWindow,SSB_REDRAW,TRUE);
}


void TAPInvoiceList::HandleListBox(RTMessage Msg)
{
 char szBuffer[10];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetData(SHWindow,1,Cell->Row,szBuffer);
 long lRecord=atol(szBuffer);
 apbtch->SetIndex("batchno");
 if(apbtch->seek(lRecord)==0)
 {
  SendMessage(Parent->HWindow,WM_COMMAND,CM_BATCH,NULL);
  return;
 }
}

void TAPAdjEntry::CurBatch(RTMessage)
{

  double lDebitTotal,lCreditTotal,lDifference;
  char szDueDate[11],szInvDate[11];
  lDebitTotal=lCreditTotal=lDifference=0;
  if(apbtch->recCount() == 0)
  {
   BWCCMessageBox(HWindow,"There are no current batches to modify!",GetString(ID_WARNING),MB_OK);
   return;
  }

  HWND List = GetDlgItem(D->HWindow,ID_DISTLIST);
  SSSetBool(List,SSB_REDRAW,FALSE);
  SSClearData(List,SS_ALLCOLS,SS_ALLROWS);


  IntSetDlgItemValue(HWindow,ID_BATCHNO,int (Str4ptr(apbtch->batchno->str())));
  SetField(ID_DESCRIPT,apbtch->descript);
  SetField(ID_VENDORNO,apbtch->vendorno);
  SetField(ID_VENDORNAME,apbtch->vdname);
  SetField(ID_INVOICENO,apbtch->invoiceno);
  SetField(ID_CURRBAL,apbtch->orderno);
  SetField(ID_REFERENCE,apbtch->reference);
  SetField(ID_APCONTROL,apbtch->account);
  SetField(ID_APDEPART,apbtch->depart);
  SetField(ID_REBATE,apbtch->rebate);
  SetField(ID_BATCHNO,apbtch->batchno);
  


  InvoiceDate.assign(apbtch->invdate->str(),"CCYYMMDD");
  InvoiceDate.format(szInvDate,"MM/DD/YY");
  SetField(ID_INVDATE,szInvDate);

  DueDate.assign(apbtch->duedate->str(),"CCYYMMDD");
  DueDate.format(szDueDate,"MM/DD/YY");
  SetField(ID_DUEDATE,szDueDate);
  
  SetField(ID_TERMS,apbtch->terms);
  SetField(ID_VATPERCENTAGE,apbtch->vatpercent);
  SetField(ID_ACCOUNTS,"");
  SetField(ID_DEPART,"");


  FloatSetDlgItemValue(HWindow,ID_INVOICE,double (Str4ptr(apbtch->payable->str())));
  
  long i=1;
  apbtchd->SetIndex("batchno");
  char *szLineNo = new char [3];


  if (apbtchd->seek(double(Str4ptr(apbtch->batchno->str()))) == 0)
   {  
   while (apbtch->lBatchno() == apbtchd->lBatchno() && !apbtch->eof() )
    {
     if(apbtchd->dDebit() > 0)
     {
      lDebitTotal = lDebitTotal + double(Str4ptr(apbtchd->debit->str()));
      ltoa(i,szLineNo,10);
      SSSetData(List,1,i,szLineNo);
      SSSetData(List,2,i,apbtchd->descript->str());
      SSSetData(List,3,i,apbtchd->account->str());
      SSSetData(List,4,i,apbtchd->dept->str());
      SSSetData(List,5,i,apbtchd->debit->str());
      SSSetData(List,6,i,apbtchd->credit->str());
      DebitTotal = lDebitTotal;
      apbtchd->entryno->assignLong(i);
      apbtchd->flush();
      apbtchd->unlock();

      i++;
     }
     apbtchd->skip();
   }
  
  }

  //Set the credit amounts.
  if (apbtchd->seek(apbtch->lBatchno()) == 0)
  {  

   while (apbtch->lBatchno() == apbtchd->lBatchno() && !apbtch->eof() )
    {
     if(apbtchd->dCredit() > 0)
     {
      lCreditTotal = lCreditTotal + double(Str4ptr(apbtchd->credit->str()));
      ltoa(i,szLineNo,10);
      SSSetData(List,1,i,szLineNo);
      SSSetData(List,2,i,apbtchd->descript->str());
      SSSetData(List,3,i,apbtchd->account->str());
      SSSetData(List,4,i,apbtchd->dept->str());
      SSSetData(List,5,i,apbtchd->debit->str());
      SSSetData(List,6,i,apbtchd->credit->str());
      apbtchd->entryno->assignLong(i);
      apbtchd->flush();
      apbtchd->unlock();

      CreditTotal = lCreditTotal;
      i++;    
     }
     apbtchd->skip();
   }
  
  }



   lDifference = lDebitTotal - lCreditTotal;
   IntSetDlgItemValue(HWindow,ID_LINENO,i);
   FloatSetMask(GetDlgItem(D->HWindow,ID_DEBITTOTAL), "999999999.99");
   FloatSetMask(GetDlgItem(D->HWindow,ID_CREDITTOTAL),"999999999.99");
   FloatSetDlgItemValue(D->HWindow,ID_CREDITTOTAL,lCreditTotal);
   FloatSetDlgItemValue(D->HWindow,ID_DEBITTOTAL,lDebitTotal);
   FloatSetDlgItemValue(D->HWindow,ID_DIFFERENCE,lDifference);
   apbtchd->iCurrentRecord = 0;
   if(i>8)
    SSSetMaxRows(List,i);
   else
    SSSetMaxRows(List,8);
   if(strcmp(apbtch->mode->str(),"0") == 0)
   {
    SetField(IDOK,GetString(ID_RECOVER));
    bRecovered = FALSE;
   }
   else
   SetField(IDOK,"&Add/Replace");
   SSSetColor(GetDlgItem(D->HWindow,ID_DISTLIST), SS_ALLCOLS,SS_ALLROWS,lpBack,lpFore);
   SSSetBool(List,SSB_REDRAW,TRUE);
   delete szLineNo;


   return;
}


void TAPInvoiceList::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 RECT rc;
 SS_CELLTYPE CellType;
 LOGFONT lfFont;
 HFONT   hFont;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 GetClientRect(HWindow,&rc);  
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));            
 MoveWindow(SHWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);

 SetListSize(8);

 short d0Width,d1Width;
 SSSetMaxCols(SHWindow,3);
 SSSetColWidth(SHWindow,1,5);
 SSSetColWidth(SHWindow,2,10);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d0Width);
 SSGetColWidthInPixels(SHWindow,2,(LPSHORT)&d1Width);

 short ColWidth = (rc.right - rc.left) - d0Width - d1Width  - GetSystemMetrics(SM_CXHSCROLL);
 SSSetColWidthInPixels(SHWindow,3,ColWidth);

 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
 SSSetCellType(SHWindow,SS_ALLCOLS, SS_ALLROWS, &CellType);

 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_CENTER | SS_TEXT_SHADOW);
 SSSetCellType(SHWindow,SS_HEADER, SS_ALLROWS, &CellType);

 apbtch->SetIndex("recNo");
 if(apbtch->recCount()>10)
 SSSetMaxRows(SHWindow,apbtch->recCount());
 else 
 SSSetMaxRows(SHWindow,10);
 SSSetData(SHWindow,1,0,"Btch No.");
 SSSetData(SHWindow,2,0,"Invoice");
 SSSetData(SHWindow,3,0,"Description");
 HourGlassOff();

}


void TAPAdjEntry::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 DebitTotal=0;
 CreditTotal=0;
 apbtch =  new Tapbtch(&cb);
 apbtchd=  new Tapbtchd(&cb);
 apvendor= new Tapvendor(&cb);
 glacct =  new Tglaccts(&cb);
 gldept =  new Tgldept(&cb);
 gldist =  new Tgldist(&cb);
 apdist =  new Tapdist(&cb);

 apdist->SetIndex("batchno");
 apbtchd->SetIndex("batchno");

 bAutomaticCalc = TRUE;
 SendDlgItemMsg(ID_AUTO,BM_SETCHECK,TRUE,NULL);



 // Get the AP Control Accounts.

 apsetup = new Tapsetup(&cb);
 apsetup->SetIndex("name");

 apsetup->go(2L);
 szIndTaxAccount = strcpy(new char[apsetup->account->len()+1],apsetup->account->str());
 szIndTaxDept    = strcpy(new char[apsetup->depart->len()+1],apsetup->depart->str());
 apsetup->go(3L);
 szCompanyTaxAccount = strcpy(new char[apsetup->account->len()+1] ,apsetup->account->str());
 szCompanyTaxDept = strcpy(new char[apsetup->depart->len()+1],apsetup->depart->str());
 apsetup->go(4L);
 szVATAccount = strcpy(new char[apsetup->account->len()+1] ,apsetup->account->str());
 szVATDept    = strcpy(new char[apsetup->depart->len()+1],apsetup->depart->str());
 apsetup->close();


 VendorList = new TAPVendorList(this,"AP_LIST",apvendor);
 VendorList->EnableAutoCreate();
 GetApplication()->MakeWindow(VendorList);


 AcctList = new TGLAcctList(this,"AP_LIST",glacct);
 AcctList->EnableAutoCreate();
 GetApplication()->MakeWindow(AcctList);


 InvoiceList  = new TAPInvoiceList(this,"AP_LIST",apbtch);
 InvoiceList->EnableAutoCreate();
 GetApplication()->MakeWindow(InvoiceList);


 D = new TAPDistList(this,"GL_DISTLIST",apbtchd);
 D->EnableAutoCreate();
 GetApplication()->MakeWindow(D);


 RECT rc;
 RECT child;
 RECT parent;

 int pos = 30;
 GetWindowRect(HWindow,&rc);
 GetWindowRect(Parent->HWindow,&parent);
 MoveWindow(HWindow,parent.left+pos,parent.top+pos,rc.right-rc.left,rc.bottom-rc.top,TRUE);
 GetWindowRect(HWindow,&rc);
 GetWindowRect(D->HWindow,&child);
 MoveWindow(D->HWindow,rc.left+pos,rc.bottom+pos,child.right-child.left,child.bottom-child.top,TRUE);
 GetWindowRect(VendorList->HWindow,&child);
 MoveWindow(VendorList->HWindow,rc.right+pos,rc.top+pos,child.right-child.left,child.bottom-child.top,TRUE);
 pos = pos + 20;
 GetWindowRect(AcctList->HWindow,&child);
 MoveWindow(AcctList->HWindow,rc.right+pos,rc.top+pos,child.right-child.left,child.bottom-child.top,TRUE);
 pos = pos + 20;
 GetWindowRect(InvoiceList->HWindow,&child);
 MoveWindow(InvoiceList->HWindow,rc.right+pos,rc.top+pos,child.right-child.left,child.bottom-child.top,TRUE);


 IntSetDlgItemValue(HWindow,ID_LINENO,1);

 IntSetDlgItemValue(HWindow,ID_PERIOD,GetCurrentPeriodn());
 FloatSetMask(GetDlgItem(HWindow,ID_CREDIT),"999999999.99");
 FloatSetMask(GetDlgItem(HWindow,ID_DEBIT),"999999999.99");
 SetFocus(GetDlgItem(HWindow,ID_INVOICENO));
 HWND SHWindow = GetDlgItem(D->HWindow,ID_DISTLIST);
 SSGetColor(SHWindow,SS_ALLCOLS,SS_ALLROWS,&lpBack,&lpFore);
 bRedraw = TRUE;

 D->Show(SW_SHOW);
 HourGlassOff();

}

void TAPAdjEntry::SList(RTMessage Msg)
{
 if(bRedraw)
  {
   //SendMessage(InvoiceList->HWindow,WM_COMMAND,CM_SETDATA,NULL);
   InvoiceList->Redraw(Msg);
   bRedraw = FALSE;
  }
 if(IsWindowVisible(InvoiceList->HWindow))
 InvoiceList->Show(SW_HIDE);
 else
 {
 InvoiceList->Show(SW_SHOW);
 }
}

void TAPAdjEntry::Ok(RTMessage Msg)
{
 if(!bRecovered)
 {
  if(BWCCMessageBox(HWindow,"Do you want to recover the current invoice?","Recover",MB_ICONQUESTION | MB_YESNO) == IDYES)
  { 
   RecoverPostedInvoice();
   SetField(IDOK,"&Add/Replace");
   SS_COORD Row,Col;
   SSGetActiveCell(GetDlgItem(InvoiceList->HWindow,ID_DISTLIST),&Col,&Row);
   SSSetColor(GetDlgItem(InvoiceList->HWindow,ID_DISTLIST),SS_ALLCOLS,Row,lpBack,lpFore);
   bRecovered = TRUE;
  }
  return;
 }
 TAPInvoiceEntry::Ok(Msg);
}



void TGlWindow::APAdjustment   (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPAdjEntry(this,"AP_ADJBATCH"));
 }


void TAPAdjEntry::RecoverPostedInvoice()
{
 
 char szAccountNo[13];
 long dBatchNo;

 IntGetValue(GetDlgItem(HWindow,ID_BATCHNO),(LPLONG)&dBatchNo);

 if(apbtch->seek(dBatchNo) == 0)
 {
     apbtch->mode->assign("1");
     apbtch->flush();
     apbtch->unlock();
 }

 gldist->SetIndex("apdist");
 if(bDeleted)
 {
  if(gldist->seek(dBatchNo) == 0 )
  {
   while (double (Str4ptr(gldist->batchno->str())) == dBatchNo)
    {
       gldist->deleteRec();
       gldist->skip();
    }
   gldist->pack();
  }
 if(apdist->seek(dBatchNo) == 0 )
  {
   while (double (Str4ptr(apdist->batchno->str())) == dBatchNo)
    {
     apdist->deleteRec();
     apdist->skip();
    }
   apdist->pack();
  }

 }
}


void TAPAdjEntry::RebateCalc(RTMessage Msg)
{
 // Check to see if the invoice has already been entered. 
 if(lGetField(ID_BATCHNO) == 0) 
 {
   BWCCMessageBox(HWindow,"Choose an invoice to modify?","Rebate Calculation",MB_OK);
   return;
 }

 if(lGetField(ID_BATCHNO) > 0 && strcmp(GetField(IDOK),GetString(ID_MODIFY)) != 0)
 {
  apbtch->SetIndex("invoiceno");
  char *szInvoiceno = strcpy(new char[apbtch->invoiceno->len()+ apbtch->vendorno->len() + 1],GetField(ID_VENDORNO));
  strcat(szInvoiceno,GetField(ID_INVOICENO));                  
  if(apbtch->seek(szInvoiceno) == 0)
  {
    if(DlgMsg(ID_INVOICECREATED,ID_ADJUSTMENT,MB_YESNO | MB_ICONQUESTION) == IDNO)
    { 
     CurBatch(Msg);
     return;
    }
  }
 }
  
 apbtch->SetIndex("batchno");

 szAdj = strcpy(new char[apbtch->adj->len() + 1],"N");
 SendDlgItemMsg(ID_DISTLIST, WM_SETREDRAW, FALSE, 0L);
 iPeriodNo = (int)lGetField(ID_PERIOD);
 szVendorNo = strcpy(new char[apbtch->vendorno->len() + 1],GetField(ID_VENDORNO));
 if(*szVendorNo==0)
   {
    CheckError(ID_VENDORNO);
    return;
   }

 if(apvendor->seek(szVendorNo)!=0)
  {
   MessageBeep(MB_ICONEXCLAMATION);
   if(IsWindowVisible(VendorList->HWindow))
    VendorList->Show(SW_HIDE);
   else
    VendorList->Show(SW_SHOW);
   SetFocus(VendorList->HWindow);
   return;
  }

 szInvoiceNo = strcpy(new char[apbtch->invoiceno->len() + 1], GetField(ID_INVOICENO));
 if(*szInvoiceNo==0)
  {
   CheckError(ID_INVOICENO);
   return;
  }

 szInvoiceDate = strcpy(new char[apbtch->date->len() + 1],GetField(ID_INVDATE));
 if(*szInvoiceDate==0)
  {
   CheckError(ID_INVDATE);
   return;
  }
 InvoiceDate.assign(szInvoiceDate,"MM/DD/YY");


 szDueDate = strcpy(new char[apbtch->date->len() + 1],GetField(ID_DUEDATE));
 if(*szDueDate==0)
   {
    CheckError(ID_DUEDATE);
    return;
   }
 DueDate.assign(szDueDate,"MM/DD/YY");

 dTerms = dGetField(ID_TERMS);
 long lDueDate = long (InvoiceDate) + dTerms;
 DueDate.assign(lDueDate);

 szOrderNo = strcpy(new char[apbtch->orderno->len() + 1],GetField(ID_CURRBAL));

 szReference = strcpy(new char[apbtch->reference->len() + 1],GetField(ID_REFERENCE));
 if(*szReference==0) {
   CheckError(ID_REFERENCE);
   return;
 }

 
 //No need for the invoice description update.
 szDescript = strcpy(new char[apbtch->descript->len() + 1],GetField(ID_JDESCRIPT));
 if(*szDescript == 0)
 {
  CheckError(ID_JDESCRIPT);
  return;
 }
 
 
 szDepart = strcpy(new char[apbtch->depart->len() + 1],GetField(ID_DEPART));
 szAccounts = strcpy(new char[apbtch->account->len() + 1],GetField(ID_ACCOUNTS));
 char *szSeek = new char[apbtch->account->len() + apbtch->depart->len() + 1];
 strcpy(szSeek,szAccounts);
 strcat(szSeek,szDepart);
 if(*szAccounts==0){CheckError(ID_ACCOUNTS);return;}
 if(!CheckGLAccount(szSeek)){SearchGL(Msg);SetFocus(GetDlgItem(HWindow,ID_ACCOUNTS));return;}


 iLineno = lGetField(ID_LINENO);

 lInvoiceAmount = dGetField(ID_INVOICE);
 if(lInvoiceAmount == 0 ){CheckError(ID_INVOICE);return;}

 
 dVatPercentage = dGetField(ID_VATPERCENTAGE);
 
 dRebatePercentage = dGetField(ID_REBATE);
 if(dRebatePercentage == 0.0)
 {
  CheckError(ID_REBATE);
  return;
 }
 iBatchno = (int)lGetField(ID_BATCHNO);

 if(DlgMsg(ID_REBATEADJ,ID_ADJUSTMENT,MB_ICONQUESTION | MB_YESNO) == IDYES)
  CreateRebateAdjustment();
 CurBatch(Msg);
}

void TAPAdjEntry::CreateRebateAdjustment()
{
   if(strcmp(apbtch->mode->str(),"0") == 0)
    RecoverPostedInvoice();
   apbtchd->SetIndex("batchno");

   char szSeekAccount[12];
   char szRebateAccount[8];
   char szRebateDepart[5];
   // Check to see if the vendor is an individual or a company.
   if(dGetField(ID_REBATE) > 0)
   {
    if(SendDlgItemMsg(ID_INDIVIDUAL,BM_GETSTATE,NULL,NULL))
     {
      strcpy(szSeekAccount,szIndTaxAccount);
      strcat(szSeekAccount,szIndTaxDept);
      strcpy(szRebateAccount,szIndTaxAccount);
      strcpy(szRebateDepart,szIndTaxDept);
     }
    else if(SendDlgItemMsg(ID_COMPANY,BM_GETSTATE,NULL,NULL))
    {
     strcpy(szSeekAccount,szCompanyTaxAccount);
     strcat(szSeekAccount,szCompanyTaxDept);
     strcpy(szRebateAccount,szCompanyTaxAccount);
     strcpy(szRebateDepart,szCompanyTaxDept);
    }
    else if(!SendDlgItemMsg(ID_COMPANY,BM_GETSTATE,NULL,NULL) && !SendDlgItemMsg(ID_INDIVIDUAL,BM_GETSTATE,NULL,NULL))
    {
     BWCCMessageBox(HWindow,"Choose either company or individual for tax rate.","Rebate",MB_ICONEXCLAMATION);
     return;
    }
   }

   // Add the entry to the batch master.


   // Calculate the Invoice Amounts.

   FloatGetDlgItemValue(HWindow,ID_INVOICE,(LPDOUBLE) &lInvoiceAmount);

   if(dGetField(ID_REBATE) > 0)
      {  
       dRebatePercentage = (dGetField(ID_REBATE))/100;
      }
   else
      dRebatePercentage = 1;

   if(dGetField(ID_VATPERCENTAGE) > 0)
      dVatPercentage = (dGetField(ID_VATPERCENTAGE)+100)/100;
   else 
      dVatPercentage = 1;

   
   double AMOUNT  = fabs(dGetField(ID_INVOICE));
   double EXPENSE = AMOUNT/dVatPercentage;
   double VAT     = AMOUNT-EXPENSE;

   double REBATE  = 0.0;
   if(dRebatePercentage != 1)
      REBATE  = -1*(EXPENSE*dRebatePercentage);

   double AP      = -1*(AMOUNT + REBATE);

   // Add Expense amount to detail database.

   UpdateBatchDetail(iBatchno,
		     szDescript,
		     GetField(ID_ACCOUNTS),
		     GetField(ID_DEPART),
		     EXPENSE);

   if(dVatPercentage != 1)
   UpdateBatchDetail(iBatchno,
		     "",
		     szVATAccount,
		     szVATDept,
		     VAT);

   if(dRebatePercentage != 1)
     UpdateBatchDetail(iBatchno,
		     "",
		     szRebateAccount,
                     szRebateDepart,
		     REBATE);

   UpdateBatchDetail(iBatchno,
		     "",
		     GetField(ID_APCONTROL),
		     GetField(ID_APDEPART),
		     (AP));

   UpdateBatchHeader(iBatchno,AMOUNT,VAT,REBATE);

   return;
}

void TAPAdjEntry::UpdateBatchHeader(long iBatchno, double AP, double VAT, double REBATE)
{
  if(apbtch->seek(iBatchno) == 0)
   {   
    apbtch->mode->assign("1");
    apbtch->adj->assign("N");
    apbtch->period->assignDouble(GetCurrentPeriodn());
    apbtch->orderno->assign(GetField(ID_CURRBAL));
    apbtch->vendorno->assign(GetField(ID_VENDORNO));
    apbtch->vdname->assign(GetField(ID_VENDORNAME));
    apbtch->invoiceno->assign(GetField(ID_INVOICENO));
    apbtch->duedate->assign(DueDate.str());
    apbtch->invdate->assign(InvoiceDate.str());
    apbtch->reference->assign(GetField(ID_REFERENCE));
    apbtch->descript->assign(GetField(ID_JDESCRIPT));
    apbtch->account->assign(GetField(ID_APCONTROL));
    apbtch->depart->assign(GetField(ID_APDEPART));    
    apbtch->vatpercent->assignDouble(dGetField(ID_VATPERCENTAGE));    
    apbtch->terms->assignLong(lGetField(ID_TERMS));
    apbtch->payable->assignDouble(AP);
    apbtch->vatamt->assignDouble(VAT);
    apbtch->rebateamt->assignDouble(REBATE);
    apbtch->rebate->assignDouble(dGetField(ID_REBATE));
    apbtch->flush();
    apbtch->unlock();
  }
  else
  MessageBox(HWindow,"Error updating batch header.","",MB_OK);
}


void TAPAdjEntry::UpdateBatchDetail(long iBatchno, char *szDescript, char *szAccount,char *szDepart,double dEntry)
{
 char szReference[41];
 if(strcmp(szDescript,"") == 0)
   {
    char *szSeek = new char[12];
    strcpy(szSeek,szAccount);
    glacct->SetIndex("accounts");
    glacct->seek(strcat(szSeek,szDepart));
    strcpy(szReference,glacct->descript->str());
    delete szSeek;
    }
  else
  strcpy(szReference,szDescript);

 if(apbtchd->seek(iBatchno) == 0)
 {
      // If the entry for that Account and Department code has already been entered just update it.
      int iEntryno = 1;
       while(iBatchno == apbtchd->lBatchno())
        {
	 if(strcmp(szAccount,apbtchd->account->str()) == 0 && strcmp(szDepart,apbtchd->dept->str()) == 0 )
	  {
		 
	     apbtchd->descript->assign(szReference);
             if(dEntry > 0)
              {
               apbtchd->debit->assignDouble(fabs(dEntry));
               apbtchd->credit->assignDouble(0.00);
              }
             else if (dEntry < 0)
              {
               apbtchd->debit->assignDouble(0.00);
               apbtchd->credit->assignDouble(fabs(dEntry));
              }
             apbtchd->flush();
	     apbtchd->unlock();
             return;
	  }
	 apbtchd->skip();
	 iEntryno++;
        }
     // if not found create a new line entry.
     apbtchd->appendStart();
     apbtchd->batchno->assignLong(iBatchno);
     apbtchd->descript->assign(szReference);
     apbtchd->account->assign(szAccount);
     apbtchd->dept->assign(szDepart);
     if(dEntry < 0)
      {
       apbtchd->debit->assignDouble(0.00);
       apbtchd->credit->assignDouble(fabs(dEntry));   
      }
     else if (dEntry > 0)
      {
       apbtchd->debit->assignDouble(fabs(dEntry));
       apbtchd->credit->assignDouble(0.00); 
      }
      apbtchd->append();
      apbtchd->unlock();      
  }
 else
 MessageBox(HWindow,"Batch not found","Detail",MB_OK);
 return;
}
