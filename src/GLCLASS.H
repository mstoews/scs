#ifndef __GLCLASS_H
#define __GLCLASS_H
#include "tbowl.h"

#include "gl.h"
#include "mdi.h"
#include "array.h"
#include "db_profi.h"
#include "db_dist.h"
#include "db_cust.h"
#include "db_dept.h"
#include "db_btch.h"
#include "db_accts.h"
#include "db_btchd.h"
#include "db_srce.h"
#include "db_itemm.h"

_CLASSDEF (TWorksheet)
class TWorksheet;

_CLASSDEF (TAccess)
class TAccess;

_CLASSDEF(TGLClient)
class TGLClient : public TMDIClient
{
 public:
  TGLClient(PTMDIFrame AParent,PTModule AModule = NULL):TMDIClient(AParent){};
  virtual void WMRButtonDown(RTMessage  Msg) = [WM_FIRST + WM_RBUTTONDOWN];
};


_CLASSDEF (TStatBar)
class TStatBar : public TDialog
{
 public:
  TStatBar(PTWindowsObject AParent,LPSTR AName): TDialog(AParent,AName){};
  virtual void WMPaint(TMessage& Msg)       = [WM_FIRST + WM_PAINT];
  virtual void WMInitDialog (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
};


_CLASSDEF (TToolBar)
class TToolBar : public TDialog
{
  public:
  TToolBar(PTWindowsObject AParent,LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void WMInitDialog (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
  virtual void WMPaint      (RTMessage Msg) = [WM_FIRST + WM_PAINT];
  virtual void Resource     (RTMessage Msg) = [ID_FIRST + ID_RESOURCE];
  virtual void Elements     (RTMessage Msg) = [ID_FIRST + ID_ELEMENTS];
  virtual void Centres      (RTMessage Msg) = [ID_FIRST + ID_CENTRES]; 
  virtual void Allocation   (RTMessage Msg) = [ID_FIRST + ID_ALLOCATION];
  virtual void Types        (RTMessage Msg) = [ID_FIRST + ID_TYPES];
  virtual void ResourceUpdate (RTMessage Msg) = [ID_FIRST + IDM_RESOURCEUPDATE];
  virtual void ViewCosts      (RTMessage Msg) = [ID_FIRST + IDM_COSTMASTER];
  virtual void Quit           (RTMessage Msg) = [ID_FIRST + ID_EXIT];

};


_CLASSDEF(TListArray)
class TListArray : public Array
{
public:
  TListArray(int upper, int lower = 0, sizeType aDelta = 0)
    : Array(upper, lower, aDelta){};
  virtual classType isA() const { return __firstUserClass + 1; }
  virtual Pchar nameOf() const { return "TListArray"; }
};


_CLASSDEF(TListItem)
class TListItem : public Object
{
 public:
  int index,rec;
  TListItem(int Aindex,int Arec){index = Aindex, rec = Arec;}
  virtual classType isA() const { return __firstUserClass; }
  virtual Pchar nameOf() const { return "TListItem"; }
  virtual hashValueType hashValue() const { return 0; }
  virtual int isEqual(RCObject AListItem) const
    { return index == ((RTListItem)AListItem).index && rec == ((RTListItem)AListItem).rec; }
  virtual void printOn(Rostream outputStream) const
    { outputStream << "(" << index << "," << rec << ")"; }

};



_CLASSDEF(TGLAcctList)
class TGLAcctList : public TDialog
{
 PTSpread List;
 public:
 PTListArray ListArray;
 PTglaccts  gl;
 char szFilterString[85];
 Field4 *account;
 Field4 *dept;
 Field4 *descript;
 TGLAcctList(PTWindowsObject AParent, LPSTR AName,PTglaccts glacct):
  TDialog (AParent, AName) {
  gl=glacct;
  ListArray = new TListArray(1000, 0, 50);
  List = new TSpread(this,ID_DISTLIST);
  strcpy(szFilterString,"ACCOUNT <> ' '");
  };

 TGLAcctList(PTWindowsObject AParent, LPSTR AName,PTglaccts glacct,char *szFilter):
  TDialog (AParent, AName)
  {
   gl=glacct;
   ListArray = new TListArray(1000, 0, 50);
   List = new TSpread(this,ID_DISTLIST);
   strcpy(szFilterString,szFilter);
 };

 ~TGLAcctList();
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};


_CLASSDEF(TSCItemList)
class TSCItemList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTscitemm  scitemm;
 TSCItemList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTscitemm item):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  scitemm=item; 
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};

_CLASSDEF(TSCItemList2)
class TSCItemList2 : public TSCItemList
{
 public:
 char szQuery[255];
 PTSpread List;
 PTscitemm  scitemm;
 TSCItemList2(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTscitemm item):
  TSCItemList (AParent, AName,AQuery,item)
  {
  strcpy(szQuery,AQuery);
  scitemm=item; 
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];

};


BOOL FAR PASCAL DistListPrintProc(HDC hDC,short dCommand,short nPageNum,long lAppData);
#endif __GLCLASS_H
