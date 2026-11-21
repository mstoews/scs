#include "glb.h"
#include "gl_pick.h"
#include "db_vend.h"
#include "db_apdst.h"
#include "db_apset.h"
#include "gl_dlg.h"
#include "db_dist.h"
#include "gl_acct.h"



extern Code4 cb;


_CLASSDEF(TAPVendorList)
class TAPVendorList : public TPickList
{
 public:
 PTListArray ListArray;
 PTapvendor  apvendor;
 Field4 *vendorno;
 Field4 *vdname;
 TAPVendorList(PTWindowsObject AParent, LPSTR AName,PTapvendor vd):
  TPickList (AParent, AName) {apvendor=vd; ListArray = new TListArray(1000, 0, 50);};
 ~TAPVendorList();
 virtual void SetupWindow();
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
};

_CLASSDEF(TAPBatchList)
class TAPBatchList : public TPickList
{
 public:
 PTListArray ListArray;
 PTapbtch  apbtch;
 PTapcash  apcash;
 TAPBatchList(PTWindowsObject AParent, LPSTR AName,PTapbtch glb):
  TPickList (AParent, AName) {apbtch=glb; ListArray = new TListArray(1000, 0, 50);};
 TAPBatchList(PTWindowsObject AParent, LPSTR AName,PTapcash glb):
  TPickList (AParent, AName) {apcash=glb; ListArray = new TListArray(1000, 0, 50);};
 ~TAPBatchList();
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void Cancel        (RTMessage Msg) = [ID_FIRST + IDCANCEL] {ShutDownWindow();};
 virtual void SetupWindow();
};



_CLASSDEF (TAPDistList)
class TAPDistList : public TDialog
{
 public:
  SS_CELLTYPE 	CellType;
  HWND SHWindow;
  PTapbtchd apbtd;
  PTapcashd apcashd;
  TAPDistList(PTWindowsObject AParent, LPSTR ATitle,PTapbtchd bdp):TDialog(AParent,ATitle)
  {  
  apbtd = bdp;
  };
  TAPDistList(PTWindowsObject AParent, LPSTR ATitle,PTapcashd bdp):TDialog(AParent,ATitle)
  {  
  apcashd = bdp;
  };

  virtual void SetupWindow();
  virtual void SelectCell     (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  virtual void Print          (RTMessage Msg) = [ID_FIRST + ID_PRINT];
  virtual void SetListSize(int Rows);
  virtual void SetField(int ID,Field4 *field);
  virtual void SetField(int ID,char  *field);
  virtual void SetField(int ID,long   field);
  virtual void SetField(int ID,double field);

};






_CLASSDEF(TAPInvoiceEntry)
class TAPInvoiceEntry : public TMainDialog
{
 public:
  char  *szVendorNo;
  char  *szAdj;
  char  *szInvoiceNo;
  char  *szOrderNo;
  char  *szBuffer;
  char  *szReference,*szAccounts,*szDepart,*szDescript;
  char  *szDueDate,*szInvoiceDate;
  char  *szCompanyTaxAccount,*szCompanyTaxDept;
  char  *szIndTaxAccount,*szIndTaxDept;
  char  *szVATAccount,*szVATDept;
  char  *szVendorName;
  double lDebit,DebitTotal,CreditTotal,lCredit,lInvoiceAmount,dTerms,dVatPercentage,dRebatePercentage;
  int iBatchno,iEntryno,iPeriodNo;
  long iLineno;

  PTGLAcctList AcctList;
  PTglaccts  glacct;
  PTAPVendorList VendorList;
  PTAPBatchList  SlipList;

  PTapbtch   apbtch;
  PTapbtchd  apbtchd;
  PTgldept   gldept;
  PTapvendor apvendor;
  PTapsetup  apsetup;
  BOOL bSlipList,bAutomaticCalc,bVendorList;
  Date4 InvoiceDate;
  Date4 DueDate;

  TAPInvoiceEntry(PTWindowsObject AParent,LPSTR ATitle):TMainDialog(AParent,ATitle)
  {
   bSlipList = FALSE;
   bVendorList = TRUE;
  };
  virtual LPSTR GetClassName(){ return "APInvoiceEntry"; }

  virtual void HandleDepart    (RTMessage Msg) = [ID_FIRST + ID_DEPART];
  virtual void VList	       (RTMessage Msg) = [ID_FIRST + ID_SEARCHCUST]{Vendorlist();};
  virtual void SList           (RTMessage Msg) = [ID_FIRST + ID_SLIPLIST]{Sliplist();};
  virtual void Cancel          (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Ok              (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void SearchGL        (RTMessage Msg) = [ID_FIRST + ID_VIEW];        //Accountlist();
  virtual void CurrentBatch    (RTMessage Msg) = [CM_FIRST + CM_CURRENTBATCH];
  virtual void New             (RTMessage Msg) = [ID_FIRST + ID_NEW];
  virtual void SetVendor       (RTMessage Msg) = [CM_FIRST + CM_SETVENDOR];
  virtual void Top             (RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom          (RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Next            (RTMessage Msg) = [ID_FIRST + ID_NEXT];
  virtual void Previous        (RTMessage Msg) = [ID_FIRST + ID_PREVIOUS];
  virtual void Delete          (RTMessage Msg) = [ID_FIRST + ID_DELETE];
  virtual void Automatic       (RTMessage Msg) = [ID_FIRST + ID_AUTO];
  virtual void SetLineItem     (RTMessage Msg) = [CM_FIRST + CM_LINEITEM];
  virtual void AddVendor       (RTMessage Msg) = [ID_FIRST + ID_APADDVENDORS];
  

  virtual void SetupWindow();
  void Sliplist();
  void Accountlist();
  void Vendorlist();
  void InvoiceData();  
  int  CheckGLAccount(char *);
  int  CheckGLDept(char *);
  void CreateNewApSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateVatSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateFiveSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateThreeSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void ModifyCurrentSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateRebateSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateVat_RebateSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void CreateLineItem(PTapbtch apbtch,PTapbtchd apbtchd);
  char *GetControlAcct(char *Account,char *Depart);
  PTAPDistList D;
};


_CLASSDEF(TAPInvoiceList)
class TAPInvoiceList : public TPickList
{
 public: 
 PTapbtch  apbtch;
 TAPInvoiceList(PTWindowsObject AParent, LPSTR AName,PTapbtch glb):
  TPickList (AParent, AName)
  {
   apbtch=glb;
  }; 
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void Cancel        (RTMessage Msg) = [ID_FIRST + IDCANCEL] {ShutDownWindow();};
 virtual void Redraw        (RTMessage Msg) = [CM_FIRST + CM_SETDATA];
 virtual void SetupWindow();
};



