
_CLASSDEF(TAccess)
class TAccess : public Data4
{
 public:
 Field4 *fUsername;
 Field4 *fDescript;
 Field4 *fDate;
 Field4 *fPassword;
 Field4 *fLevel;

 TAccess(Code4 *cb,LPSTR name=0):Data4 (cb,"access")
 {
  top();
  SetIndex("username");
  fUsername  = (Field4 *) new Field4(*(Data4 *)this,"username");
  fDescript  = (Field4 *) new Field4(*(Data4 *)this,"descript");
  fDate      = (Field4 *) new Field4(*(Data4 *)this,"date");
  fPassword  = (Field4 *) new Field4(*(Data4 *)this,"password");
  fLevel     = (Field4 *) new Field4(*(Data4 *)this,"level");
 }
 ~TAccess(){;unlock(); close();};
};

_CLASSDEF (TGLAccess)
class TGLAccess : public TDialog
{
 public :
 BOOL ACCEPT;
 PTAccess access;
 TGLAccess(PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
 virtual void SetupWindow();
 virtual void Ok     (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Add    (RTMessage Msg) = [ID_FIRST + ID_ADD];
 virtual void Cancel (RTMessage Msg) = [ID_FIRST + IDCANCEL];
};

