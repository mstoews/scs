_CLASSDEF(TTopics)
class TTopics : public TDialog
{
 public:
  double dPeriod;
  //TOPICS databases
  PTTopicsItemm TopicsItem;
  PTTopicsJiss  TopicsJiss;
  PTTopicsTpstr TopicsCoststr;
  PTTopicsTmast TopicsTmast;
  //SCS databases
  PTscitemm     Item;
  PTManResult   ManResults;
  PTJiss        Jiss;
  PTResrc       Resrc;
  PTtrstr       CostStr;
  PTprofile     Profile;

  TTopics (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle)
  {
   Profile = new Tprofile(&cb);
   Profile->top();
   dPeriod = double (Str4ptr(Profile->curr_pdn->str()));
   Profile->close();
  };
  virtual void Ok      (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel  (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  BOOL  ItemMaster(void);
  BOOL  Jobtime(void);
  BOOL  ResourceStruct(void);
  char *GetField(int ID);
  virtual void SetupWindow();
};
