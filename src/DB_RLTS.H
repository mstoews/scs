// Cost File by Elements
_CLASSDEF(TResults)
class TResults : public Data4
{
 public:
 Field4 *fParent;
 Field4 *fLotno;
 Field4 *fOpcode;
 Field4 *fKpcode;
 Field4 *fElement;
 Field4 *fCostType;
 Field4 *fLots;
 Field4 *fActRate;
 Field4 *fActQty;

 TResults(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_rslts")
 {
  top();
  SetIndex("Fparent");
  fParent	= (Field4 *) new Field4(*(Data4 *)this,"fParent");
  fLotno	= (Field4 *) new Field4(*(Data4 *)this,"fLotno");
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fOpcode");
  fKpcode	= (Field4 *) new Field4(*(Data4 *)this,"fKpcode");
  fElement	= (Field4 *) new Field4(*(Data4 *)this,"felmncd");
  fCostType     = (Field4 *) new Field4(*(Data4 *)this,"fcosttype");
  fLots 	= (Field4 *) new Field4(*(Data4 *)this,"fLots");
  fActRate	= (Field4 *) new Field4(*(Data4 *)this,"fActRate");
  fActQty	= (Field4 *) new Field4(*(Data4 *)this,"fActQty");
 }
  double dActRate()     { return double (Str4ptr(fActRate->str()) ); }
  double dActQty()      { return double (Str4ptr(fActQty->str()) ); }
 ~TResults(){close();};
};



