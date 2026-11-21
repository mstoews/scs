/* Item master class */
_CLASSDEF(Tscitemm)
class Tscitemm : public Data4
{
 public:
  Field4 *fPcode;
  Field4 *fPname;
  Field4 *fFulln;
  Field4 *fPtype;
  Field4 *fClass;
  Field4 *fSkbn;
  Field4 *fShop1;
  Field4 *fShop2;
  Field4 *fPlace;
  Field4 *fUnit;
  Field4 *fLotsize;
  Field4 *fExfact;
  Field4 *fWeight;
  Field4 *fWunit;
  Field4 *fCapa;
  Field4 *fCunit;
  Field4 *fSunit;
  Field4 *fOdtp;
  Field4 *fLtime;
  Field4 *fOnhand;
  Field4 *fOdrz;
  Field4 *fReqz;
  Field4 *fLoss;
  Field4 *fSafety;
  Field4 *fOdpoint;
  Field4 *fPrice;
  Field4 *fHosei;
  Field4 *fGan1;
  Field4 *fGan2;
  Field4 *fTkbn;
  Field4 *fComm;
  Field4 *fMax;
  Field4 *fSamp1;
  Field4 *fSamp2;
  Field4 *fEffectd;
  Field4 *fLmhand;
  Field4 *fLmprice;
  Field4 *fLmdate;
  Field4 *fAvprice;
  Field4 *fMfgflg;

  Tscitemm(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_itemm")
  {
  fPcode 	= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fPname 	= (Field4 *) new Field4(*(Data4 *)this,"fpname");
  fFulln 	= (Field4 *) new Field4(*(Data4 *)this,"ffulln");
  fPtype 	= (Field4 *) new Field4(*(Data4 *)this,"fptype");
  fClass 	= (Field4 *) new Field4(*(Data4 *)this,"fclass");
  fSkbn  	= (Field4 *) new Field4(*(Data4 *)this,"fskbn");
  fShop1 	= (Field4 *) new Field4(*(Data4 *)this,"fshop1");
  fShop2	= (Field4 *) new Field4(*(Data4 *)this,"fshop2");
  fPlace 	= (Field4 *) new Field4(*(Data4 *)this,"fplace");
  fUnit  	= (Field4 *) new Field4(*(Data4 *)this,"funit");
  fLotsize	= (Field4 *) new Field4(*(Data4 *)this,"flotsize");
  fExfact	= (Field4 *) new Field4(*(Data4 *)this,"fexfact");
  fWeight	= (Field4 *) new Field4(*(Data4 *)this,"fweight");
  fWunit	= (Field4 *) new Field4(*(Data4 *)this,"fwunit");
  fCapa		= (Field4 *) new Field4(*(Data4 *)this,"fcapa");
  fCunit	= (Field4 *) new Field4(*(Data4 *)this,"fcunit");
  fSunit	= (Field4 *) new Field4(*(Data4 *)this,"fsunit");
  fOdtp		= (Field4 *) new Field4(*(Data4 *)this,"fodtp");
  fLtime	= (Field4 *) new Field4(*(Data4 *)this,"fltime");
  fOnhand	= (Field4 *) new Field4(*(Data4 *)this,"fonhand");
  fOdrz		= (Field4 *) new Field4(*(Data4 *)this,"fodrz");
  fReqz		= (Field4 *) new Field4(*(Data4 *)this,"freqz");
  fLoss		= (Field4 *) new Field4(*(Data4 *)this,"floss");
  fSafety	= (Field4 *) new Field4(*(Data4 *)this,"fsafety");
  fOdpoint	= (Field4 *) new Field4(*(Data4 *)this,"fodpoint");
  fPrice	= (Field4 *) new Field4(*(Data4 *)this,"fprice");
  fHosei	= (Field4 *) new Field4(*(Data4 *)this,"fhosei");
  fGan1		= (Field4 *) new Field4(*(Data4 *)this,"fgan1");
  fGan2		= (Field4 *) new Field4(*(Data4 *)this,"fgan2");
  fTkbn		= (Field4 *) new Field4(*(Data4 *)this,"ftkbn");
  fComm		= (Field4 *) new Field4(*(Data4 *)this,"fcomm");
  fMax		= (Field4 *) new Field4(*(Data4 *)this,"fmax");
  fSamp1	= (Field4 *) new Field4(*(Data4 *)this,"fsamp1");
  fSamp2	= (Field4 *) new Field4(*(Data4 *)this,"fsamp2");
  fEffectd	= (Field4 *) new Field4(*(Data4 *)this,"feffectd");
  fLmhand	= (Field4 *) new Field4(*(Data4 *)this,"flmhand");
  fLmdate	= (Field4 *) new Field4(*(Data4 *)this,"flmdate");
  fLmprice	= (Field4 *) new Field4(*(Data4 *)this,"flmprice");
  fAvprice	= (Field4 *) new Field4(*(Data4 *)this,"favprice");
  fMfgflg	= (Field4 *) new Field4(*(Data4 *)this,"fmfgdflg");
  }
  virtual void Man(){SetIndex("MAN");};
  ~Tscitemm(){close();};
 };

