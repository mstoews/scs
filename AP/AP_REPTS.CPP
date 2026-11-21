
#include "ap.h"
extern HINSTANCE hInst;

BOOL PrintCR(HWND HWindow,char *szReportName)
{
  HANDLE textHandle;
  char   *szBuffer;
  short  iBufferlen;  
  short JobNumber = PEOpenPrintJob(szReportName);
  if(JobNumber == 0)
   {
   return FALSE;
   }
  else
  {
   PEGetReportTitle(JobNumber,&textHandle,&iBufferlen);
   szBuffer = (char *)malloc(iBufferlen);
   if(szBuffer != 0)
   {
    PEGetHandleString(textHandle,szBuffer,iBufferlen);
   }
   PEOutputToWindow(JobNumber,szBuffer,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL,HWindow);
   PEStartPrintJob(JobNumber,TRUE);  
   PEClosePrintJob(JobNumber);  
   free(szBuffer);
   return TRUE;
  }
}


void TGlWindow::APVenLists    (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_vend.rpt"))
     BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

// Detailed Open Receivables
void TGlWindow::APOpenAccts   (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_detl.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APPrintBatch  (RTMessage)  // Print AP posting batch file.
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"apbtchsm.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

}

void TGlWindow::APPrintCash  (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_cashb.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
}

void TGlWindow::APPrintAdj  (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_adjst.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
}



void TGlWindow::APSummary     (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_bala.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }


void TGlWindow::APOverDue     (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_overd.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APAgedCash    (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_agedc.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }

void TGlWindow::APAgedRetain  (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_agedr.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APBatchStatus (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_bstat.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }


void TGlWindow::APPostJournal (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_postj.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }

void TGlWindow::APVatAPReport   (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_vatap.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APVatGLReport   (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_vatgl.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APChequeAP(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_cheqa.rpt")) // cheque ap report
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APChequeCash(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_cheqc.rpt")) // cheque cash report
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APJournalCash(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_jsuma.rpt")) // summary of ap entries
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APJournalAP(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_jsumc.rpt")) // summary of cash entries
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APBatchCash(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_btchd.rpt")) // batch detail AP
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APBatchAP(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_cashd.rpt")) // batch detail Cash
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APGLSummary(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_jsum.rpt")) // cheque cash report
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }

void TGlWindow::APGLDetail(RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_jsumd.rpt")) // cheque cash report
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);
 }





void TGlWindow::APAdjBatchPrt (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_adjbt.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }

void TGlWindow::APPrePayment (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_prepy.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }

void TGlWindow::APRequestSummary (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_reqst.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }

void TGlWindow::APRequestDetail (RTMessage)
 {
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_reqsd.rpt"))
  BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,ID_PRINTERROR),GetString(hInst,ID_WARNING),MB_ICONEXCLAMATION);

 }


