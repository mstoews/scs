_CLASSDEF(TAPCancelBatch)
class TAPCancelBatch:public TDialog
{
public:
 HWND SHWindow;
 SS_CELLTYPE 	CellType;
 LOGFONT 	lfFont;
 HFONT   	hFont;
 RECT Rect;

 TAPCancelBatch(PTWindowsObject AParent, LPSTR AName): TDialog(AParent,AName){}
 virtual void WMInitDialog (RTMessage Msg ) = [WM_FIRST + WM_INITDIALOG];
};
