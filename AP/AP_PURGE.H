extern Code4 cb;
_CLASSDEF(TAPPurgeBatch)
class TAPPurgeBatch : public TDialog
{
 public:
 PTapdist  apdist;
 TAPPurgeBatch(PTWindowsObject AParent, LPSTR AName, PTModule AModule = NULL)
 : TDialog(AParent,AName){};
 virtual void SetupWindow();
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL]{cb.closeAll(); ShutDownWindow();};
};

