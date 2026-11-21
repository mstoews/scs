#include "scs.h"
/*** Data Update: Query of Material Usage from production amounts. ***/

_CLASSDEF(TSCSMaterialUsage)
class TSCSMaterialUsage : public TDialog
{
 public:
  TSCSMaterialUsage (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
};
void TGlWindow::SCSMaterialUsage      (RTMessage)
 {GetApplication()->ExecDialog(new TSCSMaterialUsage(this,"DUSAGE"));}


