// Cost File by Elements                     
_CLASSDEF(TCostm)
class TCostm : public Data4
{
 public:

 Field4 *fPcode;
 Field4 *fCostype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fQuantity;
 Field4 *fActcost;

 TCostm(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costm")
 {
  top();
  SetIndex("fPcode");
  fPcode	= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fCostype	= (Field4 *) new Field4(*(Data4 *)this,"fCostype");
  fCostctn	= (Field4 *) new Field4(*(Data4 *)this,"fCostctn");
  fUnit         = (Field4 *) new Field4(*(Data4 *)this,"fUnit");
  fPeriod	= (Field4 *) new Field4(*(Data4 *)this,"fPeriod");
  fLotsize      = (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
  fShop1	= (Field4 *) new Field4(*(Data4 *)this,"fshop1");
  fSemi		= (Field4 *) new Field4(*(Data4 *)this,"fSemi");
  fQuantity	= (Field4 *) new Field4(*(Data4 *)this,"fQuantity");
  fActcost	= (Field4 *) new Field4(*(Data4 *)this,"fActcost");

 }
  double dQuantity(){return double (Str4ptr(fQuantity->str()) );}
 ~TCostm(){close();};
};

