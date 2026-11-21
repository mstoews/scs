_CLASSDEF(TResrc)
class TResrc :public Data4
{
 public:

 Field4 *fCode;
 Field4 *fName;
 Field4 *fShortName;
 Field4 *fUnit;
 Field4 *fElement;
 Field4 *fLotsize;
 Field4 *fUser;
 Field4 *fDate;
 Field4 *fUpdatedate;
 Field4 *fUpdateuser;
 Field4 *fType;
 Field4 *fCostType;
 Field4 *fRate;
 Field4 *fActCost;

 TResrc(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_resrc")
 {
 SetIndex("fcode");
 top();
 fCode    	= (Field4 *) new Field4(*(Data4 *)this,"fcode");
 fName  	= (Field4 *) new Field4(*(Data4 *)this,"ffulln");
 fShortName  	= (Field4 *) new Field4(*(Data4 *)this,"fname");
 fUnit     	= (Field4 *) new Field4(*(Data4 *)this,"funit");
 fLotsize       = (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
 fElement	= (Field4 *) new Field4(*(Data4 *)this,"felmncd");
 fUser    	= (Field4 *) new Field4(*(Data4 *)this,"fentusr");
 fDate		= (Field4 *) new Field4(*(Data4 *)this,"fentdt");
 fUpdatedate    = (Field4 *) new Field4(*(Data4 *)this,"fupdtedt");
 fUpdateuser	= (Field4 *) new Field4(*(Data4 *)this,"fupdteusr");
 fType		= (Field4 *) new Field4(*(Data4 *)this,"ftype");
 fCostType      = (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
 fRate		= (Field4 *) new Field4(*(Data4 *)this,"frate");
 fActCost 	= (Field4 *) new Field4(*(Data4 *)this,"factcost");
 }
 double dRate()        {return double (Str4ptr(fRate->str()));};
 double dActCost()     {return double (Str4ptr(fActCost->str()));};

 ~TResrc(){close();};
};


_CLASSDEF(TSCResList)
class TSCResList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTResrc Resrc;
 TSCResList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTResrc t):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  Resrc = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetField(int ID,Field4 *field);
 virtual void SetupWindow();
};
