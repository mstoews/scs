#ifndef __GL_BTCHE_H
#define __GL_BTCHE_H

#ifndef __GL_G_H
#include "glb.h"
#endif

#include "ap_vend.h"

#define WM_CURRENTBATCH  WM_USER + 33

_CLASSDEF (TGLModifyBatch)
class TGLModifyBatch : public TDialog
{
 public:
  PTglbtchd glbtchd;
  TGLModifyBatch(PTWindowsObject AParent, LPSTR ATitle,PTglbtchd bdp):TDialog(AParent,ATitle)
  {
   glbtchd = bdp;
  };
  virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
};

extern Code4 cb;
// Database Declaration classes.

_CLASSDEF(Tarbtch)
class Tarbtch : public Data4
{
 public:
  Field4 *batchno;
  Field4 *mode;
  Field4 *descript;
  Field4 *invoiceno;
  Field4 *custno;
  Field4 *period;
  Field4 *date;
  Field4 *debit;
  Field4 *credit;
  Field4 *entryno;

  Tarbtch(Code4 *cb,LPSTR name=0) : Data4 (cb,"ar_btch")
  {
  batchno    = (Field4 *) new Field4(*(Data4 *) this ,"batchno");
  mode       = (Field4 *) new Field4(*(Data4 *) this ,"mode");
  descript   = (Field4 *) new Field4(*(Data4 *) this ,"descript");
  invoiceno  = (Field4 *) new Field4(*(Data4 *) this ,"invoiceno");
  custno     = (Field4 *) new Field4(*(Data4 *) this ,"custno");
  period     = (Field4 *) new Field4(*(Data4 *) this ,"period");
  date       = (Field4 *) new Field4(*(Data4 *) this ,"invdate");
  credit     = (Field4 *) new Field4(*(Data4 *) this ,"credit");
  debit      = (Field4 *) new Field4(*(Data4 *) this ,"debit");
  entryno    = (Field4 *) new Field4(*(Data4 *) this ,"entryno"); 
  };
 ~Tarbtch(){close();};
};



_CLASSDEF(Tarbtchd)
class Tarbtchd :public Data4
{
 public:

  Field4 *batchno;
  Field4 *custno;
  Field4 *billdate;
  Field4 *duedate;
  Field4 *invoiceno;
  Field4 *reference;
  Field4 *descript;
  Field4 *account;
  Field4 *dept;
  Field4 *debit;
  Field4 *credit;

  Tarbtchd(Code4 *cb,LPSTR name=0):Data4 (cb,"ar_btchd")
  {
  batchno     = (Field4 *) new Field4(*(Data4 *)this,"batchno");
  custno      = (Field4 *) new Field4(*(Data4 *)this,"custno");
  billdate    = (Field4 *) new Field4(*(Data4 *)this,"billdate");
  invoiceno   = (Field4 *) new Field4(*(Data4 *)this,"invoiceno");
  duedate     = (Field4 *) new Field4(*(Data4 *)this,"duedate");
  reference   = (Field4 *) new Field4(*(Data4 *)this,"reference");
  descript    = (Field4 *) new Field4(*(Data4 *)this,"descript");
  account     = (Field4 *) new Field4(*(Data4 *)this,"account");
  dept        = (Field4 *) new Field4(*(Data4 *)this,"dept");
  debit       = (Field4 *) new Field4(*(Data4 *)this,"debit");
  credit      = (Field4 *) new Field4(*(Data4 *)this,"credit");
  };
 ~Tarbtchd(){close();};
};



_CLASSDEF (TGLDistList)
class TGLDistList : public TDialog
{
 public:
  SS_CELLTYPE 	CellType;
  LOGFONT 	lfFont;
  HFONT   	hFont;
  HWND SHWindow;
  PTglbtchd glbtd;
  PTListArray ListArray;
  TGLDistList(PTWindowsObject AParent, LPSTR ATitle,PTglbtchd bdp):TDialog(AParent,ATitle)
  {
  ListArray = new TListArray(1000, 0, 50);
  glbtd = bdp;
  };
  virtual void SelectCell     (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  virtual void Print          (RTMessage Msg) = [ID_FIRST + ID_PRINT];
  virtual void SetupWindow();
  ~TGLDistList(){delete ListArray;};
};

_CLASSDEF(TAddTool)
class TAddTool : public TDialog
{
 public:
 TAddTool(PTWindowsObject AParent,LPSTR ATitle):TDialog(AParent,ATitle){};
 virtual void WMInitDialog     (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void HandleForwardBtn (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
 virtual void HandleBackBtn    (RTMessage Msg) = [ID_FIRST + ID_BACK];
 virtual void HandleNewBtn     (RTMessage Msg) = [ID_FIRST + ID_NEW];
 virtual void HandleSearchBtn  (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
 virtual void HandleViewBtn    (RTMessage Msg) = [ID_FIRST + ID_VIEW];
 virtual void HandleVendorBtn  (RTMessage Msg) = [ID_FIRST + ID_VENDOR];
 virtual void Ok               (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel           (RTMessage Msg) = [ID_FIRST + IDCANCEL];
};

_CLASSDEF (TARDistList)
class TARDistList : public TDialog
{
 public:
  SS_CELLTYPE 	CellType;
  LOGFONT 	lfFont;
  HFONT   	hFont;
  HWND SHWindow;
  PTarbtchd arbtd;
  PTListArray ListArray;
  TARDistList(PTWindowsObject AParent, LPSTR ATitle,PTarbtchd bdp):TDialog(AParent,ATitle)
  {
  ListArray = new TListArray(1000, 0, 50);
  arbtd = bdp;
  };
  virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
  virtual void SelectCell     (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  ~TARDistList(){delete ListArray;};
};


_CLASSDEF(TARBillingEntry)
class TARBillingEntry : public TDialog
{
 public:
  PTglaccts  glacct;  
  PTarbtch   arbtch;
  PTarbtchd  arbtchd;
  PTgldept   gldept;
  double DebitTotal;
  double CreditTotal;
  TARBillingEntry(PTWindowsObject AParent,LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual LPSTR GetClassName(){ return "ARBillingEntry"; }
  virtual void WMInitDialog    (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
  virtual void WMRButtonDown   (RTMessage Msg) = [WM_FIRST + WM_RBUTTONDOWN];
  virtual void HideDist        (RTMessage Msg) = [ID_FIRST + ID_HIDEDIST];
  virtual void ViewDist        (RTMessage Msg) = [ID_FIRST + ID_VIEWDIST];
  virtual void HandleDepart    (RTMessage Msg) = [ID_FIRST + ID_DEPART];
  virtual void Cancel          (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Ok              (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void SearchAR        (RTMessage Msg) = [WM_FIRST + WM_SEARCHAR];
  int CheckGLAccount(char *);
  int CheckGLDept(char *);
  PTARDistList D;
};



#endif