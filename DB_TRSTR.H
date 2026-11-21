/* Item master class */
_CLASSDEF(Ttrstr)
class Ttrstr : public Data4
{
 public:
  Field4 *fOpcode;
  Field4 *fType;
  Field4 *fSeqno;
  Field4 *fKpcode;
  Field4 *fStqty;
  Field4 *fKousei;
  Field4 *fLt1;
  Field4 *fLt2;
  Field4 *fComm;
  Field4 *fCorectm;
  Field4 *fSpcrtp;
  Field4 *fChk_flg;

  Ttrstr(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_cstst")
  {
  SetIndex("fopcode");
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fopcode");
  fType 	= (Field4 *) new Field4(*(Data4 *)this,"ftype");
  fSeqno	= (Field4 *) new Field4(*(Data4 *)this,"fseqno");
  fKpcode	= (Field4 *) new Field4(*(Data4 *)this,"fkpcode");
  fStqty	= (Field4 *) new Field4(*(Data4 *)this,"fstqty");
  fKousei	= (Field4 *) new Field4(*(Data4 *)this,"fkousei");
  fLt1		= (Field4 *) new Field4(*(Data4 *)this,"flt1");
  fLt2		= (Field4 *) new Field4(*(Data4 *)this,"flt2");
  fComm		= (Field4 *) new Field4(*(Data4 *)this,"fcomm");
  fCorectm	= (Field4 *) new Field4(*(Data4 *)this,"fcorectm");
  fSpcrtp	= (Field4 *) new Field4(*(Data4 *)this,"fspcrtp");
  fChk_flg	= (Field4 *) new Field4(*(Data4 *)this,"fchk_flg");
  }
  double dQuantity() {return double(Str4ptr(fStqty->str()));};
  double dKousei()   {return double(Str4ptr(fKousei->str()));};
  ~Ttrstr(){close();};
 };

_CLASSDEF(TSCtrstrList)
class TSCtrstrList : public TDialog
{
 public:
 char szQuery[255];
 PTSpread List;
 PTtrstr trstr;
 TSCtrstrList(PTWindowsObject AParent, LPSTR AName,LPSTR AQuery, PTtrstr c):
  TDialog (AParent, AName)
  {
  strcpy(szQuery,AQuery);
  trstr = c;
  List = new TSpread(this,ID_DISTLIST);
  };
 virtual void HandleListBox (RTMessage Msg) = [WM_FIRST + SSM_DBLCLK];
 virtual void SetupWindow();
};
