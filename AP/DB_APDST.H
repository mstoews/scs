#ifndef __DB_APDST_H
#define __DB_APDST_H
_CLASSDEF (Tapdist)
class Tapdist : public Data4
{
 public:
  Field4 *account;
  Field4 *dept;
  Field4 *apaccount;
  Field4 *apdepart;
  Field4 *period;
  Field4 *date;
  Field4 *duedate;
  Field4 *batchno;
  Field4 *invoiceno;
  Field4 *paymentctl;
  Field4 *vendorno;
  Field4 *chequeno;
  Field4 *reference;
  Field4 *descript;
  Field4 *amount;
  Field4 *paymentreq;
  Field4 *datepaid;
  Field4 *rebateamt;
  Field4 *amountpaid;
  Field4 *adjustment;
  Field4 *vatamt;
  Field4 *rebate;

  Tapdist(Code4 *cb,LPSTR name=0):Data4(cb,"ap_dist")
  {
   SetIndex("account");
   top();
   account    = (Field4 *) new Field4(*(Data4 *) this, "account");       // 1
   dept       = (Field4 *) new Field4(*(Data4 *) this, "dept");          // 2
   apaccount  = (Field4 *) new Field4(*(Data4 *) this, "apaccount");       // 1
   apdepart   = (Field4 *) new Field4(*(Data4 *) this, "apdepart");          // 2
   period     = (Field4 *) new Field4(*(Data4 *) this, "period");        // 3
   date       = (Field4 *) new Field4(*(Data4 *) this, "date");          // 4
   duedate    = (Field4 *) new Field4(*(Data4 *) this, "duedate");       // 5
   batchno    = (Field4 *) new Field4(*(Data4 *) this, "batchno");       // 6
   invoiceno  = (Field4 *) new Field4(*(Data4 *) this, "invoiceno");     // 7
   paymentctl = (Field4 *) new Field4(*(Data4 *) this, "paymentctl");    // 8
   vendorno   = (Field4 *) new Field4(*(Data4 *) this, "vendorno");      // 9
   chequeno   = (Field4 *) new Field4(*(Data4 *) this, "chequeno");      // 10
   reference  = (Field4 *) new Field4(*(Data4 *) this, "reference");     // 11
   descript   = (Field4 *) new Field4(*(Data4 *) this, "descript");      // 12
   amount     = (Field4 *) new Field4(*(Data4 *) this, "amount");        // 13
   paymentreq = (Field4 *) new Field4(*(Data4 *) this, "paymentreq");    // 14
   datepaid   = (Field4 *) new Field4(*(Data4 *) this, "datepaid");      // 15
   amountpaid = (Field4 *) new Field4(*(Data4 *) this, "amountpaid");    // 16
   rebateamt  = (Field4 *) new Field4(*(Data4 *) this, "rebateamt");    // 16
   adjustment = (Field4 *) new Field4(*(Data4 *) this, "adjustment");    // 17
   vatamt     = (Field4 *) new Field4(*(Data4 *) this, "vatamt");    // 17
   rebate     = (Field4 *) new Field4(*(Data4 *) this, "rebate");        // 18

  };
  ~Tapdist(){close();};
  double dAmount(){ return double (Str4ptr(amount->str()));};
  double dAmountpaid(){ return double (Str4ptr(amountpaid->str()));};
  double dRebateamt(){ return double (Str4ptr(rebateamt->str()));};
  double dAdjustment(){ return double (Str4ptr(adjustment->str()));};
  long   lRequest()   { return long   (Str4ptr(paymentreq->str()));};
 };
#endif