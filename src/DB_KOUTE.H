_CLASSDEF(TKoutei)
class TKoutei : public Data4
{
 public:
 Field4 *fKoutei;
 Field4 *fKtype;
 Field4 *fKounm;
 Field4 *fFullnm;
 Field4 *fLrate;
 Field4 *fIrate;
 Field4 *fDrate;
 Field4 *fManp;
 Field4 *fMaitd;

 TKoutei(Code4 *cb,LPSTR name=0):Data4 (cb,"tkoutei")
 {
  top();
  SetIndex("fkoutei");
  fKoutei	= (Field4 *) new Field4(*(Data4 *)this,"fkoutei");
  fKtype	= (Field4 *) new Field4(*(Data4 *)this,"fktype");
  fKounm	= (Field4 *) new Field4(*(Data4 *)this,"fkounm");
  fFullnm       = (Field4 *) new Field4(*(Data4 *)this,"ffullnm");
  fLrate	= (Field4 *) new Field4(*(Data4 *)this,"fLratio");
  fIrate	= (Field4 *) new Field4(*(Data4 *)this,"fIratio");
  fDrate	= (Field4 *) new Field4(*(Data4 *)this,"fDratio");
  fManp 	= (Field4 *) new Field4(*(Data4 *)this,"fManp");
  fMaitd	= (Field4 *) new Field4(*(Data4 *)this,"fMaitd");
 }
 ~TKoutei(){close();};
};

_CLASSDEF(TKouteiList)
class TKouteiList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTKoutei Koutei;
 TKouteiList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTKoutei t):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  Koutei = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetField   (int ID, Field4 *field);
 virtual void SetupWindow();
};
