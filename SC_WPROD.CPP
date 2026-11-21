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


static FIELD4INFO ProdInfo[] =
 {
 { "fparent"	,'C',10,0},
 { "flotsize"	,'N',14,4},
 { "fprod"	,'N',14,4},
 { 0,0,0,0}
 };

static  TAG4INFO Prodtag[] =
 {
 { "fparent","fparent",0,0,0},
 { 0,0,0,0,0}
 };


BOOL TTActual::ProdElements(char *szCostType)
{
 // Create a temporary file that holds the standard lot size and the
 // actual lot size of the production.
 
 CreateProdTemp();     // Create file sc_ptemp.dbf indexed on fparent.
 
 char szCurrentPeriod[3];
 long lPeriod = GetCurrentPeriodn();
 ltoa(lPeriod,szCurrentPeriod,10);
 Msg(ID_DESCRIPT,"Creating detail cost file...");

 Data4 Cost(&cb,"sc_prod");
 Field4info  db(Cost);
 Tag4info    tag(Cost);
 Cost.close();
 cb.safety = 0;
 Data4 data;
 data.create(cb,"sc_prod",db.fields(),tag.tags());
 data.close();


 /*
 DATA4 *create;
 
 create = d4create(&cb,"SC_PROD",coste,costetag);
 d4close(create);
 */
 CostStr = new Ttrstr(&cb);
 Product = new TProduct(&cb,"SC_PROD"); 
 Resrc   = new TResrc(&cb); 
 Item    = new Tscitemm(&cb); 
 Prod    = new TProd(&cb); 
 Centre  = new TCentre(&cb);
 PTTopicsJiss Jiss    = new TTopicsJiss(&cb);
 
 dProduction = 0;
 Item->SetIndex("fpcode");
 CostStr->SetIndex("fopcode");
 Resrc->SetIndex("fcode");
 Item->top();
 char szCode[15];
 char szSeek[15];
 strcpy(szCode,Item->fPcode->str());
 int i = 0;
 long lRecno;
 Relate4set *rQuery = new Relate4set(Prod);
 rQuery->querySet("FPARENT <> ' '");
 for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
    {
       strcpy(szSeek,szCostType);
       strcat(szSeek,Prod->fParent->str());
       strcpy(szCode,Prod->fParent->str());
       Msg(ID_ITEMNO,szCode);
       dMaterial = 0;
       dLabour   = 0;
       dIndirect = 0;
       dOverhead = 0;

       if(CostStr->seek(szSeek) == 0)
         {
	  lRecno = Item->recNo();
	  strcpy(szParent,szCode);
          if(Prod->dLotsize() > 0)
	  dProduction = Prod->dProd()/Prod->dLotsize();

	  WriteProd(1,szCode,szCostType,Item,Resrc,CostStr,1);

	          Product->appendStart(0);
		  Product->fOpcode->assign(szCode);
                  Product->fCosttype->assign(GetField(ID_COSTTYPE));
		  Product->fPeriod->assign(szCurrentPeriod);
		  Product->fCostctn->assign("N/A");
		  Product->fUnit->assign(Item->fUnit->str());
		  Product->fShop1->assign(Item->fShop1->str());
		  Product->fSemi->assign(Item->fSkbn->str());
		  Product->fLotsize->assignDouble(Prod->dProd());
		  Product->fElement->assign(Resrc->fElement->str());
		  Product->fRatio->assignDouble(dProduction);
		  Product->fMaterial->assignDouble(dMaterial);
		  Product->fLabour->assignDouble(dLabour);
		  Product->fOverhead->assignDouble(dOverhead);
		  Product->fIndirect->assignDouble(dIndirect);
                  Product->append();

	 }
   }

 rQuery->unlock();
 rQuery->free();
 {
 Relate4set *pQuery = new Relate4set(Product);
 pQuery->querySet("FOPCODE <> ' '");
 Jiss->SetIndex("fjcode");
 Proc = new TProc(&cb);
 Proc->SetIndex("fkoutei");
 for(int qc = pQuery->top();qc!=r4eof;qc=pQuery->skip(1L) )
    {
      if(Jiss->seek(Product->fOpcode->str()) == 0)
	{
	  Msg(ID_ITEMNO,Product->fOpcode->str());
	  if(Proc->seek(Jiss->fShop->str()) == 0)
	  Product->fCostctn->assign(Centre->fCode->str());
	  else
	  MessageBox(HWindow,"","",MB_OK);
	  Product->flush();
	}            
    }
 pQuery->unlock();
 pQuery->free();
 }
 cb.closeAll();
 Msg(ID_ITEMNO,"");
 Msg(ID_DESCRIPT,"Production costs calculated...");
 return TRUE;
}

void TTActual::WriteProd(int iLevel, char *szCode,char *szCostType,PTscitemm Item, PTResrc Resrc, PTtrstr CostStr,double dRequirement)
{ 
 char szCompare[15];
 char szSeek[15];
 char szKpcode[15];
 char szChildSeek[15];
 strcpy(szSeek,szCostType); 
 strcat(szSeek,szCode);
 CostStr->seek(szSeek);
 strcpy(szCompare,szSeek);
 Item->seek(szCode);

     while (strcmp(szSeek,szCompare) == 0)
	{

	  strcpy(szKpcode,CostStr->fKpcode->str());
	  strcat(szKpcode,GetField(ID_COSTTYPE));
	    if(Resrc->seek(szKpcode) == 0)
		{

		  double dTotal = dProduction * Resrc->dRate() * CostStr->dKousei();
		  if(strcmp(Resrc->fType->str(),"W ") == 0 ||
		     strcmp(Resrc->fType->str(),"WI") == 0 ||
		     strcmp(Resrc->fType->str(),"RI") == 0 ||
		     strcmp(Resrc->fType->str(),"R ") == 0)
		  {    
		   dMaterial = dMaterial + dTotal;
                  }
		  if(strcmp(Resrc->fType->str(),"L ") == 0)
                   {
		    dLabour = dLabour + dTotal;
                   }
		  if(strcmp(Resrc->fType->str(),"D ") == 0)
                  dOverhead = dOverhead + dTotal;
		  if(strcmp(Resrc->fType->str(),"I ") == 0)
		  dIndirect = dIndirect + dTotal;

		}
	     CostStr->skip();
	     strcpy(szCompare,szCostType);	     
	     strcat(szCompare,CostStr->fOpcode->str());
         }  

 return;
}

BOOL TTActual::CreateProdTemp(void)
{
 DATA4 *create;
 cb.safety = FALSE;
 create = d4create(&cb,"SC_PTEMP",ProdInfo,Prodtag);
 d4close(create);
 cb.safety = TRUE;

 Prod = new TProd(&cb);
 Prod->SetIndex("fparent");
 char szCode[11];

 Item = new Tscitemm(&cb);
 Item->SetIndex("fpcode");

 CostStr = new Ttrstr(&cb);
 CostStr->SetIndex("prod");

 Relate4set *rQuery = new Relate4set(CostStr);
 rQuery->querySet("FTYPE = 'ACT'");

 Msg(ID_DESCRIPT,"Create production results file...");

 for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
    {      
       strcpy(szCode,CostStr->fOpcode->str());
       if(Item->seek(szCode) == 0)
       {
	Msg(ID_ITEMNO,Prod->fParent->str());
	Prod->appendStart();
	Prod->fParent->assign(szCode);
	Prod->fLotsize->assignDouble(double (Str4ptr(Item->fLotsize->str())));
	Prod->fProd->assignDouble(double (Str4ptr(CostStr->fStqty->str())));
	Prod->append();
       }
    }
 rQuery->unlock();
 rQuery->free();
 cb.closeAll();
 Msg(ID_ITEMNO,"");
 Msg(ID_DESCRIPT,"Production results file completed...");
}
