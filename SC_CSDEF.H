#ifndef __SC_DEF_H
#define __SC_DEF_H
_CLASSDEF(TSCSDefinition)
class TSCSDefinition : public TDialog
{
 public:
 PTResourceUpdate ResUpdate;
 PTSpread List;
 PTResrc Resrc;
  TSCSDefinition(PTWindowsObject AParent, LPSTR ATitle,PTResourceUpdate R):TDialog(AParent,ATitle)
  {
   ResUpdate = R;
   List = new TSpread(this,ID_DISTLIST);
  }
  virtual void SetupWindow();  
  virtual BOOL SetField   (int ID, Field4 *field);
  virtual BOOL SetField   (int ID, char *field);
  virtual char *GetField   (int ID);
  virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
  virtual BOOL CanClose (){return FALSE;};

};

_CLASSDEF(TResourceUpdate)
class TResourceUpdate : public TDialog
{
 private:
   BOOL bSearch;
   BOOL bResource;
   BOOL bMultiLevel;
   double dStandard;
   double dLotSize;
   double dParentKousei;
   int iLine;
   char *szMainWindowCaption;
   char *szCompositionCaption;
   char *szCostType;

   PTSCSDefinition SCSDef;
   PTSCItemList    ITEM;
   PTSCResList     SEARCH;
   PTscitemm       Items;
   PTResrc	   Resrc;
   PTtrstr         CostStr;
   PTTypes	   Types;	

 public:
  TResourceUpdate(PTWindowsObject AParent, LPSTR ATitle, PTSCSDefinition S):TDialog(AParent,ATitle)
  {
   SCSDef = S; 
  }
  virtual void SearchItemNo   (RTMessage Msg) = [ID_FIRST + ID_ITEMSEARCH];
  virtual void SearchResource (RTMessage Msg) = [ID_FIRST + ID_RESOURCE];
  virtual void Add            (RTMessage Msg) = [ID_FIRST + ID_MODIFY];
  virtual void Top            (RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom         (RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Next           (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
  virtual void Previous       (RTMessage Msg) = [ID_FIRST + ID_BACK];
  virtual void Delete         (RTMessage Msg) = [ID_FIRST + ID_DELETE];
  virtual void SetDetail      (RTMessage Msg) = [ID_FIRST + ID_DETAIL];
  virtual void DetailReSet    (RTMessage Msg) = [CM_FIRST + CM_SETRESOURCE];
  virtual void CalQuantity    (RTMessage Msg) = [CM_FIRST + CM_QUANTITY];
  virtual void CostType       (RTMessage Msg) = [ID_FIRST + ID_COSTTYPE];
  virtual void MultiLevel     (RTMessage Msg) = [ID_FIRST + ID_LEVEL];
  virtual void DetailList     (RTMessage Msg) = [CM_FIRST + CM_DETAIL]{SetResource();};
  virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL]
  {
   cb.closeAll();
   SCSDef->ShutDownWindow();
   ShutDownWindow();
  };
  virtual void ClearResource();
  virtual void SetResource();
  virtual int  GetResource(char *szRescode,char *szCostItem,int iLevel, int i,double dRequirement);
  virtual void SetupWindow();
  virtual void UpdateWindow();
  virtual void SetResourceValues(int iLevel,int i,PTResrc Resrc,double dRequirement);
  virtual void SetManufacturedValues(int iLevel,int i,char *szRescode,double UpperKousei,double UpperLotsize);
  virtual BOOL SetField(int ID, char *field);
  virtual BOOL SetField   (int ID, Field4 *field);
  virtual char *GetField(int ID);
  virtual double dGetField(int ID);
  int GetProduction(Field4 *fPcode);
  
  ~TResourceUpdate(){cb.closeAll();};
};

#endif