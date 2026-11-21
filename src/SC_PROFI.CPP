#include "scs.h"

#define   PERIOD_LEN 9
#define   DESCRIPT_LEN 21


_CLASSDEF(TGLProfile)
class TGLProfile : public TDialog
{
 public:
 PTprofile profile;
 TGLProfile(PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
 virtual void WMInitDialog    (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void Ok              (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel          (RTMessage Msg) = [ID_FIRST + IDCANCEL];
 virtual void SetPeriodDates  (RTMessage Msg) = [ID_FIRST + ID_SETPERIOD];
};

_CLASSDEF(TGLSetPeriodDates)
class TGLSetPeriodDates : public TDialog
{
 public:
 PTprofile profile;
 TGLSetPeriodDates(PTWindowsObject AParent, LPSTR ATitle, PTprofile p): TDialog(AParent,ATitle){profile=p;};
 virtual void WMInitDialog   (RTMessage Msg) = [WM_FIRST + WM_INITDIALOG];
 virtual void Ok             (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel         (RTMessage Msg) = [ID_FIRST + IDCANCEL];
};



void TGLSetPeriodDates::Ok           (RTMessage)
{
 char *szPeriod;
 char *szPeriode;
 char *szDescript;

 if((szPeriod =   (char *) malloc(9)) == NULL) {return;}
 if((szPeriode =  (char *) malloc(9)) == NULL) {return;}
 if((szDescript = (char *) malloc(21)) == NULL) {return;}

 GetDlgItemText(HWindow,ID_PERIOD1,szPeriod,profile->period1->len());
 GetDlgItemText(HWindow,ID_PERIOD1E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD1_DESC,szDescript,21);

 profile->period1->assign    (szPeriod);
 profile->period1e->assign   (szPeriode);
 profile->pd1_desc->assign   (szDescript);

 GetDlgItemText(HWindow,ID_PERIOD2,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD2E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD2_DESC,szDescript,DESCRIPT_LEN);

 profile->period2->assign    (szPeriod);
 profile->period2e->assign   (szPeriode);
 profile->pd2_desc->assign   (szDescript);

 //3
 GetDlgItemText(HWindow,ID_PERIOD3,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD3E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD3_DESC,szDescript,DESCRIPT_LEN);
 profile->period3->assign    (szPeriod);
 profile->period3e->assign   (szPeriode);
 profile->pd3_desc->assign   (szDescript);


 //4
 GetDlgItemText(HWindow,ID_PERIOD4,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD4E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD4_DESC,szDescript,DESCRIPT_LEN);
 profile->period4->assign    (szPeriod);
 profile->period4e->assign   (szPeriode);
 profile->pd4_desc->assign   (szDescript);

 //5
 GetDlgItemText(HWindow,ID_PERIOD5,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD5E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD5_DESC,szDescript,DESCRIPT_LEN);
 profile->period5->assign    (szPeriod);
 profile->period5e->assign   (szPeriode);
 profile->pd5_desc->assign   (szDescript);

 //6
 GetDlgItemText(HWindow,ID_PERIOD6,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD6E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD6_DESC,szDescript,DESCRIPT_LEN);
 profile->period6->assign    (szPeriod);
 profile->period6e->assign   (szPeriode);
 profile->pd6_desc->assign   (szDescript);


 //7
 GetDlgItemText(HWindow,ID_PERIOD7,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD7E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD7_DESC,szDescript,DESCRIPT_LEN);
 profile->period7->assign    (szPeriod);
 profile->period7e->assign   (szPeriode);
 profile->pd7_desc->assign   (szDescript);

 //8
 GetDlgItemText(HWindow,ID_PERIOD8,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD8E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD8_DESC,szDescript,DESCRIPT_LEN);
 profile->period8->assign    (szPeriod);
 profile->period8e->assign   (szPeriode);
 profile->pd8_desc->assign   (szDescript);

 //9
 GetDlgItemText(HWindow,ID_PERIOD9,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD9E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD9_DESC,szDescript,DESCRIPT_LEN);
 profile->period9->assign    (szPeriod);
 profile->period9e->assign   (szPeriode);
 profile->pd9_desc->assign   (szDescript);

  //10
 GetDlgItemText(HWindow,ID_PERIOD10,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD10E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD10_DESC,szDescript,DESCRIPT_LEN);
 profile->period10->assign    (szPeriod);
 profile->period10e->assign   (szPeriode);
 profile->pd10_desc->assign   (szDescript);

 //11
 GetDlgItemText(HWindow,ID_PERIOD11,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD11E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD11_DESC,szDescript,DESCRIPT_LEN);
 profile->period11->assign    (szPeriod);
 profile->period11e->assign   (szPeriode);
 profile->pd11_desc->assign   (szDescript);

 //12
 GetDlgItemText(HWindow,ID_PERIOD12,szPeriod,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PERIOD12E,szPeriode,PERIOD_LEN);
 GetDlgItemText(HWindow,ID_PD12_DESC,szDescript,DESCRIPT_LEN);
 profile->period12->assign    (szPeriod);
 profile->period12e->assign   (szPeriode);
 profile->pd12_desc->assign   (szDescript);

 profile->flush();
 ShutDownWindow();
}

void TGLSetPeriodDates::Cancel       (RTMessage)
{
 ShutDownWindow();
}

void TGLSetPeriodDates::WMInitDialog (RTMessage)
{
 SetDlgItemText(HWindow,ID_PERIOD1,profile->period1->str());
 SetDlgItemText(HWindow,ID_PERIOD1E,profile->period1e->str());
 SetDlgItemText(HWindow,ID_PD1_DESC,profile->pd1_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD2, profile->period2->str());
 SetDlgItemText(HWindow,ID_PERIOD2E,profile->period2e->str());
 SetDlgItemText(HWindow,ID_PD2_DESC,profile->pd2_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD3, profile->period3->str());
 SetDlgItemText(HWindow,ID_PERIOD3E,profile->period3e->str());
 SetDlgItemText(HWindow,ID_PD3_DESC,profile->pd3_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD4, profile->period4->str());
 SetDlgItemText(HWindow,ID_PERIOD4E,profile->period4e->str());
 SetDlgItemText(HWindow,ID_PD4_DESC,profile->pd4_desc->str());


 SetDlgItemText(HWindow,ID_PERIOD5, profile->period5->str());
 SetDlgItemText(HWindow,ID_PERIOD5E,profile->period5e->str());
 SetDlgItemText(HWindow,ID_PD5_DESC,profile->pd5_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD6, profile->period6->str());
 SetDlgItemText(HWindow,ID_PERIOD6E,profile->period6e->str());
 SetDlgItemText(HWindow,ID_PD6_DESC,profile->pd6_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD7, profile->period7->str());
 SetDlgItemText(HWindow,ID_PERIOD7E,profile->period7e->str());
 SetDlgItemText(HWindow,ID_PD7_DESC,profile->pd7_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD8, profile->period8->str());
 SetDlgItemText(HWindow,ID_PERIOD8E,profile->period8e->str());
 SetDlgItemText(HWindow,ID_PD8_DESC,profile->pd8_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD9, profile->period9->str());
 SetDlgItemText(HWindow,ID_PERIOD9E,profile->period9e->str());
 SetDlgItemText(HWindow,ID_PD9_DESC,profile->pd9_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD10, profile->period10->str());
 SetDlgItemText(HWindow,ID_PERIOD10E,profile->period10e->str());
 SetDlgItemText(HWindow,ID_PD10_DESC,profile->pd10_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD11, profile->period11->str());
 SetDlgItemText(HWindow,ID_PERIOD11E,profile->period11e->str());
 SetDlgItemText(HWindow,ID_PD11_DESC,profile->pd11_desc->str());

 SetDlgItemText(HWindow,ID_PERIOD12, profile->period12->str());
 SetDlgItemText(HWindow,ID_PERIOD12E,profile->period12e->str());
 SetDlgItemText(HWindow,ID_PD12_DESC,profile->pd12_desc->str());

}




void TGLProfile::WMInitDialog (RTMessage)
{
 profile = (Tprofile *) new Tprofile(&cb,"profile");
 profile->top();

 SetDlgItemText(HWindow,ID_COMPANY,profile->company->str());
 SetDlgItemText(HWindow,ID_PERIODS,profile->period1->str());
 SetDlgItemText(HWindow,ID_PERIODE,profile->period12e->str());
 SetDlgItemText(HWindow,ID_ADDRESS1,profile->address1->str());
 SetDlgItemText(HWindow,ID_ADDRESS2,profile->address2->str());
 SetDlgItemText(HWindow,ID_ADDRESS3,profile->address3->str());
 SetDlgItemText(HWindow,ID_POST,profile->postal->str());
 SetDlgItemText(HWindow,ID_TEL,profile->telephone->str());
 SetDlgItemText(HWindow,ID_FAX,profile->fax->str());        // Add fax number.
 SetDlgItemText(HWindow,ID_PREVIOUSYR,profile->previousyr->str());
 SetDlgItemText(HWindow,ID_RETAINED,profile->retained->str());

}

void TGLProfile::Ok  (RTMessage)
{

 char *szCompany;
 char *szAddress1;
 char *szAddress2;
 char *szAddress3;
 char *szPostal;
 char *szFax;
 char *szTelephone;
 char *szPreviousyr;
 char *szRetained;


 if((szCompany    = (char *) malloc(profile->company->len())) == NULL){return;}
 if((szAddress1   = (char *) malloc(31)) == NULL){return;}
 if((szAddress2   = (char *) malloc(31)) == NULL){return;}
 if((szAddress3   = (char *) malloc(31)) == NULL){return;}
 if((szPostal     = (char *) malloc(10)) == NULL){return;}
 if((szFax        = (char *) malloc(15)) == NULL){return;}
 if((szTelephone  = (char *) malloc(15)) == NULL){return;}
 if((szPreviousyr = (char *) malloc(2)) == NULL){return;}
 if((szRetained   = (char *) malloc(7)) == NULL){return;}


 GetDlgItemText(HWindow,ID_COMPANY,szCompany,profile->company->len());
 GetDlgItemText(HWindow,ID_ADDRESS1,szAddress1,profile->address1->len());
 GetDlgItemText(HWindow,ID_ADDRESS2,szAddress2,profile->address1->len());
 GetDlgItemText(HWindow,ID_ADDRESS3,szAddress3,profile->address1->len());
 GetDlgItemText(HWindow,ID_POST,szPostal,profile->postal->len());
 GetDlgItemText(HWindow,ID_TEL,szTelephone,profile->telephone->len());
 GetDlgItemText(HWindow,ID_FAX,szFax,profile->fax->len());        // Add fax number.
 GetDlgItemText(HWindow,ID_PREVIOUSYR,szPreviousyr,profile->previousyr->len());
 GetDlgItemText(HWindow,ID_RETAINED,szRetained,profile->retained->len());

 profile->company->assign(szCompany);
 profile->address1->assign(szAddress1);
 profile->address2->assign(szAddress2);
 profile->address3->assign(szAddress3);
 profile->postal->assign(szPostal);
 profile->telephone->assign(szTelephone);
 profile->fax->assign(szFax);
 profile->previousyr->assign(szPreviousyr);
 profile->retained->assign(szRetained);
 SendMessage(hStatus, SBRM_SETITEMTEXT, IDOK, (LPARAM)szCompany);
 profile->flush();
 profile->close();
 ShutDownWindow();
}

void TGLProfile::Cancel (RTMessage)
{
 cb.closeAll();
 ShutDownWindow();
}

void TGLProfile::SetPeriodDates (RTMessage)
{
 GetApplication()->ExecDialog(new TGLSetPeriodDates(this,"GL_SETPERIOD",profile));
}

void TGlWindow::SCSCompanyProfile (RTMessage)
{
 GetApplication()->ExecDialog(new TGLProfile(this,"SC_COMPANY"));
}



