_CLASSDEF(TItemUpdate)
class TItemUpdate : public TDialog
{
 private:
   BOOL bSearch;
   BOOL bResource;
   BOOL bMultiLevel;
   char szParent[11];
   char szPUnit;
   double dStandard;
   double dMaterial;
   double dLabour;
   double dOH;
   double dIndirect;
   double dLotSize;
   double dParentKousei;
   int iLine;

   PTscitemm       Items;
   PTResrc	   Resrc;
   PTtrstr         CostStr;
   PTTypes	   Types;
   PTResults	   Results;   
   PTProc          Proc;

 public:
  TItemUpdate(PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){}
  virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK]{SetResource();};
  virtual void CostType       (RTMessage Msg) = [ID_FIRST + ID_COSTTYPE];
  virtual void MultiLevel     (RTMessage Msg) = [ID_FIRST + ID_LEVEL];
  virtual void DetailList     (RTMessage Msg) = [CM_FIRST + CM_DETAIL]{SetResource();};
  virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL]{cb.closeAll();ShutDownWindow();};

  virtual void SetResource();
  virtual int  GetResource(char *szRescode,char *szCostItem,int iLevel, int i,double dRequirement);
  virtual void SetupWindow();
  virtual void SetResourceValues(int iLevel,int i,PTResrc Resrc,double dRequirement);
  virtual char *GetField(int ID);
  virtual double dGetField(int ID);
  
  ~TItemUpdate(){cb.closeAll();};
};
