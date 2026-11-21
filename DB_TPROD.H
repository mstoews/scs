_CLASSDEF (TProd)
class TProd : public Data4
{
 public:
 Field4 *fParent;
 Field4 *fLotsize;
 Field4 *fProd;

 TProd(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_ptemp")
 {
  top();
  SetIndex("fparent");
  fParent	= (Field4 *) new Field4(*(Data4 *)this,"fparent");
  fLotsize	= (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
  fProd		= (Field4 *) new Field4(*(Data4 *)this,"fprod");
 }
 double dLotsize() {return double(Str4ptr(fLotsize->str()));};
 double dProd()   {return double(Str4ptr(fProd->str()));};
 ~TProd(){close();};
};
// Cost File by Elements
_CLASSDEF(TProduct)
class TProduct : public Data4
{
 public:

 Field4 *fOpcode;
 Field4 *fCosttype;
 Field4 *fCostctn;  
 Field4 *fPeriod;
 Field4 *fUnit;
 Field4 *fLotsize;
 Field4 *fShop1;
 Field4 *fSemi;
 Field4 *fElement;
 Field4 *fRatio;
 Field4 *fMaterial;
 Field4 *fLabour;
 Field4 *fOverhead;
 Field4 *fIndirect;

 TProduct(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_prod")
 {
  top();
  SetIndex("fopcode");
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fOpcode");
  fCosttype     = (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
  fCostctn	= (Field4 *) new Field4(*(Data4 *)this,"fCostctn");
  fUnit         = (Field4 *) new Field4(*(Data4 *)this,"fUnit");
  fPeriod	= (Field4 *) new Field4(*(Data4 *)this,"fPeriod");
  fLotsize      = (Field4 *) new Field4(*(Data4 *)this,"fLotsize");
  fShop1	= (Field4 *) new Field4(*(Data4 *)this,"fshop1");
  fSemi		= (Field4 *) new Field4(*(Data4 *)this,"fSemi");
  fElement	= (Field4 *) new Field4(*(Data4 *)this,"fElement");
  fRatio	= (Field4 *) new Field4(*(Data4 *)this,"fRatio");
  fMaterial	= (Field4 *) new Field4(*(Data4 *)this,"fMaterial");
  fLabour  	= (Field4 *) new Field4(*(Data4 *)this,"fLabour");
  fOverhead	= (Field4 *) new Field4(*(Data4 *)this,"fOverhead");
  fIndirect	= (Field4 *) new Field4(*(Data4 *)this,"fIndirect");
 }
  double dLotSize()   { return double (Str4ptr(fLotsize->str()) ); }
  double dRatio()     { return double (Str4ptr(fRatio->str()) ); }
  double dMaterial()  { return double (Str4ptr(fMaterial->str()) ); }
  double dLabour()    { return double (Str4ptr(fLabour->str()) ); }
  double dOverhead()  { return double (Str4ptr(fOverhead->str()) ); }
  double dIndirect()  { return double (Str4ptr(fIndirect->str()) ); }

 ~TProduct(){close();};
};

