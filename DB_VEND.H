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
  Field4 *descript;
  Field4 *ref;
  Field4 *status;
  Field4 *vat;

  Tapvendor(Code4 *cb,LPSTR name=0):Data4(cb,"ap_vend")
   {
    tag_select(tag("vendorno"));
    top();
    vendorno = (Field4 *) new Field4(*(Data4 *) this,"vendorno");
    vdshort  = (Field4 *) new Field4(*(Data4 *) this,"vdshort");
    vdname     = (Field4 *) new Field4(*(Data4 *) this,"name");
    address1 = (Field4 *) new Field4(*(Data4 *) this,"address1");
    address2 = (Field4 *) new Field4(*(Data4 *) this,"address2");
    address3 = (Field4 *) new Field4(*(Data4 *) this,"address3");
    postal   = (Field4 *) new Field4(*(Data4 *) this,"postal");
    phone    = (Field4 *) new Field4(*(Data4 *) this,"phone");
    fax      = (Field4 *) new Field4(*(Data4 *) this,"fax");
    contact  = (Field4 *) new Field4(*(Data4 *) this,"contact");
    account  = (Field4 *) new Field4(*(Data4 *) this,"account");
    descript = (Field4 *) new Field4(*(Data4 *) this,"descript");
    ref      = (Field4 *) new Field4(*(Data4 *) this,"ref");
    status   = (Field4 *) new Field4(*(Data4 *) this,"status");
    vat      = (Field4 *) new Field4(*(Data4 *) this,"vat");
   };
   ~Tapvendor(){close();};
   virtual int iVat() {return int ( Str4ptr(vat->str()) );}
 };
