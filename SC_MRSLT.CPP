/*** TOPICS data transfer and calculation programs ***/
#include "scs.h"

/*** Database Include Files ***/

#include "db_trstr.h"
#include "db_elem.h"
#include "db_resrc.h"
#include "db_mrslt.h"
#include "db_glcst.h"
#include "db_jiss.h"
#include "db_types.h"
#include "gl_dlg.h"
#include "db_rlts.h"
#include "db_topic.h"

/*** Cost Results Calculation ***/
_CLASSDEF(TResultsCalc)
class TResultsCalc : public TMainDialog
{
 public:
  /* Create pointers to the database */
  char szParent[11];

  PTGLcost  GLcost;
  PTtrstr   CostStr;
  PTProc    Proc;
  PTJiss    Jiss;    
  PTElement Element;
  PTResrc   Resrc;
  PTManResult ManResults;
  PTResult  Result;
  PTTypes   Types;
  PTTopicsTmast Tmast;

  TResultsCalc (PTWindowsObject AParent, LPSTR ATitle):TMainDialog(AParent,ATitle){};
  virtual void Ok (RTMessage Msg) = [ID_FIRST + IDOK];
  virtual void Cancel (RTMessage Msg) = [ID_FIRST + IDCANCEL];
  virtual void SetupWindow(void);
  BOOL ManufactureResults(char *szCostType);
  void WriteResults(int iLevel, char *szCode,char *szCostType, PTTopicsTmast Tmast, PTResrc Resrc, PTtrstr CostStr, PTResult Result,double dRequirement);
  void LoadTypes(PTTypes Types);
  ~TResultsCalc(){cb.close_all();};
};


/*** Maintenance: Accumulated Costs ***/


BOOL TResultsCalc::ManufactureResults(char *szCostType)
{
 // Calculate the cost per COST TYPE for each Resource.
 // Basic calculations must be made for STD and ACT
 // HST are calculated automatically from last period
 // ACT.

 Msg(ID_DESCRIPT,"Creating Manufacturing Results...");
 DATA4 *create;
 cb.safety = 0;
 create = d4create(&cb,szCostType,coste,costetag);
 d4close(create);

 CostStr = new Ttrstr(&cb);
 PTResult Result  = new TResult(&cb);
 Resrc   = new TResrc(&cb);
 Result  = new TResult(&cb);
 Tmast   = new TTopicsTmast(&cb);
 
 Tmast->SetIndex("fpcode");
 CostStr->SetIndex("fopcode");
 Resrc->SetIndex("fcode");

 Tmast->top();
 char szCode[15];
 char szSeek[15];
 strcpy(szCode,Tmast->fPcode->str());
 int i = 0;
 long lRecno;
   while (!Tmast->eof())
    {
       if(i>50)
       {
	cb.close_all();
        return TRUE;
       }	
       i++;
       strcpy(szSeek,szCostType);
       strcat(szSeek,Tmast->fPcode->str());
       strcpy(szCode,Tmast->fPcode->str());
       Msg(ID_ITEMNO,szCode);
       if(CostStr->seek(szSeek) == 0)
         {
	  lRecno = Tmast->recno();
	  strcpy(szParent,szCode);
	  dLotSize = CostStr->dQuantity();
	  dParentKousei = CostStr->dKousei();
	  if(dLotSize != dParentKousei)
	   dParentKousei = dLotSize;	  
	  WriteResults(1,szCode,szCostType,Tmast,Resrc,CostStr,Result,1);
	  Tmast->go(lRecno);
	 }
       Tmast->skip();
   }
 cb.close_all();
 return TRUE;
}


void TResultsCalc::WriteResults(int iLevel, char *szCode,char *szCostType,PTTopicsTmast Tmast, PTResrc Resrc, PTtrstr CostStr, PTResult Results,double dRequirement)
{ 
 char szCompare[15];
 char szSeek[15];
 char szKpcode[11];
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
	    if(Resrc->seek(szKpcode) == 0)
		{
		  Tmast->seek(szParent);
		  double dRatio = (dRequirement/CostStr->dQuantity());
		  Results->append_start(0);
		  Results->fParent;
		  Results->fOpcode;
		  Results->fLevel;
		  Results->fSeqno;
		  Results->fKpcode;
		  Results->fStdRate;
		  Results->fActRate;
		  Results->fStdQty;
		  Results->fActQty;
		  Results->append();
		}
	    else
                {
		    long lRecno = CostStr->recno();
		    double dUpperQuantity = CostStr->dQuantity();
		    double dUpperKousei   = CostStr->dKousei();
		    strcpy(szChildSeek,szCostType);
                    strcat(szChildSeek,szKpcode);
		    if(CostStr->seek(szChildSeek) == 0)
        	     {
			int CurrentLevel = iLevel;
			Results->append_start(0);
			Results->fParent;
			Results->fOpcode;
		        Results->fLevel;
		        Results->fSeqno;
		        Results->fKpcode;
		        Results->fStdRate;
		        Results->fActRate;
		        Results->fStdQty;
		        Results->fActQty;
		        Results->append();
			Results->append();
			iLevel++;
			  if(dUpperQuantity != dUpperKousei)
			     dRequirement = dRequirement * dUpperKousei/dUpperQuantity;
			  WriteResults(iLevel,szKpcode,szCostType,Tmast,Resrc,CostStr,Results,dRequirement);
	                  //dRequirement = dCurrentRequirement;
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



