#include "ap.h"
#include "db_dept.h"
#include "math.h"
#include "db_cash.h"
#include "db_apbtc.h"
#include "ap_class.h"

extern Date4 InvoiceDate;
extern Date4 DueDate;




#pragma argsused
void TAPInvoiceEntry::CreateNewApSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D)
{  


   int iLineno = 1;
   // Calculate the VAT amount
   double EXPENSE = fabs(lInvoiceAmount);

   // Add line for expense amount
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign(szDescript);
   apbtchd->account->assign(szAccounts);  // AP account is always set to the default.s
   apbtchd->dept->assign(szDepart);        // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
    apbtchd->debit->assignDouble(EXPENSE);
    apbtchd->credit->assignDouble(0.00);
   }
   else if (lInvoiceAmount < 0)
   {
    apbtchd->debit->assignDouble(0.00);
    apbtchd->credit->assignDouble(EXPENSE);
   }
   apbtchd->append();
   apbtchd->unlock();

   iLineno++;

   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign( GetControlAcct(GetField(ID_APCONTROL),GetField(ID_APDEPART)) );
   apbtchd->account->assign(GetField(ID_APCONTROL));  
   apbtchd->dept->assign(GetField(ID_APDEPART));

   if(lInvoiceAmount > 0)
   {
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(EXPENSE);
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(EXPENSE);
   apbtchd->credit->assignDouble(0.00);
   }
   apbtchd->append();
   apbtchd->unlock();

   bSlipList = TRUE;
   if(apbtch->seek(iBatchno) != 0)
   {
   apbtch->appendStart();
   apbtch->batchno->assignLong(iBatchno);
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
   apbtch->vatpercent->assignDouble(dGetField(ID_VATPERCENTAGE));
   apbtch->entryno->assignLong(lGetField(ID_LINENO));
   apbtch->terms->assignLong(lGetField(ID_TERMS));
   apbtch->append();
   apbtch->unlock();
   }
   else
   {
   apbtch->payable->assignDouble(apbtch->dPayable() + EXPENSE);
   apbtch->adjamt->assignDouble(apbtch->dAdjamt()+ EXPENSE);
   apbtch->flush();
   apbtch->unlock();
   }

   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
   return;
}



#pragma argsused
void TAPInvoiceEntry::CreateVatSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D)
{

    dVatPercentage = (dVatPercentage + 100)/100;
   // Calculate the VAT amount
   double AP      = fabs(lInvoiceAmount);
   double EXPENSE = AP/dVatPercentage;
   double VAT     = AP-EXPENSE;



   int iLineno = 1;
   DWORD nRec_no;
   
   
   // Add line for expense amount
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign(szDescript);
   apbtchd->account->assign(GetField(ID_ACCOUNTS));  // AP account is always set to the default.s
   apbtchd->dept->assign(GetField(ID_DEPART));        // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
    apbtchd->debit->assignDouble(EXPENSE);
    apbtchd->credit->assignDouble(0.00);
   }
   else if (lInvoiceAmount < 0)
   {
    apbtchd->debit->assignDouble(0.00);
    apbtchd->credit->assignDouble(EXPENSE);
   }

   apbtchd->append();
   apbtchd->unlock();

   // Add line for VAT amount.

   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign("Value Added Tax");
   apbtchd->account->assign(szVATAccount);  // AP account is always set to the default.s
   apbtchd->dept->assign(szVATDept);        // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
   apbtchd->debit->assignDouble(VAT);
   apbtchd->credit->assignDouble(0.00);    //VAT credit is set to zero.
   }
   else if(lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(VAT);    //VAT credit is set to zero.
   }
   apbtchd->append();
   apbtchd->unlock();

   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign(GetControlAcct(GetField(ID_APCONTROL),GetField(ID_APDEPART)));
   apbtchd->account->assign(GetField(ID_APCONTROL));  // AP account is always set to the default.
   apbtchd->dept->assign(GetField(ID_APDEPART));           // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {
    apbtchd->debit->assignDouble(0.00);
    apbtchd->credit->assignDouble(AP);
   }
   else if (lInvoiceAmount < 0)
   {
    apbtchd->debit->assignDouble(AP);
    apbtchd->credit->assignDouble(0.00);
   }
   apbtchd->append();
   apbtchd->unlock();

   if(apbtch->seek(iBatchno) != 0)
   {
   apbtch->appendStart();
   apbtch->batchno->assignLong(iBatchno);
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
   apbtch->payable->assignDouble(AP);
   apbtch->vatamt->assignDouble(VAT);   
   apbtch->vatpercent->assignDouble(dGetField(ID_VATPERCENTAGE));
   apbtch->rebateamt->assignDouble(0);
   apbtch->entryno->assignLong(lGetField(ID_LINENO));
   apbtch->terms->assignLong(lGetField(ID_TERMS));
   apbtch->append();
   apbtch->unlock();
   }
   else
   {
   apbtch->payable->assignDouble(apbtch->dPayable() + AP);
   apbtch->vatamt->assignDouble(apbtch->dVatamt()+VAT);
   apbtch->adjamt->assignDouble(apbtch->dAdjamt()+AP);
   apbtch->flush();
   apbtch->unlock();
   }


   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
   return;
}

#pragma argsused
void TAPInvoiceEntry::CreateRebateSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D)
{  

   char szSeekAccount[12];
   char szAccount[8];
   char szDepart[5];

   // Check to see if the vendor is an individual or a company.
   if(SendDlgItemMsg(ID_INDIVIDUAL,BM_GETSTATE,NULL,NULL))
   {
    strcpy(szAccount,szIndTaxAccount);
    strcpy(szDepart,szIndTaxDept);
   }
   else
   if(SendDlgItemMsg(ID_COMPANY,BM_GETSTATE,NULL,NULL))
   {
    strcpy(szAccount,szCompanyTaxAccount);
    strcpy(szDepart,szCompanyTaxDept);
   } 


   double EXPENSE = fabs(lInvoiceAmount);
   double REBATE  = EXPENSE*(dRebatePercentage/100);
   double AP      = EXPENSE - REBATE;
   

   int iEntryNo  = 1;
   // Add line for expense amount


   // Add Accounts Payable amount.
   
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   strcpy(szSeekAccount,GetField(ID_APCONTROL));
   strcat(szSeekAccount,GetField(ID_APDEPART));
   glacct->seek(szSeekAccount);
   apbtchd->descript->assign(glacct->descript->str());
   apbtchd->account->assign(GetField(ID_APCONTROL));  // AP account is always set to the default.
   apbtchd->dept->assign(GetField(ID_APDEPART));           // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(AP);
   }
   else if(lInvoiceAmount > 0)
   {
   apbtchd->debit->assignDouble(AP);
   apbtchd->credit->assignDouble(0.00);
   }

   apbtchd->append();
   apbtchd->unlock();


   // Add taxes payable line.
   iEntryNo++;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   strcpy(szSeekAccount,szAccount);
   strcat(szSeekAccount,szDepart);
   glacct->seek(szSeekAccount);
   apbtchd->descript->assign(glacct->descript->str());
   apbtchd->account->assign(szAccount);  // AP account is always set to the default.s
   apbtchd->dept->assign(szDepart);        // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(REBATE);    //VAT credit is set to zero.
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(REBATE);
   apbtchd->credit->assignDouble(0.00);    //VAT credit is set to zero.
   }

   apbtchd->append();
   apbtchd->unlock();

   iEntryNo++;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iLineno);
   apbtchd->descript->assign(szDescript);
   apbtchd->account->assign(GetField(ID_ACCOUNTS));  // AP account is always set to the default.s
   apbtchd->dept->assign(GetField(ID_DEPART));        // AP department code is set to the default dept code.
   if(lInvoiceAmount > 0)
   {   
    apbtchd->debit->assignDouble(EXPENSE);
    apbtchd->credit->assignDouble(0.00);
   }
   else if (lInvoiceAmount < 0)
   {
    apbtchd->debit->assignDouble(0.00);
    apbtchd->credit->assignDouble(EXPENSE);
   }

   apbtchd->append();
   apbtchd->unlock();
   if(apbtch->seek(iBatchno) != 0)
   {
   apbtch->appendStart();
   apbtch->batchno->assignLong(iBatchno);
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
   apbtch->vatpercent->assignDouble(dGetField(ID_VATPERCENTAGE));
   apbtch->rebate->assignDouble(dRebatePercentage);
   apbtch->descript->assign(GetField(ID_JDESCRIPT));
   apbtch->account->assign(GetField(ID_APCONTROL));
   apbtch->depart->assign(GetField(ID_APDEPART));   
   apbtch->payable->assignDouble(EXPENSE);
   apbtch->vatamt->assignDouble(0);
   apbtch->rebateamt->assignDouble(-REBATE);

   apbtch->entryno->assignLong(lGetField(ID_LINENO));
   apbtch->terms->assignLong(lGetField(ID_TERMS));
   apbtch->append();
   apbtch->unlock();
   }
   else
   {
   apbtch->payable->assignDouble(apbtch->dPayable() + EXPENSE);
   apbtch->rebateamt->assignDouble(apbtch->dRebateamt()+REBATE);
   apbtch->adjamt->assignDouble(apbtch->dAdjamt()+EXPENSE);
   apbtch->flush();
   apbtch->unlock();
   }



   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
   return;
}

#pragma argsused
void TAPInvoiceEntry::CreateVat_RebateSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D)
{   


   char szSeekAccount[12];
   char szAccount[8];
   char szDepart[5];
   // Check to see if the vendor is an individual or a company.
   if(SendDlgItemMsg(ID_INDIVIDUAL,BM_GETSTATE,NULL,NULL))
   {
    strcpy(szSeekAccount,szIndTaxAccount);
    strcat(szSeekAccount,szIndTaxDept);
    strcpy(szAccount,szIndTaxAccount);
    strcpy(szDepart,szIndTaxDept);
   }
   else
   if(SendDlgItemMsg(ID_COMPANY,BM_GETSTATE,NULL,NULL))
   {
    strcpy(szSeekAccount,szCompanyTaxAccount);
    strcat(szSeekAccount,szCompanyTaxDept);
    strcpy(szAccount,szCompanyTaxAccount);
    strcpy(szDepart,szCompanyTaxDept);
   } 

   // Add the entry to the batch master.


   // Calculate the Invoice Amounts.
   FloatGetDlgItemValue(HWindow,ID_VATPERCENTAGE, (LPDOUBLE) &dVatPercentage);
   dVatPercentage = (dVatPercentage + 100)/100;
   double AMOUNT  = fabs(lInvoiceAmount);   
   double EXPENSE = AMOUNT/dVatPercentage;
   double VAT     = AMOUNT-EXPENSE;
   double REBATE  = EXPENSE*(dRebatePercentage/100);
   double AP      = AMOUNT - REBATE;

   // Add Expense amount to detail database.

   int iEntryNo = 1;
   DWORD nRec_no;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   apbtchd->descript->assign(szDescript);
   apbtchd->account->assign(GetField(ID_ACCOUNTS)); 
   apbtchd->dept->assign(GetField(ID_DEPART));
   if(lInvoiceAmount > 0)
   {
   apbtchd->debit->assignDouble(EXPENSE);
   apbtchd->credit->assignDouble(0.00);
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(EXPENSE);
   }
   apbtchd->append();
   apbtchd->unlock();

   // Add VAT amount to detail database.
   iEntryNo++;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   apbtchd->descript->assign("Value Added Tax");
   apbtchd->account->assign(szVATAccount);  
   apbtchd->dept->assign(szVATDept);
   if(lInvoiceAmount > 0)
   {
   apbtchd->debit->assignDouble(VAT);      //VAT credit is set to zero.
   apbtchd->credit->assignDouble(0.00);  
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(0.00);    //VAT credit is set to zero.
   apbtchd->credit->assignDouble(VAT);   
   }
   apbtchd->append();
   apbtchd->unlock();

   // Add AP amount.

   iEntryNo++;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   apbtchd->descript->assign("Accounts Payable Control");
   apbtchd->account->assign(GetField(ID_APCONTROL));  
   apbtchd->dept->assign(GetField(ID_APDEPART));      
   if(lInvoiceAmount > 0)
   {   
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(AP);   
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(AP);
   apbtchd->credit->assignDouble(0.00); 
   }
   apbtchd->append();
   apbtchd->unlock();

   // Add REBATE amount.

   iEntryNo++;
   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(iEntryNo);
   glacct->seek(szSeekAccount);
   apbtchd->descript->assign(glacct->descript->str());
   apbtchd->account->assign(szAccount);
   apbtchd->dept->assign(szDepart);    
   if(lInvoiceAmount > 0)
   {   
   apbtchd->debit->assignDouble(0.00);
   apbtchd->credit->assignDouble(REBATE);
   }
   else if (lInvoiceAmount < 0)
   {
   apbtchd->debit->assignDouble(REBATE);
   apbtchd->credit->assignDouble(0.00);
   }


   apbtchd->append();
   apbtchd->unlock();
   if(apbtch->seek(iBatchno) != 0)
   {
   apbtch->appendStart();
   apbtch->batchno->assignLong(iBatchno);
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
   apbtch->vatpercent->assignDouble(dGetField(ID_VATPERCENTAGE));
   apbtch->rebate->assignDouble(dRebatePercentage);
   apbtch->descript->assign(GetField(ID_JDESCRIPT));
   apbtch->account->assign(GetField(ID_APCONTROL));
   apbtch->depart->assign(GetField(ID_APDEPART));   
   apbtch->payable->assignDouble(AMOUNT);
   apbtch->vatamt->assignDouble(VAT);
   apbtch->rebateamt->assignDouble(-REBATE);
   apbtch->entryno->assignLong(lGetField(ID_LINENO));
   apbtch->terms->assignLong(lGetField(ID_TERMS));
   apbtch->append();
   apbtch->unlock();
   }
   else
   {
   apbtch->payable->assignDouble(apbtch->dPayable() + AMOUNT);
   apbtch->vatamt->assignDouble(apbtch->dVatamt()+VAT);
   apbtch->rebateamt->assignDouble(apbtch->dRebateamt()+REBATE);
   apbtch->adjamt->assignDouble(apbtch->dAdjamt()+AMOUNT);
   apbtch->flush();
   apbtch->unlock();
   }



   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);
   return;
}

void TAPInvoiceEntry::CreateLineItem(PTapbtch apbtch,PTapbtchd apbtchd)
{
   int iLine;
   IntGetDlgItemValue(HWindow,3033,(LPLONG)&iLine);

   FloatGetDlgItemValue(D->HWindow,ID_DEBITTOTAL,&DebitTotal);
   FloatGetDlgItemValue(D->HWindow,ID_CREDITTOTAL,&CreditTotal);
   apbtch->appendStart();
   apbtch->batchno->assignLong(iBatchno);
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
   apbtch->append();
   apbtch->unlock();



   apbtchd->appendStart();
   apbtchd->batchno->assignLong(iBatchno);
   apbtchd->entryno->assignLong(1);
   apbtchd->descript->assign(szDescript);
   apbtchd->account->assign(GetField(ID_ACCOUNTS)); 
   apbtchd->dept->assign(GetField(ID_DEPART));
   if(dGetField(ID_DEBIT) > 0)
   {        
    apbtchd->debit->assignDouble(dGetField(ID_DEBIT));
    apbtchd->credit->assignDouble(0.00);
   }
   else if (dGetField(ID_CREDIT) > 0)
   {
    apbtchd->debit->assignDouble(0.00);
    apbtchd->credit->assignDouble(dGetField(ID_CREDIT));
   }
   apbtchd->append();
   apbtchd->unlock();

   SendMessage(HWindow,WM_COMMAND,CM_CURRENTBATCH,NULL);   
}