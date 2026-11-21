#include "ap.h"
#include "db_dept.h"
#include "db_apbtc.h"
#include "db_cash.h"
#include "ap_class.h"



static COLORREF lpBack, lpFore;


void TAPInvoiceEntry::AddVendor(RTMessage)
{
 SendMessage(Parent->HWindow,WM_COMMAND,ID_APADDVENDORS,NULL);
}

void TAPInvoiceEntry::Top(RTMessage Msg)
{
 apbtch->SetIndex("batchno");
 apbtch->top();
 CurrentBatch(Msg);
};
void TAPInvoiceEntry::Bottom(RTMessage Msg)
{
 apbtch->SetIndex("batchno");
 apbtch->bottom();
 CurrentBatch(Msg);
};

void TAPInvoiceEntry::Next(RTMessage Msg)
{
 apbtch->SetIndex("batchno");
 int iBatchno = 0;
 IntGetDlgItemValue(HWindow,ID_BATCHNO,(LPLONG)&iBatchno);
 if(apbtch->seek(iBatchno) == 0)
 { 
   if(!apbtch->eof())
   {
    apbtch->skip();
    if(apbtch->eof())
    apbtch->bottom();
    CurrentBatch(Msg);
   }
 }
 else
 return;

};
void TAPInvoiceEntry::Previous(RTMessage Msg)
{
 apbtch->SetIndex("batchno");
 int iBatchno = 0;
 IntGetDlgItemValue(HWindow,ID_BATCHNO,(LPLONG)&iBatchno);
 if(apbtch->seek(iBatchno) == 0)
 { 
   if(!apbtch->bof())
   {
    apbtch->skip(-1);
    CurrentBatch(Msg);
   }
   else if (apbtch->eof())
   {
    apbtch->skip(-1);
    CurrentBatch(Msg);
   }
 }
 else
 return;
};


void TAPInvoiceEntry::Delete(RTMessage Msg)
{

  int iEntryno;
  IntGetValue(GetDlgItem(HWindow,ID_LINENO),(LPLONG)&iEntryno);  
  apbtchd->SetIndex("batchno");
  if(apbtchd->seek(apbtch->lBatchno()) == 0)
  {
   HourGlassOn();
   if(DlgMsg(ID_DELETE,ID_BATCHNO,MB_YESNO | MB_ICONQUESTION) == IDYES)
    {
     while(apbtchd->lBatchno() == apbtch->lBatchno())
      {
       if(apbtchd->iJournal() == iEntryno)
        {
	 apbtchd->deleteRec();
	 apbtchd->pack();
         break;
        }
	 apbtchd->skip();
      }

    }
   HourGlassOff();
  }
  CurrentBatch(Msg);
  return;
};

void TAPInvoiceEntry::SetupWindow()
{
 TDialog::SetupWindow();
 SendDlgItemMsg(ID_APADDVENDORS,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ADD");
 HourGlassOn();
 /*
 LOGFONT 	lfFont; 
 HFONT  hFont = (HFONT) NULL;
    if ( (hFont =  (HFONT) SendMessage(HWindow, WM_GETFONT, 0, 0L))!=0 ){
	if (GetObject(hFont, sizeof(LOGFONT), (LPSTR) &lfFont)){
	    lfFont.lfHeight = -30;
            lfFont.lfWeight = FW_LIGHT;
	    if (hFont == CreateFontIndirect((LPLOGFONT) &lfFont)){
		SendMessage(HWindow,WM_SETFONT,(WPARAM)hFont,(LPARAM)FALSE);
	    }
	}
    }
 */
 DebitTotal=0;
 CreditTotal=0;
 apbtch =  new Tapbtch(&cb);
 apbtchd=  new Tapbtchd(&cb);
 apvendor= new Tapvendor(&cb);
 glacct =  new Tglaccts(&cb);
 gldept =  new Tgldept(&cb);

 apbtch->SetIndex("batchno");
 apvendor->SetIndex("vendorno");
 bAutomaticCalc = TRUE;
 SendDlgItemMsg(ID_AUTO,BM_SETCHECK,TRUE,NULL);
 // Get the AP Control Accounts.
 apsetup = new Tapsetup(&cb);
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
 VendorList->SetCaption("Accounts Payable Vendor List");

 AcctList = new TGLAcctList(this,"AP_LIST",glacct);
 AcctList->EnableAutoCreate();
 GetApplication()->MakeWindow(AcctList);
 AcctList->SetCaption("General Ledger Account List");

 bSlipList = TRUE;
 SlipList  = new TAPBatchList(this,"AP_LIST",apbtch);
 SlipList->EnableAutoCreate();
 GetApplication()->MakeWindow(SlipList);
 SlipList->SetCaption("Accounts Payable Slip List");

 D = new TAPDistList(this,"GL_DISTLIST",apbtchd);
 D->EnableAutoCreate();
 GetApplication()->MakeWindow(D);
 

 long db=0;
 IntSetDlgItemValue(HWindow,ID_BATCHNO,db);
 IntSetDlgItemValue(HWindow,3032,GetCurrentPeriodn());
 SetCaption(GetString(ID_APADD));

 IntSetDlgItemValue(HWindow,ID_LINENO,1);

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
 GetWindowRect(SlipList->HWindow,&child);
 MoveWindow(SlipList->HWindow,rc.right+pos,rc.top+pos,child.right-child.left,child.bottom-child.top,TRUE);

 D->Show(SW_SHOW);

 FloatSetMask(GetDlgItem(HWindow,ID_CREDIT),"999999999.99");
 SendDlgItemMsg(ID_DEPART,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
 SetFocus(GetDlgItem(HWindow,ID_DESCRIPT));
 HourGlassOff();
}

void TAPInvoiceEntry::SetVendor(RTMessage)
{
 SetField(ID_VENDORNO,apvendor->vendorno);
 SetField(ID_VENDORNAME,apvendor->vdname); 
 SetField(ID_APCONTROL,apvendor->account);
 SetField(ID_APDEPART,apvendor->depart);
 SetField(ID_REBATE,apvendor->rebate);
 SetField(ID_VATPERCENTAGE,apvendor->percentage);
 SetField(ID_TERMS,apvendor->terms);

}

void TAPDistList::Print (RTMessage)
{
  HourGlassOn(); 
  char  szBatchNo[10];
  long iBatchNo;
  IntGetDlgItemValue(Parent->HWindow,ID_BATCHNO,(LPLONG)&iBatchNo);

  short JobNumber = PEOpenPrintJob("ap_slips.rpt");

  if (JobNumber == 0)
  {
   HANDLE textHandle;   
   short  iTextLen;
   PEGetErrorText(JobNumber,&textHandle,&iTextLen);
   char   *szErrorText = new char[iTextLen];
   PEGetHandleString(textHandle,szErrorText,iTextLen);
   BWCCMessageBox(HWindow,szErrorText,"AP Slip",MB_ICONINFORMATION);
   delete szErrorText;   
   return;
  } 

  itoa((unsigned)iBatchNo,szBatchNo,10);

  if(!PESetFormula (JobNumber,"Batchno",szBatchNo))
   {
    HANDLE textHandle;
    short  iTextLen;
    PEGetErrorText(JobNumber,&textHandle,&iTextLen);
    char   *szErrorText = new char[iTextLen];
    PEGetHandleString(textHandle,szErrorText,iTextLen);
    BWCCMessageBox(HWindow,szErrorText,"AP Slip",MB_ICONINFORMATION);
    delete szErrorText; 
    PEClosePrintJob(JobNumber);
    return ;
   }
  PEOutputToPrinter(JobNumber,1);
  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
}



void TAPInvoiceEntry::CurrentBatch(RTMessage)
{
  SetField(IDOK,GetString(IDOK));
  SetCaption(GetString(ID_APMODIFY));
  double lDebitTotal,lCreditTotal,lDifference;
  char szDueDate[11],szInvDate[11];
  lDebitTotal=lCreditTotal=lDifference=0;
  if(apbtch->recCount() == 0)
  {
   DlgMsg(ID_NOBATCH,ID_SLIP,MB_ICONINFORMATION);
   return;
  }

  HWND List = GetDlgItem(D->HWindow,ID_DISTLIST);
  SSSetBool(List,SSB_REDRAW,FALSE);
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

  char *szLineNo = new char[3];


  SSClearData(List,SS_ALLCOLS,SS_ALLROWS);
  
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
      CreditTotal = lCreditTotal;
      apbtchd->entryno->assignLong(i);
      apbtchd->flush();
      apbtchd->unlock();

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
  delete szLineNo;
  
  if(bSlipList)
   SlipList->Show(SW_HIDE);
  SSSetBool(List,SSB_REDRAW,TRUE);
  SSSetColor(GetDlgItem(D->HWindow,ID_DISTLIST), SS_ALLCOLS,SS_ALLROWS,lpBack,lpFore);
  if(strcmp(apbtch->mode->str(),"0") == 0)
   SetField(IDOK,GetString(ID_RECOVER));
}

void TAPInvoiceEntry::Automatic(RTMessage)
{
 if(SendDlgItemMessage(HWindow,ID_AUTO,BM_GETCHECK,NULL,NULL))
  {
    SendDlgItemMessage(HWindow,ID_AUTO,BM_SETCHECK,TRUE,NULL);
    bAutomaticCalc = TRUE;
  }
 else
  {
    SendDlgItemMessage(HWindow,ID_AUTO,BM_SETCHECK,FALSE,NULL);
    bAutomaticCalc = FALSE;
  }

}


void TAPInvoiceEntry::Ok (RTMessage Msg )
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
     CurrentBatch(Msg); 
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
     CurrentBatch(Msg); 
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

  if(iBatchno == 0)
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

   apbtch->bottom();
   iBatchno = (int)apbtch->lBatchno();
   iBatchno++;
   if(bAutomaticCalc)
   {
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
     return;
   } 
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




void TAPInvoiceEntry::ModifyCurrentSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D)
{  

   int iLine;
   IntGetDlgItemValue(HWindow,3033,(LPLONG)&iLine);

   FloatGetDlgItemValue(D->HWindow,ID_DEBITTOTAL,&DebitTotal);
   FloatGetDlgItemValue(D->HWindow,ID_CREDITTOTAL,&CreditTotal);
   apbtch->batchno->assignLong(lGetField(ID_BATCHNO));
   apbtch->mode->assign("1");
   apbtch->adj->assign("N");
   apbtch->date->assign(InvoiceDate.str());
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
   apbtch->payable->assignDouble(dGetField(ID_INVOICE));
   apbtch->entryno->assignLong(lGetField(ID_LINENO));
   apbtch->terms->assignLong(lGetField(ID_TERMS));
   apbtch->flush();
   apbtch->unlock();

   if(apbtchd->seek(lGetField(ID_BATCHNO)) == 0)
   {
    if(apbtchd->iCurrentRecord == 0)    
    {
      //Add a new line item.
      DebitTotal = DebitTotal+ lDebit;
      CreditTotal = CreditTotal+ lCredit;
      apbtch->descript->assign(GetField(ID_JDESCRIPT));
      apbtch->flush();
      apbtch->unlock();

      apbtchd->appendStart();      
      apbtchd->batchno->assignLong(iBatchno);   
      apbtchd->entryno->assignLong(iLine);
      apbtchd->account->assign(GetField(ID_ACCOUNTS));
      apbtchd->dept->assign(GetField(ID_DEPART));
      apbtchd->descript->assign(GetField(ID_JDESCRIPT));
      apbtchd->debit->assignDouble(lDebit);
      apbtchd->credit->assignDouble(lCredit);
      apbtchd->append();
      apbtchd->unlock();
    }
    else
    {
	// Modify an existing record.
	apbtchd->go(apbtchd->iCurrentRecord);
        DebitTotal = DebitTotal  + lDebit - double (Str4ptr(apbtchd->debit->str()));
	CreditTotal = CreditTotal + lCredit - double (Str4ptr(apbtchd->credit->str()));

        apbtch->descript->assign(GetField(ID_JDESCRIPT));
	apbtch->flush();
	apbtch->unlock();
        apbtchd->batchno->assignLong(iBatchno);   
	apbtchd->entryno->assignLong(iLine);
	apbtchd->account->assign(GetField(ID_ACCOUNTS));
        apbtchd->dept->assign(GetField(ID_DEPART));
	apbtchd->descript->assign(GetField(ID_JDESCRIPT));
        apbtchd->debit->assignDouble(dGetField(ID_DEBIT));
	apbtchd->credit->assignDouble(dGetField(ID_CREDIT));
        apbtchd->flush();
	apbtchd->unlock();
    }
   }
   // Add the AP amount to the database and distr. window.
   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
   return;
}

void TAPDistList::SetupWindow()
{
 TDialog::SetupWindow();
 RECT rc;
 HourGlassOn();
 SetListSize(8);
 SendDlgItemMsg(ID_PRINT,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"GLPRINT");
 FloatSetMask(GetDlgItem(HWindow,ID_DEBITTOTAL), "99999999999.99");
 FloatSetMask(GetDlgItem(HWindow,ID_CREDITTOTAL),"99999999999.99");
 SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetColor(SHWindow,SS_ALLCOLS,SS_ALLROWS,&lpBack,&lpFore);

 SSSetMaxCols(SHWindow, 6);
 SSGetClientRect(SHWindow,&rc);
 short d0Width,d2Width,d3Width,d4Width,d5Width;

 SSSetColWidth(SHWindow,1,3);
 SSSetColWidth(SHWindow,3,6);
 SSSetColWidth(SHWindow,4,5);
 SSSetColWidth(SHWindow,5,10);
 SSSetColWidth(SHWindow,6,10);

 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d0Width);
 SSGetColWidthInPixels(SHWindow,3,(LPSHORT)&d2Width);
 SSGetColWidthInPixels(SHWindow,4,(LPSHORT)&d3Width);
 SSGetColWidthInPixels(SHWindow,5,(LPSHORT)&d4Width);
 SSGetColWidthInPixels(SHWindow,6,(LPSHORT)&d5Width);

 short ColWidth = (rc.right - rc.left)
 		  - d0Width
		  - d2Width
		  - d3Width
		  - d4Width
		  - d5Width;


 SSSetColWidthInPixels(SHWindow,2,ColWidth);
 SSSetData(SHWindow,1,SS_HEADER, "Ln");
 SSSetData(SHWindow,2,SS_HEADER, "Description");
 SSSetData(SHWindow,3,SS_HEADER, "Acct");
 SSSetData(SHWindow,4,SS_HEADER, "Dept");
 SSSetData(SHWindow,5,SS_HEADER, "Debit");
 SSSetData(SHWindow,6,SS_HEADER, "Credit");

 SSSetTypeEdit(SHWindow,&CellType,ES_LEFT,40,SS_CHRSET_CHR,SS_CASE_NOCASE);
 SSSetCellType(SHWindow,1, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,2, SS_ALLROWS, &CellType);
 SSSetTypeEdit(SHWindow,&CellType,ES_RIGHT,40,SS_CHRSET_CHR,SS_CASE_NOCASE);  // was StaticText
 SSSetCellType(SHWindow,3, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,4, SS_ALLROWS, &CellType);

 SSSetTypeFloat(SHWindow,&CellType,FS_SEPARATOR,9,2,0,999999999.99);
 SSSetCellType(SHWindow,5, SS_ALLROWS, &CellType);
 SSSetCellType(SHWindow,6, SS_ALLROWS, &CellType);

 HourGlassOff();
}




char *GetString(HINSTANCE hInstance, int ID)
{
 static char szString[85];
 LoadString(hInstance,ID,szString,sizeof(szString));
 return szString;
}




void TAPInvoiceEntry::SearchGL    (RTMessage){  Accountlist();}



void TAPInvoiceEntry::InvoiceData()
{
  double lDebitTotal,lCreditTotal,lDifference;
  lDebitTotal=lCreditTotal=lDifference=0;
  HWND List = GetDlgItem(D->HWindow,ID_DISTLIST);
  SSClearData(List,SS_ALLCOLS,SS_ALLROWS);


  IntSetDlgItemValue(HWindow,ID_BATCHNO,int (Str4ptr(apbtch->batchno->str())));
  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->descript->str());
  IntSetDlgItemValue(HWindow,3032,GetCurrentPeriodn());
  int iLineNo = int (Str4ptr(apbtch->entryno->str()) );
  iLineNo++;
  IntSetDlgItemValue(HWindow,3033,iLineNo);
  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->descript->str());

  apbtchd->SetIndex("batchno");
  apbtchd->seek(double (Str4ptr(apbtch->batchno->str())));
  SendDlgItemMsg(4001,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->vendorno->str());
  SendDlgItemMsg(4003,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->vdname->str());
  SendDlgItemMsg(ID_INVOICENO,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->invoiceno->str());
  SendDlgItemMsg(2017,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->orderno->str());
  SendDlgItemMsg(ID_REFERENCE,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->reference->str());
  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->descript->str());
  SendDlgItemMsg(ID_INVDATE,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->invdate->str());
  SendDlgItemMsg(ID_DUEDATE,WM_SETTEXT,NULL,(LONG)(LPSTR)apbtch->duedate->str());


  long i=1;
  while (int (Str4ptr(apbtch->batchno->str())) == int (Str4ptr(apbtchd->batchno->str())) )
   {
     lDebitTotal = lDebitTotal + double(Str4ptr(apbtchd->debit->str()));
     lCreditTotal = lCreditTotal + double(Str4ptr(apbtchd->credit->str()));
     SSSetData(List,1,i,apbtchd->descript->str());
     SSSetData(List,2,i,apbtchd->account->str());
     SSSetData(List,3,i,apbtchd->dept->str());
     SSSetData(List,4,i,apbtchd->debit->str());
     SSSetData(List,5,i,apbtchd->credit->str());
     DebitTotal = lDebitTotal;
     CreditTotal = lCreditTotal;
     i=i+1;
     apbtchd->skip(1);
   }
   lDifference = lDebitTotal - lCreditTotal;
   FloatSetMask(GetDlgItem(D->HWindow,ID_DEBITTOTAL), "999999999.99");
   FloatSetMask(GetDlgItem(D->HWindow,ID_CREDITTOTAL),"999999999.99");
   FloatSetDlgItemValue(D->HWindow,ID_CREDITTOTAL,lCreditTotal);
   FloatSetDlgItemValue(D->HWindow,ID_DEBITTOTAL,lDebitTotal);
   FloatSetDlgItemValue(D->HWindow,ID_DIFFERENCE,lDifference);
}



void TAPInvoiceEntry::Vendorlist  ()
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
 }
 if(IsWindowVisible(VendorList->HWindow))
 VendorList->Show(SW_HIDE);
 else
 VendorList->Show(SW_SHOW);
}


void TAPInvoiceEntry::Sliplist() //slip list search
{
 if(bSlipList)
 {
  HWND SHWindow = GetDlgItem(SlipList->HWindow,ID_DISTLIST);
  SSClearData(SHWindow,SS_ALLCOLS,SS_ALLROWS);
  SSSetBool(SHWindow,SSB_REDRAW,FALSE);
  Relate4set *rQuery = new Relate4set(apbtch);
  rQuery->querySet("MODE = '1'");
  int i = 1;
  for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   SSSetData(SHWindow,1,i,StrTrim(apbtch->batchno->str()));
   SSSetData(SHWindow,2,i,StrTrim(apbtch->invoiceno->str()));
   SSSetData(SHWindow,3,i,apbtch->reference->str());
   i++;
  }
  rQuery->unlock();
  rQuery->free();
  if(i<8)
   SSSetMaxRows(SHWindow,8);
  else
   SSSetMaxRows(SHWindow,i);
  SSSetBool(SHWindow,SSB_REDRAW,TRUE);
  bSlipList = FALSE;
 }
 if(IsWindowVisible(SlipList->HWindow))
 SlipList->Show(SW_HIDE);
 else
 SlipList->Show(SW_SHOW);
}



void TAPInvoiceEntry::Accountlist ()
{
 if(IsWindowVisible(AcctList->HWindow))
 AcctList->Show(SW_HIDE);
 else
 AcctList->Show(SW_SHOW);
}

void TAPInvoiceEntry::New(RTMessage)
{
 SetField(IDOK,GetString(IDOK));
 SetCaption(GetString(ID_APADD));
 IntSetDlgItemValue(HWindow,ID_BATCHNO,0);
 IntSetDlgItemValue(HWindow,ID_LINENO,1);
 SetField(ID_DESCRIPT,"");
 SetField(ID_VENDORNO,"");
 SetField(ID_VENDORNAME,"");
 SetField(ID_INVOICENO,"");
 SetField(ID_CURRBAL,"");
 SetField(ID_REFERENCE,"");
 SetField(ID_JDESCRIPT,"");
 SetField(ID_APCONTROL,"");
 SetField(ID_APDEPART,"");
 SetField(ID_ACCOUNTS,"");
 SetField(ID_DEPART,"");
 SetField(ID_VATPERCENTAGE,"");
 SetField(ID_TERMS,"");
 SetField(ID_REBATE,"");

 SSSetColor(GetDlgItem(D->HWindow,ID_DISTLIST), SS_ALLCOLS,SS_ALLROWS,lpBack,lpFore);
 SSClearData(GetDlgItem(D->HWindow,ID_DISTLIST),SS_ALLCOLS,SS_ALLROWS);

 FloatSetMask(GetDlgItem(HWindow,ID_DEBIT), "999999999.99");
 FloatSetMask(GetDlgItem(HWindow,ID_CREDIT), "999999999.99");
 FloatSetDlgItemValue(HWindow,ID_DEBIT,0.00);
 FloatSetDlgItemValue(HWindow,ID_CREDIT,0.00);
 FloatSetDlgItemValue(HWindow,ID_INVOICE,0.00);
 FloatSetDlgItemValue(D->HWindow,ID_DEBITTOTAL,0.00);
 FloatSetDlgItemValue(D->HWindow,ID_CREDITTOTAL,0.00);
 FloatSetDlgItemValue(D->HWindow,ID_DIFFERENCE,0.00);
 DebitTotal=CreditTotal=0;
 SetFocus(GetDlgItem(HWindow,2015));
}



void TAPInvoiceEntry::Cancel (RTMessage )
{
 cb.closeAll();
 ShutDownWindow();
 return;
}

void TAPInvoiceEntry::HandleDepart (RTMessage ){}




void TAPDistList::SelectCell (RTMessage Msg)
{
  //Check

  LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
  unsigned int nIndx = (unsigned int) Cell->Row;
  apbtd->SetIndex("batchno");
  int iBatchno;
  IntGetValue(GetDlgItem(Parent->HWindow,ID_BATCHNO),(LPLONG)&iBatchno);
  HWND hWin = GetDlgItem(HWindow,ID_DISTLIST);
  apbtd->iCurrentRecord = 0;
  if(iBatchno > 0)
  {
   if(apbtd->seek(iBatchno) == 0)
    {
     while(iBatchno == apbtd->lBatchno())
      {
	if(nIndx == apbtd->iJournal())
	 {
	  apbtd->iCurrentRecord = apbtd->recNo();
	  break;
	 }
        apbtd->skip();
      }
    }
   }
  if(apbtd->iCurrentRecord == 0)
   {   
    SSSetColor(hWin,SS_ALLCOLS,SS_ALLROWS,lpBack,lpFore);
    return;
   }

  SSSetColor(hWin,SS_ALLCOLS,SS_ALLROWS,lpBack,lpFore);
  SSSetColor(SHWindow,SS_ALLCOLS,nIndx,RGB(192,220,192), RGBCOLOR_BLUE);
  SendMessage(Parent->HWindow,WM_COMMAND,CM_LINEITEM,NULL);
  return;
}

void TAPInvoiceEntry::SetLineItem(RTMessage)
{
  SetField(IDOK,GetString(ID_MODIFY));
  SetField(ID_JDESCRIPT,apbtchd->descript);
  SetField(ID_ACCOUNTS ,apbtchd->account);
  SetField(ID_DEPART   ,apbtchd->dept);
  SetField(ID_DEBIT    ,apbtchd->debit);
  SetField(ID_CREDIT   ,apbtchd->credit);
  SetField(ID_LINENO   ,apbtchd->entryno);
}

void TAPDistList::SetField(int ID,Field4 *field)
{
  SendDlgItemMessage(Parent->HWindow,ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return ;
}

void TAPDistList::SetField(int ID, long field)
{
  IntSetDlgItemValue(Parent->HWindow,ID,field);
  return;
}

void TAPDistList::SetField(int ID, double field)
{  
  FloatSetDlgItemValue(Parent->HWindow,ID,field);
  return;
}

void TAPDistList::SetField(int ID, char *field)
{
  SendDlgItemMessage(Parent->HWindow,ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return;
}


int TAPInvoiceEntry::CheckGLAccount(char *account)
{
 if(glacct->seek(account)==0)
 return TRUE;
 else
 return FALSE;
}


TAPBatchList::~TAPBatchList(){ delete ListArray; }

void TPickList::SetListSize(int Rows)
{
 RECT rc,wd;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 LOGFONT lfFont;
 HFONT   hFont;
 lfFont.lfHeight          = -15;
 lfFont.lfWidth           = 0;
 lfFont.lfEscapement      = 0;
 lfFont.lfOrientation     = 0;
 lfFont.lfWeight          = FW_NORMAL;
 lfFont.lfItalic          = 0;
 lfFont.lfUnderline       = 0;
 lfFont.lfStrikeOut       = 0;
 lfFont.lfCharSet         = 1; //default character set.
 lfFont.lfOutPrecision    = 0;
 lfFont.lfClipPrecision   = 0;
 lfFont.lfQuality         = 0;
 lfFont.lfPitchAndFamily  = FF_SWISS;
 StrCpy (lfFont.lfFaceName, "HELV");
 hFont = CreateFontIndirect((LPLOGFONT) &lfFont);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_ALLROWS,hFont, TRUE);
 short iHeight,iHeader;
 SSGetRowHeightInPixels(SHWindow,1,&iHeight);
 SSGetRowHeightInPixels(SHWindow,0,&iHeader); 
 int Height =  GetSystemMetrics(SM_CYCAPTION)+ 2*GetSystemMetrics(SM_CXDLGFRAME) + ((Rows-1)*iHeight) + iHeader + 1 ;
 GetWindowRect(HWindow,&wd);
 GetClientRect(HWindow,&rc); 
 MoveWindow(HWindow,wd.left,wd.top,wd.right-wd.left,Height,TRUE);
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));
 MoveWindow(SHWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
 
 SSSetBool(SHWindow,SSB_ALLOWUSERSELBLOCK,TRUE);
 SSSetBool(SHWindow,SSB_SCROLLBAREXTMODE,TRUE);
 SSSetBool(SHWindow,SSB_NOBEEP,TRUE);
 SSSetBool(SHWindow,SSB_ALLOWUSERRESIZE,FALSE);
 SSSetBool(SHWindow,SSB_AUTOSIZE,TRUE);
 SSSetBool(SHWindow,SSB_PROTECT,TRUE);
 SSSetBool(SHWindow,SSB_SHOWROWHEADERS,FALSE);
 SSSetBool(SHWindow,SSB_NOBEEP,TRUE);

 
}


void TAPBatchList::SetupWindow()
 {
 TDialog::SetupWindow();
 HourGlassOn();
 SetListSize(8);
 RECT rc;
 GetClientRect(HWindow,&rc);
 SS_CELLTYPE CellType;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 short d0Width,d1Width;
 SSSetMaxCols(SHWindow,3);
 SSSetColWidth(SHWindow,1,5);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d0Width);
 SSGetColWidthInPixels(SHWindow,2,(LPSHORT)&d1Width);
 short ColWidth = (rc.right - rc.left) - d0Width  - d1Width - GetSystemMetrics(SM_CXHSCROLL);
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
 SSSetData(SHWindow,1,SS_HEADER,"Btch No.");
 SSSetData(SHWindow,2,SS_HEADER,"Inv No.");
 SSSetData(SHWindow,3,SS_HEADER,"Description");
 apbtch->SetIndex("batchno");
 HourGlassOff();

}


void TAPBatchList::HandleListBox (RTMessage Msg)
{
 char szBuffer[10];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetData(SHWindow,1,Cell->Row,szBuffer);
 long lRecord=atol(szBuffer);
 apbtch->SetIndex("batchno");
 if(apbtch->seek(lRecord)==0)
 {
  SendMessage(Parent->HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
  //Show(SW_HIDE);
 }
 SetFocus(Parent->HWindow);
}


void TGlWindow::APBatch       (RTMessage)
 {
  GetApplication()->ExecDialog(new TAPInvoiceEntry(this,"AP_BATCH"));
 }

char *TAPInvoiceEntry::GetControlAcct(char *Account,char *Depart)
{
 static char szDescript[40];
 strcpy(szDescript,Account);
 strcat(szDescript,Depart);
 if(glacct->seek(szDescript) == 0)
 strcpy(szDescript,glacct->descript->str());
 else
 strcpy(szDescript,"");
 return szDescript;

}

void TAPDistList::SetListSize (int Rows)
{
 RECT rc,wd;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 short iHeight;
 LOGFONT lfFont;
 HFONT   hFont;
 lfFont.lfHeight          = -12;
 lfFont.lfWidth           = 0;
 lfFont.lfEscapement      = 0;
 lfFont.lfOrientation     = 0;
 lfFont.lfWeight          = FW_NORMAL;
 lfFont.lfItalic          = 0;
 lfFont.lfUnderline       = 0;
 lfFont.lfStrikeOut       = 0;
 lfFont.lfCharSet         = 1; //default character set.
 lfFont.lfOutPrecision    = 0;
 lfFont.lfClipPrecision   = 0;
 lfFont.lfQuality         = 0;
 lfFont.lfPitchAndFamily  = FF_SWISS;
 StrCpy (lfFont.lfFaceName, "HELV");
 hFont = CreateFontIndirect((LPLOGFONT) &lfFont);
 SSSetFont(SHWindow,SS_ALLCOLS, SS_ALLROWS,hFont, TRUE); 

 SSSetBool(SHWindow,SSB_AUTOSIZE,FALSE);
 SSSetBool(SHWindow,SSB_ALLOWUSERSELBLOCK,TRUE);
 SSSetBool(SHWindow,SSB_SCROLLBAREXTMODE,TRUE);
 SSSetBool(SHWindow,SSB_NOBEEP,TRUE);
 SSSetBool(SHWindow,SSB_ALLOWUSERRESIZE,FALSE); 
 SSSetBool(SHWindow,SSB_PROTECT,TRUE);
 SSSetBool(SHWindow,SSB_SHOWROWHEADERS,FALSE);
 SSSetBool(SHWindow,SSB_NOBEEP,TRUE);
 SSSetAutoSizeVisible(SHWindow,6,Rows);

 SSGetRowHeightInPixels(SHWindow,1,&iHeight);
 int Height =  GetSystemMetrics(SM_CYCAPTION)+ 2*GetSystemMetrics(SM_CXDLGFRAME) + (Rows*iHeight) + 1;
 GetWindowRect(HWindow,&wd);
 MoveWindow(HWindow,wd.left,wd.top,wd.right-wd.left,Height,TRUE);
 GetClientRect(HWindow,&rc); 
 GetWindowRect(GetDlgItem(HWindow,IDD_STATUSBOX),&wd);
 MoveWindow(SHWindow,rc.left+1, rc.top, rc.right - (wd.right-wd.left) , rc.bottom - rc.top, TRUE);
 GetWindowRect(SHWindow,&rc);
 MoveWindow(GetDlgItem(HWindow,IDD_STATUSBOX),rc.right,rc.top,rc.right-rc.left -(rc.right - (wd.right-wd.left)),rc.bottom-rc.top,TRUE);
 SSSetMaxRows(SHWindow,Rows+1);
}


