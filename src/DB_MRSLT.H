/*** TOPICS Manufacturing Results Data ***/

_CLASSDEF(TManResult)
class TManResult : public Data4
{
 public:
 Field4 *fOpcode;
 Field4 *fOlotno;
 Field4 *fPcode;
 Field4 *fLotno;
 Field4 *fQty;
 Field4 *fJdate;
 Field4 *fPrice;
 Field4 *fKwei;
 Field4 *fHkbn;
 Field4 *fHosei;
 Field4 *fPdate;
 Field4 *fKkbn;
 Field4 *fAmt;
 Field4 *fHosei2;
 Field4 *fStd2;

 TManResult(Code4 *cb,LPSTR name=0):Data4 (cb,"tmrslt")
 {
  top();
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fopcode");
  fOlotno	= (Field4 *) new Field4(*(Data4 *)this,"folotno");
  fPcode	= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fLotno	= (Field4 *) new Field4(*(Data4 *)this,"flotno");
  fQty		= (Field4 *) new Field4(*(Data4 *)this,"fqty");
  fJdate	= (Field4 *) new Field4(*(Data4 *)this,"fjdate");
  fPrice	= (Field4 *) new Field4(*(Data4 *)this,"fprice");
  fKwei		= (Field4 *) new Field4(*(Data4 *)this,"fkwei");
  fHkbn		= (Field4 *) new Field4(*(Data4 *)this,"fhkbn");
  fHosei	= (Field4 *) new Field4(*(Data4 *)this,"fhosei");
  fPdate	= (Field4 *) new Field4(*(Data4 *)this,"fpdate");
  fKkbn		= (Field4 *) new Field4(*(Data4 *)this,"fkkbn");
  fAmt		= (Field4 *) new Field4(*(Data4 *)this,"famt");
  fHosei2	= (Field4 *) new Field4(*(Data4 *)this,"fhosei2");
  fStd2		= (Field4 *) new Field4(*(Data4 *)this,"fstd2");
 }
 ~TManResult(){close();};
};
