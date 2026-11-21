#include "glh.h"
#include "gl_dlg.h"

extern HINSTANCE hInst;
extern Code4 cb;
void TMainDialog::Add         (RTMessage ){};
void TMainDialog::Top         (RTMessage ){};
void TMainDialog::Bottom      (RTMessage ){};
void TMainDialog::Forward     (RTMessage ){};
void TMainDialog::Back        (RTMessage ){};
void TMainDialog::Delete      (RTMessage ){};
void TMainDialog::Modify      (RTMessage ){};
void TMainDialog::UpdateWindow(){};
void TMainDialog::ClearScreen(void){};
void TMainDialog::SetupWindow(){ TDialog::SetupWindow();};

char *TMainDialog::GetString(int ID)
{
 static char szString[255];
 LoadString(hInst,ID,szString,sizeof(szString));
 return szString;
}

void TMainDialog::CreateDbf(char *DbfName, FIELD4INFO *fields, TAG4INFO *tag)
{
 DATA4 *create;
 cb.safety = 0;
 create = d4create(&cb,DbfName,fields,tag);
 d4close(create);
}


int TMainDialog::DlgMsg(int ID,int ID_MSG,UINT ICON)
{ 
 char *szTemp1 = new char[255];
 char *szTemp2 = new char[255];
 strcpy(szTemp1,GetString(ID));
 strcpy(szTemp2,GetString(ID_MSG));
 int rc = BWCCMessageBox(HWindow,szTemp1,szTemp2,ICON);
 delete szTemp1;
 delete szTemp2;
 return rc;
}

double TMainDialog::dGetField(int ID)
{
 double dValue;
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}

long   TMainDialog::lGetField(int ID)
{
 long lValue;
 IntGetValue(GetDlgItem(HWindow,ID),(LPLONG)&lValue);
 return lValue;
}


BOOL TMainDialog::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TMainDialog::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      

char *TMainDialog::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

void TMainDialog::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.closeAll();
}

void TMainDialog::CheckError(int ID)
{ 
 char *szMsg = new char[(int)150];
 strcpy(szMsg,GetString(ID_INPUTERROR));
 strcat(szMsg,GetString(ID));
 BWCCMessageBox(HWindow,szMsg,GetString(ID_ERROR),MB_ICONEXCLAMATION);
 delete szMsg;
 return;
}

void TMainDialog::EnterError(int ID)
{
 char *szMsg = new char[(int)150];
 char *szField = new char[(int)20];
 strcpy(szField,GetString(ID));
 wsprintf(szMsg,GetString(ID_BLANKFIELD),szField);
 BWCCMessageBox(HWindow,szMsg,GetString(ID_ERROR),MB_ICONEXCLAMATION);
 delete szMsg,szField;
 return;
}
