_CLASSDEF(TElement)
class TElement : public Data4
{
 public:
 Field4 *fCode;
 Field4 *fName;
 Field4 *fShortName;
 Field4 *fTypeFlag;
 Field4 *fUser;
 Field4 *fDate;

 TElement(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_elmnt")
 {
  top();
  SetIndex("fcode");
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fcode");
  fName		= (Field4 *) new Field4(*(Data4 *)this,"fname");
  fShortName	= (Field4 *) new Field4(*(Data4 *)this,"fshort");
  fTypeFlag	= (Field4 *) new Field4(*(Data4 *)this,"fdirflg");
  fUser		= (Field4 *) new Field4(*(Data4 *)this,"fuser");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
 }
 ~TElement(){close();};
};

_CLASSDEF(TSCElementList)
class TSCElementList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTElement element;
 TSCElementList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTElement el):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  element = el;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
