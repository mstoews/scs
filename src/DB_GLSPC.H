_CLASSDEF(TGLSpec)
class TGLSpec : public Data4
{
 public:
 Field4 *fRescode;
 Field4 *fAccount;
 Field4 *fDepart;
 Field4 *fName;
 Field4 *fCosttype;
 TGLSpec(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_glspc")
 {
  top();
  SetIndex("fpcode");
  fRescode = (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fAccount = (Field4 *) new Field4(*(Data4 *)this,"account");
  fDepart  = (Field4 *) new Field4(*(Data4 *)this,"dept");
  fName    = (Field4 *) new Field4(*(Data4 *)this,"descript");
  fCosttype = (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
 }
 ~TGLSpec(){close();};
};

_CLASSDEF(TSCGLSpecList)
class TSCGLSpecList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTGLSpec GLSpec;
 TSCGLSpecList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTGLSpec el):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  GLSpec = el;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
