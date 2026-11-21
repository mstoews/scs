_CLASSDEF (Tgldist)
class Tgldist : public Data4
{
 public:
  Field4 *account;
  Field4 *dept;
  Field4 *period;
  Field4 *date;
  Field4 *batchno;
  Field4 *reference;
  Field4 *descript;
  Field4 *debit;
  Field4 *credit;
  Tgldist(Code4 *cb,LPSTR name=0):Data4(cb,"gl_dist")
  {
   top();
   account   = (Field4 *) new Field4(*(Data4 *) this, "account");
   dept      = (Field4 *) new Field4(*(Data4 *) this, "dept");
   period    = (Field4 *) new Field4(*(Data4 *) this, "period");
   date      = (Field4 *) new Field4(*(Data4 *) this, "date");
   batchno   = (Field4 *) new Field4(*(Data4 *) this, "batchno");
   reference = (Field4 *) new Field4(*(Data4 *) this, "reference");
   descript  = (Field4 *) new Field4(*(Data4 *) this, "descript");
   debit     = (Field4 *) new Field4(*(Data4 *) this, "debit");
   credit    = (Field4 *) new Field4(*(Data4 *) this, "credit");
  };
  ~Tgldist(){close();};
 };
