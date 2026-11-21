/*** Printing Master file - SCS Standard Include File***/
#include "scs.h"
// Printer allocation
#include "crpe.h"
// Database files
#include "db_elem.h"
#include "db_types.h"
#include "db_coste.h"

extern Code4 cb;

/*** Reporting: Cost ***/
_CLASSDEF(TRptCost)
class TRptCost : public TDialog
{
 public:
  TRptCost (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
  virtual void Ok (RTMessage Msg ) = [ID_FIRST + IDOK];
};

_CLASSDEF(TRptDialog)
class TRptDialog : public TDialog
{
 public:
 BOOL bDetail; 
 char szFileName[20];
 TRptDialog(PTWindowsObject AParent, LPSTR AName, LPSTR AReport):TDialog(AParent,AName){strcpy(szFileName,AReport);};
 virtual void SetupWindow(){TDialog::SetupWindow();};
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL]{ShutDownWindow();cb.closeAll();};
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
 void PrintReport(char *szFileName);
};

void TRptDialog::Ok(RTMessage)
{
  PrintReport(szFileName);
}

#pragma argsused
void TRptDialog::PrintReport (char *szFilename)
{
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  char StartItem[10];
  char EndItem[10];
  char *Start = "'100101'";
  char *End   = "'100103'";

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

  short JobNumber = PEOpenPrintJob(szFileName);

  if (JobNumber == 0){
   BWCCMessageBox(HWindow,"Print job failed","",MB_OK);
   EnableWindow(HWindow,TRUE);
   return; 
   }
  
 
  if(!PESetFormula (JobNumber,"First",Start))
   {
    BWCCMessageBox(HWindow,"Error setting first item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  if(!PESetFormula (JobNumber,"Last",End))
   {
    BWCCMessageBox(HWindow,"Error setting last item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  PEOutputToWindow(JobNumber,"Cost Analysis Reporting",CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL
  ,GetApplication()->MainWindow->HWindow);

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
  EnableWindow(HWindow,TRUE);
  ShutDownWindow();

}

/*** Reporting: Elements ***/
_CLASSDEF(TRptElements)
class TRptElements : public TRptDialog
{
 public:
  PTElement Element;
  PTSCElementList ELEMENT;
  TRptElements (PTWindowsObject AParent, LPSTR ATitle,LPSTR AReport):TRptDialog(AParent,ATitle,AReport){};
  virtual void SetupWindow();
  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Search (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
};

void TRptElements::SetupWindow()
 {
  TDialog::SetupWindow();
  Element = new TElement(&cb);
  
  ELEMENT = new TSCElementList(this,"PICKLIST","FCODE <> ' '",Element);
  ELEMENT->EnableAutoCreate();
  GetApplication()->MakeWindow(ELEMENT);
  SetCaption("Print Resource Elements List");
 }

void TRptElements::Search(RTMessage)
{
if(!IsWindowVisible(ELEMENT->HWindow))
   ELEMENT->Show(SW_SHOW);
  else
   ELEMENT->Show(SW_HIDE);
}


void TRptElements::Ok (RTMessage)
{
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  char StartItem[10];
  char EndItem[10];
  char *Start = "'500010'";
  char *End   = "'500010'";

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

  short JobNumber = PEOpenPrintJob(szFileName);

  if (JobNumber == 0){
   BWCCMessageBox(HWindow,"Unable to open print job.","E R R O R",MB_OK);
   EnableWindow(HWindow,TRUE);
   return; 
   }
  
 
  if(!PESetFormula (JobNumber,"First",Start))
   {
    BWCCMessageBox(HWindow,"Error setting first item.","E R R O R",MB_ICONEXCLAMATION);
    EnableWindow(HWindow,TRUE);
    PEClosePrintJob(JobNumber);
    return ;
   }

  if(!PESetFormula (JobNumber,"Last",End))
   {
    BWCCMessageBox(HWindow,"Error setting last item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  PEOutputToWindow(JobNumber,"Cost Analysis Reporting",CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL
  ,GetApplication()->MainWindow->HWindow);

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
  EnableWindow(HWindow,TRUE);
  ShutDownWindow();

}

_CLASSDEF (TRptResStruct)
class TRptResStruct : public TRptDialog
{
 public:
  PTscitemm Items;
  BOOL bSearch;
  PTSCItemList2  ITEM;
  TRptResStruct (PTWindowsObject AParent, LPSTR ATitle,LPSTR AReport):TRptDialog(AParent,ATitle,AReport){};
  virtual void SetupWindow();
  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Search (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
};


void TRptResStruct::SetupWindow()
 {
  TDialog::SetupWindow();
  Items = new Tscitemm(&cb);
  bSearch = TRUE;
 }

void TRptResStruct::Search(RTMessage)
{
 if(bSearch)
 {
   ITEM = new TSCItemList2(this,"PICKLIST","FPTYPE = 'M '",Items);
   ITEM->EnableAutoCreate();
   GetApplication()->MakeWindow(ITEM);
   bSearch = FALSE;

  ITEM->EnableAutoCreate();
  GetApplication()->MakeWindow(ITEM);
  SetCaption("Print Resource List");
  bSearch = FALSE;
 }
if(!IsWindowVisible(ITEM->HWindow))
   ITEM->Show(SW_SHOW);
  else
   ITEM->Show(SW_HIDE);
}


void TRptResStruct::Ok (RTMessage)
{
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  char StartItem[10];
  char EndItem[10];
  char *Start = "'111010'";
  char *End   = "'111011'";

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

  short JobNumber = PEOpenPrintJob(szFileName);
  if (JobNumber == 0){
   BWCCMessageBox(HWindow,"Print job failed.","",MB_OK);
   return; 
   }
  
 
  if(!PESetFormula (JobNumber,"Starting Item",Start))
   {
    BWCCMessageBox(HWindow,"Error setting first item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    return ;
   }

  if(!PESetFormula (JobNumber,"Ending Item",End))
   {
    BWCCMessageBox(HWindow,"Error setting last item.","E R R O R",MB_ICONEXCLAMATION);
    PEClosePrintJob(JobNumber);
    return ;
   }

  PEOutputToWindow(JobNumber,"Standard Cost Structure per Item",CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,NULL
  ,GetApplication()->MainWindow->HWindow);

  PEStartPrintJob(JobNumber,TRUE);
  PEClosePrintJob(JobNumber);
  HourGlassOff();
  EnableWindow(HWindow,TRUE);
  ShutDownWindow();

}



/*** Reporting: Variance ***/
_CLASSDEF(TRptVariance)
class TRptVariance : public TDialog
{
 public:
  TRptVariance (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
};



_CLASSDEF (TRptResSemi)
class TRptResSemi : public TRptDialog
{
 public:
  PTTypes  Types;
  PTCost   Coste;
  PTCost   CostType;
  BOOL bDetail;
  BOOL bSummary;
  char szReportTitle[50];
  TRptResSemi (PTWindowsObject AParent, LPSTR ATitle,LPSTR AReport):TRptDialog(AParent,ATitle,AReport)
  {
   strcpy(szReportTitle,"Standard Cost by Cost Type and Semi Code");
  };
  virtual void SetupWindow();
  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Detail (RTMessage Msg) = [ID_FIRST + ID_DETAIL];
  virtual void Summary (RTMessage Msg) = [ID_FIRST + ID_STDCOST];
  void Report(char *szFileName,char *szStartingCode,char *szEndingCode);
};

_CLASSDEF(TRptActualSemi)
class TRptActualSemi : public TRptResSemi
{
 public:
 TRptActualSemi (PTWindowsObject AParent, LPSTR ATitle,LPSTR AReport):TRptResSemi(AParent,ATitle,AReport)
 {
  strcpy(szReportTitle,"Actual Cost by Semi Code");
 };
 virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
};

void TRptActualSemi::Ok (RTMessage)
{
}



void TRptResSemi::SetupWindow()
 {
  TDialog::SetupWindow();
  SendDlgItemMsg(ID_ITEMSTART,WM_SETTEXT,NULL,(LPARAM)(LPSTR)"");
  Types = new TTypes(&cb);
  Types->top();
   while (!Types->eof())
   {
    SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)((LPSTR)Types->fCode->str()));
    Types->skip(1);
   }
   SendDlgItemMsg(ID_COSTTYPE,WM_SETTEXT,NULL,(LPARAM)((LPSTR)"STD"));
  bDetail = TRUE;
 }

void TRptResSemi::Detail(RTMessage)
{
  SendDlgItemMsg(ID_STDCOST,BM_SETCHECK,FALSE,0);
  SendDlgItemMsg(ID_DETAIL,BM_SETCHECK,TRUE,0);
  bDetail = TRUE;
  bSummary = FALSE;
}

void TRptResSemi::Summary(RTMessage)
{
  SendDlgItemMsg(ID_STDCOST,BM_SETCHECK,TRUE,0);
  SendDlgItemMsg(ID_DETAIL,BM_SETCHECK,FALSE,0);
  bDetail = FALSE;
  bSummary = TRUE;
}

void TRptResSemi::Ok(RTMessage)
{
 char szCostType[4];
 char szStartingCode[10];
 char szEndingCode[10];
 char szQuery[100];

 Types->SetIndex("fcode");
 SendMessage(GetDlgItem(HWindow,ID_COSTTYPE),WM_GETTEXT,sizeof(szCostType),(LPARAM)(LPSTR)szCostType);
 if(*szCostType == 0)
  {
  BWCCMessageBox(HWindow,"Choose a cost type.","Cost Structure Report",MB_OK);
  return;
  }
 if(Types->seek(szCostType) != 0)
  {
  BWCCMessageBox(HWindow,"Cost type not found. Check the Cost Type Master.","Cost Structure Report",MB_OK);
  return;
  }

  GetDlgItemText(HWindow,ID_ITEMSTART,szStartingCode,sizeof(szStartingCode));
  if(*szStartingCode == 0){
   BWCCMessageBox(HWindow,"Enter a semi code.","Semi Code",MB_ICONEXCLAMATION);
   EnableWindow(HWindow,TRUE);
   return;
  }   
  GetDlgItemText(HWindow,ID_ITEMEND,szEndingCode,sizeof(szEndingCode));
  if(*szEndingCode == 0){
    strcpy(szEndingCode,szStartingCode);
  }  
  if(strcmp(szCostType,"ACT") == 0)
  {
   Report("sc_act.rpt",szStartingCode,szEndingCode);
  }
  else
  if(strcmp(szCostType,"STD") == 0)
  {
   Report("sc_std.rpt",szStartingCode,szEndingCode);
  }
  else
  if(strcmp(szCostType,"BDG") == 0)
  {
   Report("sc_bdg.rpt",szStartingCode,szEndingCode);
  }
  else
  if(strcmp(szCostType,"HST") == 0)
  {
   Report("sc_hst.rpt",szStartingCode,szEndingCode);
  }
  return;

}

void TRptResSemi::Report(char *szFileName,char *szStartingCode,char *szEndingCode)
{
  HourGlassOn();
  EnableWindow(HWindow,FALSE);
  short JobNumber;
  char *szStart = "'001'";
  char *szEnd   = "'001'";

  strcpy(szStart,"'"); strcat(szStart,szStartingCode); strcat(szStart,"'");
  strcpy(szEnd,"'"); strcat(szEnd,szEndingCode); strcat(szEnd,"'");

  JobNumber = PEOpenPrintJob(szFileName);
  if (JobNumber == 0){
   EnableWindow(HWindow,TRUE);
   return; 
   }

  if(!PESetFormula (JobNumber,"Starting Item",szStart))
   {
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  if(!PESetFormula (JobNumber,"Ending Item",szEnd))
   {
    PEClosePrintJob(JobNumber);
    EnableWindow(HWindow,TRUE);
    return ;
   }

  PEOutputToWindow(JobNumber,szReportTitle,
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
  ShutDownWindow();
}

void TGlWindow::SCSRptVariance    (RTMessage)
 {GetApplication()->ExecDialog(new TRptVariance(this,"RPTVARIANCE"));}

void TGlWindow::SCSRptCost        (RTMessage){
  GetApplication()->ExecDialog(new TRptDialog(this,"RPTCOST","sc_stdct.rpt"));
}

void TGlWindow::SCSRptElements    (RTMessage){
  GetApplication()->ExecDialog(new TRptElements(this,"RPTCOST","sc_costf.rpt"));
}

void TGlWindow::SCSPrintResStruct    (RTMessage){
  GetApplication()->ExecDialog(new TRptActualSemi(this,"RES_ACTUAL","sc_cost1.rpt"));
return;
}

void TGlWindow::SCSPrintResSemi    (RTMessage){
  GetApplication()->ExecDialog(new TRptResSemi(this,"RES_SEMI","sc_cost1.rpt"));
return;
}


