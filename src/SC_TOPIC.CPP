#include "scs.h"
#include <alloc.h>
/* Database Files */

#include "db_topic.h"
#include "db_mrslt.h"
#include "db_jiss.h"
#include "db_resrc.h"
#include "db_trstr.h"
#include "sc_topic.h"

BOOL TTopics::ResourceStruct(void)
{
 char far * lpszKey = GetLP(45);
 char far * lpszStd = GetLP(4);
 BOOL bChanged = FALSE;
 TopicsCoststr = new TTopicsTpstr(&cb);
 CostStr       = new Ttrstr(&cb);
 
 SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)"Standard Cost Structure Master Update");
 TopicsCoststr->SetIndex("fopcode2");
 CostStr->SetIndex("fopcode2");
 lstrcpy(lpszStd,"STD");
 TopicsCoststr->top();
 while(!TopicsCoststr->eof())
 {              //ftype+fopcode+fkpcode+fseqno
   lstrcpy(lpszKey,lpszStd);
   lstrcat(lpszKey,TopicsCoststr->fOpcode->str());
   lstrcat(lpszKey,TopicsCoststr->fKpcode->str());
   lstrcat(lpszKey,TopicsCoststr->fSeqno->str());
   SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)lpszKey);
   if(CostStr->seek(lpszKey) != 0)
   {    
       SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsCoststr->fOpcode->str());
       CostStr->appendStart();
       CostStr->fOpcode->assign(TopicsCoststr->fOpcode->str());
       CostStr->fType->assign(lpszStd);
       CostStr->fSeqno->assign(TopicsCoststr->fSeqno->str());
       CostStr->fKpcode->assign(TopicsCoststr->fKpcode->str());
       CostStr->fStqty->assign(TopicsCoststr->fStqty->str());
       CostStr->fKousei->assign(TopicsCoststr->fKousei->str());
       CostStr->fLt1->assign(TopicsCoststr->fLt1->str());
       CostStr->fLt2->assign(TopicsCoststr->fLt2->str());
       CostStr->fComm->assign(TopicsCoststr->fComm->str());
       CostStr->fCorectm->assign(TopicsCoststr->fCorectm->str());
       CostStr->fSpcrtp->assign(TopicsCoststr->fSpcrtp->str());
       CostStr->fChk_flg->assign(TopicsCoststr->fChk_flg->str());
       CostStr->append();
       bChanged = TRUE;
   }
   TopicsCoststr->skip(1);
 }
 if(bChanged)
 //if(BWCCMessageBox(HWindow,"The Standard Cost Structure has changed. Do you want to modify the Actual Cost Structure?","Update Actual Structure",
 if(BWCCMessageBox(HWindow,"","Update Actual Structure",
   MB_ICONQUESTION | MB_YESNO) == IDYES)
 {
  char lpszStd[] = "ACT";
  char far * lpszKey = GetLP(45);
  TopicsCoststr->top();
  while(!TopicsCoststr->eof())
  {              //ftype+fopcode+fkpcode+fseqno
   lstrcpy(lpszKey,lpszStd);
   lstrcat(lpszKey,TopicsCoststr->fOpcode->str());
   lstrcat(lpszKey,TopicsCoststr->fKpcode->str());
   lstrcat(lpszKey,TopicsCoststr->fSeqno->str());
   if(CostStr->seek(lpszKey) != 0)
   {    
       SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsCoststr->fOpcode->str());
       CostStr->appendStart();
       CostStr->fOpcode->assign(TopicsCoststr->fOpcode->str());
       CostStr->fType->assign(lpszStd);
       CostStr->fSeqno->assign(TopicsCoststr->fSeqno->str());
       CostStr->fKpcode->assign(TopicsCoststr->fKpcode->str());
       CostStr->fStqty->assign(TopicsCoststr->fStqty->str());
       CostStr->fKousei->assign(TopicsCoststr->fKousei->str());
       CostStr->fLt1->assign(TopicsCoststr->fLt1->str());
       CostStr->fLt2->assign(TopicsCoststr->fLt2->str());
       CostStr->fComm->assign(TopicsCoststr->fComm->str());
       CostStr->fCorectm->assign(TopicsCoststr->fCorectm->str());
       CostStr->fSpcrtp->assign(TopicsCoststr->fSpcrtp->str());
       CostStr->fChk_flg->assign(TopicsCoststr->fChk_flg->str());
       CostStr->append();
       bChanged = TRUE;
   }
   TopicsCoststr->skip(1);
  }
 }
 cb.closeAll();
 return TRUE;

}



BOOL TTopics::ItemMaster()
{
 TopicsItem = new TTopicsItemm(&cb);
 TopicsItem->SetIndex("fpcode");

 Item       = new Tscitemm(&cb);
 Item->SetIndex("fpcode");
 SendDlgItemMsg(ID_DESCRIPT,WM_SETTEXT,NULL,(LPARAM)(LPSTR)"Standard Cost Item Master Update");
 TopicsItem->top(); 
 while(!TopicsItem->eof())
  {
   if(Item->seek(TopicsItem->fPcode->str()) == 0){
    Item->fOnhand->assign(TopicsItem->fOnhand->str());
    Item->fPrice->assign(TopicsItem->fPrice->str());
    Item->fEffectd->assign(TopicsItem->fEffectd->str());
    Item->fLmprice->assign(TopicsItem->fLmprice->str());
    Item->fLmdate->assign(TopicsItem->fLmdate->str());
    Item->fAvprice->assign(TopicsItem->fAvprice->str());
    Item->fMfgflg->assign(TopicsItem->fMfgflg->str());
   }
   else
   {
    Item->appendStart();
    Item->fPcode->assign(TopicsItem->fPcode->str());
    Item->fPname->assign(TopicsItem->fPname->str());
    Item->fFulln->assign(TopicsItem->fFulln->str());
    Item->fPtype->assign(TopicsItem->fPtype->str());
    Item->fClass->assign(TopicsItem->fClass->str());
    Item->fSkbn->assign(TopicsItem->fSkbn->str());
    Item->fShop1->assign(TopicsItem->fShop1->str());
    Item->fShop2->assign(TopicsItem->fShop2->str());
    Item->fPlace->assign(TopicsItem->fPlace->str());
    Item->fUnit->assign(TopicsItem->fUnit->str());
    Item->fLotsize->assign(TopicsItem->fLotsize->str());
    Item->fExfact->assign(TopicsItem->fExfact->str());
    Item->fWeight->assign(TopicsItem->fWeight->str());
    Item->fWunit->assign(TopicsItem->fWunit->str());
    Item->fCapa->assign(TopicsItem->fCapa->str());
    Item->fCunit->assign(TopicsItem->fCunit->str());
    Item->fSunit->assign(TopicsItem->fSunit->str());
    Item->fOnhand->assign(TopicsItem->fOnhand->str());
    Item->fPrice->assign(TopicsItem->fPrice->str());
    Item->fEffectd->assign(TopicsItem->fEffectd->str());
    Item->fLmprice->assign(TopicsItem->fLmprice->str());
    Item->fLmdate->assign(TopicsItem->fLmdate->str());
    Item->fAvprice->assign(TopicsItem->fAvprice->str());
    Item->fMfgflg->assign(TopicsItem->fMfgflg->str());
    Item->append();
   }
   SendDlgItemMsg(101,WM_SETTEXT,NULL,(LPARAM)(LPSTR)Item->fPcode->str());
   TopicsItem->skip();
  }
 Item->flush();
 TopicsItem->close();
 Item->close();
 return TRUE;
}


BOOL TTopics::Jobtime()
{
 Jiss = new TJiss(&cb);
 TopicsJiss = new TTopicsJiss(&cb);
 Date4 Today,Update;
 char szUpdateDate[9],szDate[9];
 strcpy(szDate,GetField(ID_DATE));
 strcpy(szUpdateDate,GetField(ID_DATEUPDATED));
 Today.assign(szDate,"MM/DD/YY");
 Update.assign(szUpdateDate,"MM/DD/YY");

 if(strcmp(Today.str(),Update.str()) < 0){
  BWCCMessageBox(HWindow,"Update date must be earlier than today.","Update SCS from TOPICS",MB_ICONEXCLAMATION);
  return FALSE;
 }
 
 TopicsJiss->top();
 while (!TopicsJiss->eof())
  {
   if(strcmp(Update.str(),TopicsJiss->fDate->str()) < 0)
   {
    SendDlgItemMsg(103,WM_SETTEXT,NULL,(LPARAM)(LPSTR)TopicsJiss->fJcode->str());
    Jiss->appendStart();
    Jiss->fShop->assign  (TopicsJiss->fShop->str());
    Jiss->fDate->assign  (TopicsJiss->fDate->str());
    Jiss->fKbn->assign   (TopicsJiss->fKbn->str());
    Jiss->fJname->assign (TopicsJiss->fJname->str());
    Jiss->fJlot->assign  (TopicsJiss->fJlot->str());
    Jiss->fTime->assign  (TopicsJiss->fTime->str());
    Jiss->fComme->assign (TopicsJiss->fComme->str());
    Jiss->fPtype->assign (TopicsJiss->fPtype->str());
    Jiss->fSkbn->assign  (TopicsJiss->fSkbn->str());
    Jiss->fTrnflg->assign(TopicsJiss->fTrnflg->str());
    Jiss->append();
   }
   TopicsJiss->skip(1);
  }
 cb.closeAll();
 return TRUE;
}
char *TTopics::GetField(int ID)
{
 DWORD len =  SendDlgItemMsg(ID,WM_GETTEXTLENGTH,NULL,NULL);
 char *szTemp = new char[(int)len + 1];
 SendDlgItemMsg(ID,WM_GETTEXT,len + 1,(LPARAM)(LPSTR)szTemp);
 return szTemp;
}
