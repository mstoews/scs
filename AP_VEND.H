#include "glclass.h"
#include "db_vend.h"

_CLASSDEF(TAPVendorList)
class TAPVendorList : public TDialog
{
 public:
 PTListArray ListArray;
 PTapvendor  apvendor;
 Field4 *vendorno;
 Field4 *vdname;
 TAPVendorList(PTWindowsObject AParent, LPSTR AName,PTapvendor vd):
  TDialog (AParent, AName) {apvendor=vd; ListArray = new TListArray(1000, 0, 50);};
 ~TAPVendorList();
 virtual void WMInitDialog  (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
};


_CLASSDEF (TAPVendorAdd)
class TAPVendorAdd : public TDialog
{
 public:
 PTAPVendorList VendorList; 
 PTapvendor apvendor;
 PTglaccts  glaccts;
 TAPVendorAdd(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL): TDialog (AParent,AName){};
 virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void WMRButtonDown  (RTMessage Msg) = [WM_FIRST + WM_RBUTTONDOWN];
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
};



_CLASSDEF (TAPVendorMod)
class TAPVendorMod : public TAPVendorAdd
{
 public:
 TAPVendorMod(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL):TAPVendorAdd(AParent,AName){};
 virtual void WMInitDialog  (RTMessage Msg)  = [WM_FIRST + WM_INITDIALOG];
};

_CLASSDEF (TAPVendorDel)
class TAPVendorDel : public TAPVendorAdd
{
 public:
 TAPVendorDel(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL):TAPVendorAdd(AParent,AName){};
 virtual void WMInitDialog  (RTMessage Msg)  = [WM_FIRST + WM_INITDIALOG];
 virtual void Ok            (RTMessage Msg)  = [ID_FIRST + IDOK];
};


