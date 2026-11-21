// Cost Detail
_CLASSDEF(TCostd)
class TCostd : public Data4
{
 public:
  Field4 *fCode;
  Field4 *fRescode;
  Field4 *fLotno;
  Field4 *fPeriod;
  Field4 *fCosttype;
  Field4 *fElement;
  Field4 *fUnitPrice;
  Field4 *fQuantity;

 TCostd(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costm")
 {
  top();
  tag_select(tag("fcode"));
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fRescode 	= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fLotno   	= (Field4 *) new Field4(*(Data4 *)this,"flotno");
  fPeriod  	= (Field4 *) new Field4(*(Data4 *)this,"fperiod");
  fCosttype	= (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
  fElement 	= (Field4 *) new Field4(*(Data4 *)this,"felement");
  fUnitPrice	= (Field4 *) new Field4(*(Data4 *)this,"fcost");
  fQuantity 	= (Field4 *) new Field4(*(Data4 *)this,"fquantity");
 }
 ~TCostd(){close();};
};

_CLASSDEF(TSCCostdList)
class TSCCostdList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTCostd Costd;
 TSCCostdList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTCostd c):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  Costd = c;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
