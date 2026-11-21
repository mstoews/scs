// Standard Cost System  - (C) Copyright 1992 by Tegara Corporation

#include "scs.h"
#include "crpe.h"
#include "db_access.h"
#include "gl_strg.h"
#include "db_topic.h"
#include "db_mrslt.h"
#include "gl_dlg.h"
#include "db_index.h"
#include <direct.h>
#include <lzexpand.h>

#define IDD_STATUSBAR_MENUHELP 2
#define STATUSBAR_HEIGHT       26
#define GWW_STATUSBAR          0
#define  BUTTON_WIDTH          45



// Define global variables for Codebase.
extern Code4 cb;
extern char   CurDir[MAXPATH];

class TTopicsCopy : public TMainDialog
{
 public:
 TTopicsCopy(PTWindowsObject AParent,LPSTR ATitle):TMainDialog(AParent,ATitle){};
 virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
 virtual void Cancel (RTMessage Msg) = [ID_FIRST + IDCANCEL]{ShutDownWindow();};
 virtual void Reindex(RTMessage Msg) = [ID_FIRST + IDM_REINDEX]
	      {SendMessage(Parent->HWindow,WM_COMMAND,IDM_REINDEX,NULL);};
 virtual void Scope  (RTMessage Msg) = [ID_FIRST + 105];
 virtual void SetupWindow();
 BOOL Copy(char *szSrc);
};

class TGLCopy : public TTopicsCopy
{
public:
 TGLCopy(PTWindowsObject AParent,LPSTR ATitle):TTopicsCopy(AParent,ATitle){};
 virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK]{};
};

void TGlWindow::SCSTransferTOPICS (RTMessage)
{
 GetApplication()->ExecDialog(new TTopicsCopy(this,"TOPICSCOPY"));
}


void TTopicsCopy::SetupWindow()
{
 TDialog::SetupWindow();
 SetField(102,CurDir);
 int disk,save;
 char szDrive[4];
 save = getdisk();
 for(disk = 0;disk < 3;++disk)
  {
   setdisk(disk);
   if(disk == getdisk())
   {
    wsprintf(szDrive,"%c:", disk + 'A');
    SendDlgItemMsg(101,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)szDrive);
   }
  }
 SendDlgItemMsg(101,CB_SETCURSEL,(WPARAM)0,NULL);
 setdisk(save);
}


BOOL TTopicsCopy::Copy(char *szSrc)
{
  SetField(104,szSrc);
  char *szDrive   = new char[5];
  char *szSource  = new char[MAXPATH];
  char *szTarget  = new char[MAXPATH];
  DWORD dwIndex = SendDlgItemMsg(101,CB_GETCURSEL,NULL,NULL);
  SendDlgItemMsg(101,CB_GETLBTEXT,(WPARAM)dwIndex,(LPARAM)(LPSTR)szDrive);
  strcat(szDrive,"\\");
  if(chdir(szDrive))
   {
    chdir(CurDir);
    char *szMsg = new char[50];
    strcpy(szMsg,"Make sure that a diskette is place in drive - ");
    strcat(szMsg,szDrive);
    strcat(szMsg,". ");
    BWCCMessageBox(HWindow,szMsg,"Transfer",MB_ICONSTOP);
    delete szMsg;
    return FALSE;
   }

  strcpy(szSource,szDrive);
  strcat(szSource,szSrc);
  strcpy(szTarget,GetField(102));
  strcat(szTarget,"\\");
  strcat(szTarget,szSrc);
  OFSTRUCT ofStrSrc;
  OFSTRUCT ofStrDest;
  HFILE hfSrcFile, hfDstFile;
  hfSrcFile = LZOpenFile(szSource, &ofStrSrc, OF_READ);
  hfDstFile = LZOpenFile(szTarget, &ofStrDest, OF_CREATE);
  LZCopy(hfSrcFile, hfDstFile);
  LZClose(hfSrcFile);
  LZClose(hfDstFile);
  delete szSource;
  delete szTarget;
  delete szDrive;
  return TRUE;
}

void TTopicsCopy::Ok(RTMessage)
{
  BOOL AllCopied;
  char *szDbf = new char[12]; 
  PTIndex Index = new TIndex(&cb);
  Relate4set *TopicsDbf = new Relate4set(Index);
  TopicsDbf->querySet("FTYPE = 'T  '");
  for(int qc=TopicsDbf->top();qc!=r4eof;qc=TopicsDbf->skip(1L))
    {
     strcpy(szDbf,Index->fDbf->str());
     StrTrim(szDbf);
     strcat(szDbf,".DBF");
     AllCopied = Copy(szDbf);
     if(!AllCopied)
     break;
    }
  TopicsDbf->unlock();
  TopicsDbf->free();
  cb.closeAll();
  delete szDbf;
  if(AllCopied)
  {
   SetField(104,"Transfer Completed");
   SendMessage(Parent->HWindow,WM_COMMAND,IDM_REINDEX,NULL);
  }
  else
   SetField(104,"Transfer Failed");

  return;
}

void TTopicsCopy::Scope(RTMessage)
{
  SetFocus(HWindow);
  char *szMsg = new char[100];
  PTprofile profile = new Tprofile(&cb);
  profile->top();

  Date4 StartDate;
  Date4 EndDate;
  switch((int)GetCurrentPeriodn())
    {
    case 1: StartDate.assign(profile->period1->str());EndDate.assign(profile->period1e->str()); break;
    case 2: StartDate.assign(profile->period2->str());EndDate.assign(profile->period2e->str()); break;
    case 3: StartDate.assign(profile->period3->str());EndDate.assign(profile->period3e->str()); break;
    case 4: StartDate.assign(profile->period4->str());EndDate.assign(profile->period4e->str()); break;
    case 5: StartDate.assign(profile->period5->str());EndDate.assign(profile->period5e->str()); break;
    case 6: StartDate.assign(profile->period6->str());EndDate.assign(profile->period6e->str()); break;
    case 7: StartDate.assign(profile->period7->str());EndDate.assign(profile->period7e->str()); break;
    case 8: StartDate.assign(profile->period8->str());EndDate.assign(profile->period8e->str()); break;
    case 9: StartDate.assign(profile->period9->str());EndDate.assign(profile->period9e->str()); break;
    case 10:StartDate.assign(profile->period10->str());EndDate.assign(profile->period10e->str()); break;
    case 11:StartDate.assign(profile->period11->str());EndDate.assign(profile->period11e->str()); break;
    case 12:StartDate.assign(profile->period12->str());EndDate.assign(profile->period12e->str()); break;
    default:StartDate.assign(profile->period1->str());EndDate.assign(profile->period1e->str()); break;
   }
 StartDate.format("MM/DD/YY");
 EndDate.format("MM/DD/YY");
 //
 SetField(104,StartDate.str());
 PTTopicsThrm  TopicsThrm  = new TTopicsThrm(&cb);
 PTTopicsJiss  TopicsJiss  = new TTopicsJiss(&cb);
 PTTopicsTmast TopicsTmast = new TTopicsTmast(&cb);
 PTManResult   ManResults  = new TManResult(&cb);

 TopicsThrm->top();
 while (!TopicsThrm->eof())
  {
   Date4 dDate(TopicsThrm->fDate->str());
   if(dDate < StartDate || dDate > EndDate)
   TopicsThrm->deleteRec();
   TopicsThrm->skip();
  }
  TopicsThrm->pack();
 TopicsTmast->top();
 while (!TopicsTmast->eof())
  {
   Date4  dDate(TopicsTmast->fJdate->str());
   if(dDate < StartDate ||  dDate > EndDate)
    {
     TopicsTmast->deleteRec();
    }
   TopicsTmast->skip();
  }
  TopicsTmast->pack();
 ManResults->top();
 while (!ManResults->eof())
  {
   Date4 dDate(ManResults->fJdate->str());
   if( dDate < StartDate ||  dDate > EndDate)
   ManResults->deleteRec();
   ManResults->skip();
  }
 ManResults->pack();
 TopicsJiss->top();
 while (!TopicsJiss->eof())
  {
   Date4 dDate(TopicsJiss->fDate->str());
   if( dDate < StartDate ||  dDate > EndDate)
   TopicsJiss->deleteRec();
   TopicsJiss->skip();
  }
 TopicsJiss->pack();
 cb.closeAll();
 delete szMsg;
 return;
}
