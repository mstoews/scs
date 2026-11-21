/*** TOPICS data transfer and calculation programs ***/
#include "scs.h"
extern HWND hStatus;
/*** Database Include Files ***/

#include "db_costd.h"
#include "db_costm.h"
#include "db_coste.h"
#include "db_trstr.h"
#include "db_elem.h"
#include "db_resrc.h"
#include "db_mrslt.h"
#include "db_glspc.h"
#include "db_centr.h"
#include "db_glcst.h"
#include "db_jiss.h"
#include "db_types.h"
#include "gl_dlg.h"
#include "db_rlts.h"
#include "db_topic.h"
#include "db_tprod.h"
#include "sc_itemu.h"
#include "sc_matr.h"
#define IDD_STATUSBAR_MENUHELP 2


double TTActual::GetHours(char *szFpcode)
{
 double dTime;
 if( GLcost->seek(szFpcode) == 0 )
  dTime = double (Str4ptr(GLcost->fRescode->str()));
 return dTime;  
}

double TTActual::GetManQuantity(char *szFpcode)
{ 
    char szCompare[21];
    double dSumQty = 0;
    strcpy(szCompare,szFpcode);
    while(strcmp(szFpcode,szCompare) == 0)
      {
       double dQty = double (Str4ptr(TopicsResults->fQty->str()));
       dSumQty = dQty + dSumQty;
       TopicsResults->skip();
       strcpy(szCompare,TopicsResults->fOpcode->str());
       strcat(szCompare,TopicsResults->fPcode->str());
      }
    return dSumQty;
}

_CLASSDEF (TGLTransfer)
class TGLTransfer : public TDialog
{
 public: 
 //PTJiss   Jiss;
 PTTopicsJiss Jiss;    // Use the topics time database rather than the SCS database.
 PTCostm  Costm;
 PTCoste  Coste;
 PTscitemm Item;
 PTProc   Proc; 
 PTGLSpec GLSpec;
 PTGLcost GLcost;
 PTResrc  Resrc;
 virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
 TGLTransfer (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
 virtual void SetupWindow();
 BOOL ActualResourceCost(void);
 BOOL ManufacturedItemUnitCost(void);
 double dGLCurrentPeriod(char *szGLAccount,int iPeriod);
 double dGetResourceCost(char *szRescode,int iPeriod);
 double GetManCost(char *szCode);
 double GetStdCost(char *szCode);
 double dGetTime(char *fKoutei,char *fKbn);
 BOOL UpdateResources(void);
 BOOL AppendRes(char *szResource, char *szRes,double dTjissTime,double dTjissIndTime,int iPeriod,char *szKoutei);
 void Msg(int ID,char *szMsg);
};

void TTActual::LoadTypes(PTTypes)
{
 DWORD Indx;
 Types->top();
 while(!Types->eof())
 {
  Indx = SendDlgItemMsg(ID_COSTTYPE,CB_ADDSTRING,NULL,(LPARAM)(LPSTR)Types->fCode->str());
  Types->skip(1);
 }
 SendDlgItemMsg(ID_COSTTYPE,CB_SETCURSEL,Indx,NULL); 
 return;
}

// Most Non Material Resources use the direct labour hours
// however indirect overhead or indirect material resources could use
// indirect hours as their calculation method.

BOOL TGLTransfer::UpdateResources(void)
{
 char szSeek[15];
 GLcost = new TGLcost(&cb);
 Resrc  = new TResrc(&cb);
 GLcost->top();
 GLcost->SetIndex("frescode");
 while(!GLcost->eof())
  {
   strcpy(szSeek,GLcost->fRescode->str());
   strcat(szSeek,"ACT"); 
   if(Resrc->seek(szSeek) == 0)
    {
     double dCost = double (Str4ptr(GLcost->fGlcost->str()));
     double dTime = 0;
     if(strcmp(Resrc->fType->str(),"I ") !=0)
       dTime = GLcost->dTime();
     else
       dTime = GLcost->dIndTime();
     if(dTime > 0)
     Resrc->fRate->assignDouble(dCost/dTime);
     Resrc->fActCost->assignDouble(dTime);
     Resrc->flush();
    }
   GLcost->skip();
  }
  cb.closeAll();
  return TRUE;
}

double TGLTransfer::dGetTime(char *fKoutei,char *fKbn)
{
  double dTime = 0;
  char szQuery[85];
  
  strcpy(szQuery,"FSHOP = '");
  strcat(szQuery,fKoutei);
  strcat(szQuery,"'");
  strcat(szQuery,".AND. FKBN = '");
  strcat(szQuery,fKbn);
  strcat(szQuery,"'");
  Relate4set *rQuery = new Relate4set(Jiss);
  rQuery->querySet(szQuery);
  for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
   {
    dTime = dTime + Jiss->dTime();
   }

  rQuery->unlock();
  rQuery->free();
  return dTime;
}

BOOL TGLTransfer::ActualResourceCost(void)
{
 Msg(ID_DESCRIPT,"General Ledger Resource ...");
 DATA4 *create;
 cb.safety = 0;
 create = d4create(&cb,"sc_glcst",SC_GLCST_Field,SC_GLCST_Tag);
 d4close(create);

 Jiss   = new TTopicsJiss (&cb);
 Proc   = new  TProc(&cb); 
 GLSpec = new TGLSpec(&cb);
 GLcost = new TGLcost(&cb);
 Resrc  = new TResrc(&cb);
 int iPeriod = GetCurrentPeriodn();
 char  szLabourRes[11];
 char  szDirectRes[11];;
 char  szIndirectRes[11];
 Resrc->SetIndex("fcode");
 GLSpec->SetIndex("fpcode");
 GLcost->SetIndex("frescode");
 Proc->top();
 Proc->SetIndex("fkoutei");
 Jiss->SetIndex("fshop");
 char szResource[15];
 char szCostType[4];
 strcpy(szCostType,"ACT");

 while(!Proc->eof())
  {

   double dTjissTime = 0;
   double dTjissIndTime = 0;
   strcpy(szLabourRes,Proc->fLabour->str());   
   strcpy(szDirectRes,Proc->fOh->str());
   strcpy(szIndirectRes,Proc->fIDoh->str());   
   
   if(Jiss->seek(Proc->fKoutei->str()) == 0)
    {
     Msg(ID_ITEMNO,Proc->fKoutei->str());
     dTjissTime = dGetTime(Proc->fKoutei->str(),"0");
     dTjissIndTime = dGetTime(Proc->fKoutei->str(),"2");
     // Labour Resource
     strcpy(szResource,szLabourRes);
     strcat(szResource,szCostType);
     AppendRes(szResource,szLabourRes,dTjissTime,dTjissIndTime,iPeriod,Proc->fKoutei->str());
     // Direct Resource
     strcpy(szResource,szDirectRes);
     strcat(szResource,szCostType);
     AppendRes(szResource,szDirectRes,dTjissTime,dTjissIndTime,iPeriod,Proc->fKoutei->str());
     //Indirect Resource
     strcpy(szResource,szIndirectRes);
     strcat(szResource,szCostType);
     AppendRes(szResource,szIndirectRes,dTjissTime,dTjissIndTime,iPeriod,Proc->fKoutei->str());
    }
  Proc->skip();
 }
 return TRUE;
}

BOOL TGLTransfer::AppendRes(char *szResource, char *szRes,double dTjissTime,double dTjissIndTime, int iPeriod,char *szKoutei)
{
  if(Resrc->seek(szResource) == 0)
    {
      if(GLcost->seek(szRes) == 0)
       {
	if(strcmp(Resrc->fType->str(),"I ") == 0)
	GLcost->fIndTime->assignDouble(GLcost->dIndTime()+dTjissIndTime);
         else	
	GLcost->fTime->assignDouble(GLcost->dTime()+dTjissTime+dTjissIndTime);
        GLcost->flush();
       }
       else      
       {
	GLcost->appendStart(0);
	GLcost->fRescode->assign(szRes);
	GLcost->fKoutei->assign(szKoutei);
	if(strcmp(Resrc->fType->str(),"I ") ==0)
	 {
	  GLcost->fTime->assignDouble(0);
	  GLcost->fIndTime->assignDouble(dTjissIndTime);
	 }
	else
	 {
	  GLcost->fTime->assignDouble(dTjissTime+dTjissIndTime);
	  GLcost->fIndTime->assignDouble(0);
         }
	GLcost->fGlcost->assignDouble(dGetResourceCost(szRes,iPeriod));
        GLcost->fPeriod->assignLong(iPeriod);
	GLcost->append();
       }
   }
  else   
  return FALSE;
  return TRUE;
 }  // Direct Resources


double TGLTransfer::dGLCurrentPeriod(char *szGLAccount,int iPeriod)
{
 double dCurrentCost = 0;
  PTglaccts glaccts = new Tglaccts(&cb);
 glaccts->SetIndex("account");

 if(glaccts->seek(szGLAccount) == 0)
  {
   switch(iPeriod){
    case 1: dCurrentCost  = glaccts->dPeriod1(); break;
    case 2: dCurrentCost  = glaccts->dPeriod2(); break;
    case 3: dCurrentCost  = glaccts->dPeriod3(); break;
    case 4: dCurrentCost  = glaccts->dPeriod4(); break;
    case 5: dCurrentCost  = glaccts->dPeriod5(); break;
    case 6: dCurrentCost  = glaccts->dPeriod6(); break;
    case 7: dCurrentCost  = glaccts->dPeriod7(); break;
    case 8: dCurrentCost  = glaccts->dPeriod8(); break;
    case 9: dCurrentCost  = glaccts->dPeriod9(); break;
    case 10: dCurrentCost = glaccts->dPeriod10(); break;
    case 11: dCurrentCost = glaccts->dPeriod11(); break;
    case 12: dCurrentCost = glaccts->dPeriod12(); break;
    default: dCurrentCost = 0; break;
   }
 }
 glaccts->close();
 return dCurrentCost;
}

double TGLTransfer::dGetResourceCost(char *szRescode,int iPeriod)
{
 double dCost = 0;
 char szGLAccount[12];
 char szDepart[5];
 PTglaccts glaccts = new Tglaccts(&cb);
 glaccts->SetIndex("account");
 double dCurrentCost = 0;
 if(GLSpec->seek(szRescode)==0)
 {
  while(strcmp(GLSpec->fRescode->str(),szRescode) == 0 )
   {
    strcpy(szGLAccount,GLSpec->fAccount->str());
    strcat(szGLAccount,GLSpec->fDepart->str());
    if(glaccts->seek(szGLAccount) == 0)
     {
      switch(iPeriod){
      case 1: dCurrentCost  = glaccts->dPeriod1(); break;
      case 2: dCurrentCost  = glaccts->dPeriod2(); break;
      case 3: dCurrentCost  = glaccts->dPeriod3(); break;
      case 4: dCurrentCost  = glaccts->dPeriod4(); break;
      case 5: dCurrentCost  = glaccts->dPeriod5(); break;
      case 6: dCurrentCost  = glaccts->dPeriod6(); break;
      case 7: dCurrentCost  = glaccts->dPeriod7(); break;
      case 8: dCurrentCost  = glaccts->dPeriod8(); break;
      case 9: dCurrentCost  = glaccts->dPeriod9(); break;
      case 10: dCurrentCost = glaccts->dPeriod10(); break;
      case 11: dCurrentCost = glaccts->dPeriod11(); break;
      case 12: dCurrentCost = glaccts->dPeriod12(); break;
      default: dCurrentCost = 0; break;
      }
    }
   // dCost = dCost + dGLCurrentPeriod(szGLAccount,iPeriod);
    dCost = dCost + dCurrentCost;
    GLSpec->skip();    
    Msg(ID_ITEMNO,szRescode);
   }
 }
 return dCost;
}


void TGLTransfer::SetupWindow()
{
 TDialog::SetupWindow();
}



void TGLTransfer::Ok (RTMessage)
{
   ActualResourceCost();
   UpdateResources();
   BWCCMessageBox(HWindow,"General Ledger costs have been transferred.","Update Standard Cost",MB_ICONEXCLAMATION);
   ShutDownWindow();

}


void TTActual::Ok (RTMessage)
{
   if(*GetField(ID_COSTTYPE) != 0)
    StandardCostElements(GetField(ID_COSTTYPE));
   else
    return;  
   ShutDownWindow();
}

void TTActual::Production(RTMessage)
{
   ProdElements("STD");
}

void TTActual::SetupWindow()
{
 TDialog::SetupWindow();
 bDetail = TRUE;
 bSummary = FALSE;
 bAllCosts = FALSE;
 SendDlgItemMsg(ID_DETAIL,BM_SETCHECK,TRUE,0);
 Types = new TTypes(&cb);
 LoadTypes(Types);
}

void TGlWindow::SCSTransActual    (RTMessage)
 {  
  GetApplication()->ExecDialog(new TTActual(this,"TACTUAL"));
 }

void TTActual::ProdResults(RTMessage)
{
 CreateProdTemp();
}

void TGlWindow::SCSTransSCS       (RTMessage)
 {  
  GetApplication()->ExecDialog(new TGLTransfer (this,"TRANSGL"));
 }

void TGLTransfer::Msg(int ID,char *szMsg)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)szMsg);
}


void TTActual::Msg(int ID,char *szMsg)
{
  SendDlgItemMsg(ID,WM_SETTEXT,NULL,(LPARAM)(LPSTR)szMsg);
}


BOOL TTActual::StandardCostElements(char *szCostType)
{
 // Calculate the cost per COST TYPE for each Resource.
 // Basic calculations must be made for STD and ACT
 // HST are calculated automatically from last period
 // ACT.

 Msg(ID_DESCRIPT,"Creating detail cost file...");
 DATA4 *create; 
 cb.safety = 0;
 Data4 Cost(&cb,"Sc_coste");
 Field4info  db(Cost);
 Tag4info    tag(Cost);
 Data4 data;
 data.create(cb,szCostType,db.fields(),tag.tags());
 Cost.close();
 data.close();


 CostStr = new Ttrstr(&cb);
 PTCost Coste   = new TCost(&cb,szCostType);
 Resrc   = new TResrc(&cb);
 Item    = new Tscitemm(&cb);
 //Centre  = new TCentre(&cb);
 
 Item->SetIndex("fpcode");
 CostStr->SetIndex("fopcode");
 Resrc->SetIndex("fcode");
 Item->top();
 char szCode[15];
 char szSeek[15];
 strcpy(szCode,Item->fPcode->str());
 int i = 0;
 long lRecno;
   while (!Item->eof())
    {
       strcpy(szSeek,szCostType);
       strcat(szSeek,Item->fPcode->str());
       strcpy(szCode,Item->fPcode->str());
       Msg(ID_ITEMNO,szCode);
       if(CostStr->seek(szSeek) == 0)
         {
	  lRecno = Item->recNo();
	  strcpy(szParent,szCode);
	  dLotSize = CostStr->dQuantity();
	  dParentKousei = CostStr->dKousei();
	  if(dLotSize != dParentKousei)
	   dParentKousei = dLotSize;	  
	  WriteElements(1,szCode,szCostType,Item,Resrc,CostStr,Coste,1);
	  Item->go(lRecno);                            	
	 }
       Item->skip();
   }


 cb.closeAll();
 return TRUE;
}

static int i;

void TTActual::WriteElements(int iLevel, char *szCode,char *szCostType,PTscitemm Item, PTResrc Resrc, PTtrstr CostStr, PTCost Coste,double dRequirement)
{ 
 char szCompare[15];
 char szSeek[15];
 char szKpcode[15];
 char szChildSeek[15];
 char szCurrentPeriod[3];
 itoa(GetCurrentPeriodn(),szCurrentPeriod,10);
 strcpy(szSeek,szCostType);
 strcat(szSeek,szCode);
 CostStr->seek(szSeek);
 strcpy(szCompare,szSeek);
     while (strcmp(szSeek,szCompare) == 0)
	{
	  strcpy(szKpcode,CostStr->fKpcode->str());
	  strcat(szKpcode,szCostType);	  
	    if(Resrc->seek(szKpcode) == 0)
		{
		  Item->seek(szParent);		  
		  double dRatio = (dRequirement/CostStr->dQuantity());
		  Coste->appendStart(0);
		  Coste->fLevel->assignLong(iLevel);
                  Coste->fParent->assign(szParent);
		  Coste->fOpcode->assign(szCode);
		  Coste->fSeqno->assign(CostStr->fSeqno->str());
		  Coste->fKpcode->assign(szKpcode);
		  Coste->fCostype->assign(szCostType);
		  Coste->fPeriod->assign(szCurrentPeriod);
		  Coste->fCostctn->assign("CENTRE");
		  Coste->fUnit->assign(Item->fUnit->str());
		  Coste->fShop1->assign(Item->fShop1->str());
		  Coste->fSemi->assign(Item->fSkbn->str());		  
		  Coste->fLotsize->assignDouble(CostStr->dQuantity());
		  Coste->fElement->assign(Resrc->fElement->str());
                  Coste->fRatio->assignDouble(dRatio);
		  Coste->fRate->assignDouble(Resrc->dRate());
		  Coste->fActRate->assignDouble(Resrc->dActCost());
		  Coste->fQuantity->assignDouble(CostStr->dKousei());
		  double dTotal = dRatio * Resrc->dRate() * CostStr->dKousei();
		  Coste->fCost->assignDouble(dTotal);
		  Coste->append();
		}
	    else
                {
		    long lRecno = CostStr->recNo();
		    double dUpperQuantity = CostStr->dQuantity();
		    double dUpperKousei   = CostStr->dKousei();
		    strcpy(szKpcode,CostStr->fKpcode->str());
		    strcpy(szChildSeek,szCostType);
		    strcat(szChildSeek,szKpcode);
		    if(CostStr->seek(szChildSeek) == 0)
		     {
                           
			   int CurrentLevel = iLevel;
                           double dCurrentRequirement = dRequirement;
			   Item->seek(szParent);
			   Coste->appendStart(0);
			   Coste->fLevel->assignLong(iLevel);
                	   Coste->fParent->assign(szParent);
			   Coste->fOpcode->assign(szCode);
			   Coste->fSeqno->assign(CostStr->fSeqno->str());
			   Coste->fKpcode->assign(szKpcode);
			   Coste->fCostype->assign(szCostType);
			   Coste->fPeriod->assign(szCurrentPeriod);
			   Coste->fCostctn->assign("CENTRE");
			   Coste->fUnit->assign(Item->fUnit->str());
			   Coste->fShop1->assign(Item->fShop1->str());		  
			   Coste->fSemi->assign(Item->fSkbn->str());
			   Coste->fLotsize->assignDouble(dParentKousei);
			   Coste->fElement->assign("M");
			   Coste->fRate->assignDouble(0);
			   Coste->fQuantity->assignDouble(0);
			   Coste->fCost->assignDouble(0);
			   Coste->append();
			   iLevel++;
			  if(dUpperQuantity != dUpperKousei)
			     dRequirement = dRequirement * dUpperKousei/dUpperQuantity;
			  WriteElements(iLevel,szKpcode,szCostType,Item,Resrc,CostStr,Coste,dRequirement);
	                  dRequirement = dCurrentRequirement;
			  iLevel = CurrentLevel;
                      }
        	      CostStr->go(lRecno);		  	
                    }
		   CostStr->skip();
                   strcpy(szCompare,szCostType);
		   strcat(szCompare,CostStr->fOpcode->str());
         }	  
 return;
}


_CLASSDEF(TTransGL)
class TTransGL : public TDialog
{
 public:
  TTransGL (PTWindowsObject AParent, LPSTR ATitle):TDialog(AParent,ATitle){};
  virtual void SetupWindow(){};
};
void TGlWindow::SCSTransGL         (RTMessage)
 {
  
  GetApplication()->ExecDialog(new TGLTransfer(this,"TRANSGL"));
 }

BOOL TGLTransfer::ManufacturedItemUnitCost(void)
{
 // Calculate the cost per COST TYPE for each Resource.
 // Basic calculations must be made for STD and ACT
 // HST are calculated automatically from last period
 // ACT.

 return TRUE;
}

double TGLTransfer::GetManCost(char *szCode)
{
 double dTotal = 0;
 char szManResults[11];
 strcpy(szManResults,szCode);
  while (strcmp(szManResults,szCode) == 0)
   {
     dTotal = ((Coste->dRate() * Coste->dQuantity()) + dTotal);
     Coste->skip();
     strcpy(szManResults,Coste->fParent->str());
   }
 return dTotal;
}

double TGLTransfer::GetStdCost(char *szCode)
{
 double dTotal = 0;
 char szManResults[11];
 strcpy(szManResults,szCode);

  while (strcmp(szManResults,szCode) == 0)
   {
     dTotal = ((Coste->dActRate() * Coste->dQuantity()) + dTotal);
     Coste->skip();
     strcpy(szManResults,Coste->fParent->str());
   }
 return dTotal;
}
void TTActual::Cancel(RTMessage)
{
 cb.closeAll();
 ShutDownWindow();
}