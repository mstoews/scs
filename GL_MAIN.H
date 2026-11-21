#ifndef __GL_MAIN_H
#define __GL_MAIN_H

LPSTR GetLP(int iLength);

_CLASSDEF (TSCSDefinition)
class TSCSDefinition;

_CLASSDEF(TResourceUpdate)
class TResourceUpdate;


BOOL PrintCR(HWND HWindow, char *szReportTitle, char *szReportName);
extern HWND hStatus;
double GetCurrentPeriodn();
char *GetString(HINSTANCE,int ID);

_CLASSDEF (TGlWindow)
class TGlWindow:public TMDIFrame
{
 public:
 char Period[25];
 HWND HToolBar;
 HWND HStatusBar;
 TGlWindow(LPSTR ATitle,LPSTR MenuName);
 PTToolBar ToolBar;
 PTStatBar Status;
 PTprofile profile;       
 PTWorksheet Worksheet;
 PTAccess Access;
 PTSCSDefinition SCSDef;
 PTResourceUpdate ResUpdate;
 char  *GetCurrentPeriod ();
 void CMDialogPrint (RTMessage Msg) = [CM_FIRST + CM_DIALOGPRINT];
 void PopUpMenu (RTMessage Msg)      = [WM_FIRST + WM_POPUPMENU];
 void InitClientWindow() { ClientWnd = new TGLClient(this); }
 void CMHelp    (RTMessage Msg)     = [CM_FIRST + ID_HELP];
 virtual void CMAbout   (RTMessage Msg)     = [CM_FIRST + ID_ABOUT];
 virtual void SendStatusMsg (int ID);
 virtual void WMMenuSelect(RTMessage Msg)   = [WM_FIRST + WM_MENUSELECT];


 // General Ledger virtual functions
 virtual void GLMenu               (RTMessage Msg)   = [CM_FIRST + ID_GL];
 virtual void SCSDataResource      (RTMessage Msg)   = [CM_FIRST + IDM_RESOURCE];
 virtual void SCSDataCentres       (RTMessage Msg)   = [CM_FIRST + IDM_CENTRE];
 virtual void SCSDataElements      (RTMessage Msg)   = [CM_FIRST + IDM_ELEMENTS];
 virtual void SCSDataTypes         (RTMessage Msg)   = [CM_FIRST + IDM_TYPES];
 virtual void SCSDataAllocation    (RTMessage Msg)   = [CM_FIRST + IDM_ALLOCATION];
 virtual void SCSDataJobtime       (RTMessage Msg)   = [CM_FIRST + IDM_JOBTIME];
 virtual void SCSMaterialUsage     (RTMessage Msg)   = [CM_FIRST + IDM_USAGE];
 virtual void SCSTransStruct       (RTMessage Msg)   = [CM_FIRST + IDM_STRUCT];
 virtual void SCSTransActual       (RTMessage Msg)   = [CM_FIRST + IDM_ACTUAL];
 virtual void SCSTransAverage      (RTMessage Msg)   = [CM_FIRST + IDM_AVERAGE];
 virtual void SCSTransGL           (RTMessage Msg)   = [CM_FIRST + IDM_GLTRANSFER];
 virtual void SCSGLSpecification   (RTMessage Msg)   = [CM_FIRST + IDM_GLACCOUNTS];
 virtual void SCSRptCost           (RTMessage Msg)   = [CM_FIRST + IDM_COST];
 virtual void SCSRptElements       (RTMessage Msg)   = [CM_FIRST + IDM_ELEMENTRPT];
 virtual void SCSRptResource       (RTMessage Msg)   = [CM_FIRST + IDM_RESOURCERPT];
 virtual void SCSRptAllocate       (RTMessage Msg)   = [CM_FIRST + IDM_ALLOCATIONRPT];
 virtual void SCSRptVariance       (RTMessage Msg)   = [CM_FIRST + IDM_VARIANCERPT];
 virtual void SCSSysReindex        (RTMessage Msg)   = [CM_FIRST + IDM_REINDEX];
 virtual void SCSSysArchive        (RTMessage Msg)   = [CM_FIRST + IDM_ARCHIVE];
 virtual void SCSSysAccess         (RTMessage Msg)   = [CM_FIRST + IDM_ACCESS];
 virtual void SCSWorkSheet         (RTMessage Msg)   = [CM_FIRST + IDM_WORKSHEET];
 //virtual void SCSMaterialVariance  (RTMessage Msg)   = [CM_FIRST + IDM_MATVARIANCE];
 virtual void SCSResourceUpdate    (RTMessage Msg)   = [CM_FIRST + IDM_RESOURCEUPDATE];
 //virtual void SCSCostMaster        (RTMessage Msg)   = [CM_FIRST + IDM_COSTMASTER];
 virtual void GLCustomReport       (RTMessage Msg)   = [CM_FIRST + ID_CUSTOMREPORT];
 virtual void SCSCompanyProfile    (RTMessage Msg)   = [CM_FIRST + IDM_COMPANY];
 virtual void SCSPrintTypesRpt     (RTMessage Msg)   = [CM_FIRST + IDM_TYPESRPT];
 virtual void SCSPrintCostCentre   (RTMessage Msg)   = [CM_FIRST + IDM_COSTCENTRES];
 virtual void SCSPrintAllocation   (RTMessage Msg)   = [CM_FIRST + IDM_ALLOCATERPT];
 virtual void SCSPrintElements     (RTMessage Msg)   = [CM_FIRST + IDM_ELEMENTSRPT];
 virtual void SCSPrintLabourCosts  (RTMessage Msg)   = [CM_FIRST + IDM_LABOURCOSTRPT];
 virtual void SCSPrintGLAllocate   (RTMessage Msg)   = [CM_FIRST + IDM_GLALLOCATERPT];
 virtual void SCSPrintResStruct    (RTMessage Msg)   = [CM_FIRST + IDM_RESSTRUCTRPT];
 virtual void SCSPrintResSemi 	   (RTMessage Msg)   = [CM_FIRST + IDM_RESSEMI];
 virtual void SCSPrintGLCost       (RTMessage Msg)   = [CM_FIRST + IDM_GLCOSTS];
 virtual void SCSPrintGL           (RTMessage Msg)   = [CM_FIRST + ID_GLACCOUNT];
 virtual void SCSOpenWriter        (RTMessage Msg)   = [CM_FIRST + IDM_OPENWRITER];
 virtual void SCSSetCurrentPeriod  (RTMessage Msg)   = [CM_FIRST + IDM_SETPERIOD];
 virtual void SCSTransSCS          (RTMessage Msg)   = [CM_FIRST + IDM_SCSTOPICS];
 virtual void SCSTransferGL        (RTMessage Msg)   = [CM_FIRST + IDM_TRANSFERGL];
 virtual void SCSTransferTOPICS    (RTMessage Msg)   = [CM_FIRST + IDM_TRANSFERTOPICS];

// GLWindows Windows class registration - this is different than a C++ class.
 virtual LPSTR GetClassName(){ return "SCSWindow"; }
 virtual void GetWindowClass(WNDCLASS& wc);
 virtual void PrtReport(char *szReportName,char *szReportTitle);
 virtual void WMSize        (RTMessage Msg)   = [WM_FIRST + WM_SIZE];
 HWND GLCreateStatusBar();
 HWND GlCreateToolBar(HWND HWindow);
 void SetupWindow();
};
#endif