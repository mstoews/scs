_CLASSDEF(TJiss)
class TJiss : public Data4
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


 TJiss(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_jiss")
 {
  top();
  SetIndex("fshop");
  fShop		= (Field4 *) new Field4(*(Data4 *)this,"fshop");
  fDate		= (Field4 *) new Field4(*(Data4 *)this,"fdate");
  fKbn		= (Field4 *) new Field4(*(Data4 *)this,"fkbn");
  fJcode	= (Field4 *) new Field4(*(Data4 *)this,"fjcode");
  fJname	= (Field4 *) new Field4(*(Data4 *)this,"fjname");
  fJlot		= (Field4 *) new Field4(*(Data4 *)this,"fjlot");
  fTime		= (Field4 *) new Field4(*(Data4 *)this,"ftime");
  fComme	= (Field4 *) new Field4(*(Data4 *)this,"fcomme");
  fPtype	= (Field4 *) new Field4(*(Data4 *)this,"ftype");
  fSkbn		= (Field4 *) new Field4(*(Data4 *)this,"fskbn");
  fTrnflg	= (Field4 *) new Field4(*(Data4 *)this,"ftrnflg");
 }
 ~TJiss(){close();};
};
