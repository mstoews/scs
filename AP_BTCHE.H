

// Accounts Payable Entry header file.

_CLASSDEF(Tapbtch)
class Tapbtch : public Data4
{
 public:
   Field4 *batchno;
   Field4 *mode;
   Field4 *period;
   Field4 *vendorno;
   Field4 *invoiceno;
   Field4 *orderno;
   Field4 *duedate;
   Field4 *invdate;
   Field4 *date;
   Field4 *reference;
   Field4 *cheque;
   Field4 *descript;
   Field4 *payable;
   Field4 *entryno;


  Tapbtch(Code4 *cb,LPSTR name=0) : Data4 (cb,"ap_btch")
  {
   tag_select(tag("batchno"));
   batchno   = (Field4 *) new Field4(*(Data4 *)this,"batchno");
   vendorno  = (Field4 *) new Field4(*(Data4 *)this,"vendorno");
   invoiceno = (Field4 *) new Field4(*(Data4 *)this,"invoiceno");
   orderno   = (Field4 *) new Field4(*(Data4 *)this,"orderno");
   duedate   = (Field4 *) new Field4(*(Data4 *)this,"duedate");
   invdate   = (Field4 *) new Field4(*(Data4 *)this,"invdate");
   mode      = (Field4 *) new Field4(*(Data4 *)this,"mode");
   period    = (Field4 *) new Field4(*(Data4 *)this,"period");
   date      = (Field4 *) new Field4(*(Data4 *)this,"date");
   reference = (Field4 *) new Field4(*(Data4 *)this,"reference");
   descript  = (Field4 *) new Field4(*(Data4 *)this,"descript");
   cheque    = (Field4 *) new Field4(*(Data4 *)this,"cheque");
   payable   = (Field4 *) new Field4(*(Data4 *)this,"payable");
   entryno   = (Field4 *) new Field4(*(Data4 *)this,"entryno");
  };
};



_CLASSDEF(Tapbtchd)
class Tapbtchd :public Data4
{
 public:
  int iCurrentRecord;

  Field4 *batchno;
  Field4 *entryno;
  Field4 *descript;
  Field4 *account;
  Field4 *dept;
  Field4 *debit;
  Field4 *credit;

  Tapbtchd(Code4 *cb,LPSTR name=0):Data4 (cb,"ap_btchd")
  {
  tag_select(tag("batchno"));
  batchno     = (Field4 *) new Field4(*(Data4 *)this,"batchno");
  entryno     = (Field4 *) new Field4(*(Data4 *)this,"entryno");
  descript    = (Field4 *) new Field4(*(Data4 *)this,"descript");
  account     = (Field4 *) new Field4(*(Data4 *)this,"account");
  dept        = (Field4 *) new Field4(*(Data4 *)this,"dept");
  debit       = (Field4 *) new Field4(*(Data4 *)this,"debit");
  credit      = (Field4 *) new Field4(*(Data4 *)this,"credit");
  iCurrentRecord = 0;
  };
 ~Tapbtchd(){close();};
};

_CLASSDEF (TAPDistList)
class TAPDistList : public TDialog
{
 public:
  SS_CELLTYPE 	CellType;
  LOGFONT 	lfFont;
  HFONT   	hFont;
  HWND SHWindow;
  PTapbtchd apbtd;
  PTListArray ListArray;
  TAPDistList(PTWindowsObject AParent, LPSTR ATitle,PTapbtchd bdp):TDialog(AParent,ATitle)
  {
  ListArray = new TListArray(1000, 0, 50);
  apbtd = bdp;
  };
  virtual void SetupWindow();
  virtual void SelectCell     (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  ~TAPDistList(){delete ListArray;};
};


_CLASSDEF(TAPBatchList)
class TAPBatchList : public TDialog
{
 public:
 PTListArray ListArray;
 PTapbtch  apbtch;
 TAPBatchList(PTWindowsObject AParent, LPSTR AName,PTapbtch glb):
  TDialog (AParent, AName) {apbtch=glb; ListArray = new TListArray(1000, 0, 50);};
 ~TAPBatchList();
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void Cancel        (RTMessage Msg) = [ID_FIRST + IDCANCEL] {ShutDownWindow();};
 virtual void SetupWindow();

};

_CLASSDEF(TAPInvoiceEntry)
class TAPInvoiceEntry : public TDialog
{
 private:
  char  szVendorNo[7];
  char  szInvoiceNo[10],szOrderNo[10],szBuffer[85];
  char  szReference[20],szAccounts[8],szDepart[8],szDescript[41];
  char  szDueDate[10],szInvoiceDate[10];
  double lDebit,DebitTotal,CreditTotal,lCredit,lInvoiceAmount;
  int iBatchno,iEntryno,iPeriodNo;
  long iLineno;

 public:
  PTGLAcctList AcctList;
  PTglaccts  glacct;  
  PTAddTool T;
  PTAPVendorList VendorList;
  PTAPBatchList  SlipList;

  PTapbtch   apbtch;
  PTapbtchd  apbtchd;
  PTgldept   gldept;
  PTapvendor apvendor;

  TAPInvoiceEntry(PTWindowsObject AParent,LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual LPSTR GetClassName(){ return "APInvoiceEntry"; }
  virtual void WMInitDialog    (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
  virtual void WMRButtonDown   (RTMessage Msg) = [WM_FIRST + WM_RBUTTONDOWN];
  virtual void HandleDepart    (RTMessage Msg) = [ID_FIRST + ID_DEPART];
  virtual void Cancel          (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Ok              (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void ToolCancel      (RTMessage Msg) = [WM_FIRST + WM_DATACANCEL];
  virtual void ToolOk          (RTMessage Msg) = [ID_FIRST + ID_ADD];
  virtual void ToolForward     (RTMessage Msg) = [WM_FIRST + WM_DATAFORWARD];
  virtual void ToolBack        (RTMessage Msg) = [WM_FIRST + WM_DATABACK];
  virtual void ToolView        (RTMessage Msg) = [ID_FIRST + ID_VIEWBATCH];   //Sliplist(); Slip list
  virtual void ToolNew         (RTMessage Msg) = [ID_FIRST + ID_NEW];         //New()
  virtual void ToolVendor      (RTMessage Msg) = [ID_FIRST + ID_VENDOR];      //Vendorlist();
  virtual void SearchGL        (RTMessage Msg) = [ID_FIRST + ID_VIEW];        //Accountlist();  
  virtual void CurrentBatch     (RTMessage Msg) = [WM_FIRST + WM_CURRENTBATCH];
  void Forward ();
  void Back();  
  void Sliplist();  
  void Accountlist();
  void Vendorlist();
  void New();    
  void InvoiceData();
  int  CheckGLAccount(char *);
  int  CheckGLDept(char *);
  void CreateNewApSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  void ModifyCurrentSlip(PTapbtch apbtch,PTapbtchd apbtchd,PTAPDistList D);
  PTAPDistList D;
};

