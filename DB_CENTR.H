_CLASSDEF(TCentre)
class TCentre : public Data4
{
 public:
 Field4 *fCode;
 Field4 *fName;
 Field4 *fShortName;
 Field4 *fOverhead;
 Field4 *fUser;
 Field4 *fDate;

 TCentre(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_cntre")
 {
  top();
  SetIndex("fcode");
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fcode");
  fName		= (Field4 *) new Field4(*(Data4 *)this,"fname");
  fShortName	= (Field4 *) new Field4(*(Data4 *)this,"fshortname");
  fUser		= (Field4 *) new Field4(*(Data4 *)this,"fuser");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
 }
 ~TCentre(){close();};
};

_CLASSDEF(TSCCentreList)
class TSCCentreList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTCentre centre;
 TSCCentreList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTCentre t):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  centre = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};

_CLASSDEF(TProc)
class TProc : public Data4
{
 public:
 Field4 *fKoutei;
 Field4 *fCode;
 Field4 *fKname;
 Field4 *fLabour;
 Field4 *fOh;
 Field4 *fIDoh;

 TProc(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_proc")
 {
  top();
  SetIndex("fkoutei");
  fKoutei	= (Field4 *) new Field4(*(Data4 *)this,"fkoutei");
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fcode");
  fKname	= (Field4 *) new Field4(*(Data4 *)this,"fkname");
  fLabour	= (Field4 *) new Field4(*(Data4 *)this,"flabour");
  fOh    	= (Field4 *) new Field4(*(Data4 *)this,"fOh");
  fIDoh   	= (Field4 *) new Field4(*(Data4 *)this,"fIDoh");
 }
 ~TProc(){close();};
};
