#include "scs.h"
#include <alloc.h>
/* Database Files */

#include "db_topic.h"
#include "db_mrslt.h"
#include "db_jiss.h"
#include "db_resrc.h"
#include "db_ctemp.h"
#include "db_trstr.h"
#include "sc_trans.h"

/*
   For manufacturing results we should check the current period
   scope to get the start and end date for transferring current
   monthly information. The current period must be updated then to
   reflect which cost period you wish to create standard cost
   information for.
*/


void TGlWindow::SCSTransStruct    (RTMessage)
 {GetApplication()->ExecDialog(new TTStruct(this,"TSTRUCT"));}

char *TTStruct::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}


char far * GetLP(int iLength)
{
 //GLOBALHANDLE hString = GlobalAlloc(GMEM_MOVEABLE,iLength);
 //char far * lpString = (char far *)GlobalLock(hString);
 //GlobalUnlock(hString);
 char *lpString = new char[iLength];
 return lpString;
}


BOOL TTStruct::ProductionResults(void)
{
  // Delete last months records....
  //char szStd[4];
  //char szCompare[11];
  //char szOpcode[11];
                      
  Item          = new Tscitemm(&cb);
  CostStr       = new Ttrstr(&cb);
  TopicsTmast   = new TTopicsTmast(&cb);
  ManResults    = new TManResult(&cb);
  ManResults->SetIndex("fopcode2");

  char *szStd = new char[CostStr->fType->len()+1];
  char *szCompare = new char[CostStr->fOpcode->len()+1];
  char *szOpcode  = new char[CostStr->fOpcode->len()+1];

  strcpy(szStd,GetString(ID_ACT));
  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1354));
  Relate4set *rQuery = new Relate4set(CostStr);
  rQuery->querySet(GetString(1361));
  for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
   {
      SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)CostStr->fOpcode->str());
      CostStr->deleteRec();
   }  
  rQuery->unlock();
  rQuery->free();
  CostStr->pack();
  CostStr->reindex();
  char szTemp[4];
  char szSeqno[4];

  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1355));
   // Add this months records for material.
  ManResults->top();
  int iSeqno = 0;
  while(!ManResults->eof())
   {
      SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)ManResults->fOpcode->str());
      strcpy(szOpcode,ManResults->fOpcode->str());
      iSeqno = 0;
     while(strcmp(szOpcode,ManResults->fOpcode->str()) == 0)
     {      
      double dOutput = 0;
      iSeqno = iSeqno + 10;
      if(iSeqno < 100)
      {
       strcpy(szSeqno,"0");
       itoa(iSeqno,szTemp,10);
       strcat(szSeqno,szTemp);
      }
      else      
       itoa(iSeqno,szSeqno,10);

      CostStr->appendStart();
      CostStr->fOpcode->assign(ManResults->fOpcode->str());
      CostStr->fType->assign(szStd);
      CostStr->fSeqno->assign(szSeqno);
      CostStr->fLt2->assign("");
      CostStr->fComm->assign("");
      CostStr->fCorectm->assign("");
      CostStr->fSpcrtp->assign("");
      CostStr->fChk_flg->assign("");
      int iLots = 0;
      strcpy(szCompare,ManResults->fPcode->str());      
      while(strcmp(szCompare,ManResults->fPcode->str()) == 0)
      {
       iLots++;
       dOutput = dOutput + double (Str4ptr(ManResults->fQty->str()));
       ManResults->skip();
      }
      CostStr->fLt1->assignLong(iLots);
      CostStr->fKpcode->assign(szCompare);
      CostStr->fStqty->assignDouble(0);
      CostStr->fKousei->assignDouble(dOutput);
      CostStr->append();
   }
 }
    //ManResults->skip();
 delete szStd;
 delete szCompare;
 delete szOpcode;
 cb.closeAll();
 return TRUE;
}

// Creates a temporary file called sc_temp.
// This file is used to hold a structure based upon the cost structure.
// All of the same resources that are used for a standard item are transferred to this file.

BOOL TTStruct::NonMatResource(void)
{
   char szSeek[11];
   CostStr       = new Ttrstr(&cb);
   cb.safety = FALSE;
   Data4  test;
   Field4info fields(CostStr->data);
   test.create(cb,"SC_TEMP",fields.fields());
   test.close();
   cb.safety = TRUE;

   CostTemp      = new TCostTemp(&cb);
   Item          = new Tscitemm(&cb);
   TopicsTmast   = new TTopicsTmast(&cb);
   TopicsTmast->SetIndex("FPCODE");
   Item->SetIndex("fpcode");
   CostStr->SetIndex("fpcode");

   SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1356));
   // First while not eof transfer the structures each item.
   Relate4set *rQuery1 = new Relate4set(CostStr);
   rQuery1->querySet("FTYPE = 'STD'");   
   for(int qc = rQuery1->top();qc!=r4eof;qc=rQuery1->skip(1L) )
   {
       SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)CostStr->fOpcode->str());
       strcpy(szSeek,CostStr->fKpcode->str());
       if(TopicsTmast->seek(CostStr->fOpcode->str())== 0 )  // if the has been manufactured this month
							    // check to see if it is a non material resource.
       {
       if(Item->seek(szSeek) !=0)          		    // if the item is a non material resource add it to the 	
	{						    // database.	
          CostTemp->appendStart();
          CostTemp->fOpcode->assign(CostStr->fOpcode->str());
          CostTemp->fType->assign("ACT");
          CostTemp->fSeqno->assign(CostStr->fSeqno->str());
          CostTemp->fKpcode->assign(CostStr->fKpcode->str());
          CostTemp->fStqty->assignDouble(double (Str4ptr(CostStr->fStqty->str())));
          CostTemp->append();
        }
       }
   }
   rQuery1->unlock();
   rQuery1->free();
   // Append all of these items to the cost structure as they are not included when
   // when the items transferred from the TOPICS were added.
   for(CostTemp->top();!CostTemp->eof();CostTemp->skip())
    {
       CostStr->appendStart();
       CostStr->fOpcode->assign(CostTemp->fOpcode->str());
       CostStr->fType->assign("ACT");
       CostStr->fSeqno->assign(CostTemp->fSeqno->str());
       CostStr->fKpcode->assign(CostTemp->fKpcode->str());
       CostStr->fStqty->assignDouble(double (Str4ptr(CostTemp->fStqty->str())));
       CostStr->append();
    }
 cb.closeAll();
 return TRUE;
}

BOOL TTStruct::ProductionTotals(void)
{
  CostStr       = new Ttrstr(&cb);
  TopicsTmast   = new TTopicsTmast(&cb);
  TopicsTmast->SetIndex("fpcode");
   SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1357));
   Relate4set *rQuery = new Relate4set(CostStr);
   rQuery->querySet(GetString(1361));
   for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
   {
      SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)CostStr->fOpcode->str());
      double dProduction = GetProduction(CostStr->fOpcode);
      CostStr->fStqty->assignDouble(dProduction);
      CostStr->flush();
   }
   SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1353));
   rQuery->unlock();
   rQuery->free();
}



double TTStruct::GetProduction(Field4 *fPcode)
{
 char *szCompare = new char[TopicsTmast->fPcode->len()+1];
 double dProduction = 0;
 if (TopicsTmast->seek(fPcode->str()) == 0)
  { 
   strcpy(szCompare,TopicsTmast->fPcode->str());
   SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsTmast->fPcode->str());
   while (strcmp(szCompare,TopicsTmast->fPcode->str()) == 0)
    {
     dProduction  = dProduction + double (Str4ptr(TopicsTmast->fOutqty->str()));
     TopicsTmast->skip();
    }
  }
 if (dProduction == 0)
  dProduction = 1;
 delete szCompare;
 return dProduction;
}

BOOL TTStruct::LabourHours(void)
{
  TopicsJiss = new TTopicsJiss(&cb);
  TopicsJiss->SetIndex("FJCODE");
  Resrc       = new TResrc(&cb);
  Resrc->SetIndex("FCODE");
  CostStr    = new Ttrstr(&cb);
  CostStr->SetIndex("FOPCODE");

  Relate4set *rQuery = new Relate4set(TopicsJiss);
  rQuery->querySet("FKBN = '0'");

  char szSeek[14];
  char szCompare[14];
  SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1358));
  for(int qc = rQuery->top();qc!=r4eof;qc=rQuery->skip(1L) )
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsJiss->fJcode->str());
    strcpy(szSeek,"ACT");
    strcat(szSeek,TopicsJiss->fJcode->str());
    if(CostStr->seek(szSeek) == 0)
     {
        strcpy(szCompare,"ACT");
        strcat(szCompare,CostStr->fOpcode->str());
	 while(strcmp(szSeek,szCompare) == 0)
	  {
	   char szResrc[14];
	   strcpy(szResrc,CostStr->fKpcode->str());
           strcat(szResrc,"ACT");
	   if(Resrc->seek(szResrc) == 0)
	    {
	      if(strcmp(Resrc->fType->str(),"R ") != 0 &&
		 strcmp(Resrc->fType->str(),"RI") != 0 &&
		 strcmp(Resrc->fType->str(),"WI") != 0 &&
		 strcmp(Resrc->fType->str(),"W ") != 0 &&
		 strcmp(Resrc->fType->str(),"I ") !=0)
		 { 
                  double dTime = CostStr->dKousei() + TopicsJiss->dTime();
		  CostStr->fKousei->assignDouble(dTime);
		  CostStr->flush();
                 }
	    }
	   CostStr->skip();
   	   strcpy(szCompare,"ACT");
	   strcat(szCompare,CostStr->fOpcode->str());
	  }
     }
   }
   rQuery->unlock();
   rQuery->free();
   SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1359));
   Relate4set *dQuery = new Relate4set(TopicsJiss);
   dQuery->querySet("FKBN = '2'");
   for(qc = dQuery->top();qc!=r4eof;qc=dQuery->skip(1L) )
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsJiss->fJcode->str());
    strcpy(szSeek,"ACT");
    strcat(szSeek,TopicsJiss->fJcode->str());
    if(CostStr->seek(szSeek) == 0)
     {
        strcpy(szCompare,"ACT");
        strcat(szCompare,CostStr->fOpcode->str());
	 while(strcmp(szSeek,szCompare) == 0)
	  {
	   char szResrc[14];
	   strcpy(szResrc,CostStr->fKpcode->str());
           strcat(szResrc,"ACT");
	   if(Resrc->seek(szResrc) == 0)
	    {
	      if(strcmp(Resrc->fType->str(),"R ") != 0 &&
		 strcmp(Resrc->fType->str(),"RI") != 0 &&
		 strcmp(Resrc->fType->str(),"WI") != 0 &&
		 strcmp(Resrc->fType->str(),"W ") != 0 &&
		 strcmp(Resrc->fType->str(),"I ") ==0)
		 {
		  double dTime = CostStr->dKousei() + TopicsJiss->dTime();
		  CostStr->fKousei->assignDouble(dTime);
		  //CostStr->fKousei->assignDouble(TopicsJiss->dTime());
		  CostStr->flush();
                 }
	    }
	   CostStr->skip();
   	   strcpy(szCompare,"ACT");
	   strcat(szCompare,CostStr->fOpcode->str());
	  }
     }
   }
   dQuery->unlock();
   dQuery->free();

   Relate4set *lQuery = new Relate4set(TopicsJiss);
   lQuery->querySet("FKBN = '2'");
   for(qc = lQuery->top();qc!=r4eof;qc=lQuery->skip(1L) )
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsJiss->fJcode->str());
    strcpy(szSeek,"ACT");
    strcat(szSeek,TopicsJiss->fJcode->str());
    if(CostStr->seek(szSeek) == 0)
     {
        strcpy(szCompare,"ACT");
        strcat(szCompare,CostStr->fOpcode->str());
	 while(strcmp(szSeek,szCompare) == 0)
	  {
	   char szResrc[14];
	   strcpy(szResrc,CostStr->fKpcode->str());
           strcat(szResrc,"ACT");
	   if(Resrc->seek(szResrc) == 0)
	    {
	      if(strcmp(Resrc->fType->str(),"R ") != 0 &&
		 strcmp(Resrc->fType->str(),"RI") != 0 &&
		 strcmp(Resrc->fType->str(),"WI") != 0 &&
		 strcmp(Resrc->fType->str(),"W ") != 0 &&
		 strcmp(Resrc->fType->str(),"L ") ==0 ||
		 strcmp(Resrc->fType->str(),"D ") ==0)
		 {
		  double dTime = double (Str4ptr(CostStr->fKousei->str())) + TopicsJiss->dTime();
		  CostStr->fKousei->assignDouble(dTime);
		  CostStr->flush();
                 }
	    }
	   CostStr->skip();
   	   strcpy(szCompare,"ACT");
	   strcat(szCompare,CostStr->fOpcode->str());
	  }
     }
   }
   lQuery->unlock();
   lQuery->free();


   cb.closeAll();
}
void TTStruct::Ok (RTMessage Msg)
{
     Resources();
     ProductionResults();
     NonMatResource();
     ProductionTotals();
     LabourHours();
     cb.closeAll();
     ShutDownWindow();  
}

void TTStruct::Cancel(RTMessage)
{
 cb.closeAll();
 ShutDownWindow();
}

void TTStruct::SetupWindow()
{
 TDialog::SetupWindow();
 PeriodScope(dPeriod);
}

BOOL  TTStruct::Resources(void)
{
 Item  = new Tscitemm(&cb,"titemm");

 Resrc = new TResrc(&cb);
 Resrc->SetIndex("FCODE");
 Item->top();
 Date4 Today;
 Today.today();
 SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)GetString(1360));
 char szFptype[4];
 char szFpcode[15];
 while(!Item->eof())
 {
  strcpy(szFptype,Item->fPtype->str());
  if(strcmp(szFptype,"RI") == 0 || strcmp(szFptype,"R ") == 0 || strcmp(szFptype,"WI") == 0 || strcmp(szFptype,"W ") == 0 )
  {
  strcpy(szFpcode,Item->fPcode->str());
  strcat(szFpcode,"STD");
  if(Resrc->seek(szFpcode) != 0)
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Item->fPcode->str());
    Resrc->appendStart();
    Resrc->fCode->assign(Item->fPcode->str());
    Resrc->fName->assign(Item->fFulln->str());
    Resrc->fShortName->assign(Item->fPname->str());
    Resrc->fCostType->assign("STD");
    Resrc->fType->assign(Item->fPtype->str());
    Resrc->fUnit->assign(Item->fUnit->str());
    if(strcmp(Item->fPtype->str(),"W ") == 0 || strcmp(Item->fPtype->str(),"WI") == 0)
    Resrc->fElement->assign("WRAP");
    if(strcmp(Item->fPtype->str(),"R ") == 0 || strcmp(Item->fPtype->str(),"RI") == 0)
    Resrc->fElement->assign("RAW");
    Resrc->fLotsize->assignDouble(double (Str4ptr(Item->fLotsize->str())) );
    Resrc->fDate->assign(Today.str());
    Resrc->fUser->assign(szUser);
    Resrc->fUpdatedate->assign(Today.str());
    Resrc->fUpdateuser->assign(szUser);
    Resrc->fRate->assignDouble(double (Str4ptr(Item->fAvprice->str())) );
    Resrc->fActCost->assignDouble(double (Str4ptr(Item->fAvprice->str ())));
    Resrc->append();
   }
  }
  Item->skip();
 }
 Item->top();
 while(!Item->eof())
 {
  strcpy(szFptype,Item->fPtype->str());
  if(strcmp(szFptype,"RI") == 0 || strcmp(szFptype,"R ") == 0 || strcmp(szFptype,"WI") == 0 || strcmp(szFptype,"W ") == 0 )
  {
  strcpy(szFpcode,Item->fPcode->str());
  strcat(szFpcode,"ACT");
  if(Resrc->seek(szFpcode) != 0)
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Item->fPcode->str());
    Resrc->appendStart();
    Resrc->fCode->assign(Item->fPcode->str());
    Resrc->fName->assign(Item->fFulln->str());
    Resrc->fCostType->assign("ACT");
    Resrc->fShortName->assign(Item->fPname->str());
    Resrc->fType->assign(Item->fPtype->str());
    Resrc->fUnit->assign(Item->fUnit->str());
    if(strcmp(Item->fPtype->str(),"W ") == 0 || strcmp(Item->fPtype->str(),"WI") == 0)
    Resrc->fElement->assign("WRAP");
    if(strcmp(Item->fPtype->str(),"R ") == 0 || strcmp(Item->fPtype->str(),"RI") == 0)
    Resrc->fElement->assign("RAW");
    Resrc->fLotsize->assignDouble(double (Str4ptr(Item->fLotsize->str())) );
    Resrc->fDate->assign(Today.str());
    Resrc->fUser->assign(szUser);
    Resrc->fUpdatedate->assign(Today.str());
    Resrc->fUpdateuser->assign(szUser);
    Resrc->fRate->assignDouble(double (Str4ptr(Item->fAvprice->str())) );
    Resrc->fActCost->assignDouble(double (Str4ptr(Item->fAvprice->str ())));
    Resrc->append();
   }
   else
   {
    SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Item->fPcode->str());
    Resrc->fCode->assign(Item->fPcode->str());
    Resrc->fName->assign(Item->fFulln->str());
    Resrc->fShortName->assign(Item->fPname->str());
    Resrc->fType->assign(Item->fPtype->str());
    Resrc->fUnit->assign(Item->fUnit->str());
    if(strcmp(Item->fPtype->str(),"W ") == 0 || strcmp(Item->fPtype->str(),"WI") == 0)
    Resrc->fElement->assign("WRAP");
    if(strcmp(Item->fPtype->str(),"R ") == 0 || strcmp(Item->fPtype->str(),"RI") == 0)
    Resrc->fElement->assign("RAW");
    Resrc->fLotsize->assignDouble(double (Str4ptr(Item->fLotsize->str())) );
    Resrc->fDate->assign(Today.str());
    Resrc->fUser->assign(szUser);
    Resrc->fUpdatedate->assign(Today.str());
    Resrc->fUpdateuser->assign(szUser);
    Resrc->fRate->assignDouble(double (Str4ptr(Item->fAvprice->str())) );
    Resrc->fActCost->assignDouble(double (Str4ptr(Item->fAvprice->str ())));
    Resrc->flush();

   }
  }
  Item->skip();
 }

 cb.closeAll();
 return TRUE;
}

void TGlWindow::SCSTransAverage   (RTMessage){}

