_CLASSDEF(TAlloc)
class TAlloc : public Data4
{
 public:
 Field4 *fCode;
 Field4 *fName;
 Field4 *fShortName;
 Field4 *fRate;
 Field4 *fCentre;
 Field4 *fUser;
 Field4 *fDate;
 Field4 *fGlacct;
 Field4 *fGldepart;

 TAlloc(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_alloc")
 {
  top();
  SetIndex("fcode");
  fCode		= (Field4 *) new Field4(*(Data4 *)this,"fcode");
  fName		= (Field4 *) new Field4(*(Data4 *)this,"fname");
  fShortName	= (Field4 *) new Field4(*(Data4 *)this,"fshortname");
  fRate 	= (Field4 *) new Field4(*(Data4 *)this,"fcrate");
  fCentre	= (Field4 *) new Field4(*(Data4 *)this,"fcentre");
  fUser		= (Field4 *) new Field4(*(Data4 *)this,"fuser");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
  fGlacct	= (Field4 *) new Field4(*(Data4 *)this,"fglacct");
  fGldepart	= (Field4 *) new Field4(*(Data4 *)this,"fdepart");
 }
 ~TAlloc(){close();};
};

_CLASSDEF(TSCAllocList)
class TSCAllocList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTAlloc  alloc;
 TSCAllocList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTAlloc t):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  alloc = t;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
