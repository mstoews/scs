/*** Cost Calculation ***/
_CLASSDEF(TTActual)
class TTActual : public TMainDialog
{
 public:
  /* Create pointers to the database */
  char szParent[11];
  BOOL bDetail;
  BOOL bSummary;
  BOOL bAllCosts;
  double    dPeriod;
  double    dLabour;
  double    dTotal;
  double    dMaterial;
  double    dOverhead;
  double    dIndirect;
  double    dJobtime;
  double    dLotSize;
  double    dParentKousei;
  double    dRate;
  double    dQuantity;
  double    dProduction;
  long      lLots;
  PTProduct Product;
  PTCost    Cost;
  PTCostm   Costm;
  PTCostd   Costd;
  PTCoste   Coste;
  PTCentre  Centre;
  PTGLSpec  GLSpec;
  PTGLcost  GLcost;
  PTtrstr   CostStr;
  PTProc    Proc;
  PTProd    Prod;
  PTJiss    Jiss;    
  PTElement Element;
  PTscitemm Item;
  PTResrc   Resrc;
  PTManResult TopicsResults;
  PTResults  Results;
  PTTypes   Types;
  PTTopicsTmast Tmast;

  TTActual (PTWindowsObject AParent, LPSTR ATitle):TMainDialog(AParent,ATitle){};

  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel      (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void Production  (RTMessage Msg) = [ID_FIRST + ID_ACTUAL];
  virtual void ProdResults (RTMessage Msg) = [ID_FIRST + ID_RESULTS];
  
  virtual void SetupWindow(void);
  BOOL ActualCost(void);
  BOOL CreateProdTemp(void);
  BOOL ManufacturedResults(char *szCostType);
  BOOL StandardCostElements(char *szCostType);
  BOOL BudgetCost(void);
  BOOL HistoricalCost(void);
  void Msg(int ID,char *szMsg);
  double GetManQuantity(char *szFpcode);
  double GetHours(char *szFpcode);  
  BOOL ProdElements(char *szCostType);
  void WriteElements(int iLevel, char *szCode,char *szCostType,PTscitemm Item, PTResrc Resrc, PTtrstr CostStr, PTCost Coste,double dRequirement);
  void WriteProd(int iLevel, char *szCode,char *szCostType,PTscitemm Item, PTResrc Resrc, PTtrstr CostStr,double dRequirement);
  void LoadTypes(PTTypes Types);
  ~TTActual(){cb.closeAll();}; 
};
