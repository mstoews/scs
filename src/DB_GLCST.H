_CLASSDEF(TGLcost)
class TGLcost : public Data4
{
 public:
 Field4 *fRescode;
 Field4 *fKoutei;
 Field4 *fTime;
 Field4 *fIndTime;
 Field4 *fGlcost;
 Field4 *fPeriod;

 TGLcost(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_glcst")
 {
  top();
  SetIndex("fRescode");
  fRescode	= (Field4 *) new Field4(*(Data4 *)this,"frescode");
  fKoutei	= (Field4 *) new Field4(*(Data4 *)this,"fkoutei");
  fTime		= (Field4 *) new Field4(*(Data4 *)this,"ftime");
  fIndTime	= (Field4 *) new Field4(*(Data4 *)this,"findtime");
  fGlcost	= (Field4 *) new Field4(*(Data4 *)this,"fglcost");
  fPeriod       = (Field4 *) new Field4(*(Data4 *)this,"fperiod");
 }
 double dTime() {return double (Str4ptr(fTime->str()));};
 double dIndTime() {return double (Str4ptr(fIndTime->str()));};
 ~TGLcost(){close();};
};


static FIELD4INFO SC_GLCST_Field[] =
{
   { "FRESCODE", 'C', 10, 0 },
   { "FKOUTEI" , 'C',  4, 0 },
   { "FTIME"   , 'N', 10, 1 },
   { "FINDTIME", 'N', 10, 1 },
   { "FGLCOST" , 'N', 14, 2 },
   { "FPERIOD" , 'N',  2, 0 },
   { 0,0,0,0 }
} ;

static TAG4INFO SC_GLCST_Tag[] =
{
   { "FRESCODE", "FRESCODE", 0, 0, 0 },
   { 0,0,0,0,0 }
} ;
