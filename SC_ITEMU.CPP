// Standard Cost System  - (C) Copyright 1992 by Tegara Corporation
#include "scs.h"
#include "db_resrc.h"
#include "db_trstr.h"
#include "db_elem.h"
#include "db_types.h"
#include "db_reslt.h"
#include "db_centr.h"
#include "db_koute.h"
#include "sc_itemu.h"


void TItemUpdate::MultiLevel (RTMessage)
{
 if(bMultiLevel)
 bMultiLevel = FALSE;
 else
 bMultiLevel = TRUE;
}

void TItemUpdate::CostType (RTMessage Msg)
{
 DWORD Indx;
 char szType[4];
 if(Msg.LP.Hi == CBN_SELCHANGE)
 {
  Indx = SendDlgItemMsg(ID_COSTTYPE,CB_GETCURSEL,0,0);
  if(Indx != CB_ERR)
    SendDlgItemMsg(ID_COSTTYPE,CB_GETLBTEXT, (WPARAM)Indx, (LPARAM) ((LPSTR) szType));
  Types->seek(szType);
  char szMainWindowCaption[100];
  strcpy(szMainWindowCaption,"Cost Item Update - ");
  strcat(szMainWindowCaption,Types->fShortName->str());
  SetCaption(szMainWindowCaption);
 }
}


void TItemUpdate::SetupWindow ()
 {
   TDialog::SetupWindow();
   HourGlassOn();
   cb.optimize = -1 ;
   Items    = new Tscitemm  (&cb);
   Resrc    = new TResrc    (&cb);
   CostStr  = new Ttrstr    (&cb);  // Cost Structure Master
   Types    = new TTypes    (&cb);
   Proc     = new TProc     (&cb);
   Proc->SetIndex("fkoutei");


   bSearch = TRUE;
   bResource = TRUE;

   Items->SetIndex("FPCODE");

   Types->top();
   while (!Types->eof())
   {
    SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)((LPSTR)Types->fCode->str()));
    Types->skip(1);
   }
   SendDlgItemMsg(ID_COSTTYPE,WM_SETTEXT,NULL,(LPARAM)((LPSTR)"STD"));

   char szMainWindowCaption[100];
   char szCompositionCaption[100];
   strcpy(szMainWindowCaption,"Production Cost Update - ");
   strcat(szMainWindowCaption,"Actual");
   SetCaption(szMainWindowCaption);

   bMultiLevel = TRUE;
   SendDlgItemMsg(ID_LEVEL,BM_SETCHECK,FALSE,NULL);

   HourGlassOff();
 }




char *TItemUpdate::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}

double TItemUpdate::dGetField(int ID)
{
 double dValue; 
 FloatGetValue(GetDlgItem(HWindow,ID),(LPDOUBLE)&dValue);
 return dValue;
}

/*** Recursive Get Resource Function ***/
/***
    This is a recursive routine that moves down the levels
    of the structure master when it encounters a manufactured
    item. It is recursive because it calls itself as many times as
    there are levels in the the tree.
*/


int TItemUpdate::GetResource(char *szRescode,char *szCostType, int iLevel, int i,double dRequirement)
{
  char szOpcode[15];
  char szKpcode[15];
  char szKpcodeSeek[15];
  char szKpItemCode[15];

  if(CostStr->seek(szRescode) == 0)
  {
   
   strcpy(szOpcode,szCostType);              // create a comparison key for the cost structure file.
   strcat(szOpcode,CostStr->fOpcode->str());   

   while (strcmp(szRescode,szOpcode) == 0)     // loop through the database.
     {      
      strcpy(szKpcode,CostStr->fKpcode->str());
      strcpy(szKpcodeSeek,CostStr->fKpcode->str());
      strcat(szKpcodeSeek,szCostType);
       if(Resrc->seek(szKpcodeSeek) == 0)
	 {
	  SetResourceValues(iLevel,i,Resrc,dRequirement);
	  i++;
	 }
       else                                      // Set an upper level manfactured items costs.
       {	 
	long lRecno = CostStr->recNo();
	double dUpperQuantity = CostStr->dQuantity();
	double dUpperKousei = CostStr->dKousei();	
	strcpy(szKpItemCode,szCostType);
        strcat(szKpItemCode,szKpcode);
	if(CostStr->seek(szKpItemCode) == 0)
	 {  
	  int iCurrentLevel = iLevel;
	  double CurrentRequirement  = dRequirement;
	  iLevel++;
	  i++;
	  if(dUpperKousei != dUpperQuantity)
	    dRequirement = dRequirement * dUpperKousei/dUpperQuantity;
	  i = GetResource(szKpItemCode,szCostType,iLevel,i,dRequirement);
          dRequirement = CurrentRequirement;
	  iLevel = iCurrentLevel;       

         }
	CostStr->go(lRecno);
       }
       CostStr->skip();
       strcpy(szOpcode,szCostType);
       strcat(szOpcode,CostStr->fOpcode->str());
    }
  }    
 return i;
}


void TItemUpdate::SetResourceValues(int iLevel,int i,PTResrc Resrc,double dRequirement)
       {
	  SendDlgItemMsg(ID_ITEMNO,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Resrc->fCode->str());
          char szType[3];
	  double dSum = 0;
	  double dRatio = (dRequirement/CostStr->dQuantity());
	  dSum = (Resrc->dRate() * dRatio * CostStr->dKousei());
          strcpy(szType,Resrc->fType->str());
	  if(strcmp(szType,"R ") == 0 || strcmp(szType,"RI") == 0 || strcmp(szType,"W ") == 0 || strcmp(szType,"WI") == 0)
           {
	    dMaterial = dMaterial + dSum;
           }
	  if(strcmp(szType,"D ") == 0)
	   dOH = dOH + dSum;
	  if(strcmp(szType,"O ") == 0)
	   dIndirect = dIndirect + dSum;
	  if(strcmp(szType,"L ") == 0)
           dLabour = dLabour + dSum;
	  dStandard = dStandard + dSum;
       }

void TItemUpdate::SetResource()
{


 DATA4 *create;
 cb.safety = 0;
 create = d4create(&cb,"sc_rslts",ResultsInfo,ResultsTag);
 d4close(create);

 Results = new TResults(&cb);

 char szCostItem[15];
 char szFpcode[11];
 char szCostType[4];

 SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)"Creating productions results...");

 CostStr->SetIndex("fopcode");
 Items->SetIndex("man");
 for(Items->top();!Items->eof();Items->skip())
 {
  strcpy(szParent,Items->fPcode->str());
  strcpy(szFpcode,Items->fPcode->str());
  strcpy(szCostType,"ACT");
  strcpy(szCostItem,"ACT");
  strcat(szCostItem,szFpcode);
  SendDlgItemMsg(ID_CODE,WM_SETTEXT,NULL,(LPARAM)(LPSTR)szParent);
  if(CostStr->seek(szCostItem) == 0)
  {
  dStandard = 0.0000;
  dLabour   = 0.0000;
  dMaterial = 0.0000;
  dOH       = 0.0000;
  dIndirect = 0.0000;
  iLine = 1;  
  int i = GetResource(szCostItem,szCostType,1,1,1);

  Items->seek(szFpcode);
  double dItemCost = (dStandard * double (Str4ptr(Items->fLotsize->str())));

  Results->appendStart(0);
  Results->fParent->assign(szParent);
  Results->fUnit->assign(Resrc->fUnit->str());
  if(Proc->seek(Items->fShop1->str()) == 0)
   Results->fCostctn->assign(Proc->fCode->str());
  Results->fShop1->assign(Items->fShop1->str());
  Results->fMaterial->assignDouble(dMaterial);
  Results->fLabour->assignDouble(dLabour);
  Results->fOH->assignDouble(dOH);
  Results->fIndirect->assignDouble(dIndirect);
  Results->fCost->assignDouble(dStandard);
  Results->append();
  Items->fPrice->assignDouble(dItemCost);
  Items->flush();
  }
 }
 cb.closeAll();
 ShutDownWindow();
 return;
}


