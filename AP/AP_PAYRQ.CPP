#include "ap.h"
#include "gl_dlg.h"
#include "db_apbtc.h"
#include "db_vend.h"
#include "db_dist.h"
#include "math.h"
#include "db_reqst.h"
#include "db_apdst.h"



extern Code4 cb;

_CLASSDEF (TAPCreatePayment)
class TAPCreatePayment : public TMainDialog
{
 public:
  BOOL          bHasInvoices;
  SS_CELLTYPE 	CellType;
  LOGFONT 	lfFont;
  HFONT   	hFont;
  HWND 		SHWindow;
  PTapdist      apdist;
  PTapvendor    apvendor;
  PTapbtch	apbtch; 
  PTapreqst     apreqst;
  PTSpread      InvoiceList;
  TAPCreatePayment(PTWindowsObject AParent, LPSTR ATitle):TMainDialog(AParent,ATitle)
  {
  InvoiceList = new TSpread(this,ID_DISTLIST);
  bHasInvoices = FALSE;
  };
  virtual void SetupWindow();
  virtual void SelectCell     (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  virtual void SetDate        (RTMessage Msg) = [ID_FIRST + ID_DUEDATE];
  virtual void Ok	      (RTMessage Msg) = [ID_FIRST + IDOK]; // Create the request.
  virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Print          (RTMessage Msg) = [ID_FIRST + ID_PRINT];
  virtual void ViewAll        (RTMessage Msg) = [ID_FIRST + ID_VIEWALL];
  virtual BOOL DistList(char *szQuery);
  void 	  SetRequestNumber(char *Vendorno,int iStartingReq);
  virtual void UpdateRequestSummary();
  void         PrintPaymentList(void);
};

class TAPHoldForce: public TMainDialog
{
 public: 
 PTapdist apdist;
 BOOL Hold;
 BOOL Force;
 TAPHoldForce(PTWindowsObject AParent, LPSTR AName,PTapdist a): TMainDialog(AParent,AName)
 {
  apdist = a;
 };
 virtual void SetupWindow();
 virtual void Ok            (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel        (RTMessage Msg) = [ID_FIRST + IDCANCEL] {ShutDownWindow();};
};

void TAPHoldForce::SetupWindow()
{
 TDialog::SetupWindow();
 SetField(ID_VENDORNO,apdist->vendorno);
 SetField(ID_INVOICENO,apdist->invoiceno);
}

void TAPHoldForce::Ok(RTMessage)
  {
   if(SendDlgItemMsg(ID_FORCE,BM_GETSTATE,NULL,NULL))
    {
     apdist->paymentctl->assign("F");
     apdist->flush();
    }
   else if (SendDlgItemMsg(ID_HOLD,BM_GETSTATE,NULL,NULL))
    {
     apdist->paymentctl->assign("H");
     apdist->flush();
    }
   else
    {
     apdist->paymentctl->assign("O");
     apdist->flush();
    }
   ShutDownWindow();
  };


void TAPCreatePayment::SelectCell(RTMessage Msg)
{
 char szVendorno[21];
 char szInvoiceno[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 SSGetData(SHWindow,3,Cell->Row,szVendorno);
 SSGetData(SHWindow,4,Cell->Row,szInvoiceno);
 strcat(szVendorno,szInvoiceno);
 apdist->SetIndex("invoiceno");
 if(apdist->seek(szVendorno)==0)
 {
  GetApplication()->ExecDialog(new TAPHoldForce(this,"AP_HOLDFORCE",apdist));
 }
 SSSetData(SHWindow,2,Cell->Row,apdist->paymentctl->str());
}

void TAPCreatePayment::UpdateRequestSummary()
{

   apdist->SetIndex("paymentreq");
   apreqst->SetIndex("paymentreq");
   Relate4set *rQuery = new Relate4set(apdist);
   rQuery->querySet("PAYMENTCTL = 'R'");
   for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
    {
      if(apreqst->seek(apdist->paymentreq->str()) != 0)
       {
	apreqst->appendStart();
        apreqst->vendorno->assign(apdist->vendorno->str());
	apreqst->paymentreq->assign(apdist->paymentreq->str());
	apreqst->reference ->assign(apdist->reference->str());
	apreqst->amountpaid->assignDouble(apdist->dAmount()+apdist->dAdjustment()-fabs(apdist->dRebateamt()));
	apreqst->datepaid  ->assign(apdist->datepaid->str());
        apreqst->append();
       }
       else
       {
	apreqst->amountpaid->assignDouble(apdist->dAmount()
					  +apdist->dAdjustment()
					  -fabs(apdist->dRebateamt())
					  +apreqst->dAmountpaid());
        apreqst->flush();
       }
    }
    rQuery->unlock();
    rQuery->free();   
}


void TAPCreatePayment::Print(RTMessage)
{
  if(DlgMsg(ID_INVSUMMARY,ID_CHEQUE,MB_YESNO|MB_ICONQUESTION) == IDYES)
  PrintPaymentList();
}

void TGlWindow::APCreateGLSlip(RTMessage)
{
 GetApplication()->ExecDialog(new TAPCreatePayment(this,"AP_CREATESLIP"));
}


void TAPCreatePayment::Ok (RTMessage)
{ 
 HourGlassOn();
 unsigned int iStartingReq = 0;
 IntGetDlgItemValue(HWindow,ID_APCONTROL,(LPLONG)&iStartingReq);
 if(iStartingReq  == 0)
 {
  DlgMsg(ID_REQUESTNO,ID_CHEQUE,MB_OK|MB_ICONINFORMATION);   // If no request number return.
  return;
 }
 apdist->SetIndex("vendorno");
 Date4 dPayment;
 dPayment.assign(GetField(ID_DATE),"MM/DD/YY");
 dPayment.format("CCCCMMDD");
 BOOL used = FALSE;
 iStartingReq--;
 if(bHasInvoices)
 {
  if(DlgMsg(8003,ID_CHEQUE,MB_ICONQUESTION | MB_YESNO) == IDYES)
  {
   //loop to add amount paid and to register requested flag flag.
    apdist->top();  
    char szVendorNo[7];
    strcpy(szVendorNo,apdist->vendorno->str());
     while(!apdist->eof())
     {
      Date4 dDueDate(apdist->duedate->str());
      if(dDueDate <= dPayment)
      if(strcmp(apdist->paymentctl->str(),"O") == 0 || strcmp(apdist->paymentctl->str(),"F") == 0)
	 {
	  apdist->paymentctl->assign("R");
	  if(strcmp(szVendorNo,apdist->vendorno->str()) != 0)
	    iStartingReq++;
	  apdist->paymentreq->assignLong(iStartingReq);
          strcpy(szVendorNo,apdist->vendorno->str());
	  apdist->flush();
          SetField(IDD_STATUSBOX,apdist->paymentreq->str());
	 }
      apdist->skip();
    }
  }
 UpdateRequestSummary();
 bHasInvoices = DistList("PAYMENTCTL = 'H' .OR. PAYMENTCTL = 'F' .OR. PAYMENTCTL = 'O'");
 }
 else
 DlgMsg(ID_NOINVOICES,ID_CHEQUE,MB_OK); 
 HourGlassOff();
 return;
}

void TAPCreatePayment::SetRequestNumber(char *szVendorno,int iRequestNumber)
 {
 }

void TAPCreatePayment::SetDate (RTMessage)
{
 bHasInvoices = DistList("PAYMENTCTL = 'H' .OR. PAYMENTCTL = 'F' .OR. PAYMENTCTL = 'O'");
 if(!bHasInvoices)
  DlgMsg(ID_NOINVOICES,ID_SETDATE,MB_OK);
}

void TAPCreatePayment::ViewAll(RTMessage)
{
 DistList("PAYMENTCTL <> ' '");
}

void TAPCreatePayment::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 RECT rc;
 InvoiceList->GetClientRect(&rc);
 short d0Width,d2Width,d3Width,d4Width,d5Width,d6Width,d7Width,d8Width,d9Width;
 InvoiceList->SetMaxCols(9);
 InvoiceList->SetColWidth(2,3);
 InvoiceList->SetColWidth(3,6);
 InvoiceList->SetColWidth(4,10);
 InvoiceList->SetColWidth(5,10);
 InvoiceList->SetColWidth(6,10);
 InvoiceList->SetColWidth(7,10);
 InvoiceList->SetColWidth(8,10);
 InvoiceList->SetColWidth(9,10);


 InvoiceList->GetColWidthInPixels(0,(LPSHORT)&d0Width);
 InvoiceList->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 InvoiceList->GetColWidthInPixels(3,(LPSHORT)&d3Width);
 InvoiceList->GetColWidthInPixels(4,(LPSHORT)&d4Width);
 InvoiceList->GetColWidthInPixels(5,(LPSHORT)&d5Width);
 InvoiceList->GetColWidthInPixels(6,(LPSHORT)&d6Width);
 InvoiceList->GetColWidthInPixels(7,(LPSHORT)&d7Width);
 InvoiceList->GetColWidthInPixels(8,(LPSHORT)&d8Width);
 InvoiceList->GetColWidthInPixels(9,(LPSHORT)&d9Width);
 
 short ColWidth = (rc.right - rc.left)
		  - d0Width
		  - d2Width
		  - d3Width
		  - d4Width
		  - d5Width
		  - d6Width
		  - d7Width
		  - d8Width
		  - d9Width;

 InvoiceList->SetColWidthInPixels(1,ColWidth);

 lfFont.lfHeight          = 8;
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

 InvoiceList->SetBool(SSB_ALLOWUSERSELBLOCK,FALSE);
 InvoiceList->SetBool(SSB_PROTECT,TRUE);
 InvoiceList->SetBool(SSB_SCROLLBAREXTMODE,TRUE);
 InvoiceList->SetBool(SSB_NOBEEP,TRUE);
 InvoiceList->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 InvoiceList->SetBool(SSB_VERTSCROLLBAR,TRUE);

 InvoiceList->SetMaxRows(1000);
 InvoiceList->SetFont(SS_ALLCOLS, SS_ALLROWS,hFont, TRUE);
 InvoiceList->SetFont(SS_ALLCOLS, SS_HEADER,GetStockObject(hFont), TRUE);
 InvoiceList->SetData(1,SS_HEADER, GetString(ID_VENDORNAME));
 InvoiceList->SetData(2,SS_HEADER, GetString(ID_CONTROL));
 InvoiceList->SetData(3,SS_HEADER, GetString(ID_VENDORCODE));
 InvoiceList->SetData(4,SS_HEADER, GetString(ID_INVOICENO));
 InvoiceList->SetData(5,SS_HEADER, GetString(ID_DUEDATE));
 InvoiceList->SetData(6,SS_HEADER, GetString(ID_AMOUNT));
 InvoiceList->SetData(7,SS_HEADER, GetString(ID_ADJUSTMENT));
 InvoiceList->SetData(8,SS_HEADER, GetString(ID_REBATE));
 InvoiceList->SetData(9,SS_HEADER, GetString(ID_AMOUNTDUE));
 InvoiceList->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 InvoiceList->SetTypeEdit(&CellType,ES_LEFT,40,SS_CHRSET_CHR,SS_CASE_NOCASE);
 InvoiceList->SetCellType(1, SS_ALLROWS, &CellType);
 InvoiceList->SetTypeEdit(&CellType,ES_LEFT,10,SS_CHRSET_CHR,SS_CASE_NOCASE);  // was StaticText
 InvoiceList->SetCellType(2, SS_ALLROWS, &CellType);
 InvoiceList->SetTypeEdit(&CellType,ES_RIGHT,10,SS_CHRSET_CHR,SS_CASE_NOCASE);  // was StaticText
 InvoiceList->SetCellType(3, SS_ALLROWS, &CellType);
 InvoiceList->SetCellType(4, SS_ALLROWS, &CellType);
 InvoiceList->SetCellType(5, SS_ALLROWS, &CellType);
 InvoiceList->SetTypeFloat(&CellType,FS_SEPARATOR,9,2,0,999999999.99);
 InvoiceList->SetCellType(6, SS_ALLROWS, &CellType);
 InvoiceList->SetCellType(7, SS_ALLROWS, &CellType);
 InvoiceList->SetCellType(8, SS_ALLROWS, &CellType);
 InvoiceList->SetCellType(9, SS_ALLROWS, &CellType);


 // Open databases...
 apdist   =  new Tapdist(&cb);
 apvendor =  new Tapvendor(&cb);
 apreqst  =  new Tapreqst(&cb);
 apbtch   =  new Tapbtch(&cb);
 apreqst->SetIndex("paymentreq");

 //bHasInvoices = DistList();
 bHasInvoices = FALSE;
 apreqst->bottom();
 int iNewRequestNumber =1;
 if(int (Str4ptr(apreqst->paymentreq->str())) > 0)
 {
  iNewRequestNumber = int (Str4ptr(apreqst->paymentreq->str())) + 1;
 }
 IntSetDlgItemValue(HWindow,ID_APCONTROL,iNewRequestNumber);
 HourGlassOff();

}

void TAPCreatePayment::Cancel  (RTMessage)
{
 cb.closeAll();
 ShutDownWindow();
}
BOOL TAPCreatePayment::DistList(char *szQuery)
{
 HourGlassOn();
 int l = 1;
 char *szControl = new char [2];
 SetField(IDD_STATUSBOX,"Checking for invoices that are due.");
 Date4 dPayment;
 dPayment.assign(GetField(ID_DATE),"MM/DD/YY");
 dPayment.format("CCCCMMDD");
 InvoiceList->SetBool(SSB_REDRAW,FALSE);
 InvoiceList->ClearData(SS_ALLCOLS,SS_ALLROWS);
 apdist->SetIndex("invoiceno");
 Relate4set *rQuery = new Relate4set(apdist);
 rQuery->querySet(szQuery);
    for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
    {
       Date4 dDueDate(apdist->duedate->str());
	 if( dDueDate <= dPayment )
	 {
	    apvendor->seek(apdist->vendorno->str());
	    InvoiceList->SetData(1,l,apvendor->vdshort->str());  // Vendor Name
            InvoiceList->SetData(2,l,apdist->paymentctl->str()); // Payment Control
	    InvoiceList->SetData(3,l,apdist->vendorno->str());  // Vendor Code
	    InvoiceList->SetData(4,l,apdist->invoiceno->str()); // Invoice Number
	    InvoiceList->SetData(5,l,apdist->duedate->str());   // Due Date
	    InvoiceList->SetData(6,l,apdist->amount->str());   // Invoice Amount
	    InvoiceList->SetData(7,l,apdist->adjustment->str()); // Adjustment
	    InvoiceList->SetData(8,l,apdist->rebateamt->str()); // Adjustment
	    double dAmountDue = apdist->dAmount()+apdist->dAdjustment()-fabs(apdist->dRebateamt());
	    char szBuf[16];
	    gcvt(dAmountDue,16,szBuf);
	    InvoiceList->SetData(9,l,szBuf); // Adjustment
	    l++;
         }    
    } 
    rQuery->unlock();
    rQuery->free();

 if(l>15)
 InvoiceList->SetMaxRows(l);
 else
 InvoiceList->SetMaxRows(16);
 InvoiceList->SetBool(SSB_REDRAW,TRUE);

 apreqst->bottom();
 int iNewRequestNumber =1;
 if(int (Str4ptr(apreqst->paymentreq->str())) > 0)
 {
  iNewRequestNumber = int (Str4ptr(apreqst->paymentreq->str())) + 1;
 }
 IntSetDlgItemValue(HWindow,ID_APCONTROL,iNewRequestNumber);
 SetField(IDD_STATUSBOX,"Completed search...");
 delete szControl;
 HourGlassOff();
 if(l>1)
 return TRUE;
 else
 return FALSE;
}


void TAPCreatePayment::PrintPaymentList(void)
{
  if(!PrintCR(GetApplication()->MainWindow->HWindow,"ap_reqst.rpt"))
  BWCCMessageBox(HWindow,GetString(ID_PRINTERROR),GetString(ID_WARNING),MB_ICONEXCLAMATION);
  return;
}
