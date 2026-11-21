#include "scs.h"
// Typess maintainance

extern Code4 cb;
_CLASSDEF(TSysSetPeriod)
class TSysSetPeriod : public TDialog
{
 public:
  PTprofile profile;
  char szCurrentPeriod[21];
  int  iCurrentPd;
  TSysSetPeriod (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow();
  void SetButton(int ID,Field4 *field);
  virtual void Period1     (RTMessage Msg) = [ID_FIRST + 101]
  {
   strcpy(szCurrentPeriod,profile->pd1_desc->str());
   iCurrentPd = 1;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
   
  };
  virtual void Period2     (RTMessage Msg) = [ID_FIRST + 102]
  {
   strcpy(szCurrentPeriod,profile->pd2_desc->str());
   iCurrentPd = 2;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period3     (RTMessage Msg) = [ID_FIRST + 103]
   {
   strcpy(szCurrentPeriod,profile->pd3_desc->str());
   iCurrentPd = 3;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period4     (RTMessage Msg) = [ID_FIRST + 104]
   {
   strcpy(szCurrentPeriod,profile->pd4_desc->str());
   iCurrentPd = 4;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period5     (RTMessage Msg) = [ID_FIRST + 105]
  {
   strcpy(szCurrentPeriod,profile->pd5_desc->str());
   iCurrentPd = 5;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period6     (RTMessage Msg) = [ID_FIRST + 106]
  {
   strcpy(szCurrentPeriod,profile->pd6_desc->str());
   iCurrentPd = 6;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period7     (RTMessage Msg) = [ID_FIRST + 107]
  {
   strcpy(szCurrentPeriod,profile->pd7_desc->str());
   iCurrentPd = 7;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period8     (RTMessage Msg) = [ID_FIRST + 108]
  {
   strcpy(szCurrentPeriod,profile->pd8_desc->str());
   iCurrentPd = 8;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period9     (RTMessage Msg) = [ID_FIRST + 109]
  {
   strcpy(szCurrentPeriod,profile->pd9_desc->str());
   iCurrentPd = 9;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period10    (RTMessage Msg) = [ID_FIRST + 110]
  {
   strcpy(szCurrentPeriod,profile->pd10_desc->str());
   iCurrentPd = 10;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period11    (RTMessage Msg) = [ID_FIRST + 111]
  {
   strcpy(szCurrentPeriod,profile->pd11_desc->str());
   iCurrentPd = 11;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Period12    (RTMessage Msg) = [ID_FIRST + 112]
  {
   strcpy(szCurrentPeriod,profile->pd12_desc->str());
   iCurrentPd = 12;
   profile->curr_pd->assign(szCurrentPeriod);
   profile->curr_pdn->assignDouble(iCurrentPd);
   SendMessage(HWindow,WM_COMMAND,IDCANCEL,NULL);
  };

  virtual void Cancel      (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void End         (RTMessage Msg) = [CM_FIRST + IDCANCEL]
  {
   Cancel(Msg);
  }
};

void TGlWindow::SCSSetCurrentPeriod (RTMessage)
 {
  GetApplication()->ExecDialog(new TSysSetPeriod(this,"SYSSETPERIOD"));
  SendMessage(HStatusBar, SBRM_SETITEMTEXT,3,(LPARAM)GetCurrentPeriod());
 }

void TSysSetPeriod::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 
 profile = new Tprofile(&cb);
 profile->top(); 
 SetButton(101,profile->pd1_desc);
 SetButton(102,profile->pd2_desc);
 SetButton(103,profile->pd3_desc);
 SetButton(104,profile->pd4_desc);
 SetButton(105,profile->pd5_desc);
 SetButton(106,profile->pd6_desc);
 SetButton(107,profile->pd7_desc);
 SetButton(108,profile->pd8_desc);
 SetButton(109,profile->pd9_desc);
 SetButton(110,profile->pd10_desc);
 SetButton(111,profile->pd11_desc);
 SetButton(112,profile->pd12_desc);
 HourGlassOff();
}

void TSysSetPeriod::SetButton(int ID,Field4 *field)
{
 Str4large szPeriod;
 szPeriod.assign(field->str());
 szPeriod.trim();
 SetDlgItemText(HWindow,ID,(LPCSTR)szPeriod.str());
 return;
}

void TSysSetPeriod::Cancel(RTMessage)
{
 profile->flush();
 cb.closeAll();
 ShutDownWindow();
}
