// Access dialog box
// General Accounting Module Password Protection...
#include "scs.h"
#include "gl_dlg.h"
#include "db_acces.h"



extern HMENU HSCSMenuA,HGLExMenu, HARMenu, HAPMenu;


_CLASSDEF (TGLAdministration)
class TGLAdministration : public TMainDialog
{
 public:
 PTAccess access;
 TGLAdministration(PTWindowsObject AParent, LPSTR ATitle,PTAccess ac) : TMainDialog(AParent,ATitle){access = ac;}
 virtual void Add      (RTMessage Msg) = [ID_FIRST + ID_ADD];
 virtual void Modify   (RTMessage Msg) = [ID_FIRST + ID_MODIFY];
 virtual void Cancel   (RTMessage Msg) = [ID_FIRST + IDCANCEL]{ShutDownWindow();};
 virtual void Delete   (RTMessage Msg) = [ID_FIRST + ID_DELETE];
 virtual void Next     (RTMessage Msg) = [ID_FIRST + ID_NEXT];
 virtual void Previous (RTMessage Msg) = [ID_FIRST + ID_PREVIOUS];
 virtual void Search   (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
 virtual void Update   (RTMessage Msg) = [CM_FIRST + ID_UPDATEWINDOW];
 virtual void Clear    (RTMessage Msg) = [CM_FIRST + ID_CLEAR];
 virtual void SetupWindow();
};

void TGLAdministration::Update(RTMessage)
{
 SetField(ID_USER,access->fUsername);
 SetField(ID_DESCRIPT,access->fDescript);
 SetField(ID_PASSWORD,access->fPassword);
 SetField(ID_ACCESSLEVEL,access->fLevel);
}

void TGLAdministration::Search(RTMessage)
{
 if(access->seek(GetField(ID_USER)) == 0)
    SendMessage(HWindow,WM_COMMAND,ID_UPDATEWINDOW,NULL);
 else
 BWCCMessageBox(HWindow,"User not found!","Access",MB_OK); 
}

void TGLAdministration::Clear(RTMessage)
{
 SetField(ID_USER,"");
 SetField(ID_DESCRIPT,"");
 SetField(ID_DATE,"");
 SetField(ID_PASSWORD,"");
 SetField(ID_ACCESSLEVEL,"");
}

void TGLAdministration::Next(RTMessage)
{
 if(*GetField(ID_USER) !=0)
  if(access->seek(GetField(ID_USER)) == 0 && !access->eof())
    access->skip();
 else
 access->top();  
 SendMessage(HWindow,WM_COMMAND,ID_UPDATEWINDOW,NULL);
}

void TGLAdministration::Previous(RTMessage)
{
 if(*GetField(ID_USER) !=0)
  {
  if(access->seek(GetField(ID_USER)) == 0 && !access->bof())
   {
    access->skip(-1L);  
   }
 }
 access->top();
 SendMessage(HWindow,WM_COMMAND,ID_UPDATEWINDOW,NULL);
}



void TGlWindow::SCSSysAccess      (RTMessage)
 {
  PTAccess access;
  access = new TAccess(&cb);
  GetApplication()->ExecDialog(new TGLAdministration(this,"GL_ADMINISTRATION",access));
  delete access;
 }

void TGLAdministration::Delete(RTMessage)
{
 if(access->seek(GetField(ID_USER)) ==0)
  if(BWCCMessageBox(HWindow,"Are you sure you want to delete this user.","Access",MB_ICONQUESTION|MB_YESNO) == IDYES)
  {
   access->deleteRec();
   access->pack();
  }
}

void TGLAdministration::Add(RTMessage Msg)
{
 if(access->seek(GetField(ID_USER)) !=0)
  {
   access->appendStart();
   access->fUsername->assign(GetField(ID_USER));
   access->fDescript->assign(GetField(ID_DESCRIPT));
   access->fDate->assign(GetField(ID_DATE));
   access->fPassword->assign(GetField(ID_PASSWORD));
   access->fLevel->assign(GetField(ID_ACCESSLEVEL));
   access->append();
  }
 else
 Modify(Msg);
}

void TGLAdministration::Modify(RTMessage)
{
 if(access->seek(GetField(ID_USER))==0)
  if(BWCCMessageBox(HWindow,"Are you sure you want to modify this user.","Access",MB_YESNO |MB_ICONQUESTION) == IDYES)
  {
   access->fUsername->assign(GetField(ID_USER));
   access->fDescript->assign(GetField(ID_DESCRIPT));
   access->fDate->assign(GetField(ID_DATE));
   access->fPassword->assign(GetField(ID_PASSWORD));
   access->fLevel->assign(GetField(ID_ACCESSLEVEL));
   access->flush();
  }
 else
 BWCCMessageBox(HWindow,"Unable to locate the user.","Access",MB_ICONEXCLAMATION | MB_OK);
}


void TGLAdministration::SetupWindow()
{
 TDialog::SetupWindow();
 access->SetIndex("username");
 access->top();
 while(!access->eof())
 {
  if(strcmp(access->fUsername->str(),"") ==0)
  access->deleteRec();
  access->skip();
 }
 access->pack();
 access->top();
 SendMessage(HWindow,WM_COMMAND,ID_UPDATEWINDOW,NULL);
}

void TGLAccess::SetupWindow()
 {
  ACCEPT = FALSE;
  access = new TAccess(&cb);
  access->top();
  access->SetIndex("username");
  while (!access->eof())
   {
    SendDlgItemMsg(ID_USER,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)access->fUsername->str());
    access->skip(1);
   } 
 }

 void TGLAccess::Ok (RTMessage)
 {
  char szPassword[11];
  DWORD len =  SendDlgItemMsg(ID_USER,WM_GETTEXTLENGTH,NULL,NULL);
  char *szUserID = new char[(int)len + 1];
  SendDlgItemMsg(ID_USER,WM_GETTEXT,sizeof(szUserID),(LPARAM)(LONG)szUserID);
  len =  SendDlgItemMsg(ID_PASSWORD,WM_GETTEXTLENGTH,NULL,NULL);
  SendDlgItemMsg(ID_PASSWORD,WM_GETTEXT,sizeof(szPassword),(LPARAM)(LONG)szPassword);
  if (access->seek(szUserID) == 0)
  {
   access->fPassword->trim();
   access->fPassword->setLen(sizeof(szPassword));
   if (strcmp(szPassword,access->fPassword->str()) == 0)
     {
      ACCEPT = TRUE;
      SendDlgItemMsg(ID_ACCESSLEVEL,WM_SETTEXT,NULL,(LPARAM)(LONG)access->fLevel->str());
      strcpy(szAccessLevel,access->fLevel->str());
      strcpy(szUser,access->fUsername->str());
      if(strcmp(szAccessLevel,"B") == 0)
      {
       HSCSMenuA = LoadMenu(GetApplication()->hInstance,"scsB");
       HMENU HPopUpMenu = GetSubMenu(HSCSMenuA,0); // Get the child menu position.
       SendMessage(Parent->HWindow,WM_MDISETMENU,0,MAKELONG(HSCSMenuA,HPopUpMenu));
       DrawMenuBar(GetApplication()->MainWindow->HWindow);
      }

     }
      else
     {
      BWCCMessageBox(HWindow,"Incorrect password.","E R R O R",MB_ICONEXCLAMATION);
      ACCEPT = FALSE;
     }
  }
  else
  BWCCMessageBox(HWindow,"User not found.","E R R O R",MB_ICONEXCLAMATION);
  if (!ACCEPT)
   return;
  else
  {
   access->close();
   ShutDownWindow();
  }
 } 

 void TGLAccess::Cancel(RTMessage)
 {
  access->close();
  exit(1);
  ShutDownWindow();
 }

void TGLAccess::Add (RTMessage)
{
  char szPassword[11];
  char szUserID[11];
  SendDlgItemMsg(ID_USER,WM_GETTEXT,sizeof(szUserID),(LPARAM)(LONG)szUserID);
  SendDlgItemMsg(ID_PASSWORD,WM_GETTEXT,sizeof(szPassword),(LPARAM)(LONG)szPassword);
  if (access->seek(szUserID) == 0){
    access->fUsername->trim();
    access->fUsername->setLen(sizeof(szUserID));
     if(strcmp("Admin",access->fUsername->str()) !=0){
      BWCCMessageBox(HWindow,"You must log on as 'Admin' to do administration.","E R R O R",MB_ICONSTOP);   
      return;
     }
    access->fPassword->trim();
    access->fPassword->setLen(sizeof(szPassword));
     if (strcmp(szPassword,access->fPassword->str()) == 0){  
      GetApplication()->ExecDialog(new TGLAdministration(this,"GL_ADMINISTRATION",access));
      return;
     }
     else{
      BWCCMessageBox(HWindow,"Incorrect password.","E R R O R",MB_ICONEXCLAMATION);
      ACCEPT = FALSE;
     }
  }
  else
   BWCCMessageBox(HWindow,"User not found.","E R R O R",MB_ICONEXCLAMATION);
  return;
}  