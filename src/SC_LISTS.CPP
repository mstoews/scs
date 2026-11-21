/*
   General Ledger  - (C) Copyright 1992 by Tegara Corporation
   Modify, add and delete accounts functions.
*/
#include "scs.h"


 
TGLAcctList::~TGLAcctList()
{
 delete ListArray;
}


void TGLAcctList::HandleListBox (RTMessage Msg)
{
 int nIndx;
 int nRecord;
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 RArrayIterator ListIterator =(RArrayIterator)(ListArray->initIterator());
   while (int (ListIterator) != 0)
    {
      RObject AnObject = ListIterator++;
      nIndx = ((PTListItem)(&AnObject))->index;
      if( nIndx == Cell->Row)
       nRecord = ((PTListItem)(&AnObject))->rec;
    }    
  gl->go(nRecord);
  SendMessage(GetDlgItem(Parent->HWindow,ID_ACCOUNTS) ,WM_SETTEXT,NULL,(LONG)(LPSTR)gl->accounts->str());
  if(lstrcmp(gl->dept->str(),"    ") != 0)
  {
   SendMessage(GetDlgItem(Parent->HWindow,ID_DEPART)  ,WM_SETTEXT,NULL,(LONG)(LPSTR)gl->dept->str());
  }
  else
  SendMessage(GetDlgItem(Parent->HWindow,ID_DEPART)   ,WM_SETTEXT,NULL,(LONG)(LPSTR)"");
  SendMessage(GetDlgItem(Parent->HWindow,ID_DESCRIPT) ,WM_SETTEXT,NULL,(LONG)(LPSTR)gl->descript->str());
  SendMessage(GetDlgItem(Parent->HWindow,ID_CURRBAL)  ,WM_SETTEXT,NULL,(LPARAM)(LPSTR)gl->curr_bal->str());
  //Show(SW_HIDE);
  SetFocus(Parent->HWindow);
}


void TGLAcctList::SetupWindow ()
{
 TDialog::SetupWindow();
 RECT rc;
 SS_CELLTYPE 	CellType;
 Relate4set     *rQuery;
 HourGlassOn();
 HWND SHWindow = GetDlgItem(HWindow,ID_DISTLIST);
 GetClientRect(HWindow,&rc);
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));
 MoveWindow(SHWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
 short d1Width,d2Width;
 SSSetColWidth(SHWindow,1,7);
 SSSetColWidth(SHWindow,2,5);
 SSGetColWidthInPixels(SHWindow,1,(LPSHORT)&d1Width);
 SSGetColWidthInPixels(SHWindow,2,(LPSHORT)&d2Width);
 short ColWidth = (rc.right - rc.left) - d1Width - d2Width - GetSystemMetrics(SM_CXHSCROLL);
 SSSetColWidthInPixels(SHWindow,3,ColWidth);


 gl->bottom();
 SSSetMaxCols(SHWindow, 3);
 SSSetMaxRows(SHWindow,gl->recCount());
 SSSetData(SHWindow,1,SS_HEADER, "Account");
 SSSetData(SHWindow,2,SS_HEADER, "Dept");
 SSSetData(SHWindow,3,SS_HEADER, "Description");
 SSSetBool(SHWindow,SSB_MOVEACTIVEONFOCUS, FALSE);
 SSSetTypeStaticText(SHWindow,&CellType,SS_TEXT_LEFT);
 SSSetCellType(SHWindow,SS_ALLCOLS, SS_ALLROWS, &CellType);

 rQuery = new Relate4set(gl);
 rQuery->querySet(szFilterString);
 long i;
 i=1;

 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   SSSetData(SHWindow,1,i,gl->accounts->str());
   SSSetData(SHWindow,2,i,gl->dept->str());
   SSSetData(SHWindow,3,i,gl->descript->str());
   DWORD nRec_no = gl->recNo();
   ListArray->add(* (new TListItem((int)i,(int)nRec_no)));
   i++;
  }
 rQuery->unlock();
 rQuery->free(0);
 SSSetMaxRows(SHWindow,i-1);
 HourGlassOff();
}


void TSCItemList::SetupWindow ()
{
 TDialog::SetupWindow();
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);

 GetClientRect(HWindow,&rc);
 InflateRect(&rc, GetSystemMetrics(SM_CXBORDER),GetSystemMetrics(SM_CYBORDER));
 MoveWindow(List->HWindow,rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
 short d1Width,d2Width;

 List->SetColWidth(1,7);
 List->SetColWidth(2,7);
 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 short ColWidth = (rc.right - rc.left) - d1Width - d2Width;// - GetSystemMetrics(SM_CXHSCROLL);
 List->SetColWidthInPixels(3,ColWidth);
 List->SetMaxCols( 3);
 List->SetMaxRows(scitemm->recCount());
 List->SetData(1,SS_HEADER, "Item No.");
 List->SetData(2,SS_HEADER, "Semi-Code");
 List->SetData(3,SS_HEADER, "Description");
 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 Relate4set *rQuery;
 scitemm->SetIndex("fpcode");
 long i = 1;
 rQuery = new Relate4set(scitemm);
 rQuery->querySet(szQuery);
 for(int qc=rQuery->top();qc!=r4eof;qc=rQuery->skip(1L))
  {
   List->SetData(1,i,scitemm->fPcode->str());
   List->SetData(2,i,scitemm->fSkbn->str());
   List->SetData(3,i,scitemm->fFulln->str());
   i++;
  }

 rQuery->unlock();
 rQuery->free(0);
 List->SetMaxRows(i);
 SetCaption("TOPICS Item Number Search");
 ShowCursor (FALSE);
 SetCursor (hCursor) ;
}

void TSCItemList::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem);
 if(scitemm->seek(szItem) == 0)
  {
    SendMessage(GetDlgItem(Parent->HWindow,ID_ITEMNO)    ,WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fPcode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_CODE)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fPcode->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_FSKBN)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fSkbn->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_FULLN)     ,WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fFulln->str());
    SendMessage(GetDlgItem(Parent->HWindow,ID_TYPE)      ,WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fPtype->str());
    SendMessage(Parent->HWindow,WM_COMMAND,CM_DETAIL,NULL);
    SendMessage(Parent->HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
  }
  else
  BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}

void TSCItemList2::HandleListBox (RTMessage Msg)
{
 char szItem[11];
 LPSS_CELLCOORD Cell = (LPSS_CELLCOORD)Msg.LParam;
 List->GetData(1,Cell->Row,szItem); 
 if(scitemm->seek(szItem) == 0)
  {   
   SendMessage(GetDlgItem(Parent->HWindow,ID_ITEMEND),WM_SETTEXT,NULL,(LONG)(LPSTR)scitemm->fPcode->str());
  }
  else
 BWCCMessageBox(HWindow,"Item number could not be found.","E R R O R",MB_ICONEXCLAMATION);
 Show(SW_HIDE);
 SetFocus(Parent->HWindow);
}
