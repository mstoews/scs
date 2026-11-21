// Elements maintainance
#include "scs.h"

#include "db_types.h"
#include "db_resrc.h"
#include "db_trstr.h"


extern Code4 cb;
_CLASSDEF(TDCostf)
class TDCostf : public TDialog
{
 public:
  BOOL bItemList;
  double dStandard;
  double dLotSize;
  PTSpread List;
  char szQuery[20];
  char szCostType[4];
  char szItemNo[11];
  BOOL bStandard,bHistorical,bActual,bBudget,bRemakeSearchWindow;
  PTSCItemList    ITEM;
  PTTypes   Types;  
  PTResrc   Resrc;
  PTtrstr   CostStr;
  PTscitemm  Items;

  TDCostf (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle)
   {
   List = new TSpread(this,ID_DISTLIST);
   dStandard = 0;
  };
  virtual void SetupWindow();
  virtual void Search     (RTMessage Msg) = [ID_FIRST + ID_ITEMSEARCH];
  virtual void Find       (RTMessage Msg) = [ID_FIRST + ID_SEARCH];
  virtual void Top        (RTMessage Msg) = [ID_FIRST + ID_TOP];
  virtual void Bottom     (RTMessage Msg) = [ID_FIRST + ID_BOTTOM];
  virtual void Next       (RTMessage Msg) = [ID_FIRST + ID_FORWARD];
  virtual void Previous   (RTMessage Msg) = [ID_FIRST + ID_BACK];
  virtual void Cancel     (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void SetDetail  (RTMessage Msg) = [ID_FIRST + ID_DETAIL];
  virtual void SetResource(RTMessage Msg) = [CM_FIRST + CM_SETRESOURCE];
  int GetResource(char *szRescode,char *szCostType, int iLevel, int i,double dRequirement);
  virtual void UpdateWindow();
  BOOL  SetField   (int ID, Field4 *field);
  BOOL  SetField   (int ID, char *);
  void  SetListTable(void);
  char *GetField(int ID);
  char *GetCostType(void);
  double dGetField(int ID);
  void  ClearScreen(void);
  void  EnterError(int ID);
};

void TDCostf::SetResource(RTMessage Msg){
 Find(Msg);
}

void TGlWindow::SCSCostMaster   (RTMessage){GetApplication()->ExecDialog(new TDCostf(this,"SCS_COST"));}

int TDCostf::GetResource(char *szRescode,char *szCostType, int iLevel, int i,double dRequirement)
{
  char szOpcode[11];
  char szKpcode[11];
  char szBuffer[20];
  char szLevel[2];
  if(CostStr->seek(szRescode) == 0)
  {
   strcpy(szOpcode,CostStr->fOpcode->str());   
   while (strcmp(szRescode,szOpcode) == 0)
    {    
     strcpy(szKpcode,CostStr->fKpcode->str());   
     if(Resrc->seek(szKpcode) == 0)
	 {
	  itoa(iLevel,szLevel,10);
          List->SetData(1,i,szLevel);	  
	  List->SetData(2,i,Resrc->fCode->str());	  
	  List->SetData(3,i,Resrc->fElement->str());
	  List->SetData(4,i,Resrc->fUnit->str());
	  List->SetData(5,i,Resrc->fType->str());
	  List->SetData(6,i,CostStr->fKousei->str());
	  List->SetData(7,i,Resrc->fRate->str());
	   double dTotal = (Resrc->dRate() * CostStr->dKousei());
	   gcvt(dTotal,14,szBuffer);	 
	  List->SetData(8,i,szBuffer);
	  dStandard = dStandard + dTotal;
   	  i++;
         }
      else
      {
       long lRecno = CostStr->recno();
       double dUpperQuantity = CostStr->dQuantity();
       double dUpperKousei   = CostStr->dKousei();
      if(CostStr->seek(szKpcode) == 0)
      {
       itoa(iLevel,szLevel,10);
       double dCurrentLevel = dRequirement;
       List->SetData(1,i,szLevel);	  
       List->SetData(2,i,Items->fPcode->str());	  
       List->SetData(3,i,"MAN");
       List->SetData(5,i,Items->fPtype->str());
       i++;
       int iCurrentLevel = iLevel;
       iLevel++;
       if(dUpperKousei != dUpperQuantity)
          dRequirement = dRequirement * dUpperKousei/dUpperQuantity; 
       i = GetResource(szKpcode,szCostType,iLevel,i,dRequirement);  // Recursive call of 'GetResource'
       dRequirement = dCurrentLevel;
       iLevel = iCurrentLevel;
      }
      CostStr->go(lRecno);
    }
    CostStr->skip();
    strcpy(szOpcode,CostStr->fOpcode->str());
  }    
 }
 return i;
}

void TDCostf::SetDetail(RTMessage)
{
 Items->SetIndex("MAN");
 Items->seek(GetField(ID_CODE));
 SetField(ID_CODE, Items->fPcode);
 Items->SetIndex("FPCODE");
 SendMessage(HWindow,WM_COMMAND,CM_SETRESOURCE,NULL);
 return;
}



void TDCostf::Find (RTMessage)
{
 HourGlassOn();
 CostStr->SetIndex("FPCODE");
 Resrc->SetIndex("FCODE");

 char szCostType[4];
 dStandard = 0;
 char szItem[11];
 if(*GetField(ID_CODE) == 0)
  return;
 else
 strcpy(szItem,GetField(ID_CODE));
 if(Items->seek(szItem) == 0)
 {
  SetField(ID_CODE,    		Items->fPcode);
  SetField(ID_SNAME, 		Items->fPname);
  SetField(ID_PTYPE,		Items->fPtype);
  SetField(ID_SHOP,		Items->fShop1);
  SetField(ID_LOTSIZE,		Items->fLotsize);
  SetField(ID_UNIT,		Items->fUnit);
  SetField(ID_AVPRICE,		Items->fAvprice);
  SetField(ID_SEMICODE,		Items->fSkbn);
 }
 else
 return;
 List->ClearData(SS_ALLCOLS,SS_ALLROWS);
 List->SetBool(SSB_REDRAW,FALSE);
 int i = GetResource(szItem,szCostType,1,1,1);
 if (i<20)
  List->SetMaxRows(20);
 else
  List->SetMaxRows(i);
 List->SetBool(SSB_REDRAW,TRUE);
 FloatSetValue(GetDlgItem(HWindow,ID_STANDARD),dStandard);
 double dItemCost = dStandard/(dGetField(ID_LOTSIZE)) ;
 FloatSetValue(GetDlgItem(HWindow,ID_AVPRICE),dItemCost);
 HourGlassOff();
 return;
}


void TDCostf::SetupWindow()
{
 TDialog::SetupWindow();
 HourGlassOn();
 SetListTable();
 Types   = new TTypes(&cb);
 CostStr = new Ttrstr(&cb);
 Resrc   = new TResrc(&cb);
 Items   = new Tscitemm(&cb);
 Items->top();
 Items->Index("MAN");


 Types->top();
 while (!Types->eof())
  {
   SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)((LPSTR)Types->fCode->str()));
   Types->skip(1);
  }
  SendDlgItemMsg(ID_COSTTYPE,WM_SETTEXT,NULL,(LPARAM)((LPSTR)"STD"));
  cb.optimize = -1;
  strcpy(szQuery,"FCOSTTYPE = 'STD'");
  bItemList = TRUE;

 cb.optimize = 0;
 HourGlassOff();
}

#pragma argsused
void TDCostf::EnterError(int ID)
{
 BWCCMessageBox(HWindow,"All fields must be completed.","E R R O R",MB_ICONEXCLAMATION);
 return;
}



BOOL TDCostf::SetField(int ID, Field4 *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field->str());
  return TRUE;
}      

BOOL TDCostf::SetField(int ID, char *field)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)field);
  return TRUE;
}      



char *TDCostf::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

double TDCostf::dGetField(int ID)
{
 double dValue; 
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}

void TDCostf::UpdateWindow()
{
 SetField(ID_CODE,     		Items->fPcode);
 SetField(ID_SNAME, 		Items->fPname);
 SetField(ID_PTYPE,		Items->fPtype);
 SetField(ID_SHOP,		Items->fShop1);
 SetField(ID_LOTSIZE,		Items->fLotsize);
 SetField(ID_UNIT,		Items->fUnit);
 SetField(ID_AVPRICE,		Items->fAvprice);
 SetField(ID_SEMICODE,		Items->fSkbn);
 SetFocus(GetDlgItem(HWindow,ID_CODE));
}

void TDCostf::ClearScreen()
{
 SetField(ID_CODE, ""); 
 SetField(ID_SNAME, "");
 SetField(ID_PTYPE,"");
 SetField(ID_SHOP,"");
 SetField(ID_LOTSIZE,"");
 SetField(ID_UNIT,"");
 SetField(ID_AVPRICE,"");
 SetField(ID_SEMICODE,"");
 SetFocus(GetDlgItem(HWindow,ID_RESCODE));
}


void TDCostf::Cancel (RTMessage)
{
 ShutDownWindow();
 cb.close_all();
}



void TDCostf::Search(RTMessage)
{
 if(bItemList)
 {
  ITEM = new TSCItemList(this,"PICKLIST","FPTYPE = 'M '",Items);
  ITEM->EnableAutoCreate();
  GetApplication()->MakeWindow(ITEM);
  bItemList = FALSE;
 }

 if(!IsWindowVisible(ITEM->HWindow))
   ITEM->Show(SW_SHOW);
 else
   ITEM->Show(SW_HIDE);
}

void TDCostf::Top(RTMessage Msg)
{
 Items->Index("MAN");
 Items->top();
 Items->SetIndex("FPCODE");
 UpdateWindow();
 Find(Msg);
}

void TDCostf::Next(RTMessage Msg)
{
  Items->Index("MAN");
  Items->seek(GetField(ID_CODE));
  Items->SetIndex("FPCODE");
  Items->skip(1);
  UpdateWindow();
  Find(Msg);
  return;
}

void TDCostf::Previous(RTMessage Msg)
{
 Items->Index("MAN");
 Items->seek(GetField(ID_CODE));
 Items->SetIndex("FPCODE");
 Items->skip(-1);
 if(Items->bof())
 Items->top();
 UpdateWindow();
 Find(Msg);
}


void TDCostf::Bottom(RTMessage Msg)
{
 Items->Index("MAN");
 Items->bottom();
 Items->SetIndex("FPCODE");
 UpdateWindow();
 Find(Msg);
} 




void TDCostf::SetListTable ()
{
 RECT rc;
 SS_CELLTYPE 	CellType;
 HCURSOR  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
 ShowCursor (TRUE);

 short  d1Width,d2Width,d3Width,d4Width,d5Width,d6Width,d7Width;
 List->SetMaxCols(8);
 List->GetClientRect(&rc);

 List->SetColWidth(1,3);
 List->SetColWidth(2,7);
 List->SetColWidth(3,5);
 List->SetColWidth(4,5);
 List->SetColWidth(5,5);
 List->SetColWidth(6,10);
 List->SetColWidth(7,10);

 List->GetColWidthInPixels(1,(LPSHORT)&d1Width);
 List->GetColWidthInPixels(2,(LPSHORT)&d2Width);
 List->GetColWidthInPixels(3,(LPSHORT)&d3Width);
 List->GetColWidthInPixels(4,(LPSHORT)&d4Width);
 List->GetColWidthInPixels(5,(LPSHORT)&d5Width);
 List->GetColWidthInPixels(6,(LPSHORT)&d6Width);
 List->GetColWidthInPixels(7,(LPSHORT)&d7Width);

 short ColWidth = (rc.right - rc.left) - d1Width
 				       - d2Width
				       - d3Width
				       - d4Width
				       - d5Width
				       - d6Width
				       - d7Width;

 List->SetColWidthInPixels(8,ColWidth);
 List->SetData(1,SS_HEADER, "Lv");
 List->SetData(2,SS_HEADER, "Resource");
 List->SetData(3,SS_HEADER, "Elem");
 List->SetData(4,SS_HEADER, "Unit");
 List->SetData(5,SS_HEADER, "Type");
 List->SetData(6,SS_HEADER, "Quantity");
 List->SetData(7,SS_HEADER, "Cost");
 List->SetData(8,SS_HEADER, "Total");

 List->SetBool(SSB_MOVEACTIVEONFOCUS, FALSE);
 List->SetTypeStaticText(&CellType,SS_TEXT_LEFT);
 List->SetCellType(SS_ALLCOLS, SS_ALLROWS, &CellType);
 List->SetTypeFloat(&CellType,FS_SEPARATOR,9,4,0,9999999999.9999);
 List->SetCellType(6, SS_ALLROWS, &CellType);
 List->SetCellType(7, SS_ALLROWS, &CellType);
 List->SetCellType(8, SS_ALLROWS, &CellType);


 ShowCursor (FALSE);
 SetCursor (hCursor) ;
 }