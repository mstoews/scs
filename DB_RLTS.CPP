// Cost File by Elements                     
_CLASSDEF(TCoste)
class TCoste : public Data4
{
 public:
 Field4 *fParent;
 Field4 *fOpcode;
 Field4 *fLevel;
 Field4 *fSeqno;
 Field4 *fKpcode;
 Field4 *fCostype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fElement;
 Field4 *fRatio;
 Field4 *fRate;
 Field4 *fActRate;
 Field4 *fQuantity;
 Field4 *fCost;
 Field4 *fJobtime;


 TCoste(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_coste")
 {
  top();
  SetIndex("Fparent");
  fParent	= (Field4 *) new Field4(*(Data4 *)this,"fParent");
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
  fElement	= (Field4 *) new Field4(*(Data4 *)this,"fElement");
  fCost		= (Field4 *) new Field4(*(Data4 *)this,"fCost");
  fRatio	= (Field4 *) new Field4(*(Data4 *)this,"fRatio");
  fRate		= (Field4 *) new Field4(*(Data4 *)this,"fRate");
  fActRate	= (Field4 *) new Field4(*(Data4 *)this,"fActRate");
  fQuantity	= (Field4 *) new Field4(*(Data4 *)this,"fQuantity");
  fJobtime	= (Field4 *) new Field4(*(Data4 *)this,"fJobtime");
 }
  double dLotSize()  { return double (Str4ptr(fLotsize->str()) ); }
  double dRate()     { return double (Str4ptr(fRate->str()) ); }
  double dQuantity() { return double (Str4ptr(fQuantity->str()) ); }
  double dActRate()  { return double (Str4ptr(fActRate->str()) ); }
 ~TCoste(){close();};
};

_CLASSDEF(TCost)
class TCost : public Data4
{
 public:
 Field4 *fParent;
 Field4 *fOpcode;
 Field4 *fLevel;
 Field4 *fSeqno;
 Field4 *fKpcode;
 Field4 *fCostype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fElement;
 Field4 *fRatio;
 Field4 *fRate;
 Field4 *fActRate;
 Field4 *fQuantity;
 Field4 *fCost;
 Field4 *fJobtime;


 TCost(Code4 *cb,LPSTR name):Data4 (cb,name)
 {
  top();
  SetIndex("Fparent");
  fParent	= (Field4 *) new Field4(*(Data4 *)this,"fParent");
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
  fElement	= (Field4 *) new Field4(*(Data4 *)this,"fElement");
  fCost		= (Field4 *) new Field4(*(Data4 *)this,"fCost");
  fRatio	= (Field4 *) new Field4(*(Data4 *)this,"fRatio");
  fRate		= (Field4 *) new Field4(*(Data4 *)this,"fRate");
  fActRate	= (Field4 *) new Field4(*(Data4 *)this,"fActRate");
  fQuantity	= (Field4 *) new Field4(*(Data4 *)this,"fQuantity");
  fJobtime	= (Field4 *) new Field4(*(Data4 *)this,"fJobtime");
 }
  double dLotSize()  { return double (Str4ptr(fLotsize->str()) ); }
  double dRate()     { return double (Str4ptr(fRate->str()) ); }
  double dQuantity() { return double (Str4ptr(fQuantity->str()) ); }
  double dActRate()  { return double (Str4ptr(fActRate->str()) ); }
 ~TCost(){close();};
};



FIELD4INFO coste[] =
 {
 { "fparent"	,'C',10,0},
 { "fopcode"	,'C',10,0},
 { "fLevel"	,'N',2,0},
 { "fSeqno"	,'N',3,0},
 { "fKpcode"	,'C',10,0},
 { "fCostype"	,'C',4,0},
 { "fCostctn"	,'C',10,0},
 { "fPeriod"	,'C',2,0},
 { "fUnit"	,'C',3,0},
 { "fLotsize"	,'C',10,0},
 { "fshop1"     ,'C',4,0},
 { "fSemi"	,'C',4,0},
 { "fElement"	,'C',4,0},
 { "fRatio"	,'N',14,6},
 { "fCost"	,'N',14,6},
 { "fRate"	,'N',14,6},
 { "fActRate"	,'N',14,6},
 { "fQuantity"	,'N',14,6},
 { "fJobtime"	,'N',7,2},
 { 0,0,0,0}
 };

 TAG4INFO costetag[] =
 {
 { "fparent","fparent",0,0,0},
 { 0,0,0,0,0}
 };

