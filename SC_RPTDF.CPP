/*** Printing Master file - SCS Standard Include File***/
#include "scs.h"
// Printer allocation
#include "crpe.h"
// Database files
#include "db_elem.h"


/*** Reporting: Source ***/
_CLASSDEF(TRptSource)
class TRptSource : public TDialog
{
 public:
  TRptSource (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
};


extern char *szCompanyName;


/*** Reporting: Allocation ***/
_CLASSDEF(TRptDefn)
class TRptDefn : public TDialog
{

  BOOL bMaterialUsage;
  BOOL bMaterialPrice;
  BOOL bLabourUsage;
  BOOL bLabourPrice;
  BOOL bOverheadUsage;
  BOOL bOverheadPrice;
  BOOL bSearch;
 public:
  TRptDefn (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  PTSCItemList2  ITEM;
  PTscitemm  Items;
  virtual void SetupWindow();
  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Search(RTMessage Msg) = [ID_FIRST + ID_SEARCH];
  virtual void MaterialUsage(RTMessage Msg) = [ID_FIRST + ID_MATERIAL_U];
  virtual void MaterialPrice(RTMessage Msg) = [ID_FIRST + ID_MATERIAL_P];
  virtual void LabourUsage  (RTMessage Msg) = [ID_FIRST + ID_LABOUR_U];
  virtual void LabourPrice  (RTMessage Msg) = [ID_FIRST + ID_LABOUR_P];
  virtual void OverheadUsage(RTMessage Msg) = [ID_FIRST + ID_OVERHEAD_U];
  virtual void OverheadPrice(RTMessage Msg) = [ID_FIRST + ID_OVERHEAD_P];
  void    PrtReport(char *szReportName, char *szReportTitle);
};

void TRptDefn::SetupWindow()
{
 TDialog::SetupWindow();
 bMaterialUsage      = FALSE;
 bMaterialPrice      = FALSE;
 bLabourUsage        = FALSE;
 bLabourPrice        = FALSE;
 bOverheadUsage      = FALSE;
 bOverheadPrice      = FALSE;
 bSearch             = TRUE;
 Items   = new Tscitemm(&cb);
 SetDlgItemText(HWindow,ID_ITEMSTART,(LPCSTR)"111010");
 SetDlgItemText(HWindow,ID_ITEMEND,(LPCSTR)"700820");
}

void TRptDefn::Search (RTMessage)
{
 if(bSearch)
  {
   ITEM = new TSCItemList2(this,"PICKLIST","FPTYPE = 'M '",Items);
   ITEM->EnableAutoCreate();
   GetApplication()->MakeWindow(ITEM);
   bSearch = FALSE;
 }
 if(!IsWindowVisible(ITEM->HWindow))
   ITEM->Show(SW_SHOW);
 else
   ITEM->Show(SW_HIDE);
}

void TRptDefn::MaterialUsage(RTMessage )
 {
  if(bMaterialUsage){
    bMaterialUsage = FALSE;
    SendDlgItemMsg(ID_MATERIAL_U,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bMaterialUsage = TRUE;
   SendDlgItemMsg(ID_MATERIAL_U,BM_SETCHECK,TRUE,NULL);
   }
 }

void TRptDefn::MaterialPrice(RTMessage )
 {
  if(bMaterialPrice){
    bMaterialPrice = FALSE;
    SendDlgItemMsg(ID_MATERIAL_P,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bMaterialPrice = TRUE;
   SendDlgItemMsg(ID_MATERIAL_P,BM_SETCHECK,TRUE,NULL);
   }
 }

void TRptDefn::LabourUsage  (RTMessage )
 {
  if(bLabourUsage){
    bLabourUsage = FALSE;
    SendDlgItemMsg(ID_LABOUR_U,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bLabourUsage = TRUE;
   SendDlgItemMsg(ID_LABOUR_U,BM_SETCHECK,TRUE,NULL);
   }
 }

void TRptDefn::LabourPrice  (RTMessage )
 {
  if(bLabourPrice){
    bLabourPrice = FALSE;
    SendDlgItemMsg(ID_LABOUR_P,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bLabourPrice = TRUE;
   SendDlgItemMsg(ID_LABOUR_P,BM_SETCHECK,TRUE,NULL);
   }
 }

void TRptDefn::OverheadUsage(RTMessage )
 {
  if(bOverheadUsage){
    bOverheadUsage = FALSE;
    SendDlgItemMsg(ID_OVERHEAD_U,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bOverheadUsage = TRUE;
   SendDlgItemMsg(ID_OVERHEAD_U,BM_SETCHECK,TRUE,NULL);
   }
 }

void TRptDefn::OverheadPrice(RTMessage )
 {
  if(bOverheadPrice){
    bOverheadPrice = FALSE;
    SendDlgItemMsg(ID_OVERHEAD_P,BM_SETCHECK,FALSE,NULL);
   }
  else {
   bOverheadPrice = TRUE;
   SendDlgItemMsg(ID_OVERHEAD_P,BM_SETCHECK,TRUE,NULL);
   }
 }


void TGlWindow::SCSRptAllocate    (RTMessage)
 {GetApplication()->ExecDialog(new TRptDefn(this,"RPTDEFN"));}

 void TRptDefn::Ok (RTMessage)
 {
  
  if(bMaterialUsage){
     PrtReport("sc_matus.rpt","Material Usage Variance");
     }
  if(bMaterialPrice)
     PrtReport("sc_matpr.rpt","Material Price Variance");

  if(bLabourUsage){
     PrtReport("sc_labus.rpt","Labour Usage Variance");
     }
  if(bLabourPrice)
     PrtReport("sc_labpr.rpt","Labour Price Variance");

  if(bOverheadUsage){
     PrtReport("sc_ovrus.rpt","Overhead Usage Variance");
     }
  if(bOverheadPrice)
     PrtReport("sc_ovrpr.rpt","Overhead Price Variance");

 }

 void TRptDefn::PrtReport(char *szReportName,char *szReportTitle)
 {
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  char StartItem[11];
  char EndItem[11];
  char *Start = "'111030'";
  char *End   = "'700820'";

  GetDlgItemText(HWindow,ID_ITEMSTART,StartItem,sizeof(StartItem));
  if(*StartItem == 0){
   BWCCMessageBox(HWindow,"Enter a starting resource code.","Resource Code",MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return;
  }   
  GetDlgItemText(HWindow,ID_ITEMEND,EndItem,sizeof(EndItem));
  if(*EndItem == 0){
   BWCCMessageBox(HWindow,"Enter a ending resource code.","Resource Code",MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return;
  }   

  strcpy(Start,"'"); strcat(Start,StartItem); strcat(Start,"'");
  strcpy(End,"'");  strcat(End,EndItem);  strcat(End,"'");  
  short JobNumber = PEOpenPrintJob(szReportName);

  if (JobNumber == 0){
   char szMsg[100];
   strcpy(szMsg,"Print job failed.\n\nMake sure the report : ");
   strcat(szMsg,szReportName);
   strcat(szMsg," is available in C:\\GL\\DBF.");
   BWCCMessageBox(HWindow,szMsg,"Print Failed",MB_OK|MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return; 
   }

  if(!PESetFormula (JobNumber,"Starting Item",Start))
   {
    BWCCMessageBox(HWindow,"Error setting first item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  if(!PESetFormula (JobNumber,"Ending Item",End))
   {
    BWCCMessageBox(HWindow,"Error setting last item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }


  PEOutputToWindow(JobNumber,
  		   szReportTitle,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   NULL,
		   GetApplication()->MainWindow->HWindow);
  

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
  EnableWindow(HWindow,TRUE);
 }

void TGlWindow::SCSPrintTypesRpt     (RTMessage )
{
  PrtReport("sc_types.rpt","Standard Cost System - Cost Types");
  return;
}


void TGlWindow::SCSPrintCostCentre   (RTMessage )
{
 PrtReport("sc_centr.rpt","Standard Cost System - Cost Centres");
 return;
}

void TGlWindow::SCSPrintAllocation   (RTMessage )
{
 PrtReport("sc_alloc.rpt","Standard Cost System - GL Cost Allocations");
 return;
}

void TGlWindow::SCSPrintElements     (RTMessage )
{
PrtReport("sc_elemt.rpt","Standard Cost System - Cost Element Types");
return;
}

void TGlWindow::SCSPrintLabourCosts  (RTMessage )
{
 PrtReport("sc_labor.rpt","Standard Cost System - Labour Costs");
 return;
}
void TGlWindow::SCSRptResource    (RTMessage)
 {
  /*
  MessageBox(HWindow,"","",MB_OK);
  GetApplication()->ExecDialog(new TRptSource(this,"RPTSOURCE"));
  */
 PrtReport("sc_resrc.rpt","Standard Cost System - Resource Listing");
 return;
 }

void TGlWindow::SCSPrintGLAllocate   (RTMessage)
{
 PrtReport("sc_glall.rpt","Standard Cost System - Production by Cost Centre");
 return;
}

void TGlWindow::SCSPrintGLCost    (RTMessage)
{
 PrtReport("sc_glcst.rpt","Standard Cost System - General Ledger Costs");
 return;
}

void TGlWindow::SCSPrintGL (RTMessage)
{
 PrtReport("sc_tjiss.rpt","Standard Cost System - Working Hours");
 return;
}


void TGlWindow::PrtReport(char *szReportName,char *szReportTitle)
 {
  short JobNumber = PEOpenPrintJob(szReportName);
  //HANDLE textHandle;
  //short  textLength;
  //char  *szCompany;
  //PEGetFormula(JobNumber,"Company Name",&textHandle,&textLength);
  //szCompany = new char[textLength+3];
  //PEGetHandleString(textHandle,szCompany,textLength);
  //StrTrim(szCompany);
  //strncpy(szCompany,"'",sizeof(szCompany));
  //strncat(szCompany,szCompanyName,sizeof(szCompany));
  //strcpy(szCompany,"'");
  //strcat(szCompany,szCompanyName);
  //strcat(szCompany,"'");
  //MessageBox(HWindow,szCompany,szCompanyName,MB_OK);
  //PESetFormula(JobNumber,"Company Name",szCompany);
  //delete szCompany;
  if (JobNumber == 0){
   char *szMsg = new char[100];
   strcpy(szMsg,"Print job failed.\n\nMake sure the report : ");
   strcat(szMsg,szReportName);
   strcat(szMsg,"\n is available in C:\\GL\\DBF.");
   BWCCMessageBox(HWindow,szMsg,"Print Failed",MB_OK|MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   delete szMsg;
   return; 
   }

  PEOutputToWindow(JobNumber,
  		   szReportTitle,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   CW_USEDEFAULT,
		   NULL,
		   GetApplication()->MainWindow->HWindow);
  

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  
 }
