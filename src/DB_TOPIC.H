/* TOPICS Database Files Used for transferring data */

_CLASSDEF(TTopicsItemm)
class TTopicsItemm : public Data4
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
  Field4 *fLmdate;
  Field4 *fLmprice;
  Field4 *fAvprice;
  Field4 *fMfgflg;

  TTopicsItemm(Code4 *cb,LPSTR name=0):Data4 (cb,"titemm")
  {
  SetIndex("fpcode");
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
  ~TTopicsItemm(){close();};
 };

_CLASSDEF(TTopicsJiss)
class TTopicsJiss : public Data4
{
 public:
 Field4 *fShop;
 Field4	*fDate;
 Field4	*fKbn;
 Field4	*fJcode;
 Field4	*fJname;
 Field4	*fJlot;
 Field4	*fTime;
 Field4	*fComme;
 Field4	*fPtype;
 Field4	*fSkbn;
 Field4	*fTrnflg;


 TTopicsJiss(Code4 *cb,LPSTR name=0):Data4 (cb,"tjiss")
 {
  top();
  fShop		= (Field4 *) new Field4(*(Data4 *)this,"fshop");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
  fKbn		= (Field4 *) new Field4(*(Data4 *)this,"fkbn");
  fJcode	= (Field4 *) new Field4(*(Data4 *)this,"fjcode");
  fJname	= (Field4 *) new Field4(*(Data4 *)this,"fjname");
  fJlot		= (Field4 *) new Field4(*(Data4 *)this,"fjlot");
  fTime		= (Field4 *) new Field4(*(Data4 *)this,"ftime");
  fComme	= (Field4 *) new Field4(*(Data4 *)this,"fcomme");
  fPtype	= (Field4 *) new Field4(*(Data4 *)this,"fptype");
  fSkbn		= (Field4 *) new Field4(*(Data4 *)this,"fskbn");
  fTrnflg	= (Field4 *) new Field4(*(Data4 *)this,"ftrnflg");
 }
 ~TTopicsJiss(){close();};
 double dTime() {return double (Str4ptr(fTime->str()));};
};
_CLASSDEF(TTopicsThrm)
class TTopicsThrm : public Data4
{
 public:
 Field4 *fShop;
 Field4	*fDate;
 Field4	*fInphr;
 Field4	*fTeiji;
 Field4	*fZang1;
 Field4	*fZang2;
 Field4	*fZang3;
 Field4	*fRecv;
 Field4	*fHelp;
 Field4	*fBhelp1;
 Field4	*fBhelp2;
 Field4	*fBhelp3;
 Field4	*fHelp1;
 Field4	*fHelp2;
 Field4	*fHelp3;


 TTopicsThrm(Code4 *cb,LPSTR name=0):Data4 (cb,"thrm")
 {
  top();
  fShop		= (Field4 *) new Field4(*(Data4 *)this,"fshop");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
  fInphr	= (Field4 *) new Field4(*(Data4 *)this,"finphr");
  fTeiji	= (Field4 *) new Field4(*(Data4 *)this,"fteiji");
  fZang1	= (Field4 *) new Field4(*(Data4 *)this,"fzang1");
  fZang2	= (Field4 *) new Field4(*(Data4 *)this,"fzang2");
  fZang3	= (Field4 *) new Field4(*(Data4 *)this,"fzang3");
  fRecv		= (Field4 *) new Field4(*(Data4 *)this,"frecv");
  fHelp		= (Field4 *) new Field4(*(Data4 *)this,"fhelp");
  fBhelp1	= (Field4 *) new Field4(*(Data4 *)this,"fbhelp1");
  fBhelp2	= (Field4 *) new Field4(*(Data4 *)this,"fbhelp2");
  fBhelp3	= (Field4 *) new Field4(*(Data4 *)this,"fbhelp3");
  fHelp1	= (Field4 *) new Field4(*(Data4 *)this,"fhelp1");
  fHelp2	= (Field4 *) new Field4(*(Data4 *)this,"fhelp2");
  fHelp3	= (Field4 *) new Field4(*(Data4 *)this,"fhelp3");

 }
 ~TTopicsThrm(){close();};
};


/* Composition Master Class */
_CLASSDEF(TTopicsTpstr)
class TTopicsTpstr : public Data4
{
 public:
  Field4 *fOpcode;
  Field4 *fSeqno;
  Field4 *fKpcode;
  Field4 *fStqty;
  Field4 *fKousei;
  Field4 *fLt1;
  Field4 *fLt2;
  Field4 *fComm;
  Field4 *fCorectm;
  Field4 *fSpcrtp;
  Field4 *fChk_flg;

  TTopicsTpstr(Code4 *cb,LPSTR name=0):Data4 (cb,"tpstr")
  {
  SetIndex("fopcode");
  fOpcode	= (Field4 *) new Field4(*(Data4 *)this,"fopcode");
  fSeqno	= (Field4 *) new Field4(*(Data4 *)this,"fseqno");
  fKpcode	= (Field4 *) new Field4(*(Data4 *)this,"fkpcode");
  fStqty	= (Field4 *) new Field4(*(Data4 *)this,"fstqty");
  fKousei	= (Field4 *) new Field4(*(Data4 *)this,"fkousei");
  fLt1		= (Field4 *) new Field4(*(Data4 *)this,"flt1");
  fLt2		= (Field4 *) new Field4(*(Data4 *)this,"flt2");
  fComm		= (Field4 *) new Field4(*(Data4 *)this,"fcomm");
  fCorectm	= (Field4 *) new Field4(*(Data4 *)this,"fcorectm");
  fSpcrtp	= (Field4 *) new Field4(*(Data4 *)this,"fspcrtp");
  fChk_flg	= (Field4 *) new Field4(*(Data4 *)this,"fchk_flg");
  }
  double dQuantity() {return double(Str4ptr(fStqty->str()));};
  double dKousei()   {return double(Str4ptr(fKousei->str()));};
  ~TTopicsTpstr(){close();};
 };

_CLASSDEF(TTopicsTmast)
class TTopicsTmast : public Data4
{
 public:
  Field4 *fPcode;
  Field4 *fLot;
  Field4 *fJdate;
  Field4 *fInqty;
  Field4 *fOutqty;
  Field4 *fPtype;
  Field4 *fSkbn;
  Field4 *fMfgdate;

  TTopicsTmast(Code4 *cb,LPSTR name=0):Data4 (cb,"tmmast")
  {
  SetIndex("fpcode");
  fPcode	= (Field4 *) new Field4(*(Data4 *)this,"fpcode");
  fLot		= (Field4 *) new Field4(*(Data4 *)this,"flot");
  fJdate	= (Field4 *) new Field4(*(Data4 *)this,"fjdate");
  fInqty	= (Field4 *) new Field4(*(Data4 *)this,"finqty");
  fOutqty	= (Field4 *) new Field4(*(Data4 *)this,"foutqty");
  fPtype	= (Field4 *) new Field4(*(Data4 *)this,"fptype");
  fSkbn		= (Field4 *) new Field4(*(Data4 *)this,"fskbn");
  fMfgdate	= (Field4 *) new Field4(*(Data4 *)this,"fmfgdate");
  }
  ~TTopicsTmast(){close();};
 };







