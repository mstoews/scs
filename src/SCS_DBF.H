_CLASSDEF(Tscalloc)
class Tscalloc :public Data4
{
 public:

  Field4 *fcallcode;
  Field4 *fdescript;
  Field4 *fcostctr;
  Field4 *fcallcrate;
  Field4 *fcallccoef;

  Tscalloc(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_alloc")
  {
  fcallcode    = (Field4 *) new Field4(*(Data4 *)this,"fcallcode");
  fdescript    = (Field4 *) new Field4(*(Data4 *)this,"fdescript");
  fcostctr     = (Field4 *) new Field4(*(Data4 *)this,"fcostctr");
  fcallcrate   = (Field4 *) new Field4(*(Data4 *)this,"fcallcrate");
  fcallccoef   = (Field4 *) new Field4(*(Data4 *)this,"fcallccoef");
  };
 ~Tscalloc(){close();};
  double dFrate() { return double(Str4ptr(fcallcrate->str()));};
  double dFcoef()   { return double(Str4ptr(fcallccoef->str()));};
};

_CLASSDEF(Tsccntre)
class Tsccntre :public Data4
{
 public:

 Field4 *fcostctr;
 Field4 *fglaccnt;
 Field4 *fgldepart;
 Field4 *fdescript;
 Tsccntre(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_cntre")
 {
  fcostctr    = (Field4 *) new Field4(*(Data4 *)this,"fcostctr");
  fglaccnt    = (Field4 *) new Field4(*(Data4 *)this,"fglaccnt");
  fgldepart   = (Field4 *) new Field4(*(Data4 *)this,"fgldepart");
  fdescript   = (Field4 *) new Field4(*(Data4 *)this,"fdescript");
 }
 ~Tsccntre(){close();};
};

_CLASSDEF(Tsccosth)
class Tsccosth : public Data4
{
 public:
  Field4 *fpcode;
  Field4 *fskbn;
  Field4 *felemno;
  Field4 *frsccd;
  Field4 *fcallcode;
  Field4 *fdatec;
  Field4 *fdateu;
  Tsccosth(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costh")
  {
   fpcode     = (Field4 *) new Field4(*(Data4 *)this,"fpcode");
   fskbn      = (Field4 *) new Field4(*(Data4 *)this,"fskbn");
   felemno    = (Field4 *) new Field4(*(Data4 *)this,"felemno");
   frsccd     = (Field4 *) new Field4(*(Data4 *)this,"frsccd");
   fcallcode  = (Field4 *) new Field4(*(Data4 *)this,"fcallcode");
   fdatec     = (Field4 *) new Field4(*(Data4 *)this,"fdatec");
   fdateu     = (Field4 *) new Field4(*(Data4 *)this,"fdateu");
  }
  ~Tsccosth(){close();};
 };

_CLASSDEF(Tsccostm)
class Tsccostm : public Data4
{
 public:
  Field4 *fpcode;
  Field4 *fpname;
  Field4 *fcosttypcd;
  Field4 *felemno;
  Field4 *flot;
  Field4 *frsccd;
  Field4 *fcallcode;
  Field4 *fdatec;
  Field4 *fdateu;
  Field4 *fcost;
  Field4 *fquantity;
  Field4 *frate;

  Tsccostm(Code4 *cb,LPSTR name=0):Data4 (cb,"sc_costm")
  {
   fpcode = (Field4 *) new Field4(*(Data4 *)this,"fpcode");
   fpname      = (Field4 *) new Field4(*(Data4 *)this,"fpname");
   fcosttypcd  = (Field4 *) new Field4(*(Data4 *)this,"fcosttypcd");
   felemno     = (Field4 *) new Field4(*(Data4 *)this,"felemno");
   fcallcode   = (Field4 *) new Field4(*(Data4 *)this,"fcallcode");
   fdatec      = (Field4 *) new Field4(*(Data4 *)this,"fdatec");
   fdateu      = (Field4 *) new Field4(*(Data4 *)this,"fdateu");
   fcost       = (Field4 *) new Field4(*(Data4 *)this,"fcost");
   fquantity   = (Field4 *) new Field4(*(Data4 *)this,"fquantity");
   frate       = (Field4 *) new Field4(*(Data4 *)this,"frate");
  }

  ~Tsccostm(){close();};
 };
