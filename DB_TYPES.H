_CLASSDEF(TTypes)
class TTypes : public Data4
{
 public:
 Field4 *fCode;
 Field4 *fName;
 Field4 *fShortName;
 Field4 *fActual;
 Field4 *fUser;
 Field4 *fDate;

 TTypes(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_typem")
 {
  top();
  SetIndex("fcode");
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fcode");
  fName		= (Field4 *) new Field4(*(Data4 *)this,"fname");
  fShortName	= (Field4 *) new Field4(*(Data4 *)this,"fshortname");
  fActual       = (Field4 *) new Field4(*(Data4 *)this,"factual");
  fUser		= (Field4 *) new Field4(*(Data4 *)this,"fuser");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
 }
 ~TTypes(){close();};
};

_CLASSDEF(TSCTypesList)
class TSCTypesList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTTypes typem;
 TSCTypesList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTTypes t):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  typem = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
