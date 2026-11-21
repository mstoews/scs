_CLASSDEF (Tapvendor)
class Tapvendor : public Data4
{
 public:
  Field4 *vendorno;
  Field4 *vdshort;
  Field4 *vdname;
  Field4 *address1;
  Field4 *address2;
  Field4 *address3;
  Field4 *postal;
  Field4 *fax;
  Field4 *phone;
  Field4 *contact;
  Field4 *account;
  Field4 *depart;
  Field4 *descript;
  Field4 *type;
  Field4 *ref;
  Field4 *status;
  Field4 *terms;
  Field4 *vat;
  Field4 *percentage;
  Field4 *rebate;

  Tapvendor(Code4 *cb,LPSTR name=0):Data4(cb,"ap_vend")
   {
    //SetIndex("Vendorno");
    top();
    vendorno = (Field4 *) new Field4(*(Data4 *) this,"vendorno");
    vdshort  = (Field4 *) new Field4(*(Data4 *) this,"vdshort");
    vdname    = (Field4 *) new Field4(*(Data4 *) this,"vdname");
    address1 = (Field4 *) new Field4(*(Data4 *) this,"address1");
    address2 = (Field4 *) new Field4(*(Data4 *) this,"address2");
    address3 = (Field4 *) new Field4(*(Data4 *) this,"address3");
    postal   = (Field4 *) new Field4(*(Data4 *) this,"postal");
    phone    = (Field4 *) new Field4(*(Data4 *) this,"phone");
    fax      = (Field4 *) new Field4(*(Data4 *) this,"fax");
    contact  = (Field4 *) new Field4(*(Data4 *) this,"contact");
    account  = (Field4 *) new Field4(*(Data4 *) this,"account");
    depart   = (Field4 *) new Field4(*(Data4 *) this,"depart");
    descript = (Field4 *) new Field4(*(Data4 *) this,"descript");
    type     = (Field4 *) new Field4(*(Data4 *) this,"type");
    ref      = (Field4 *) new Field4(*(Data4 *) this,"ref");
    status   = (Field4 *) new Field4(*(Data4 *) this,"status");
    terms      = (Field4 *) new Field4(*(Data4 *) this,"terms");
    vat        = (Field4 *) new Field4(*(Data4 *) this,"vat");
    percentage = (Field4 *) new Field4(*(Data4 *) this,"percentage");
    rebate     = (Field4 *) new Field4(*(Data4 *) this,"rebate");
   };
   ~Tapvendor(){close();};
   virtual int iVat() {return int ( Str4ptr(vat->str()) );}
   virtual double dPercentage(){return double ( Str4ptr(percentage->str()) );}
   virtual double dRebate(){return double ( Str4ptr(rebate->str()) );}

 };
