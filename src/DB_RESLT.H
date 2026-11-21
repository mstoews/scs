_CLASSDEF(TResults)
class TResults : public Data4
{
 public:
 Field4 *fParent;
 Field4 *fCostype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fElement;
 Field4 *fLabour;
 Field4 *fMaterial;
 Field4 *fOH;
 Field4 *fIndirect;
 Field4 *fRate;
 Field4 *fCost;


 TResults(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_rslts")
 {
  top();
  SetIndex("fParent");
  fParent	= (Field4 *) new Field4(*(Data4 *)this,"fParent");
  fCostype	= (Field4 *) new Field4(*(Data4 *)this,"fCosttype");
  fCostctn	= (Field4 *) new Field4(*(Data4 *)this,"fCostctn");
  fUnit         = (Field4 *) new Field4(*(Data4 *)this,"fUnit");
  fPeriod	= (Field4 *) new Field4(*(Data4 *)this,"fPeriod");
  fLotsize      = (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
  fShop1	= (Field4 *) new Field4(*(Data4 *)this,"fshop1");
  fSemi		= (Field4 *) new Field4(*(Data4 *)this,"fSemi");
  fElement	= (Field4 *) new Field4(*(Data4 *)this,"fElement");
  fLabour	= (Field4 *) new Field4(*(Data4 *)this,"fLabour");
  fMaterial	= (Field4 *) new Field4(*(Data4 *)this,"fMaterial");
  fOH		= (Field4 *) new Field4(*(Data4 *)this,"fOH");
  fIndirect	= (Field4 *) new Field4(*(Data4 *)this,"fIndirect");
  fCost		= (Field4 *) new Field4(*(Data4 *)this,"fCost");  
  fRate		= (Field4 *) new Field4(*(Data4 *)this,"fRate");
 }
  double dLotSize()  { return double (Str4ptr(fLotsize->str()) ); }
  double dRate()     { return double (Str4ptr(fRate->str()) ); }
  
 ~TResults(){close();};
};



