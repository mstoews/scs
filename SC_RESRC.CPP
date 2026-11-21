/*** Resource Master file - SCS Standard Include File***/
#include "scs.h"

extern Code4 cb;
extern char   CurDir[MAXPATH];
/*** Data Types Usage ***/
_CLASSDEF(TDJobtime)
class TDJobtime : public TDialog
{
 private:
  PTSpread List;
 public:
  TDJobtime (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle)
  {
   List = new TSpread(this,ID_DISTLIST);
  };
  virtual void SetupWindow();
};

void TGlWindow::SCSDataJobtime      (RTMessage){GetApplication()->ExecDialog(new TDJobtime(this,"DJOBTIME"));}

void TDJobtime::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 RECT rc;
 SS_CELLTYPE 	CellType;
 List->SetMaxCols(8);
 List->SetColWidth(2,6);
 List->SetColWidth(3,6);
 List->SetColWidth(4,8);
 List->SetColWidth(5,7);
 List->SetColWidth(6,7);
 List->SetColWidth(7,7);
 List->SetColWidth(8,7);

 List->GetClientRect(&rc);

 short dWidth[7],Width,ColWidth;
 Width = 0;
 for(int i = 2; i < 9; i++){
  List->GetColWidthInPixels(i,(LPSHORT)&dWidth[i]);
  Width = Width + dWidth[i];
 }
 ColWidth = (rc.right - rc.left) - Width;// - GetSystemMetrics(SM_CXHSCROLL);
 List->SetColWidthInPixels(1,ColWidth);
 List->SetData(1,SS_HEADER, GetString(GetApplication()->hInstance,ID_ITEMCODE));
 List->SetData(2,SS_HEADER, GetString(GetApplication()->hInstance,ID_LOTNUMBER));
 List->SetData(3,SS_HEADER, GetString(GetApplication()->hInstance,ID_SECTION));
 List->SetData(4,SS_HEADER, GetString(GetApplication()->hInstance,ID_DATE));
 List->SetData(5,SS_HEADER, GetString(GetApplication()->hInstance,ID_STDHRS));
 List->SetData(6,SS_HEADER, GetString(GetApplication()->hInstance,ID_ACTHOURS)),
 List->SetData(7,SS_HEADER, GetString(GetApplication()->hInstance,ID_TOTALHRS)),
 List->SetData(8,SS_HEADER, GetString(GetApplication()->hInstance,ID_OVERTIMEHRS)),
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 HourGlassOff();
}

/*** System Archive***/
_CLASSDEF(TSysArchive)
class TSysArchive : public TDialog
{
 public:
  TSysArchive (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
};
void TGlWindow::SCSSysArchive     (RTMessage)
{
 if(chdir("a:\\"))
 {
  chdir(CurDir);
  BWCCMessageBox(HWindow,"Make sure that a diskette is placed in drive A.","Transfer",MB_ICONSTOP);
  return;
 }
 else
 {
  cb.closeAll();
  if(BWCCMessageBox(HWindow,GetString(GetApplication()->hInstance,IDM_ARCHIVE),"Backup Database Files",MB_ICONQUESTION | MB_YESNO) == IDYES)
   WinExec("backup.pif",SW_SHOW);
 }
}

