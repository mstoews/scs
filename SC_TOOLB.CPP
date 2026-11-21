#include "scs.h"
#define BUTTON_WIDTH 45


void TToolBar::Resource    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_RESOURCE,NULL);}
void TToolBar::Elements    (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_ELEMENTS,NULL);}
void TToolBar::Centres     (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_CENTRE,NULL);}
void TToolBar::Allocation  (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_GLACCOUNTS,NULL);}
void TToolBar::Types       (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_TYPES,NULL);}
void TToolBar::ResourceUpdate       (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_COSTMASTER,NULL);}
void TToolBar::ViewCosts   (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,IDM_RESOURCEUPDATE,NULL);}
void TToolBar::Quit        (RTMessage){SendMessage(Parent->HWindow,WM_COMMAND,24340,NULL);}

void TToolBar::WMInitDialog (RTMessage)
{
 RECT rect;
 GetClientRect(HWindow,&rect);

 SendDlgItemMsg(ID_RESOURCE,        SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"RESOURCE");
 SendDlgItemMsg(ID_ELEMENTS,        SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ELEMENT");
 SendDlgItemMsg(ID_CENTRES,         SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"CENTRE");
 SendDlgItemMsg(ID_ALLOCATION,      SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"ALLOCATE");
 SendDlgItemMsg(ID_TYPES,           SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"TYPES");
// SendDlgItemMsg(IDM_RESOURCEUPDATE ,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"STRUCT");
 SendDlgItemMsg(IDM_COSTMASTER     ,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"STRUCT");
 SendDlgItemMsg(ID_EXIT            ,SBM_SETPICT,SUPERBTN_PICT_BITMAP,(long)(LPSTR)"QUIT");

 MoveWindow(GetDlgItem(HWindow,ID_RESOURCE)    ,rect.left+10,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_ELEMENTS)    ,rect.left+10+(BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_CENTRES)     ,rect.left+10+(2*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_ALLOCATION)  ,rect.left+10+(3*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_TYPES)       ,rect.left+10+(4*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 //MoveWindow(GetDlgItem(HWindow,IDM_RESOURCEUPDATE)       ,rect.left+10+(5*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,IDM_COSTMASTER) ,rect.left+10+(5*BUTTON_WIDTH) ,rect.top+4,40,30,0);
 MoveWindow(GetDlgItem(HWindow,ID_EXIT)        ,rect.left+10+(6*BUTTON_WIDTH) ,rect.top+4,40,30,0);

}

void TToolBar::WMPaint(TMessage&)
{
  PAINTSTRUCT PS;
  HDC PaintDC;
  PaintDC = BeginPaint(HWindow, &PS);
  HDC memDC;
  RECT clientRect;
  HANDLE oldBmp;
  HBITMAP bmpButtonLine;
  GetClientRect(HWindow, &clientRect);
  memDC = CreateCompatibleDC(PaintDC);
  bmpButtonLine = LoadBitmap(GetApplication()->hInstance, "TOOLBAR");
  oldBmp = SelectObject(memDC, bmpButtonLine);
  StretchBlt(PaintDC,0,0, clientRect.right - clientRect.left,38,memDC,0,0,10,38, SRCCOPY);
  SelectObject(memDC, oldBmp);
  DeleteDC(memDC);
  EndPaint(HWindow, &PS);
}
