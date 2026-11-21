
_CLASSDEF(TGLCloseMonth)
class TGLCloseMonth : public TDialog
{
 public:
 Tglaccts *glaccts; 
 Tgldept  *gldept;
 Tprofile *profile;
 Tglbtchd *glbtchd;
 Tglbtch  *glbtch;
 Tgldist  *gldist;
 char  NewPeriod[8];
 char Period[8];
 short PeriodNo;
 double NewPeriodn;


 TGLCloseMonth(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL): TDialog(AParent,AName){};
 virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
};
