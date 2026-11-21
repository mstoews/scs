#include "gl_dlg.h"
_CLASSDEF(TTStruct)
class TTStruct : public TMainDialog
{
 public:
  double dPeriod;
  //TOPICS databases
  PTTopicsThrm  TopicsThrm;
  PTTopicsItemm TopicsItem;
  PTTopicsJiss  TopicsJiss;
  PTTopicsTpstr TopicsCoststr;
  PTTopicsTmast TopicsTmast;
  PTprofile     profile;
  //SCS databases
  PTscitemm     Item;
  PTManResult   ManResults;
  PTJiss        Jiss;
  PTResrc       Resrc;
  PTtrstr       CostStr;
  PTprofile     Profile;
  PTCostTemp    CostTemp;

  TTStruct (PTWindowsObject AParent, LPSTR ATitle):TMainDialog(AParent,ATitle)
  {
   Profile = new Tprofile(&cb);
   Profile->top();
   dPeriod = double (Str4ptr(Profile->curr_pdn->str()));
   Profile->close();
  };
  virtual void Ok      (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel  (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  //BOOL  ItemMaster(void);
  BOOL  PeriodScope(int dPeriod);
  BOOL  ProductionResults(void);
  //BOOL  Jobtime(void);
  BOOL  ProductionTotals(void);
  BOOL  NonMatResource(void);
  BOOL  Resources(void);
  BOOL  LabourHours(void);
  //BOOL  ResourceStruct(void);
  char *GetField(int ID);
  double GetProduction(Field4 *fPcode);
  virtual void SetupWindow();
};

