// Cost File                     
_CLASSDEF(TCostd)
class TCostd : public Data4
{
 public:
 Field4 *fLevel;
 Field4 *fOpcode;
 Field4 *fSeqno;
 Field4 *fKpcode;
 Field4 *fCostype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fMaterial;
 Field4 *fOverhead;
 Field4 *fLabour;
 Field4 *fIndirectO;
 Field4 *fJobtime;


 TCostd(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costd")
 {
  top();
  SetIndex("Fopcode");
  fLevel	= (Field4 *) new Field4(*(Data4 *)this,"fLevel");
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fOpcode");
  fSeqno 	= (Field4 *) new Field4(*(Data4 *)this,"fSeqno");
  fKpcode	= (Field4 *) new Field4(*(Data4 *)this,"fKpcode");
  fCostype	= (Field4 *) new Field4(*(Data4 *)this,"fCostype");
  fCostctn	= (Field4 *) new Field4(*(Data4 *)this,"fCostctn");
  fUnit         = (Field4 *) new Field4(*(Data4 *)this,"fUnit");
  fPeriod	= (Field4 *) new Field4(*(Data4 *)this,"fPeriod");
  fLotsize      = (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
  fShop1	= (Field4 *) new Field4(*(Data4 *)this,"fshop1");
  fSemi		= (Field4 *) new Field4(*(Data4 *)this,"fSemi");
  fMaterial	= (Field4 *) new Field4(*(Data4 *)this,"fMaterial");
  fOverhead	= (Field4 *) new Field4(*(Data4 *)this,"fOverhead");
  fLabour	= (Field4 *) new Field4(*(Data4 *)this,"fLabour");
  fIndirectO	= (Field4 *) new Field4(*(Data4 *)this,"fIndirectO");
  fJobtime	= (Field4 *) new Field4(*(Data4 *)this,"fJobtime");
 }
  double dQuantity()
  {
   return double (Str4ptr(fLotsize->str()) );
  }
 ~TCostd(){close();};
};


